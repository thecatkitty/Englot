Englot
======

Wrzutka mojego projektu na przedmiot Podstawy programowania ze stycznia 2017 roku.

Projekt ten jest z czasów, kiedy nie do końca ogarniałem zarządzanie pamięcią w C++, toteż występują w nim błędy, nas szczęście ujawniające się wyłącznie przy zamykaniu programu. Projekt ten został umieszczony na GitHubie wyłącznie w celach archiwalnych.

Oryginalny tekst pomocy z 2017 roku
-----------------------------------

Pomoc dla programu wspierającego powtarzanie wyrażeń w językach obcych Englot
© 2016-2017 Mateusz Karcz, PG ETI

### Menu główne
W dolnej części ekranu znajduje się pole *Twój wybór*. Wpisz cyfrę przypisaną danemu działaniu i naciśnij klawisz ENTER w celu jego wykonania.

#### Test
Jeśli wcześniej wybrano słownik, to po wpisaniu cyfry 1 i naciśnięciu klawisza ENTER wyświetli się okno *TEST WIEDZY*. Wpisz ilość wyrażeń, które chcesz powtórzyć (minimum 5, po w pisaniu wartości większej niż liczba wyrażeń w słowniku program przepyta z całego słownika). Po wpisaniu wartości i naciśnięciu klawisza ENTER pojawi się komunikat *Wpis z polskie tłumaczenie wyrażenia*, a poniżej losowo wybrane wyrażenie. Wpisz jego tłumaczenie w pole *Twoja odpowiedź* i naciśnij klawisz ENTER. Jeśli Twoja odpowiedź okaże się błędna, program poda poniżej poprawne tłumaczenie. Nie ma możliwości przerwania trwającego testu.
Po zakończeniu testu tytuł okna zmieni się na *KONIEC TESTU*, a poniżej pojawi się Twój wynik. Po naciśnięciu klawisza ENTER program wraca do głównego menu.

#### Moje statystyki
Przy załadowanym słowniku po wpisaniu cyfry 2 (lub cyfry 1 bez słownika) i zatwierdzeniu jej klawiszem ENTER wyświetli się okno *STATYSTYKI UŻYTKOWNIKA*. Poniżej tytułu, po lewej stronie zostanie wyświetlony login, a po prawej pełna nazwa zalogowanego użytkownika.
Poniżej tych danych zostanie wyświetlona tabela zawierająca cztery kolumny — datę i godzinę, nazwę słownika, wynik i wartość procentową.
Pod tabelą wyświetla się liczba rozwiązanych testów oraz łączny wynik i procent obliczony z wszystkich ukończonych testów. Po naciśnięciu klawisza ENTER program wraca do głównego menu.

#### Wyniki według testów
Przy załadowanym słowniku wybranie opcji 3 (lub odpowiednio 2 bez słownika) skutkuje wyświetleniem okna *RANKING TESTÓW*.
W oknie tym znajduje się tabela zawierająca pięć kolumn — datę i godzinę, pełną nazwę użytkownika, nazwę słownika, wynik i wartość procentowę. Program wyświetla wszystkie wyniki wszystkich użytkowników posortowane według ilości zdobytych punktów. Po naciśnięciu klawisza ENTER program wraca do głównego menu.

#### Wyniki według użytkowników
Po wybraniu opcji 4 (lub 3 bez wybranego słownika) program wyświetli okno *RANKING UŻYTKOWNIKÓW*. Znajdzie się w nim tabela zawierająca trzy kolumny — nazwę użytkownika, liczbę wszystkich jego testów i zdobytych przez niego punktów, które stanowią również kryterium sortowania tej tabeli.
Po naciśnięciu klawisza ENTER program wraca do głównego menu.

#### Wybór słownika
Jeśli nie wybrano wcześniej słownika, to po wpisaniu cyfry 4 i naciśnięciu klawisza ENTER wyświetli się okno *WYBÓR SŁOWNIKA* z komunikatem *Wpisz nazwę pliku słownika*. W przypadku, gdy słownik został już wcześniej wczytany, to cyfra 5 jest przypisana tej akcji.
Wpisz nazwę pliku słownika i naciśnij klawisz ENTER. Zostanie wyświetlony komunikat *Otwarto słownik <nazwa słownika>*. Po naciśnięciu klawisza ENTER program wraca do głównego menu. Przy nazwie czynności numer 5 zostanie wyświetlona nazwa wybranego słownika.

#### Pomoc
Po wpisaniu cyfry 6 (lub 5, gdy nie załadowano słownika) i naciśnięciu klawisza ENTER wyświetli się obecne okno *POMOC*. Naciśnięcie klawisza ENTER poskutkuje przewinięciem tekstu o jedną linijkę. Po odczytaniu całości dokumentacji i naciśnięcie klawisza ENTER program wraca do głównego menu.

#### Wyloguj
Po wpisaniu cyfry 9 i naciśnięciu klawisza ENTER wyświetli się okno *LOGOWANIE I REJESTRACJA*. W zależności od żądanej czynności proszę wpisać L (zaloguj) lub N (utwórz nowe konto). Po wpisaniu odpowiedniej litery i naciśnięciu klawisza ENTER zostanie wyświetlony formularz logowania lub rejestracji. Proszę go wypełnić zgodnie z upodobaniem, po czym nacisnąć klawisz ENTER skutkujący wyświetleniem okna *MENU GŁÓWNE*.

#### Zakończ
Po wpisaniu cyfry 0 i naciśnięciu klawisza ENTER wyświetlą się dane twórcy programu. Po przeczytaniu i naciśnięciu klawisza ENTER program zostanie zakończony.


### Pliki danych programu
Program wspierający powtarzanie wyrażeń w językach obcych Englot działa na danych zawartych w plikach. Poniżej znajduje się ich krótki opis.

#### Pliki słownika `*.CSV` (domyślnie)
Plikami słownika nazywa się każdy plik tekstowy systemu Windows, w którym w kolejnych liniach występują: jako pierwsze wyrażenie w języku obcym, po średniku jego polskie tłumaczenie. Każde kolejne hasło słownika zaczyna się od nowej linii.
Jeśli pierwsza linia pliku rozpoczyna się sekwencją `#;`, to tekst po średniku stanowi tytuł słownika.

#### Pliki indeksu `*.IND`
Plik indeksu jest tworzony dla każdego pliku słownika. Jest on plikiem binarnym zawierającym położenia kolejnych haseł w pliku słownika.

#### Plik zapisu `ENGLOT.DAT`
W pliku Englot.dat przy zamykaniu programu zapisywane są wszystkie dane o użytkownikach, testach i wynikach. Są one wczytywane przy kolejnym uruchomieniu programu.

#### Plik dziennika `ENGLOT.LOG`
W pliku tekstowym Englot.log zapisywane są techniczne komunikaty opisujące przebieg działania danej sesji programu.

© dokumentacja: Marta Kaczor