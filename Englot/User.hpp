#pragma once

#include <string>

#include "Serializable.hpp"

namespace Englot {
  // Klasa opisuj¹ca u¿ytkownika
  class User : public Serializable {
    public:
      User();
      User(std::string login, std::string password, std::string name);
      ~User();
      bool serialize();
      bool deserialize();

      // Funkcja skrótu
      static uint32_t get_hash(std::string str);

      // Dane u¿ytkownika
      std::string login;
      std::string name;
      uint32_t hash;
  };
}
