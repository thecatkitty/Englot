#include "Serializable.hpp"

using namespace std;

// Przypisanie strumienia serializacji
void Serializable::attach(iostream& st) {
  stream = &st;
}

// Zwolnienie strumienia serializacji
void Serializable::detach() {
  stream = nullptr;
}