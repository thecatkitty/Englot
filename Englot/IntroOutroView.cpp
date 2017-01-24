#include <iomanip>

#include "Helpers.hpp"

using namespace std;

// Widok otwarcia programu
int cbv_intro(void* m, void* p) {
  output_clear();

  // Wymiary winiety
  const size_t wordmark_w = 68, wordmark_h = 13;

  // Wyr�wnanie pionowe
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
    cout<<setw((SCREEN_WIDTH-wordmark_w)/2)<<""; // Wyr�wnanie poziome

    auto pch = wordmark[i];
    // Iteracja po wszystkich znakach
    while(*pch) {
      switch(*pch) {
        // Zmiany koloru
        case 'B': cout<<color::blue; break;
        case 'W': cout<<color::white; break;
        case 'R': cout<<color::red; break;
        // Wy�wietlenie znaku
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

  // Wy�wietlenie wszystkich linii wy�rodkowanych
  for(size_t i = 0; i < wordmark_h; i++)
    output_center(wordmark[i]);
#endif

  // Informacje o programie
  cout<<color::white;
  output_center("Program wspieraj�cy powtarzanie s��wek w j�zykach obcych");
  cout<<color::gray;
  output_center("� 2016-2017 Mateusz Karcz, PG ETI");
  cout<<endl<<color::silver;
  output_center("Naci�nij klawisz ENTER, aby kontynuowa�");

  // Przywr�cenie koloru pocz�tkowego
  cout<<color::silver;
  input_wait();
  return 0;
}
View<void> Englot::IntroView(cbv_intro);

// Widok zamkni�cia programu
int cbv_outro(void* m, void* p) {
  output_clear();
  cin.ignore();

  // Wy�wietlenie informacji o kompilacji i autorze
  output_center("Englot");
#if PROJECT_RULES_UNSAFE
  output_center("Kompilacja: " __TIMESTAMP__ " (PROJECT_RULES_UNSAFE)");
#else
  output_center("Kompilacja: " __TIMESTAMP__);
#endif
  output_center("� 2016-2017 Mateusz Karcz");
  output_center("Politechnika Gda�ska");
  output_center("Wydzia� Elektroniki, Telekomunikacji i Informatyki");
  output_center("Elektronika i telekomunikacja 2016/2017");

  input_wait();
  return 0;
}
View<void> Englot::OutroView(cbv_outro);
