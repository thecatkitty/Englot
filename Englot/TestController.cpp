#include "Englot.hpp"

using namespace std;
using namespace Englot;

// Kontroler modelu testu
int cbc_test(Test* e, void* p) {
  clog<<__FUNCTION__<<endl;

  auto v = (View<Test>*) p;
  if(v == &TestNumberView) // Wybór liczby hase³
    return v->render(e);

  if(v != &TestView)
    throw invalid_argument("Unknown View for Test model.");

  // Widok testu wiedzy
  e->result = v->render(e);

  return 0;
}
Controller<Test> Englot::TestController(cbc_test);
