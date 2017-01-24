#include <iostream>
#include <iomanip>
#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "Helpers.hpp"

// Przy kompilacji "niebezpiecznej" program wywo³uje bezpoœrednio instrukcje Win32
#if PROJECT_RULES_UNSAFE
#include <windows.h>
#endif

using namespace std;

// Inicjalizacja programu
time_t initialize() {
  // Czas rozpoczêcia i inicjalizacja generatora liczb pseudolosowych
  auto startt = time(NULL);
  srand((unsigned int)startt);

  // Parametry okna konsoli
  system("title Englot by Mateusz Karcz");
  system("mode con cp select=1250 >nul");
  system("mode con cols=80 lines=25 >nul");
  system("color 07 >nul");

  // Przechwytywanie sygna³ów w celu zamiany ich na wyj¹tki
  signal(SIGABRT, cb_signal_handler);
  signal(SIGFPE, cb_signal_handler);
  signal(SIGILL, cb_signal_handler);
  signal(SIGSEGV, cb_signal_handler);
  signal(SIGTERM, cb_signal_handler);

  return startt;
}

// Przetwarzanie sygna³ów na wyj¹tki
void cb_signal_handler(int signal) {
  switch(signal) {
    case SIGABRT:
      throw runtime_error("Abnormal termination requested");
    case SIGFPE:
      throw runtime_error("Floating-point error");
    case SIGILL:
      throw runtime_error("Illegal instruction");
    case SIGSEGV:
      throw runtime_error("Memory access violation");
    case SIGTERM:
      throw runtime_error("Termination request");

    default:
      char msg[32];
      snprintf(msg, 32, "Unknown signal %d", signal);
      throw runtime_error(msg);
  }
}

// Uruchomienie œledzenia przebiegu
ofstream* log_start(time_t startt) {
  // Otwarcie pliku i przekierowanie nañ strumienia clog
  ofstream* log = new ofstream("Englot.log");
  clog.rdbuf(log->rdbuf());

  // Zapis nag³ówka
  clog<<"Englot, compiled "<<__TIMESTAMP__
#if PROJECT_RULES_UNSAFE
      <<", project rules unsafe version"
#endif
      <<endl
      <<"(C) 2016-2017 Mateusz Karcz"<<endl
      <<"    Gdañsk University of Technology"<<endl
      <<"    Faculty of Electronics, Telecommunications and Informatics"<<endl
      <<"    Electronics and Telecommunications 2016/2017"<<endl
      <<"-----"<<endl
      <<"Program start: "<<ctime(&startt);

  return log;
}

// Zakoñczenie œledzenia przebiegu
void log_stop(ofstream* log, time_t startt) {
  auto stopt = time(NULL);
  auto diff = stopt - startt;

  // Zapis czasu wykonania
  clog<<ctime(&stopt)
      <<"Runtime:";

  if(diff >= 3600) {
    clog<<' '<<(diff/3600)<<'h';
    diff %= 3600;
  }

  if(diff >= 60) {
    clog<<' '<<(diff/60)<<'m';
    diff %= 60;
  }

  if(diff)
    clog<<' '<<diff<<'s';

  clog<<endl;
  log->close();

  delete log;
}

// Oczekiwanie na klawisz ENTER
void input_wait() {
  cin.get();
}

// Wejœcie rodzaju TAK/NIE
bool input_yesno() {
  string input;
  while(true) {
    cout<<"[T/N] ";
    cin>>input;

    if(input == "t" || input == "T" || input == "y" || input == "Y")
      return true;
    if(input == "n" || input == "N")
      return false;
  }
}

// Wejœcie rodzaju A lub B
bool input_ab(char a, char b) {
  string sal, sau, sbl, sbu, input;

  // Nierozró¿nianie wielkoœci znaków
  a = tolower(a); b = tolower(b);
  sal += a;        sbl += b;
  a = toupper(a); b = toupper(b);
  sau += a;        sbu += b;

  while(true) {
    cout<<'['<<a<<'/'<<b<<"] ";
    cin>>input;

    if(input == sau || input == sal)
      return true;
    if(input == sbu || input == sbl)
      return false;
  }
}

// Wczytanie has³a
string input_password() {
  string ret;

#if PROJECT_RULES_UNSAFE
  // Ukrycie wejœcia w kompilacji "niebezpiecznej"
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;

  GetConsoleMode(hStdin, &mode);
  mode &= ~ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);
#endif

  getline(cin, ret);

#if PROJECT_RULES_UNSAFE
  // Przywrócenie wejœcia w kompilacji "niebezpiecznej"
  GetConsoleMode(hStdin, &mode);
  mode |= ENABLE_ECHO_INPUT;
  SetConsoleMode(hStdin, mode);
  cout<<endl;
#endif

  return string(ret);
}

// Czyszczenie okna konsoli
void output_clear() {
  system("cls");
}

// Wyœwietlanie poziomej linii
void output_line() {
  for(int i = 0; i < SCREEN_WIDTH; i++)
    cout.put('—');
}

// Wyœwietlanie wyœrodkowanego tekstu
void output_center(std::string str) {
  cout<<right<<setw((str.length()+SCREEN_WIDTH)/2)<<str<<endl;
}

// Przycinanie tekstu do d³ugoœci pola
string str_trim(string str, size_t max) {
  if(str.length() > max)
    return str.substr(0, max - 3) + "...";
  return str;
}

// Scalanie ze sob¹ posortowanych tabel z wynikami
void merge_results(Englot::Test* a[], size_t an, Englot::Test* b[], size_t bn, Englot::Test* out[]) {
  // Tworzenie tabeli tymczasowej
  auto tmp = new const Englot::Test*[an+bn];
  
  // Iteratory:
  // i - pierwsza tabela
  // j - druga tabela
  // k - tabela tymczasowa
  size_t i = 0, j = 0, k = 0;
  while(true) {
    if(i == an) {
      // Gdy obie tabele siê skoñczy³y, zakoñcz
      if(j == bn)
        break;
      // Gdy pierwsza tabela siê skoñczy³a, kopiuj drug¹
      else
        tmp[k] = b[j++];
    // Gdy druga tabela siê skoñczy³a, kopiuj pierwsz¹
    } else if(j == bn)
      tmp[k] = a[i++];
    // Kopiuj wiêkszy element
    else {
      if(a[i]->result > b[j]->result)
        tmp[k] = a[i++];
      else
        tmp[k] = b[j++];
    }
    // IdŸ do nastêpnego
    k++;
  }

  // Skopiowanie do tabeli wynikowej
  memcpy(out, tmp, (an+bn) * sizeof(Englot::Test*));
  delete tmp;
}

// Sortowanie tabeli wyników (sortowanie przez scalanie)
void sort_results(Englot::Test* rs[], size_t n) {
  // Tabele puste i jednoelementowe nie s¹ sortowane
  if(n < 2) return;

  // Oddzielne sortowanie po³ów tabeli
  auto middle = n / 2;
  sort_results(rs, middle);
  auto right = rs + middle;
  sort_results(right, n - middle);

  // Scalanie po³ów tabeli
  merge_results(rs, middle, right, n-middle, rs);
}

// Ustawianie koloru tekstu na wyjœciu
ostream& operator<<(ostream& iostr, color cl) {
#if PROJECT_RULES_UNSAFE
  // W kompilacji "niebezpiecznej" zmiana koloru tekstu
  SetConsoleTextAttribute(
    GetStdHandle(STD_OUTPUT_HANDLE),
    (WORD)cl
  );
#endif

  return iostr;
}
