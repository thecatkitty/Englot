#include <stdexcept>
#include <cstring>
#include <iostream>

#include "Englot.hpp"

using namespace std;
using namespace Englot;

// Sygnatura pliku indeksu
const size_t IND_MAGIC_SIZE = 8;
const char* IND_MAGIC = "PGET97MK";

// Maksymalna d³ugoœæ wyra¿enia
const int DIC_ENTRY_MAX_SIZE = 127;

// Konstruktor domyœlny s³ownika
Dictionary::Dictionary() {

}

// Otwarcie s³ownika
Dictionary::Dictionary(std::string file) {
  clog<<"Dictionary open: file '"<<file<<"'";
  dic.open(file);

  // Sprawdzenie, czy uda³o siê otworzyæ s³ownik
  if(!dic.good()) {
    dic.close();
    clog<<" - error"<<endl;
    throw invalid_argument("Opening of the dictionary file failed. Cannot read.");
  }

  clog<<", index '"<<get_ind(file)<<"'";
  ind.open(get_ind(file), ios::binary);
  // Sprawdzenie, czy uda³o siê otworzyæ indeks
  if(!ind.good()) {
    ind.close();
    clog<<" - read error"<<endl;
    throw no_index();
  }

  // Sprawdzenie sygnatury indeksu
  char signature[IND_MAGIC_SIZE];
  ind.read(signature, IND_MAGIC_SIZE);
  if(memcmp(signature, IND_MAGIC, IND_MAGIC_SIZE)) {
    ind.close();
    clog<<" - format error"<<endl;
    throw invalid_argument("Invalid index file signature.");
  }

  // Sprawdzenie rozmiaru indeksu
  clog<<endl;
  name = file;
  ind.seekg(0, ind.end);
  ind_bytes = (size_t)ind.tellg();
  if(ind_bytes < IND_MAGIC_SIZE + sizeof(size_t)) {
    ind.close();
    clog<<" - format error"<<endl;
    throw invalid_argument("Index is empty.");
  }
}

// Zamkniêcie s³ownika
Dictionary::~Dictionary() {
  clog<<"Dictionary close: ";
  if(dic.is_open()) {
    clog<<"file '"<<name<<"', ";
    dic.close();
  }

  if(ind.is_open()) {
    clog<<"index '"<<get_ind(name)<<"'";
    ind.close();
  }
  clog<<endl;
}

// Odczytanie has³a ze s³ownika
DictionaryEntry Dictionary::operator[](size_t index) {
  clog<<"Dictionary read: file '"<<name<< "', entry "<<index;

  if(index > size()) {
    clog<<" - out of range error"<<endl;
    throw out_of_range("There is not so much entries in the dictionary.");
  }

  // Odczytanie po³o¿enia has³a
  ind.seekg(IND_MAGIC_SIZE, ind.beg);
  ind.ignore(index * sizeof(size_t));
  size_t offset;
  ind.read((char*)&offset, sizeof(size_t));

  clog<<", offset "<<offset<<endl;

  char buff[DIC_ENTRY_MAX_SIZE+1];
  string left, right;

  // Wczytanie tekstu z odpowiedniego miejsca
  dic.seekg(dic.beg + offset);
  dic.getline(buff, DIC_ENTRY_MAX_SIZE, ';');
  left = buff;
  dic.getline(buff, DIC_ENTRY_MAX_SIZE);
  right = buff;

  return { left, right };
}

// Odczytanie liczby hase³
size_t Dictionary::size() {
  return (ind_bytes - IND_MAGIC_SIZE) / sizeof(size_t);
}

// Odczytanie nazwy pliku s³ownika
std::string Dictionary::get_name() {
  return name;
}

// Odczytanie tytu³u s³ownika
std::string Englot::Dictionary::get_title() {
  string ret;

  auto t = (*this)[0];
  // Jeœli pierwszy wiersz zaczyna siê krzy¿ykiem, to jest tam tytu³
  if(t.left == "#")
    ret = t.right;
  // Inaczej zwróæ nazwê
  else {
    ret = name;
    if(ret.find('\\') != string::npos)
      ret = ret.substr(ret.find_last_of('\\') + 1);
  }

  return string(ret);
}

// Tworzenie indeksu
bool Dictionary::create_ind(std::string file) {
  clog<<"Index create: dictionary '"<<file<<"'";

  // Otwieranie s³ownika do odczytu
  ifstream fin(file);
  if(fin.bad()) {
    fin.close();
    clog<<" - dictionary read error"<<endl;
    throw invalid_argument("Opening of the dictionary file failed. Cannot create index.");
  }

  // Otwieranie indeksu do zapisu
  clog<<", index '"<<get_ind(file)<<"'";
  ofstream fout(get_ind(file), ios::binary);
  if(fout.bad()) {
    fout.close();
    clog<<" - index write error"<<endl;
    throw invalid_argument("Creating of the index file failed.");
  }
  fout<<IND_MAGIC;

  // Przetwarzanie s³ownika linia po linii
  size_t n = 0;
  while(fin.good()) {
    size_t offset = (size_t)fin.tellg();
    char buff[DIC_ENTRY_MAX_SIZE * 2 + 2];
    fin.getline(buff, DIC_ENTRY_MAX_SIZE * 2 + 2);
    if(!strcmp(buff, "")) continue;    // Pomijanie pustych linii
    if(strstr(buff, ";") == nullptr) { // Szukanie œrednika
      fin.close();
      fout.close();
      clog<<" - line "<<n<<": dictionary format error"<<endl;
      throw invalid_argument("Malformed entry in the dictionary file. Missing semicolon.");
    }
    // Zapis po³o¿enia
    fout.write((const char*)&offset, sizeof(size_t));
    n++;
  }
  fin.close();
  fout.close();

  clog<<" - "<<n<<" entries processed"<<endl;
  return true;
}

// Generowanie nazwy pliku indeksu
std::string Dictionary::get_ind(std::string name) {
  if(name.find('.') == string::npos)
    return string(name + ".ind");

  return string(name.substr(0, name.find_last_of('.')) + ".ind");
}
