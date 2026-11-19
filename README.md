# LunariaAdaptive
Konsolowy, adaptacyjny system analizy cyklu biologicznego i rekomendacji w C++  
## O projekcie  
Wiele popularnych narzędzi ślepo zakłada uniwersalny, sztywny model cyklu, sprowadzając biologię do prostej matematyki kalendarzowej i całkowicie ignorując unikalność fizjologii każdego człowieka.
`LunariaAdaptive` wprowadza **podejście adaptacyjne oparte na obiektowej analizie historii użytkownika oraz modelowaniu profilu hormonalnego.**  
* **Adaptacyjność:** System oblicza parametry na podstawie realnie wprowadzanych danych. Im więcej cykli użytkownik wprowadzi do bazy, tym precyzyjniej program estymuje długość nadchodzących faz.  
* **Polimorficzny system rekomendacji:** Każda faza biologiczna oparta na profilu hormonalnym jest osobną klasą. Dzięki temu program w sposób dynamiczny generuje dedykowane wskazówki.
* **Automatyczne liczenie dnia cyklu.** Podajesz tylko datę początku cyklu,
  resztę program liczy sam.

## Funkcje
- dziennik: energia, stres, objawy, nastrój i notatka dla każdego dnia
- dodawanie, wyszukiwanie, edycja i usuwanie wpisów
- rekomendacje na dziś lub dowolną datę (także prognoza)
- podgląd tego, czego nauczył się model
- tryb demo z przykładowymi danymi, żeby szybko wypróbować program
- automatyczny zapis i odczyt danych z pliku, z ochroną przed uszkodzonym zapisem

## Kompilacja i uruchomienie

Wymagania: kompilator obsługujący **C++17** (np. `g++` / MinGW-w64 / MSYS2, Clang lub MSVC).

Pobierz repozytorium i przejdź do folderu projektu:

```bash
git clone [https://github.com/mag-sud/LunariaAdaptive.git](https://github.com/mag-sud/LunariaAdaptive.git)
cd LunariaAdaptive
```

### Windows (PowerShell / MSYS2 / MinGW)

Kompilacja:

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Isrc src/*.cpp -o lunaria.exe
```

Uruchomienie:

```powershell
.\lunaria.exe
```

### Linux / macOS

Kompilacja:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Isrc src/*.cpp -o lunaria
```

Uruchomienie:

```bash
./lunaria
```

- Przy pierwszym uruchomieniu (gdy nie ma jeszcze pliku `lunaria_dane.txt`) program poprosi o podanie daty początku bieżącego cyklu w formacie **`DD-MM-RRRR`**.
- W menu głównym możesz wybrać opcję **`9`**, aby wczytać dane demonstracyjne i od razu przetestować działanie modelu adaptacyjnego, wpisów z dziennika oraz rekomendacji.
  - Twoje dane zapisują się lokalnie w pliku `lunaria_dane.txt`. Aplikacja nie łączy się z Internetem i niczego nie wysyła.

> Na Windowsie program korzysta z kodowania UTF-8 w konsoli (polskie znaki i emoji), dlatego najlepiej uruchamiać go w **Windows Terminal** lub **PowerShellu**.

## Struktura projektu

```
src/
├── main.cpp                   interfejs konsolowy i menu
├── FazaCyklu.hpp              abstrakcyjna klasa bazowa faz
├── FazaMenstruacyjna.*        ┐
├── FazaFolikularna.*          │ klasy pochodne (polimorfizm)
├── FazaOwulacyjna.*           │
├── FazaLutealna.*             ┘
├── ProfilAdaptacyjny.*        model uczący się z historii
├── HistoriaUzytkowniczki.*    wszystkie cykle, zapis i odczyt
├── Cykl.*                     pojedynczy cykl i jego wpisy
├── WpisDziennika.*            jeden dzień w dzienniku
└── Kalendarz.*                daty i liczenie dnia cyklu
```
