#include <iomanip>

#include "Helpers.hpp"

using namespace std;

// Widok otwarcia programu
int cbv_intro(void* m, void* p) {
  output_clear();

  // Wymiary winiety
  const size_t wordmark_w = 68, wordmark_h = 13;

  // Wyrównanie pionowe
  for(int i = 0, max = (SCREEN_HEIGHT-wordmark_h)/3; i < max; i++)
    cout<<endl;

#if PROJECT_RULES_UNSAFE
  // Winieta w wersji "niebezpiecznej", BWR - zmiana barwy
  const char* wordmark[wordmark_h] = {
    "                                              W    .-'''-.           ",
    "                                         B.---.W   '   _    \\         ",
    "B      __.....__     W   _..._   R          B|   |W /   /` '.   \\        ",
    "B  .-''         '.   W .'     '. R  .--./)  B|   |W.   |     \\  '        ",
    "B /     .-''''-.  `. W.   .-.   .R /.''\\\\   B|   |W|   '      |  '  R.|   ",
    "B/     /________\\   \\W|  '   '  |R| |  | |  B|   |W\\    \\     / / R.' |_  ",
    "B|                  |W|  |   |  |R \\`-' /   B|   |W `.   ` ..' /R.'     | ",
    "B\\    .-------------'W|  |   |  |R /(''`    B|   |W    '-...-'`R'--.  .-' ",
    "B \\    '-.____...---.W|  |   |  |R \\ '---.  B|   |W            R   |  |   ",
    "B  `.             .' W|  |   |  |R  /''''.\\ B|   |W            R   |  |   ",
    "B    `''-...... -'   W|  |   |  |R ||     ||B'---'W            R   |  '.' ",
    "                    W|  |   |  |R \\'. __//                     |   /  ",
    "                    W'--'   '--' R `'---'                      `'-'   "
  };

  for(size_t i = 0; i < wordmark_h; i++) {
    cout<<setw((SCREEN_WIDTH-wordmark_w)/2)<<""; // Wyrównanie poziome

    auto pch = wordmark[i];
    // Iteracja po wszystkich znakach
    while(*pch) {
      switch(*pch) {
        // Zmiany koloru
        case 'B': cout<<color::blue; break;
        case 'W': cout<<color::white; break;
        case 'R': cout<<color::red; break;
        // Wyœwietlenie znaku
        default: cout.put(*pch);
      }
      pch++;
    }
    cout<<endl;
  }

#else
  // Winieta w wersji "bezpiecznej"
  const char* wordmark[wordmark_h] = {
    "                                                  .-'''-.           ",
    "                                         .---.   '   _    \\         ",
    "      __.....__        _..._             |   | /   /` '.   \\        ",
    "  .-''         '.    .'     '.   .--./)  |   |.   |     \\  '        ",
    " /     .-''''-.  `. .   .-.   . /.''\\\\   |   ||   '      |  '  .|   ",
    "/     /________\\   \\|  '   '  || |  | |  |   |\\    \\     / / .' |_  ",
    "|                  ||  |   |  | \\`-' /   |   | `.   ` ..' /.'     | ",
    "\\    .-------------'|  |   |  | /(''`    |   |    '-...-'`'--.  .-' ",
    " \\    '-.____...---.|  |   |  | \\ '---.  |   |               |  |   ",
    "  `.             .' |  |   |  |  /''''.\\ |   |               |  |   ",
    "    `''-...... -'   |  |   |  | ||     ||'---'               |  '.' ",
    "                    |  |   |  | \\'. __//                     |   /  ",
    "                    '--'   '--'  `'---'                      `'-'   "
  };

  // Wyœwietlenie wszystkich linii wyœrodkowanych
  for(size_t i = 0; i < wordmark_h; i++)
    output_center(wordmark[i]);
#endif

  // Informacje o programie
  cout<<color::white;
  output_center("Program wspieraj¹cy powtarzanie s³ówek w jêzykach obcych");
  cout<<color::gray;
  output_center("© 2016-2017 Mateusz Karcz, PG ETI");
  cout<<endl<<color::silver;
  output_center("Naciœnij klawisz ENTER, aby kontynuowaæ");

  // Przywrócenie koloru pocz¹tkowego
  cout<<color::silver;
  input_wait();
  return 0;
}
View<void> Englot::IntroView(cbv_intro);

// Widok zamkniêcia programu
int cbv_outro(void* m, void* p) {
  output_clear();
  cin.ignore();

  // Wyœwietlenie informacji o kompilacji i autorze
  output_center("Englot");
#if PROJECT_RULES_UNSAFE
  output_center("Kompilacja: " __TIMESTAMP__ " (PROJECT_RULES_UNSAFE)");
#else
  output_center("Kompilacja: " __TIMESTAMP__);
#endif
  output_center("© 2016-2017 Mateusz Karcz");
  output_center("Politechnika Gdañska");
  output_center("Wydzia³ Elektroniki, Telekomunikacji i Informatyki");
  output_center("Elektronika i telekomunikacja 2016/2017");

  input_wait();
  return 0;
}
View<void> Englot::OutroView(cbv_outro);
