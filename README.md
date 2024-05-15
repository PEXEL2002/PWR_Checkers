# Gra w Szachy z AI

## Opis projektu
Projekt to gra w szachy stworzona w języku C++ z wykorzystaniem biblioteki Qt do obsługi graficznego interfejsu użytkownika. Gra oferuje możliwość rywalizacji z zaawansowanym algorytmem sztucznej inteligencji lub grę dla dwóch graczy na jednym komputerze.

## Technologie
- **Język programowania:** C++
- **Biblioteka GUI:** Qt
- **Algorytm AI:** MinMax z cięciami alfa-beta

## Funkcjonalności
1. **Tryb gry z komputerem:** Możliwość wyboru poziomu trudności AI.
2. **Tryb gry dwuosobowej:** Gra umożliwia rozgrywkę dla dwóch graczy na jednym urządzeniu.
3. **Historia ruchów:** Wszystkie ruchy w partii są wyświetlane w notacji szachowej.

## Uruchomienie
Instrukcje dotyczące kompilacji i uruchomienia projektu:
```bash
# Kompilacja
g++ -o chess main.cpp -lstdc++ -lQt5Core -lQt5Gui -lQt5Widgets
# Uruchomienie
./chess
