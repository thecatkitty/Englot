#pragma once

// Szablon obiektu z wywo³aniem zwrotnym
template<class T> class Callbacked {
  public:
    typedef int(*callback)(T*,void*);

    Callbacked();
    Callbacked(callback pfn);
    ~Callbacked();

  protected:
    callback cbf;
};

// Szablon widoku
template<class T> class View : public Callbacked<T> {
  public:
    using Callbacked<T>::Callbacked;
    int render(T* model);
};

// Szablon kontrolera
template<class T> class Controller : public Callbacked<T> {
  public:
    using Callbacked<T>::Callbacked;
    int run(T* model, View<T>* view);
};

// Implementacja metod szablonów
template<class T> inline Callbacked<T>::Callbacked() {
  cbf = nullptr;
}

template<class T> inline Callbacked<T>::Callbacked(callback pfn) {
  cbf = pfn;
}

template<class T> inline Callbacked<T>::~Callbacked() {
  cbf = nullptr;
}

template<class T> inline int View<T>::render(T* model) {
  if(!cbf)
    return false;

  clog<<"View render: ";
  system("cls");
  return cbf(model, nullptr);
}

template<class T> inline int Controller<T>::run(T* model, View<T>* view) {
  if(!cbf)
    return false;

  clog<<"Controller run: ";
  return cbf(model, view);
}
