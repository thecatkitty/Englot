#include <ctime>
#include <cstdlib>

#include "Englot.hpp"

using namespace std;
using namespace Englot;

// Konstruktor domyœlny wyniku
Test::Test() {
  result = 0;
  timestamp = 0;
  indices = nullptr;
  indices_s = 0;
  dictionary = nullptr;
  deserialized = false;
}

// Konstruktor testu
Test::Test(Dictionary* dict, User* usr, size_t entries) {
  result = 0;
  timestamp = time(nullptr);
  dictionary = dict;
  user = usr;
  deserialized = false;

  size_t dic_s = dictionary->size();

  clog<<"Test create: dictionary '"<<dict->get_name()<<"', user '"<<usr->name<<"', "<<entries<<" entries of "<<dic_s<<endl;

  bool has_title = (*dictionary)[0].left == "#";

  // Dopasowanie liczby ¿¹danych hase³ do wymiaru s³ownika
  if(entries >= dic_s)
    entries = has_title ? dic_s-1 : dic_s;

  // Utworzenie tabeli indeksów hase³
  indices_s = entries;
  indices = new size_t[indices_s];
  memset(indices, 0, indices_s * sizeof(size_t));

  // Losowanie hase³
  while(entries) {
    size_t selected = 0;
    while(already_selected(selected))
      if(has_title)
        selected = rand() % (dic_s-1) + 1;
      else
        selected = rand() % dic_s;
    indices[entries-1] = selected;
    entries--;
  }
}

// Destruktoe testu lub wyniku
Test::~Test() {
  result = 0;
  timestamp = 0;
  indices = nullptr;
  indices_s = 0;
  dictionary = nullptr;

  if(deserialized)
    delete dictionary;
  deserialized = false;
}

// Serializacja wyniku
bool Test::serialize() {
  if(stream->bad())
    return false;

  clog<<"Test serialize: "<<ctime(&timestamp);

  stream->write((char*)&timestamp, sizeof(size_t));
  stream->write((char*)&indices_s, sizeof(size_t));
  stream->write((char*)&result, sizeof(size_t));

  return true;
}

// Deserializacja wyniku
bool Test::deserialize() {
  if(stream->bad())
    return false;

  clog<<"Test deserialize: ";

  stream->read((char*)&timestamp, sizeof(size_t));
  stream->read((char*)&indices_s, sizeof(size_t));
  stream->read((char*)&result, sizeof(size_t));
  deserialized = true;

  clog<<ctime(&timestamp);

  return true;
}

// Metoda sprawdzaj¹ca, czy podane has³o zosta³o ju¿ wczeœniej wylosowane
bool Test::already_selected(size_t index) {
  for(size_t i = indices_s; i; i--)
    if(indices[i-1] == index)
      return true;
  return false;
}
