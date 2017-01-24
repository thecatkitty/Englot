#include "Englot.hpp"

using namespace std;
using namespace Englot;

// Konstruktor domyœlny u¿ytkownika
User::User() {
  login = "";
  name = "";
  hash = 0;
}

// Konstruktor u¿ytkownika
User::User(std::string login, std::string password, std::string name) {
  clog<<"User create: "<<login<<" ["<<name<<"]"<<endl;
  this->login = login;
  this->hash = get_hash(password);
  this->name = name;
}

// Destruktor u¿ytkownika
User::~User() {
  login = "";
  name = "";
  hash = 0;
}


// Serializacja u¿ytkownika
bool User::serialize() {
  if(login == "")
    return false;
  if(stream->bad())
    return false;

  clog<<"User serialize: "<<login<<" ["<<name<<"]"<<endl;

  size_t login_s = login.length();
  size_t name_s = name.length();

  stream->write((char*)&login_s, sizeof(size_t));
  stream->write((char*)&hash, sizeof(size_t));
  stream->write((char*)&name_s, sizeof(size_t));
  stream->write(login.c_str(), login_s);
  stream->write(name.c_str(), name_s);

  return true;
}

// Deserializacja u¿ytkownika
bool User::deserialize() {
  if(stream->bad())
    return false;

  clog<<"User deserialize: ";

  size_t login_s, name_s;

  stream->read((char*)&login_s, sizeof(size_t));
  stream->read((char*)&hash, sizeof(size_t));
  stream->read((char*)&name_s, sizeof(size_t));

  char* login_d = new char[login_s+1];
  stream->read(login_d, login_s);
  login_d[login_s] = 0;
  login = login_d;
  delete login_d;

  char* name_d = new char[name_s+1];
  stream->read(name_d, name_s);
  name_d[name_s] = 0;
  name = name_d;
  delete name_d;

  clog<<login<<" ["<<name<<"]"<<endl;

  return true;
}

// Funkcja skrótu (dla has³a)
uint32_t User::get_hash(std::string str) {
  int ret = 0;

  for(size_t i = 0, max = str.length(); i < max; i++) {
    ret += str[i];
    ret <<= 2;
  }

  return ret;
}
