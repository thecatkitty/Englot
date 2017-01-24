#include <iomanip>

#include "Englot.hpp"
#include "Helpers.hpp"

#if PROJECT_RULES_UNSAFE
#include <windows.h>
#endif

using namespace std;
using namespace Englot;

// Widok g³ówny (menu)
int cbv_englot(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  int input;

  // Wyœiwetlenie nag³ówka
  cout<<setw(SCREEN_WIDTH/2-3)<<""
      <<color::blue<<'E'
      <<color::white<<'N'
      <<color::red<<'G'
      <<color::blue<<'L'
      <<color::white<<'O'
      <<color::red<<'T'
      <<color::white<<endl;
  output_center("Program wspieraj¹cy powtarzanie wyra¿eñ w jêzykach obcych");
  output_center("© 2016-2017 Mateusz Karcz, PG ETI");
  cout<<color::silver;
  output_line();

  // Wyœwietlenie listy opcji
  cout<<color::white<<" Menu g³ówne:"<<endl<<color::silver;
  if(e->get_dictionary())
    cout<<"  1. Test"<<endl
        <<"  2. Moje statystyki"<<endl
        <<"  3. Wyniki wed³ug testów"<<endl
        <<"  4. Wyniki wed³ug u¿ytkowników"<<endl
        <<endl
        <<"  5. Wybór s³ownika ["<<color::gray<<e->get_dictionary()->get_title()<<color::silver<<"]"<<endl
        <<"  6. Pomoc"<<endl;
  else
    cout<<"  1. Moje statystyki"<<endl
        <<"  2. Wyniki wed³ug testów"<<endl
        <<"  3. Wyniki wed³ug u¿ytkowników"<<endl
        <<endl
        <<"  4. Wybór s³ownika"<<endl
        <<"  5. Pomoc"<<endl;

  cout<<endl
      <<"  9. Wyloguj"<<endl
      <<"  0. Zakoñcz"<<endl;

  // Wczytanie wyboru
  cout<<endl;
  output_line();
  cout<<setw(SCREEN_WIDTH/2)<<""<<" Twój wybór: ";
  cin>>input;

  // Przesuniêcie przy braku s³ownika
  if(!e->get_dictionary() && input && input != 9)
    input++;

  // Sprawdzenie wejœcia
  if(input < 0 || input > 9 || input == 7 || input == 8)
    return -1;

  return input;
}

// Szerokoœci kolumn w tabelach
const int W_TIMEDATE     = 25; // "Data i godzina"
const int W_USER_P       = 14; // "U¿ytkownik" w rankingu testów
const int W_USER_R       = 39; // "Uzytkownik" w rankingu u¿ytkowników
const int W_DICTIONARY_U = 34; // "S³ownik" w statystykach u¿ytkownika
const int W_DICTIONARY_P = 20; // "S³ownik" w rankingu testów
const int W_TESTS        = 19; // "Liczba testów"
const int W_RESULT       = 11; // "Wynik"
const int W_POINTS       = 20; // "Liczba punktów"
const int W_PERCENT      = 8;  // "Procent

// Szerokosæ pola "Testów ³¹cznie"
const int W_NTESTS = W_TIMEDATE + W_DICTIONARY_U + 1;

// Widok statystyk u¿ytkownika
int cbv_englot_user(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  User* usr = e->get_user(e->get_active_user());

  // Wyœwietlenie nag³ówka
  cout<<color::white;
  output_center("STATYSTYKI U¯YTKOWNIKA");
  cout<<color::silver;
  
  string tmp;
  tmp = " Login: " + usr->login;
  cout<<left<<setw(SCREEN_WIDTH/2)<<tmp;
  tmp = "Nazwa pe³na: " + usr->name + " ";
  cout<<right<<setw(SCREEN_WIDTH/2)<<tmp;

  cout<<endl<<endl
      <<" "<<left<<setw(W_TIMEDATE)<<"Data i godzina"
      <<setw(W_DICTIONARY_U)<<"S³ownik"
      <<right<<setw(W_RESULT-3)<<"Wynik"
      <<right<<setw(W_PERCENT+3)<<"Procent"<<endl;

  output_line();

  // Wyœwietlenie tabeli
  int tests = 0, points = 0, entries = 0;
  for(int i = 0; i < MAX_RESULTS; i++) {
    auto r = e->get_result(i);
    if(!r) break;

    if(r->user == usr) {
      string timestamp = ctime(&(r->timestamp));
      timestamp = timestamp.substr(0, timestamp.length()-1);

      cout<<" "<<left<<setw(W_TIMEDATE)<<timestamp
          <<setw(W_DICTIONARY_U)<<str_trim(r->dictionary ? r->dictionary->get_title() : "Nieznany s³ownik", W_DICTIONARY_U-1)
          <<right<<setw(W_RESULT/2)<<r->result<<'/'<<left<<setw(W_RESULT/2)<<r->indices_s
          <<right<<setw(W_PERCENT-1)<<(r->result * 100 / r->indices_s)<<'%'<<endl;

      // Sumowanie
      tests++;
      points += r->result;
      entries += r->indices_s;
    }
  }

  // Wyœwietlenie podsumowania
  if(tests) {
    output_line();

    cout<<" Testów ³¹cznie: "<<left<<setw(W_NTESTS-17)<<tests
      <<right<<setw(W_RESULT/2)<<points<<'/'<<left<<setw(W_RESULT/2)<<entries
      <<right<<setw(W_PERCENT-1)<<(points * 100 / entries)<<'%'<<endl;
  } else {
    cout<<" Brak rozwi¹zanych testów."<<endl;
  }
      
  input_wait();
  return 0;
}

// Widok rankingu testów
int cbv_englot_results_points(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  string tmp;

  // Wyœwietlenie nag³ówka
  cout<<color::white;
  output_center("RANKING TESTÓW");
  cout<<color::silver;

  // Pobranie i posortowanie wyników
  Test* rs[MAX_RESULTS];
  size_t rsn;
  for(rsn = 0; rsn < MAX_RESULTS; rsn++) {
    rs[rsn] = e->get_result(rsn);
    if(!rs[rsn]) break;
  }

  sort_results(rs, rsn);

  cout<<endl
    <<" "<<left<<setw(W_TIMEDATE)<<"Data i godzina"
    <<setw(W_USER_P)<<"U¿ytkownik"
    <<setw(W_DICTIONARY_P)<<"S³ownik"
    <<right<<setw(W_RESULT-3)<<"Wynik"
    <<right<<setw(W_PERCENT+3)<<"Procent"<<endl;

  output_line();

  // Wyœwietlenie tabeli
  if(rsn) {
    for(size_t i = 0; i < rsn; i++) {
      auto r = rs[i];

      string timestamp = ctime(&(r->timestamp));
      timestamp = timestamp.substr(0, timestamp.length()-1);

      cout<<" "<<left<<setw(W_TIMEDATE)<<timestamp
          <<setw(W_USER_P)<<str_trim(r->user->name, W_USER_P-1)
          <<setw(W_DICTIONARY_P)<<str_trim(r->dictionary ? r->dictionary->get_title() : "Nieznany s³ownik", W_DICTIONARY_P-1)
          <<right<<setw(W_RESULT/2)<<r->result<<'/'<<left<<setw(W_RESULT/2)<<r->indices_s
          <<right<<setw(W_PERCENT-1)<<(r->result * 100 / r->indices_s)<<'%'<<endl;
    }
  } else {
    cout<<" Brak rozwi¹zanych testów."<<endl;
  }

  input_wait();
  return 0;
}

// Widok rankingu u¿ytkowników
int cbv_englot_results_users(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  // Wyœwietlenie nag³ówka
  cout<<color::white;
  output_center("RANKING U¯YTKOWNIKÓW");
  cout<<color::silver;

  // Podsumowanie wyników wszystkich u¿ytkowników
  Test* urs[MAX_USERS];
  size_t ursn;
  for(ursn = 0; ursn < MAX_USERS; ursn++) {
    // Stworzenie obiektu wyniku
    urs[ursn] = new Test;
    urs[ursn]->indices_s = 1;
    urs[ursn]->result = 0;
    urs[ursn]->tests = 0;
    // Przypisanie u¿ytkownika
    urs[ursn]->user = e->get_user(ursn);
    if(!urs[ursn]->user) break;

    // Podsumowanie wyników u¿ytkownika
    for(int i = 0; i < MAX_RESULTS; i++) {
      auto r = e->get_result(i);
      if(!r) break;

      if(r->user == urs[ursn]->user) {
        urs[ursn]->result += r->result;
        urs[ursn]->tests++;
      }
    }
  }

  sort_results(urs, ursn);

  cout<<endl<<" "
      <<left<<setw(W_USER_R)<<"U¿ytkownik"
      <<right<<setw(W_TESTS)<<"Liczba testów"
      <<right<<setw(W_POINTS)<<"Liczba punktów"<<endl;

  output_line();

  // Wyœwietlenie tabeli
  for(size_t i = 0; i < ursn; i++)
    cout<<" "
        <<left<<setw(W_USER_R)<<str_trim(urs[i]->user->name, W_USER_R-1)
        <<right<<setw(W_TESTS)<<urs[i]->tests
        <<right<<setw(W_POINTS)<<urs[i]->result<<endl;

  input_wait();
  return 0;
}

// Widok wyboru s³ownika
int cbv_englot_set_dictionary(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  // Wyœwietl nag³ówek
  cout<<color::white;
  output_center("WYBÓR S£OWNIKA");
  cout<<color::silver;

  string file;

#if PROJECT_RULES_UNSAFE
  // W kompilacji "niebiezpiecznej" zostaje otwarte okno dialogowe otwierania pliku

  // Utworzenie struktury okna dialogowego
  OPENFILENAMEA ofn;
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);

  // Jeœli za³adowane wczeœniej s³ownik, zachowaj jego nazwê
  char filename[MAX_PATH];
  if(e->get_dictionary())
    strcpy(filename, e->get_dictionary()->get_name().c_str());
  else
    filename[0] = 0;
  ofn.lpstrFile = filename;

  // Filtr (typy plików)
  ofn.lpstrFilter = "S³owniki (*.csv)\0"      "*.CSV\0"
                    "Wszystkie pliki (*.*)\0" "*.*\0"
                    "\0";
  ofn.nFilterIndex = 1;

  // Okno dialogowe jest modalne wzglêdem okna konsoli
  ofn.hwndOwner = GetConsoleWindow();

  // Ustawienie procedury wywo³ania zwracaj¹cej wartoœæ 'fa³sz'
  // powoduje wyœwietlanie okna dialogowego w stylu Windows 3.x
  ofn.Flags = OFN_ENABLEHOOK;
  ofn.lpfnHook = [](HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) -> UINT_PTR {
    return FALSE;
  };

  // Pozosta³e parametry
  ofn.lpstrTitle = "Wybieranie s³ownika";
  ofn.Flags |= OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST;
  ofn.nMaxFile = MAX_PATH;

  // Wyœwietlenie okna
  GetOpenFileNameA(&ofn);

  // Ustawienie nowej nazwy pliku
  file = filename;

#else

  // W kompilacji "bezpiecznej" wczytaj nazwê pliku ze standardowego wejœcia
  cin.ignore();
  cout<<"Wpisz nazwê pliku s³ownika: ";
  getline(cin, file);
#endif

  // Próba za³adowania znanego s³ownika
  Dictionary* dict = e->get_known_dictionary(file);
  if(dict) {
    try {
      e->set_dictionary(dict);
      cout<<color::green<<"Wybrano s³ownik '"<<e->get_dictionary()->get_title()<<"'."<<endl<<color::silver;
    } catch(const exception& x) {
      cerr<<color::red<<"Nie mo¿na wybraæ s³ownika: "<<x.what()<<endl<<color::silver;
    }

  // Próba za³adowania nowego s³ownika
  } else {
    try {
      dict = new Dictionary(file);
      e->set_dictionary(dict);
      cout<<color::green<<"Otwarto s³ownik '"<<e->get_dictionary()->get_title()<<"'."<<endl<<color::silver;
    } catch(const Dictionary::no_index& x) {
      cerr<<color::red<<x.what()<<endl<<color::silver;
      cout<<"Czy chcesz utworzyæ plik indeksu? ";
      if(input_yesno()) {
        // Próba stworzenia pliku indeksu
        try {
          delete dict;
          Dictionary::create_ind(file);
          dict = new Dictionary(file);
          e->set_dictionary(dict);
          cout<<color::green<<"Otwarto s³ownik '"<<e->get_dictionary()->get_title()<<"'."<<endl<<color::silver;
        } catch(const exception& x) {
          cerr<<color::red<<"Nie mo¿na utworzyæ indeksu: "<<x.what()<<endl<<color::silver;
        }
      }
    } catch(const exception& x) {
      cerr<<color::red<<"Nie mo¿na otworzyæ s³ownika: "<<x.what()<<endl<<color::silver;
    }
  }

  return 0;
}

// Widok pomocy
extern const char* txt_readme[];
int cbv_englot_help(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  cin.ignore();

  bool cont = true; // Przyjmuje wartoœæ 'fa³sz' po osi¹gniêciu ostatniej linii pomocy
  int begin = 0;    // Indeks pierwszej wyœwietlanej linii

  while(cont) { // Pomoc wyœwietla siê przewijalnymi fragmentami
    output_clear();

    // Wyœwietlenie nag³ówka
    cout<<color::white;
    output_center("POMOC");
    cout<<color::silver;
    output_line();

    // Wyœwietlenie okreœlonej liczby linii
    for(int i = begin, max = begin + SCREEN_HEIGHT - 3; i < max; i++) {
      if(txt_readme[i]) {
        // Pierwsza linia nie wymaga poprzedzenia znakiem nowej linii
        if(i != begin)
          cout<<endl;

#if PROJECT_RULES_UNSAFE
        // W kompilacji "niebezpiecznej" pomoc jest wielobarwna

        // Pierwsza linia jest ¿ó³ta
        if(!i)
          cout<<color::yellow<<txt_readme[i]<<color::silver;
        // Linie rozpoczynaj¹ce siê znakiem praw autorskich s¹ ciemnoszare
        else if(txt_readme[i][0] == '©')
          cout<<color::gray<<txt_readme[i]<<color::silver;
        // Przetwarzanie nag³ówków
        else if(txt_readme[i][0] == '»') {
          // Nag³ówki dzia³ów s¹ niebieskie
          if(txt_readme[i][strlen(txt_readme[i])-1] == '«')
            cout<<color::blue<<txt_readme[i]<<color::silver;
          // Nag³ówki sekcji s¹ granatowe
          else {
            cout<<color::navy;
            auto pch = txt_readme[i];
            // Tekst w nag³ówkach zawarty w nawiasach kwadratowych jest morski
            while(*pch) {
              if(*pch == '[')
                cout<<color::teal;

              cout.put(*pch);

              if(*pch == ']')
                cout<<color::navy;

              pch++;
            }
            cout<<color::silver;
          }
        // Koniec przetwarzania nag³ówków
        } else {
          // Tekst zwyk³y jest jasnoszary
          cout<<"  ";
          auto pch = txt_readme[i];
          bool bold = false;
          while(*pch) {
            // Tekst pomiêdzy apostrofami jest bia³y
            bool just_bold = false;
            if(*pch == '\'')
              if(!bold) {
                cout<<color::white;
                bold = true;
                just_bold = true; // Biel w³¹czono tym znakiem
              }

            cout.put(*pch);

            if(*pch == '\'')
              // Wy³¹cz biel, jeœli nie w³¹czono jej tym znakiem
              if(bold && !just_bold) {
                cout<<color::silver;
                bold = false;
              }

            pch++;
          }
        }
#else
        // W kompilacji "bezpiecznej" po prostu po chamsku wyœwietl liniê
        cout<<' '<<txt_readme[i];
#endif

      // Osi¹gniêto ostatni¹ liniê (txt_readme[i] = nullptr)
      } else
        cont = false;
    }

    input_wait();
    begin++;
  }

  return 0;
}

int cbv_englot_add_user(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  cout<<color::white;
  output_center("DODAWANIE NOWEGO U¯YTKOWNIKA");
  cout<<color::silver;

  string login, name, password, repassw;
  while(true) {
    cout<<"Wpisz ¿¹dan¹ nazwê u¿ytkownika: ";
    cin>>login;
    if(e->get_user_by_login(login) == MAX_USERS)
      break;
    cerr<<color::red<<"Wybrana nazwa u¿ytkownika jest ju¿ zajêta!"<<endl<<color::silver;
  }

  cin.ignore();
  while(true) {
    cout<<"Wpisz has³o: ";
    password = input_password();
    if(password.length() < 8) {
      cerr<<color::red<<"Has³o jest zbyt krótkie! Jego d³ugoœæ musi wynosiæ co najmniej 8 znaków."<<endl<<color::silver;
      continue;
    }
#if PROJECT_RULES_UNSAFE
    cout<<"Wpisz powtórnie has³o: ";
    repassw = input_password();
    if(password != repassw) {
      cerr<<color::red<<"Wpisane has³a ró¿ni¹ siê!"<<endl<<color::silver;
      continue;
    }
#endif
    cout<<"Wpisz imiê i nazwisko: ";
    getline(cin, name);

    return (int) new User(login, password, name);
  }
}

int cbv_englot_login(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  cout<<color::white;
  output_center("LOGOWANIE I REJESTRACJA");
  cout<<color::silver;

  if(e->get_user(0) == nullptr) {
    cout<<"Nie ma jeszcze ¿adnych kont u¿ytkowników. Czy utworzyæ? ";
    if(input_yesno())
      return EnglotController.run(e, &EnglotAddUserView);
    return -1;
  } else {
    cout<<"Za[l]oguj siê lub utwórz [n]owe konto: ";
    if(!input_ab('l', 'n'))
      return EnglotController.run(e, &EnglotAddUserView);

    string login, password;
    cout<<"Wpisz login: ";
    cin>>login;

    cout<<"Wpisz has³o: ";
    cin.ignore();
    password = input_password();

    auto usr = e->get_user_by_login(login);
    if(usr < MAX_USERS)
      if(e->get_user(usr)->hash == User::get_hash(password))
        return usr;
  }
  
  return MAX_USERS;
}

View<EnglotModel> Englot::EnglotView(cbv_englot);
View<EnglotModel> Englot::EnglotUserView(cbv_englot_user);
View<EnglotModel> Englot::EnglotResultsPointsView(cbv_englot_results_points);
View<EnglotModel> Englot::EnglotResultsUsersView(cbv_englot_results_users);
View<EnglotModel> Englot::EnglotSetDictionaryView(cbv_englot_set_dictionary);
View<EnglotModel> Englot::EnglotHelpView(cbv_englot_help);
View<EnglotModel> Englot::EnglotAddUserView(cbv_englot_add_user);
View<EnglotModel> Englot::EnglotLoginView(cbv_englot_login);
