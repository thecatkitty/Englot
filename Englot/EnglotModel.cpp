#include <iostream>

#include "Englot.hpp"

using namespace Englot;
using namespace std;

// Nag³ówki sekcji pliku zapisu
const size_t ENGLOT_MAGIC_SIZE = 8;
const char* ENGLOT_MAGIC = "ENGLOTMK";

const size_t SECTION_MAGIC_SIZE = 4;
const char* USRS_MAGIC = "USRS";
const char* KNDS_MAGIC = "KNDS";
const char* RESS_MAGIC = "RESS";

// Konstruktor modelu g³ównego
EnglotModel::EnglotModel() {
  memset(users, 0, MAX_USERS * sizeof(User*));
  memset(results, 0, MAX_RESULTS * sizeof(User*));

  dictionary = nullptr;
  users_s = results_s = known_dics_s = 0;
  active_user = MAX_USERS;
}

// Destruktor modelu g³ównego
EnglotModel::~EnglotModel() {
  // Wyczyœæ bazê
  for(size_t i = 0; i < users_s; i++)
    delete users[i];
  for(size_t i = 0; i < results_s; i++)
    delete results[i];
  for(size_t i = 0; i < known_dics_s; i++)
    delete known_dics[i];

  // Wyzeruj liczniki
  dictionary = nullptr;
  users_s = results_s = known_dics_s = 0;
  active_user = 0;
}

// Serializacja modelu g³ównego (zapis stanu programu)
bool EnglotModel::serialize() {
  if(stream->bad())
    return false;

  clog<<"Model serialize"<<endl;

  // Zapisanie sygnatury
  (*stream)<<ENGLOT_MAGIC;

  // Zapisanie tabeli u¿ytkowników
  (*stream)<<USRS_MAGIC;
  stream->write((const char*)&users_s, sizeof(size_t));

  for(size_t i = 0; i < users_s; i++) {
    users[i]->attach(*stream);
    users[i]->serialize();
  }

  clog<<"Serialized "<<users_s<<" users."<<endl;

  // Zapisanie tabeli znanych s³owników
  (*stream)<<KNDS_MAGIC;
  stream->write((const char*)&known_dics_s, sizeof(size_t));
  for(size_t i = 0; i < known_dics_s; i++) {
    size_t name_s = 0;
    // Skasowane s³owniki przyjmuj¹ nazwê o zerowej d³ugoœci
    if(known_dics[i])
      name_s = known_dics[i]->get_name().length();
    stream->write((const char*)&name_s, sizeof(size_t));
    if(known_dics[i])
      stream->write(known_dics[i]->get_name().c_str(), name_s);
  }

  clog<<"Serialized "<<known_dics_s<<" known dictionaries."<<endl;

  // Zapisanie tabeli wyników
  (*stream)<<RESS_MAGIC;
  stream->write((const char*)&results_s, sizeof(size_t));

  for(size_t i = 0; i < results_s; i++) {
    auto un = get_user_number(results[i]->user);
    // Dla skasowanych s³owników ich numer przyjmuje siê wartoœæ UINT32_MAX
    auto dn = results[i]->dictionary ? get_dictionary_number(results[i]->dictionary) : UINT32_MAX;
    stream->write((const char*)&un, sizeof(size_t));
    stream->write((const char*)&dn, sizeof(size_t));
    results[i]->attach(*stream);
    results[i]->serialize();
  }

  clog<<"Serialized "<<results_s<<" results."<<endl;

  return true;
}

// Deserializacja modelu g³ównego (wczytanie stanu programu)
bool EnglotModel::deserialize() {
  if(stream->bad())
    return false;

  clog<<"Model deserialize"<<endl;

  // Wczytanie sygnatury
  char buff[ENGLOT_MAGIC_SIZE];
  stream->read(buff, ENGLOT_MAGIC_SIZE);
  if(memcmp(buff, ENGLOT_MAGIC, ENGLOT_MAGIC_SIZE))
    throw invalid_argument("Model deserialization failed. Invalid file signature.");

  // Wczytanie tabeli u¿ytkowników
  stream->read(buff, SECTION_MAGIC_SIZE);
  if(memcmp(buff, USRS_MAGIC, SECTION_MAGIC_SIZE))
    throw invalid_argument("Model deserialization failed. Invalid users section signature.");
  stream->read((char*)&users_s, sizeof(size_t));
  if(users_s > MAX_USERS)
    throw out_of_range("Model deserialization failed. Too many users.");

  for(size_t i = 0; i < users_s; i++) {
    users[i] = new User;
    users[i]->attach(*stream);
    users[i]->deserialize();
  }

  clog<<"Deserialized "<<users_s<<" users."<<endl;

  // Wczytanie tabeli znanych s³owników
  stream->read(buff, SECTION_MAGIC_SIZE);
  if(memcmp(buff, KNDS_MAGIC, SECTION_MAGIC_SIZE))
    throw invalid_argument("Model deserialization failed. Invalid known dictionaries section signature.");
  stream->read((char*)&known_dics_s, sizeof(size_t));
  if(known_dics_s > MAX_DICTIONARIES)
    throw out_of_range("Model deserialization failed. Too many dictionaries.");

  for(size_t i = 0; i < known_dics_s; i++) {
    size_t name_s;
    stream->read((char*)&name_s, sizeof(size_t));
    // Skasowany s³ownik
    if(!name_s)
      known_dics[i] = nullptr;
    // Wczytanie nazwy pliku s³ownika
    else {
      auto name = new char[name_s+1];
      stream->read(name, name_s);
      name[name_s] = 0;

      // Próba otwarcia s³ownika
      try {
        known_dics[i] = new Dictionary(name);
      } catch(const exception& e) {
        clog<<"Cannot load known dictionary: file '"<<name<<"' - "<<e.what()<<endl;
        known_dics[i] = nullptr;
      }
      delete name;
    }
  }

  clog<<"Deserialized "<<known_dics_s<<" known dictionaries."<<endl;

  // Wczytanie tabeli wyników
  stream->read(buff, SECTION_MAGIC_SIZE);
  if(memcmp(buff, RESS_MAGIC, SECTION_MAGIC_SIZE))
    throw invalid_argument("Model deserialization failed. Invalid results section signature.");
  stream->read((char*)&results_s, sizeof(size_t));
  if(results_s > MAX_RESULTS)
    throw out_of_range("Model deserialization failed. Too many results.");

  for(size_t i = 0; i < results_s; i++) {
    results[i] = new Test;

    size_t un, dn;
    stream->read((char*)&un, sizeof(size_t));
    stream->read((char*)&dn, sizeof(size_t));
    results[i]->user = users[un];
    // S³ownik usuniêty
    if(dn == UINT32_MAX)
      results[i]->dictionary = nullptr;
    // S³ownik znany
    else
      results[i]->dictionary = known_dics[dn];

    results[i]->attach(*stream);
    results[i]->deserialize();
  }

  clog<<"Deserialized "<<results_s<<" results."<<endl;

  return true;
}

// Pobierz wskaŸnik na podstawie indeksu u¿ytkownika
User* const EnglotModel::get_user(size_t index) {
  if(index < users_s)
    return users[index];
  return nullptr;
}

// Dodaj u¿ytkownika do bazy
size_t EnglotModel::add_user(User* usr) {
  clog<<"Model add user";

  if(!usr)
    throw std::invalid_argument("Cannot add user. Null pointer exception.");
  clog<<": "<<usr->login<<" ["<<usr->name<<"]"<<endl;
  if(users_s == MAX_USERS)
    throw std::out_of_range("Cannot add user. The list is full.");

  users[users_s] = usr;
  return users_s++;
}

// Pobierz indeks zalogowanego u¿ytkownika; zwraca MAX_USERS, gdy nie zalogowano
size_t const Englot::EnglotModel::get_active_user() {
  return active_user;
}

// Ustaw indeks zalogowanego u¿ytkownika; MAX_USERS, gdy wylogowanie
size_t Englot::EnglotModel::set_active_user(size_t index) {
  clog<<"Model set active user: ";

  if(index == MAX_USERS) {
    clog<<"null";
    return active_user = MAX_USERS;
  }
  clog<<index;

  if(index > MAX_USERS)
    throw std::out_of_range("Cannot set active user. Index from outside the list.");
  if(!users[index])
    throw std::invalid_argument("Cannot set active user. Null pointer exception.");

  clog<<" ["<<users[index]->login<<"]"<<endl;
  return active_user = index;
}

// Pobierz wskaŸnik na aktywny s³ownik
Dictionary* const EnglotModel::get_dictionary() {
  return dictionary;
}

// Ustaw aktywny s³ownik
Dictionary* EnglotModel::set_dictionary(Dictionary* dict) {
  clog<<"Model set dictionary";

  if(!dict)
    throw std::invalid_argument("Cannot change dictionary. Null pointer exception.");

  // Sprawdzanie, czy s³ownik nie zosta³ wczeœniej za³adowany
  bool known = false;
  for(size_t i = 0; i < known_dics_s; i++) 
    if(known_dics[i] == dict) {
      known = true;
      break;
    }

  if(!known)
    if(known_dics_s == MAX_DICTIONARIES)
      throw std::out_of_range("Cannot change dictionary. The list is full.");

  clog<<": file '"<<dict->get_name()<<"'"<<endl;

  // Jeœli nie za³adowano wczeœniej, dodaj do listy i wybierz
  if(!known)
    known_dics[known_dics_s++] = dict;
  return dictionary = dict;
}

// Pobierz wskaŸnik na podstawie indeksu wyniku
Test* const EnglotModel::get_result(size_t index) {
  if(index < results_s)
    return results[index];
  return nullptr;
}

// Dodaj wynik do bazy
Test* EnglotModel::add_result(Test* tst) {

  clog<<"Model add results";

  if(!tst)
    throw std::invalid_argument("Cannot add results. Null pointer exception.");
  clog<<": dictionary '"<<tst->dictionary->get_name()<<"', user '"<<tst->user->name<<"', "<<tst->result<<" answers of "<<tst->indices_s<<endl;
  if(users_s == MAX_USERS)
    throw std::out_of_range("Cannot add results. The list is full.");

  return results[results_s++] = tst;
}

// Wyszukaj indeks u¿ytkownika na podstawie jego nazwy, u¿ywane przy tworzeniu nowego u¿ytkownika
size_t EnglotModel::get_user_by_login(std::string login) {
  for(size_t i = 0; i < users_s; i++) {
    if(users[i]->login == login)
      return i;
  }
  return MAX_USERS;
}

// Wyszukaj wskaŸnik na znany s³ownik na podstawie jego nazwy pliku
Dictionary* Englot::EnglotModel::get_known_dictionary(std::string name) {
  for(size_t i = 0; i < known_dics_s; i++)
    if(known_dics[i])
      if(name == known_dics[i]->get_name())
        return known_dics[i];
  return nullptr;
}

// Wyszukaj indeks u¿ytkownika na podstawie wskaŸnika
size_t Englot::EnglotModel::get_user_number(User* user) {
  for(size_t i = 0; i < users_s; i++)
    if(users[i] == user)
      return i;
  return MAX_USERS;
}

// Wyszukaj indeks znanego s³ownika na podstawie wskaŸnika
size_t Englot::EnglotModel::get_dictionary_number(Dictionary* dict) {
  for(size_t i = 0; i < known_dics_s; i++)
    if(known_dics[i] == dict)
      return i;
  return MAX_DICTIONARIES;
}
