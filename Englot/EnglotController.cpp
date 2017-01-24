#include "Englot.hpp"
#include "Helpers.hpp"

using namespace std;
using namespace Englot;

// Kontroler modelu g³ównego
int cbc_englot(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  auto v = (View<EnglotModel>*) p;

  // Widok g³ówny
  if(v == &EnglotView) {
    // Logowanie
    if(e->get_active_user() == MAX_USERS) {
      size_t usr = MAX_USERS;
      while(true) {
        usr = EnglotController.run(e, &EnglotLoginView);
        if(usr == -1)
          return -1;
        if(usr != MAX_USERS) {
          e->set_active_user(usr);
          break;
        }
        cerr<<color::red<<"B³¹d logowania!"<<endl<<color::silver;
        input_wait();
      }
    }

    // Zamiana cyfr na widoki
    View<EnglotModel>* in2v[] = {
      &EnglotView,
      nullptr,
      &EnglotUserView,
      &EnglotResultsPointsView,
      &EnglotResultsUsersView,
      &EnglotSetDictionaryView,
      &EnglotHelpView
    };

    // Wyœwietlenie menu i odczytanie wyboru
    int input = INT32_MAX;
    input = v->render(e);

    // Wykonanie akcji
    Test* t;
    switch(input) {
      case 1: // Test
        t = new Test( // Wygenerowanie testu
          e->get_dictionary(), 
          e->get_user(e->get_active_user()), 
          TestController.run(nullptr, &TestNumberView) // Pytanie o liczbê hase³
        );
        TestController.run(t, &TestView); // Wyœwietlenie testu
        e->add_result(t); // Dopisanie wyniku do bazy
        return 0;

      case 9: // Wyloguj
        e->set_active_user(MAX_USERS);
        return 0;

      case 0: // Zakoñcz
        return -1;

      case -1: // B³êdny wybór
        return 0;

      default: // Wywo³anie innego widoku
        EnglotController.run(e, in2v[input]);
    }
    return 0;

  // Dodawanie u¿ytkownika
  } else if(v == &EnglotAddUserView) {
    auto usr = (User*)v->render(e);
    if(usr == nullptr) return 0;
    return e->add_user(usr);

  // Logowanie
  } else if(v == &EnglotLoginView) {
    return v->render(e);

  // Pozosta³e widoki
  } else {
    auto ret = v->render(e);
    input_wait();
    return ret;
  }
}
Controller<EnglotModel> Englot::EnglotController(cbc_englot);
