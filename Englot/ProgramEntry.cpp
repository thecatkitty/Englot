#include <iostream>
#include <iomanip>

#include "Englot.hpp"
#include "Helpers.hpp"

using namespace std;
using namespace Englot;

int main() {
  // Inicjalizacja programu, logowania i okna konsoli
  int exit_status = EXIT_SUCCESS;

  auto startt = initialize();
  auto log = log_start(startt);
  fstream df;
  EnglotModel e;

  IntroView.render(nullptr);

  // Wczytanie stanu programu z pliku
  try {
    df.open("Englot.dat", ios::binary | ios::in);
    e.attach(df);
    e.deserialize();
    e.detach();
    df.close();
  } catch(const exception& x) {
    clog<<"Cannot deserialize: "<<x.what()<<endl;
  }

  try {
    // G³ówna pêtla programu - kontroler Englot, widok g³ówny
    while(EnglotController.run(&e, &EnglotView) != -1);

    // Zapis stanu programu do pliku
    df.open("Englot.dat", ios::binary | ios::out);
    e.attach(df);
    e.serialize();
    e.detach();
    df.close();

  // Obs³uga nieprzechwyconych wczeœniej wyj¹tków
  } catch(const exception& x) {
    cerr<<x.what()<<endl;
    time_t abortt = time(NULL);

    clog<<"Exception: "<<x.what()<<endl;
    exit_status = EXIT_FAILURE;
  } catch(...) {
    clog<<"Unknown exception";
    exit_status = EXIT_FAILURE;
  }

  OutroView.render(nullptr);

  // Zakoñczenie dzia³ania programu
  if(exit_status == EXIT_SUCCESS)
    clog<<"Program stop: ";
  else
    clog<<"Program aborted: ";

  e.~EnglotModel();
  log_stop(log, startt);
  return exit_status;
}
