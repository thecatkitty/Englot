#pragma once

#include <stdexcept>

#include "VC.hpp"
#include "Test.hpp"

// Parametry bazy danych programu
const size_t MAX_USERS = 32;
const size_t MAX_DICTIONARIES = 16;
const size_t MAX_RESULTS = 128;

namespace Englot {
  // G³ówny model programu, opisuj¹cy ca³y jego stan
  class EnglotModel : public Serializable {
    public:
      EnglotModel();
      ~EnglotModel();

      bool serialize();
      bool deserialize();

      // Dostêp do bazy
      User* const get_user(size_t index);
      size_t add_user(User* usr);

      size_t const get_active_user();
      size_t set_active_user(size_t index);

      Dictionary* const get_dictionary();
      Dictionary* set_dictionary(Dictionary* dict);

      Test* const get_result(size_t index);
      Test* add_result(Test* tst);

      // Pomocnicze metody odczytu
      size_t get_user_by_login(std::string login);
      Dictionary* get_known_dictionary(std::string name);

    private:
      // Baza danych
      User* users[MAX_USERS];
      Dictionary* dictionary;
      Dictionary* known_dics[MAX_DICTIONARIES];
      Test* results[MAX_RESULTS];
      // Liczniki bazy
      size_t users_s, results_s, active_user, known_dics_s;

      // Pomocnicze metody poszukiwania
      size_t get_user_number(User* user);
      size_t get_dictionary_number(Dictionary* dict);
  };

  // Kontroler i widoki modelu g³ównego
  extern Controller<EnglotModel> EnglotController;
  extern View<EnglotModel> EnglotView;
  extern View<EnglotModel> EnglotUserView;
  extern View<EnglotModel> EnglotResultsPointsView;
  extern View<EnglotModel> EnglotResultsUsersView;
  extern View<EnglotModel> EnglotSetDictionaryView;
  extern View<EnglotModel> EnglotHelpView;
  extern View<EnglotModel> EnglotLoginView;
  extern View<EnglotModel> EnglotAddUserView;

  // Widoki pocz¹tku i zakoñczenia programu
  extern View<void> IntroView;
  extern View<void> OutroView;
}

