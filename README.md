# Gra w Warcaby z AI

## Opis projektu
Projekt to gra w warcaby stworzona w języku C++ z wykorzystaniem biblioteki Windows API do obsługi graficznego interfejsu użytkownika. Gra oferuje możliwość rywalizacji z zaawansowanym algorytmem sztucznej inteligencji lub grę dla dwóch graczy na jednym komputerze.

## Technologie
- **Język programowania:** C++
- **Biblioteka GUI:** Windows API
- **Algorytm AI:** MinMax z cięciami alfa-beta

## Funkcjonalności
1. **Tryb gry z komputerem:** Możliwość wyboru poziomu trudności AI.
2. **Tryb gry dwuosobowej:** Gra umożliwia rozgrywkę dla dwóch graczy na jednym urządzeniu.
3. **Historia ruchów:** Wszystkie ruchy w partii są wyświetlane w notacji warcabowej.

## Uruchomienie
Instrukcje dotyczące kompilacji i uruchomienia projektu:
```bash
# Kompilacja
g++ -o checkers main.cpp -lstdc++ -lgdi32 -luser32
# Uruchomienie
./checkers
