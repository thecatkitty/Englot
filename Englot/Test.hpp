#pragma once

#include <string>

#include "Serializable.hpp"
#include "Dictionary.hpp"
#include "User.hpp"

namespace Englot {
  // Model testu lub wyniku
  class Test : public Serializable {
    public:
      Test();
      Test(Dictionary* dict, User* usr, size_t entries);
      ~Test();
      bool serialize();
      bool deserialize();

      int result;
      union {
        time_t timestamp; // W przypadku pojedynczych test�w
        size_t tests;     // W przypadku wynik�w zbiorczych
      };
      Dictionary* dictionary;
      User* user;

      // Wylosowane has��
      size_t* indices;
      size_t indices_s;

    private:
      bool already_selected(size_t index); // Metoda pomocnicza przy losowaniu hase�
      bool deserialized; // Zmienna pomocnicza zapobiegaj�ca nieprawid�owym destrukcjom obiekt�w
  };

  // Kontroler i widoki modelu testu lub wyniku
  extern Controller<Test> TestController;
  extern View<Test> TestView;
  extern View<Test> TestNumberView;
}
