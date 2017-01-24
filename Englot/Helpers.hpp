#pragma once

// Flaga definiuj¹ca, czy program mo¿e wywo³ywaæ funkcje spoza biblioteki standardowej (kompilacja "niebezpieczna")
#define PROJECT_RULES_UNSAFE true

#include <string>

#include <csignal>

#include "Englot.hpp"

// Wymiary ekranu
const int SCREEN_WIDTH  = 80;
const int SCREEN_HEIGHT = 25;

// Inicjalizacja programu i przetwarzania sygna³ów na wyj¹tki
time_t initialize();
void cb_signal_handler(int);

// Œledzenie przebiegu
std::ofstream* log_start(time_t);
void log_stop(std::ofstream*, time_t);

// Funkcje pomocnicze wejœcia
void input_wait();
bool input_yesno();
bool input_ab(char, char);
std::string input_password();

// Funkcje pomocnicze wyjœcia
void output_clear();
void output_line();
void output_center(std::string);

// Przycinanie tekstu do d³ugoœci pola
std::string str_trim(std::string, size_t max = std::string::npos);

// Sortowanie wyników
void merge_results(Englot::Test*[], size_t, Englot::Test*[], size_t, Englot::Test*[]);
void sort_results(Englot::Test*[], size_t);

// Kolory wyœwietlanego tekstu
enum class color {
  black   = 0,
  navy    = 1,
  green   = 2,
  teal    = 3,
  maroon  = 4,
  purple  = 5,
  olive   = 6,
  silver  = 7,
  gray    = 8,
  blue    = 9,
  lime    = 10,
  aqua    = 11,
  red     = 12,
  fuchsia = 13,
  yellow  = 14,
  white   = 15
};
std::ostream& operator<<(std::ostream&, color);
