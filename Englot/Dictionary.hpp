#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

namespace Englot {
  // Definicja pary wyra¿enie-t³umaczenie
  struct DictionaryEntry {
    std::string left, right;
  };

  // Klasa abstrakcji s³ownika
  class Dictionary {
    public:
      Dictionary();
      Dictionary(std::string file);
      ~Dictionary();

      DictionaryEntry operator[](size_t index);
      size_t size();
      std::string get_name();
      std::string get_title();

      // Tworzenie pliku indeksu
      static bool create_ind(std::string file);
      static std::string get_ind(std::string name);

      // Definicja wyj¹tku braku pliku indeksu
      class no_index : public std::exception {
        public:
        virtual char const * what() const { return "Opening of the index file failed, perhaps not created."; }
      };

    private:
      std::ifstream dic, ind;
      std::string name;
      size_t ind_bytes;
  };

}
