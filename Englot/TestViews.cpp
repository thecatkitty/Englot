#include <iomanip>

#include "Englot.hpp"
#include "Helpers.hpp"

using namespace std;
using namespace Englot;

// Widok testu wiedzy
int cbv_test(Test* t, void* p) {
  clog<<__FUNCTION__<<endl;
  cin.ignore();

  int result = 0;
  // Dla wszystkich wylosowanych hase³
  for(size_t i = 0, max = t->indices_s; i < max; i++) {
    auto e = (*t->dictionary)[t->indices[i]];

    // Czyszczenie ekranu i wyœwietlenie nag³ówka
    output_clear();
    cout<<left<<setw(SCREEN_WIDTH/2)<<" TEST WIEDZY";
    char tmp[SCREEN_WIDTH/2];
    snprintf(tmp, SCREEN_WIDTH/2, "%d z %d, %d b³êdów", i+1, max, i-result);
    cout<<right<<setw(SCREEN_WIDTH/2-1)<<tmp<<endl;
    output_line();

    // Zapytanie
    cout<<endl<<" Wpisz polskie t³umaczenie wyra¿enia:"<<endl
        <<"  "<<e.left<<endl
        <<endl
        <<" Twoja odpowiedŸ:"<<endl
        <<"  ";

    // OdpowiedŸ
    string answer;
    getline(cin, answer);
    cout<<endl;
    if(answer == e.right) {
      result++;
      cout<<color::green<<" Poprawna odpowiedŸ!"<<color::silver;
    } else {
      cout<<color::red<<" Poprawn¹ odpowiedzi¹ jest: "<<color::silver<<endl
          <<"  "<<e.right;
    }

    input_wait();
  }

  // Ekran wyniku
  output_clear();
  output_center("KONIEC TESTU");
  output_line();

  cout<<endl<<" Twój wynik:"<<endl
      <<"  "<<result<<" odpowiedzi poprawnych z "<<t->indices_s<<"."<<endl;

  input_wait();
  return result;
}
View<Test> Englot::TestView(cbv_test);

// Widok ustalania liczby hase³
int cbv_test_number(Test* t, void* p) {
  clog<<__FUNCTION__<<endl;
  cin.ignore();

  // Czyszczenie ekranu i wyœwietlenie nag³ówka
  output_clear();
  cout<<" TEST WIEDZY"<<endl;
  output_line();

  int ret;
  while(true) {
    // Zapytanie i wejœcie
    cout<<endl<<" Wpisz liczbê wyra¿eñ (co najmniej 5): ";
    cin>>ret;
    if(ret >= 5)
      return ret;
  }
}
View<Test> Englot::TestNumberView(cbv_test_number);
