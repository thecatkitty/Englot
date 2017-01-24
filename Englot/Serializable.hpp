#pragma once

#include <iostream>

// Klasa bazowa dla obiekt�w serializowalnych
class Serializable {
  public:
    // Metody serializacji
    virtual bool serialize() = 0;
    virtual bool deserialize() = 0;

    // Metody przypisania strumienia
    void attach(std::iostream& st);
    void detach();

  protected:
    std::iostream* stream;
};
