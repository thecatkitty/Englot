#include <iomanip>

#include "Englot.hpp"
#include "Helpers.hpp"

#if PROJECT_RULES_UNSAFE
#include <windows.h>
#endif

using namespace std;
using namespace Englot;

// Widok g��wny (menu)
int cbv_englot(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  int input;

  // Wy�iwetlenie nag��wka
  cout<<setw(SCREEN_WIDTH/2-3)<<""
      <<color::blue<<'E'
      <<color::white<<'N'
      <<color::red<<'G'
      <<color::blue<<'L'
      <<color::white<<'O'
      <<color::red<<'T'
      <<color::white<<endl;
  output_center("Program wspieraj�cy powtarzanie wyra�e� w j�zykach obcych");
  output_center("� 2016-2017 Mateusz Karcz, PG ETI");
  cout<<color::silver;
  output_line();

  // Wy�wietlenie listy opcji
  cout<<color::white<<" Menu g��wne:"<<endl<<color::silver;
  if(e->get_dictionary())
    cout<<"  1. Test"<<endl
        <<"  2. Moje statystyki"<<endl
        <<"  3. Wyniki wed�ug test�w"<<endl
        <<"  4. Wyniki wed�ug u�ytkownik�w"<<endl
        <<endl
        <<"  5. Wyb�r s�ownika ["<<color::gray<<e->get_dictionary()->get_title()<<color::silver<<"]"<<endl
        <<"  6. Pomoc"<<endl;
  else
    cout<<"  1. Moje statystyki"<<endl
        <<"  2. Wyniki wed�ug test�w"<<endl
        <<"  3. Wyniki wed�ug u�ytkownik�w"<<endl
        <<endl
        <<"  4. Wyb�r s�ownika"<<endl
        <<"  5. Pomoc"<<endl;

  cout<<endl
      <<"  9. Wyloguj"<<endl
      <<"  0. Zako�cz"<<endl;

  // Wczytanie wyboru
  cout<<endl;
  output_line();
  cout<<setw(SCREEN_WIDTH/2)<<""<<" Tw�j wyb�r: ";
  cin>>input;

  // Przesuni�cie przy braku s�ownika
  if(!e->get_dictionary() && input && input != 9)
    input++;

  // Sprawdzenie wej�cia
  if(input < 0 || input > 9 || input == 7 || input == 8)
    return -1;

  return input;
}

// Szeroko�ci kolumn w tabelach
const int W_TIMEDATE     = 25; // "Data i godzina"
const int W_USER_P       = 14; // "U�ytkownik" w rankingu test�w
const int W_USER_R       = 39; // "Uzytkownik" w rankingu u�ytkownik�w
const int W_DICTIONARY_U = 34; // "S�ownik" w statystykach u�ytkownika
const int W_DICTIONARY_P = 20; // "S�ownik" w rankingu test�w
const int W_TESTS        = 19; // "Liczba test�w"
const int W_RESULT       = 11; // "Wynik"
const int W_POINTS       = 20; // "Liczba punkt�w"
const int W_PERCENT      = 8;  // "Procent

// Szerokos� pola "Test�w ��cznie"
const int W_NTESTS = W_TIMEDATE + W_DICTIONARY_U + 1;

// Widok statystyk u�ytkownika
int cbv_englot_user(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  User* usr = e->get_user(e->get_active_user());

  // Wy�wietlenie nag��wka
  cout<<color::white;
  output_center("STATYSTYKI U�YTKOWNIKA");
  cout<<color::silver;
  
  string tmp;
  tmp = " Login: " + usr->login;
  cout<<left<<setw(SCREEN_WIDTH/2)<<tmp;
  tmp = "Nazwa pe�na: " + usr->name + " ";
  cout<<right<<setw(SCREEN_WIDTH/2)<<tmp;

  cout<<endl<<endl
      <<" "<<left<<setw(W_TIMEDATE)<<"Data i godzina"
      <<setw(W_DICTIONARY_U)<<"S�ownik"
      <<right<<setw(W_RESULT-3)<<"Wynik"
      <<right<<setw(W_PERCENT+3)<<"Procent"<<endl;

  output_line();

  // Wy�wietlenie tabeli
  int tests = 0, points = 0, entries = 0;
  for(int i = 0; i < MAX_RESULTS; i++) {
    auto r = e->get_result(i);
    if(!r) break;

    if(r->user == usr) {
      string timestamp = ctime(&(r->timestamp));
      timestamp = timestamp.substr(0, timestamp.length()-1);

      cout<<" "<<left<<setw(W_TIMEDATE)<<timestamp
          <<setw(W_DICTIONARY_U)<<str_trim(r->dictionary ? r->dictionary->get_title() : "Nieznany s�ownik", W_DICTIONARY_U-1)
          <<right<<setw(W_RESULT/2)<<r->result<<'/'<<left<<setw(W_RESULT/2)<<r->indices_s
          <<right<<setw(W_PERCENT-1)<<(r->result * 100 / r->indices_s)<<'%'<<endl;

      // Sumowanie
      tests++;
      points += r->result;
      entries += r->indices_s;
    }
  }

  // Wy�wietlenie podsumowania
  if(tests) {
    output_line();

    cout<<" Test�w ��cznie: "<<left<<setw(W_NTESTS-17)<<tests
      <<right<<setw(W_RESULT/2)<<points<<'/'<<left<<setw(W_RESULT/2)<<entries
      <<right<<setw(W_PERCENT-1)<<(points * 100 / entries)<<'%'<<endl;
  } else {
    cout<<" Brak rozwi�zanych test�w."<<endl;
  }
      
  input_wait();
  return 0;
}

// Widok rankingu test�w
int cbv_englot_results_points(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  string tmp;

  // Wy�wietlenie nag��wka
  cout<<color::white;
  output_center("RANKING TEST�W");
  cout<<color::silver;

  // Pobranie i posortowanie wynik�w
  Test* rs[MAX_RESULTS];
  size_t rsn;
  for(rsn = 0; rsn < MAX_RESULTS; rsn++) {
    rs[rsn] = e->get_result(rsn);
    if(!rs[rsn]) break;
  }

  sort_results(rs, rsn);

  cout<<endl
    <<" "<<left<<setw(W_TIMEDATE)<<"Data i godzina"
    <<setw(W_USER_P)<<"U�ytkownik"
    <<setw(W_DICTIONARY_P)<<"S�ownik"
    <<right<<setw(W_RESULT-3)<<"Wynik"
    <<right<<setw(W_PERCENT+3)<<"Procent"<<endl;

  output_line();

  // Wy�wietlenie tabeli
  if(rsn) {
    for(size_t i = 0; i < rsn; i++) {
      auto r = rs[i];

      string timestamp = ctime(&(r->timestamp));
      timestamp = timestamp.substr(0, timestamp.length()-1);

      cout<<" "<<left<<setw(W_TIMEDATE)<<timestamp
          <<setw(W_USER_P)<<str_trim(r->user->name, W_USER_P-1)
          <<setw(W_DICTIONARY_P)<<str_trim(r->dictionary ? r->dictionary->get_title() : "Nieznany s�ownik", W_DICTIONARY_P-1)
          <<right<<setw(W_RESULT/2)<<r->result<<'/'<<left<<setw(W_RESULT/2)<<r->indices_s
          <<right<<setw(W_PERCENT-1)<<(r->result * 100 / r->indices_s)<<'%'<<endl;
    }
  } else {
    cout<<" Brak rozwi�zanych test�w."<<endl;
  }

  input_wait();
  return 0;
}

// Widok rankingu u�ytkownik�w
int cbv_englot_results_users(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  // Wy�wietlenie nag��wka
  cout<<color::white;
  output_center("RANKING U�YTKOWNIK�W");
  cout<<color::silver;

  // Podsumowanie wynik�w wszystkich u�ytkownik�w
  Test* urs[MAX_USERS];
  size_t ursn;
  for(ursn = 0; ursn < MAX_USERS; ursn++) {
    // Stworzenie obiektu wyniku
    urs[ursn] = new Test;
    urs[ursn]->indices_s = 1;
    urs[ursn]->result = 0;
    urs[ursn]->tests = 0;
    // Przypisanie u�ytkownika
    urs[ursn]->user = e->get_user(ursn);
    if(!urs[ursn]->user) break;

    // Podsumowanie wynik�w u�ytkownika
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
      <<left<<setw(W_USER_R)<<"U�ytkownik"
      <<right<<setw(W_TESTS)<<"Liczba test�w"
      <<right<<setw(W_POINTS)<<"Liczba punkt�w"<<endl;

  output_line();

  // Wy�wietlenie tabeli
  for(size_t i = 0; i < ursn; i++)
    cout<<" "
        <<left<<setw(W_USER_R)<<str_trim(urs[i]->user->name, W_USER_R-1)
        <<right<<setw(W_TESTS)<<urs[i]->tests
        <<right<<setw(W_POINTS)<<urs[i]->result<<endl;

  input_wait();
  return 0;
}

// Widok wyboru s�ownika
int cbv_englot_set_dictionary(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;

  // Wy�wietl nag��wek
  cout<<color::white;
  output_center("WYB�R S�OWNIKA");
  cout<<color::silver;

  string file;

#if PROJECT_RULES_UNSAFE
  // W kompilacji "niebiezpiecznej" zostaje otwarte okno dialogowe otwierania pliku

  // Utworzenie struktury okna dialogowego
  OPENFILENAMEA ofn;
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);

  // Je�li za�adowane wcze�niej s�ownik, zachowaj jego nazw�
  char filename[MAX_PATH];
  if(e->get_dictionary())
    strcpy(filename, e->get_dictionary()->get_name().c_str());
  else
    filename[0] = 0;
  ofn.lpstrFile = filename;

  // Filtr (typy plik�w)
  ofn.lpstrFilter = "S�owniki (*.csv)\0"      "*.CSV\0"
                    "Wszystkie pliki (*.*)\0" "*.*\0"
                    "\0";
  ofn.nFilterIndex = 1;

  // Okno dialogowe jest modalne wzgl�dem okna konsoli
  ofn.hwndOwner = GetConsoleWindow();

  // Ustawienie procedury wywo�ania zwracaj�cej warto�� 'fa�sz'
  // powoduje wy�wietlanie okna dialogowego w stylu Windows 3.x
  ofn.Flags = OFN_ENABLEHOOK;
  ofn.lpfnHook = [](HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) -> UINT_PTR {
    return FALSE;
  };

  // Pozosta�e parametry
  ofn.lpstrTitle = "Wybieranie s�ownika";
  ofn.Flags |= OFN_HIDEREADONLY | OFN_LONGNAMES | OFN_PATHMUSTEXIST;
  ofn.nMaxFile = MAX_PATH;

  // Wy�wietlenie okna
  GetOpenFileNameA(&ofn);

  // Ustawienie nowej nazwy pliku
  file = filename;

#else

  // W kompilacji "bezpiecznej" wczytaj nazw� pliku ze standardowego wej�cia
  cin.ignore();
  cout<<"Wpisz nazw� pliku s�ownika: ";
  getline(cin, file);
#endif

  // Pr�ba za�adowania znanego s�ownika
  Dictionary* dict = e->get_known_dictionary(file);
  if(dict) {
    try {
      e->set_dictionary(dict);
      cout<<color::green<<"Wybrano s�ownik '"<<e->get_dictionary()->get_title()<<"'."<<endl<<color::silver;
    } catch(const exception& x) {
      cerr<<color::red<<"Nie mo�na wybra� s�ownika: "<<x.what()<<endl<<color::silver;
    }

  // Pr�ba za�adowania nowego s�ownika
  } else {
    try {
      dict = new Dictionary(file);
      e->set_dictionary(dict);
      cout<<color::green<<"Otwarto s�ownik '"<<e->get_dictionary()->get_title()<<"'."<<endl<<color::silver;
    } catch(const Dictionary::no_index& x) {
      cerr<<color::red<<x.what()<<endl<<color::silver;
      cout<<"Czy chcesz utworzy� plik indeksu? ";
      if(input_yesno()) {
        // Pr�ba stworzenia pliku indeksu
        try {
          delete dict;
          Dictionary::create_ind(file);
          dict = new Dictionary(file);
          e->set_dictionary(dict);
          cout<<color::green<<"Otwarto s�ownik '"<<e->get_dictionary()->get_title()<<"'."<<endl<<color::silver;
        } catch(const exception& x) {
          cerr<<color::red<<"Nie mo�na utworzy� indeksu: "<<x.what()<<endl<<color::silver;
        }
      }
    } catch(const exception& x) {
      cerr<<color::red<<"Nie mo�na otworzy� s�ownika: "<<x.what()<<endl<<color::silver;
    }
  }

  return 0;
}

// Widok pomocy
extern const char* txt_readme[];
int cbv_englot_help(EnglotModel* e, void* p) {
  clog<<__FUNCTION__<<endl;
  cin.ignore();

  bool cont = true; // Przyjmuje warto�� 'fa�sz' po osi�gni�ciu ostatniej linii pomocy
  int begin = 0;    // Indeks pierwszej wy�wietlanej linii

  while(cont) { // Pomoc wy�wietla si� przewijalnymi fragmentami
    output_clear();

    // Wy�wietlenie nag��wka
    cout<<color::white;
    output_center("POMOC");
    cout<<color::silver;
    output_line();

    // Wy�wietlenie okre�lonej liczby linii
    for(int i = begin, max = begin + SCREEN_HEIGHT - 3; i < max; i++) {
      if(txt_readme[i]) {
        // Pierwsza linia nie wymaga poprzedzenia znakiem nowej linii
        if(i != begin)
          cout<<endl;

#if PROJECT_RULES_UNSAFE
        // W kompilacji "niebezpiecznej" pomoc jest wielobarwna

        // Pierwsza linia jest ��ta
        if(!i)
          cout<<color::yellow<<txt_readme[i]<<color::silver;
        // Linie rozpoczynaj�ce si� znakiem praw autorskich s� ciemnoszare
        else if(txt_readme[i][0] == '�')
          cout<<color::gray<<txt_readme[i]<<color::silver;
        // Przetwarzanie nag��wk�w
        else if(txt_readme[i][0] == '�') {
          // Nag��wki dzia��w s� niebieskie
          if(txt_readme[i][strlen(txt_readme[i])-1] == '�')
            cout<<color::blue<<txt_readme[i]<<color::silver;
          // Nag��wki sekcji s� granatowe
          else {
            cout<<color::navy;
            auto pch = txt_readme[i];
            // Tekst w nag��wkach zawarty w nawiasach kwadratowych jest morski
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
        // Koniec przetwarzania nag��wk�w
        } else {
          // Tekst zwyk�y jest jasnoszary
          cout<<"  ";
          auto pch = txt_readme[i];
          bool bold = false;
          while(*pch) {
            // Tekst pomi�dzy apostrofami jest bia�y
            bool just_bold = false;
            if(*pch == '\'')
              if(!bold) {
                cout<<color::white;
                bold = true;
                just_bold = true; // Biel w��czono tym znakiem
              }

            cout.put(*pch);

            if(*pch == '\'')
              // Wy��cz biel, je�li nie w��czono jej tym znakiem
              if(bold && !just_bold) {
                cout<<color::silver;
                bold = false;
              }

            pch++;
          }
        }
#else
        // W kompilacji "bezpiecznej" po prostu po chamsku wy�wietl lini�
        cout<<' '<<txt_readme[i];
#endif

      // Osi�gni�to ostatni� lini� (txt_readme[i] = nullptr)
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
  output_center("DODAWANIE NOWEGO U�YTKOWNIKA");
  cout<<color::silver;

  string login, name, password, repassw;
  while(true) {
    cout<<"Wpisz ��dan� nazw� u�ytkownika: ";
    cin>>login;
    if(e->get_user_by_login(login) == MAX_USERS)
      break;
    cerr<<color::red<<"Wybrana nazwa u�ytkownika jest ju� zaj�ta!"<<endl<<color::silver;
  }

  cin.ignore();
  while(true) {
    cout<<"Wpisz has�o: ";
    password = input_password();
    if(password.length() < 8) {
      cerr<<color::red<<"Has�o jest zbyt kr�tkie! Jego d�ugo�� musi wynosi� co najmniej 8 znak�w."<<endl<<color::silver;
      continue;
    }
#if PROJECT_RULES_UNSAFE
    cout<<"Wpisz powt�rnie has�o: ";
    repassw = input_password();
    if(password != repassw) {
      cerr<<color::red<<"Wpisane has�a r�ni� si�!"<<endl<<color::silver;
      continue;
    }
#endif
    cout<<"Wpisz imi� i nazwisko: ";
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
    cout<<"Nie ma jeszcze �adnych kont u�ytkownik�w. Czy utworzy�? ";
    if(input_yesno())
      return EnglotController.run(e, &EnglotAddUserView);
    return -1;
  } else {
    cout<<"Za[l]oguj si� lub utw�rz [n]owe konto: ";
    if(!input_ab('l', 'n'))
      return EnglotController.run(e, &EnglotAddUserView);

    string login, password;
    cout<<"Wpisz login: ";
    cin>>login;

    cout<<"Wpisz has�o: ";
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
