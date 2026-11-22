#include <iostream>
#include <vector> // odpowiednik tradycyjnych tablic, który potrafi sam zmieniać swój rozmiar w trakcie działania programu
#include <memory> // inteligentne wskaźniki, make_unique
#include <limits> // kontrola błędów w konsoli
#include <iomanip> // setprecision przy wypisywaniu średnich
#include <numeric> // matematyka na kolekcjach danych
#include <algorithm>
#include <string>
#include <filesystem>
#include <stdexcept>
#include <cstdlib>
#include "FazaCyklu.hpp"
#include "FazaMenstruacyjna.hpp"
#include "FazaFolikularna.hpp"
#include "FazaOwulacyjna.hpp"
#include "FazaLutealna.hpp"
#include "WpisDziennika.hpp"
#include "Kalendarz.hpp"             // program sam liczy dzień cyklu z dat
#include "Cykl.hpp"                  
#include "HistoriaUzytkowniczki.hpp"
#include "ProfilAdaptacyjny.hpp"
#ifdef _WIN32
#include <windows.h> // polskie znaki i emoji w konsoli Windows
#endif
using namespace std;

// funkcja do czyszczenia bufora wejścia
void wyczyscBufor() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// pobieranie liczby z walidacją; przy końcu wejścia rzuca wyjątek, żeby program nie kręcił się w pętli
int pobierzInt(const string& komunikat, int min, int max) {
    while (true) {
        cout << komunikat;
        int x;
        if (cin >> x && x >= min && x <= max) {
            wyczyscBufor();
            return x;
        }
        if (cin.eof()) throw ios_base::failure("koniec wejścia");
        cout << "Podaj liczbę całkowitą z zakresu " << min << "-" << max << ".\n";
        wyczyscBufor();
    }
}

// pobieranie linii tekstu (pusta linia = wartość domyślna)
string pobierzLinie(const string& komunikat, const string& domyslna) {
    cout << komunikat;
    string linia;
    if (!getline(cin, linia)) throw ios_base::failure("koniec wejścia");
    return linia.empty() ? domyslna : linia;
}

// pobieranie daty (DD-MM-RRRR); pusta linia = wartość domyślna, błędny format = pytamy ponownie
Kalendarz pobierzDate(const string& komunikat, const Kalendarz& domyslna) {
    while (true) {
        string linia = pobierzLinie(komunikat, "");
        if (linia.empty()) return domyslna;
        try {
            return Kalendarz::zTekstu(linia);
        } catch (const invalid_argument& e) {
            cout << e.what() << "\n";
        }
    }
}

// objawy wybierane z listy
string wybierzObjawy() {
    cout << "Objawy: 1) brak  2) skurcze  3) krwawienie  4) inne (wpisz sama)\n";
    switch (pobierzInt("Wybór: ", 1, 4)) {
        case 1: return "brak";
        case 2: return "skurcze";
        case 3: return "krwawienie";
        default: return pobierzLinie("Opisz objawy: ", "brak");
    }
}

// Klasy pochodne dziedziczą po klasie bazowej FazaCyklu. Zwracamy je przez wskaźnik, dzięki czemu funkcja może dynamicznie zwrócić różne podklasy w zależności od dnia cyklu.
// unique_ptr pilnuje zasady wyłącznej własności - gdy obiekt fazy przestaje być potrzebny lub funkcja kończy działanie, pamięć jest automatycznie zwalniana przez system --> zapobiegamy wyciekom pamięci

// rekomendacja na dany dzień: faza (polimorfizm) + adaptacyjna korekta stresowa
void pokazRekomendacje(int dzien, const Kalendarz& data, const ProfilAdaptacyjny& profil, const WpisDziennika& wpis) {
    unique_ptr<FazaCyklu> faza = wyznaczFazeAdaptacyjnie(dzien, profil);

    cout << "\n===== " << data << " | dzień cyklu: " << dzien << (data < Kalendarz::dzisiaj() || data == Kalendarz::dzisiaj() ? "" : " (prognoza)") << " =====\n";
    cout << faza->getNazwaFazy() << "\n\n";
    cout << "Hormony:         " << faza->getOpisHormonalny() << "\n";
    cout << "Dieta:           " << faza->getRekomendowanaDieta() << "\n";
    cout << "Trening:         " << faza->getRekomendowanyTrening() << "\n";
    cout << "Produktywność:   " << faza->getProfilProduktywnosci() << "\n";
    cout << "Zioła:           " << faza->getZiola() << "\n";
    cout << "Regeneracja:     " << faza->getStylRegeneracji() << "\n";
    cout << "Wsparcie:        " << faza->getHolistyczneWsparcie() << "\n";

    OcenaStresu stres = profil.ocenStres(dzien);
    cout << fixed << setprecision(1);
    if (stres.podwyzszony) {
        cout << "\n--- Korekta adaptacyjna: wsparcie wyciszające ---\n";
        if (stres.liczbaProbek > 0) {
            cout << "W Twojej historii w okolicach tego dnia cyklu stres wynosi średnio " << stres.sredniStresHistoryczny
                 << "/5 (Twoja norma: " << profil.getSredniaStresu() << "/5, " << stres.liczbaProbek << " pomiarów).\n";
        }
        if (stres.maWpisDzisiaj) {
            cout << "Dzisiejszy wpis pokazuje stres na poziomie " << stres.stresDzisiaj << "/5.\n";
        }
    } else if (stres.liczbaProbek < 2) {
        cout << "\n(Za mało danych o stresie w tym okresie cyklu - model jeszcze się uczy.)\n";
    } else {
        cout << "\nStres w tym okresie cyklu mieści się w Twojej normie - bez dodatkowej korekty.\n";
    }
    cout << "\nWsparcie na dziś: " << faza->getWsparcieWyciszajace(wpis) << "\n";
    if (profil.getLiczbaCykliUczacych() == 0) {
        cout << "(Granice faz to na razie szacunek startowy. Po zakończeniu pierwszego cyklu model dopasuje się do Ciebie.)\n";
    }
    cout << "\nLunaria to inspiracja wellness, nie porada medyczna. W razie ciąży, chorób lub przyjmowania leków\n"
         << "skonsultuj zioła z lekarzem lub farmaceutą.\n";
}

void wypiszCykl(const Cykl& c) {
    cout << "\nCykl " << c.getNumerCyklu() << " (start: " << c.getDataStartu() << ") - ";
    if (c.czyZakonczony()) cout << "długość: " << c.getDlugoscTrwania() << " dni\n";
    else cout << "w trakcie\n";
    if (c.getWpisy().empty()) cout << "  (brak wpisów)\n";
    for (const auto& w : c.getWpisy()) cout << "  " << w << "\n";
}

//  wiersz statusu: program sam mówi, który jest dzień cyklu - użytkowniczka niczego nie liczy
void wypiszStatus(const HistoriaUzytkowniczki& h) {
    Kalendarz dzis = Kalendarz::dzisiaj();
    const Cykl& c = h.biezacyCykl();
    cout << "Dziś: " << dzis;
    if (c.czyMaDateStartu() && !(dzis < c.getDataStartu())) {
        cout << " | cykl " << c.getNumerCyklu() << ", dzień " << c.numerDniaDla(dzis);
    }
    cout << "\n";
}

//  dane demonstracyjne: trzy zakończone cykle o RÓŻNEJ długości (stres rośnie pod koniec cyklu) i czwarty, trwający.
// To tylko przykładowe dane do pokazu - model nie zna tych liczb, uczy się ich z wpisów i dat.
struct DemoCykl { int dlugosc; int koniecMenstr; int szczyt; };

void dodajDniDemo(HistoriaUzytkowniczki& h, int numerCyklu, int doDnia, const DemoCykl& dc) {
    for (int d = 1; d <= doDnia; d++) {
        int energia, stres;
        string objawy = "brak";
        if (d <= dc.koniecMenstr) {
            energia = 2; stres = 2;
            objawy = (d <= 2) ? "krwawienie" : "skurcze";
        } else if (abs(d - dc.szczyt) <= 1) {
            energia = 5; stres = 1;
        } else if (d < dc.szczyt) {
            energia = (d > dc.koniecMenstr + 3) ? 4 : 3; stres = 2;
        } else if (dc.dlugosc - d <= 4) {
            energia = 2; stres = (dc.dlugosc - d <= 2) ? 5 : 4;
        } else {
            energia = 3; stres = 3;
        }
        h.dodajWpis(WpisDziennika(numerCyklu, d, energia, stres, "dane demo", objawy, "brak"));
    }
}

void wczytajDaneDemo(HistoriaUzytkowniczki& h) {
    const vector<DemoCykl> demo = {{33, 5, 19}, {31, 5, 17}, {34, 6, 20}};
    const int DNI_TRWAJACEGO = 12; // czwarty cykl trwa: wpisy z dni 1-12, a dziś jest dzień 13
    int lacznie = 0;
    for (const auto& dc : demo) lacznie += dc.dlugosc;

    Kalendarz dzis = Kalendarz::dzisiaj();
    Kalendarz start = dzis.dodajDni(-(lacznie + DNI_TRWAJACEGO));
    h.wyczysc();
    h.ustawDateStartuBiezacego(start);
    for (const auto& dc : demo) {
        dodajDniDemo(h, h.biezacyCykl().getNumerCyklu(), dc.dlugosc, dc);
        start = start.dodajDni(dc.dlugosc);
        h.rozpocznijNowyCykl(start); // długość cyklu sama wynika z różnicy dat
    }
    dodajDniDemo(h, h.biezacyCykl().getNumerCyklu(), DNI_TRWAJACEGO, demo.back());
}

// przy pierwszym uruchomieniu pytamy o JEDNĄ datę - resztę program liczy sam
bool zapytajODateStartu(HistoriaUzytkowniczki& historia) {
    cout << "\nAby liczyć dzień cyklu za Ciebie, potrzebuję daty początku obecnego cyklu (pierwszy dzień krwawienia).\n";
    Kalendarz dzis = Kalendarz::dzisiaj();
    while (true) {
        try {
            Kalendarz data = pobierzDate("Data początku (DD-MM-RRRR, Enter = dziś): ", dzis);
            if (dzis < data) throw invalid_argument("Data początku nie może być z przyszłości");
            historia.ustawDateStartuBiezacego(data);
            return true;
        } catch (const ios_base::failure&) {
            return false;
        } catch (const exception& e) {
            cout << "Błąd: " << e.what() << "\n";
        }
    }
}

// R"BANNER(...)BANNER" to surowy napis - ukośniki nie trzeba podwajać)
void wypiszBaner() {
    cout << R"BANNER(
    __                           _       ___       __            __  _
   / /   __  ______  ____ ______(_)___ _/   | ____/ /___ _____  / /_(_)   _____
  / /   / / / / __ \/ __ `/ ___/ / __ `/ /| |/ __  / __ `/ __ \/ __/ / | / / _ \
 / /___/ /_/ / / / / /_/ / /  / / /_/ / ___ / /_/ / /_/ / /_/ / /_/ /| |/ /  __/
/_____/\__,_/_/ /_/\__,_/_/  /_/\__,_/_/  |_\__,_/\__,_/ .___/\__/_/ |___/\___/
                                                      /_/
)BANNER";
    cout << "      ~  Twój cykl. Twój rytm. Bez gotowych szablonów.  ~\n\n";
}

void wypiszMenu() {
    cout << "\n===== LUNARIA ADAPTIVE =====\n"
         << "1. Dodaj wpis do dziennika\n"
         << "2. Rekomendacja na dziś (lub inną datę)\n"
         << "3. Czego nauczył się model (Twój profil)\n"
         << "4. Pokaż historię wpisów\n"
         << "5. Znajdź / edytuj wpis\n"
         << "6. Usuń wpis\n"
         << "7. Dziś zaczął się nowy cykl\n"
         << "8. Popraw datę początku bieżącego cyklu\n"
         << "9. Wczytaj dane demonstracyjne\n"
         << "0. Zakończ program\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    wypiszBaner();
    const string PLIK = "lunaria_dane.txt";
    HistoriaUzytkowniczki historia;

    // wczytanie zapisu; uszkodzony plik nie zostanie nadpisany - przenosimy go na bok
    try {
        historia.wczytaj(PLIK);
        cout << "Wczytano zapis z pliku " << PLIK << ".\n";
    } catch (const runtime_error& e) {
        cout << e.what() << "\n";
        if (filesystem::exists(PLIK)) {
            filesystem::rename(PLIK, PLIK + ".uszkodzony");
            cout << "Uszkodzony plik zachowano jako " << PLIK << ".uszkodzony. Zaczynamy od pustej historii.\n";
        } else {
            cout << "Zaczynamy od pustej historii.\n";
        }
    }

    if (!historia.biezacyCykl().czyMaDateStartu()) {
        if (!zapytajODateStartu(historia)) return 0;
        historia.zapisz(PLIK);
    }

    bool dziala = true;
    while (dziala) {
        cout << "\n";
        wypiszStatus(historia);
        wypiszMenu();
        try {
            int wybor = pobierzInt("Wybór: ", 0, 9);
            bool zmieniono = false;
            switch (wybor) {
                case 1: {
                    Kalendarz dzis = Kalendarz::dzisiaj();
                    Kalendarz data = pobierzDate("Data wpisu (Enter = dziś): ", dzis);
                    if (dzis < data) throw invalid_argument("Wpis nie może dotyczyć przyszłości");
                    const Cykl& cykl = historia.znajdzCyklDlaDaty(data); // rzuca out_of_range, gdy data poza historią
                    int numerCyklu = cykl.getNumerCyklu();
                    int dzien = cykl.numerDniaDla(data);
                    cout << "Cykl " << numerCyklu << ", dzień cyklu: " << dzien << "\n";

                    bool nadpisz = false;
                    if (const WpisDziennika* istniejacy = cykl.znajdzWpis(dzien)) {
                        cout << "Ten dzień ma już wpis: " << *istniejacy << "\n";
                        if (pobierzInt("Nadpisać go? (1 = tak, 0 = nie): ", 0, 1) == 0) break;
                        nadpisz = true;
                    }
                    int energia = pobierzInt("Energia (1-5): ", WpisDziennika::SKALA_MIN, WpisDziennika::SKALA_MAX);
                    int stres = pobierzInt("Stres (1-5): ", WpisDziennika::SKALA_MIN, WpisDziennika::SKALA_MAX);
                    string objawy = wybierzObjawy();
                    string nastroj = pobierzLinie("Nastrój (Enter = brak): ", "brak");
                    string notatka = pobierzLinie("Notatka (Enter = brak): ", "brak");
                    if (nadpisz) {
                        WpisDziennika& w = historia.znajdzWpis(numerCyklu, dzien);
                        w.setPoziomEnergii(energia);
                        w.setPoziomStresu(stres);
                        w.setObjawy(objawy);
                        w.setNastroj(nastroj);
                        w.setNotatka(notatka);
                    } else {
                        historia.dodajWpis(WpisDziennika(numerCyklu, dzien, energia, stres, nastroj, objawy, notatka));
                    }
                    cout << "Zapisano wpis.\n";
                    zmieniono = true;
                    break;
                }
                case 2: {
                    Kalendarz dzis = Kalendarz::dzisiaj();
                    Kalendarz data = pobierzDate("Data (Enter = dziś): ", dzis);
                    const Cykl* cyklPtr = nullptr;
                    try {
                        cyklPtr = &historia.znajdzCyklDlaDaty(data);
                    } catch (const out_of_range&) {
                        const Cykl& biezacy = historia.biezacyCykl();
                        if (biezacy.czyMaDateStartu() && !(data < biezacy.getDataStartu())) {
                            cyklPtr = &biezacy;
                        } else {
                            throw invalid_argument("Ta data jest przed początkiem bieżącego cyklu i nie występuje w historii");
                        }
                    }

                    const Cykl& cykl = *cyklPtr;
                    int dzien = cykl.numerDniaDla(data);

                    // wpis z tego dnia, a jeśli go nie ma, neutralny wpis zastępczy
                    const WpisDziennika* istniejacy = cykl.znajdzWpis(dzien);
                    WpisDziennika wpis = istniejacy
                        ? *istniejacy
                        : WpisDziennika(cykl.getNumerCyklu(), dzien, 3, 3, "brak", "brak", "brak");

                    ProfilAdaptacyjny profil(historia);
                    pokazRekomendacje(dzien, data, profil, wpis);
                    break;
                }
                case 3: {
                    ProfilAdaptacyjny profil(historia);
                    cout << "\n" << profil.opisModelu();
                    break;
                }
                case 4:
                    for (const auto& c : historia.getCykle()) wypiszCykl(c);
                    break;
                case 5: {
                    Kalendarz data = pobierzDate("Data wpisu (Enter = dziś): ", Kalendarz::dzisiaj());
                    const Cykl& cykl = historia.znajdzCyklDlaDaty(data);
                    WpisDziennika& w = historia.znajdzWpis(cykl.getNumerCyklu(), cykl.numerDniaDla(data)); // rzuca out_of_range, gdy brak wpisu
                    cout << "Znaleziono: " << w << "\n";
                    if (pobierzInt("Edytować? (1 = tak, 0 = nie): ", 0, 1) == 1) {
                        int e = pobierzInt("Nowa energia (0 = bez zmian): ", 0, WpisDziennika::SKALA_MAX);
                        if (e != 0) w.setPoziomEnergii(e);
                        int st = pobierzInt("Nowy stres (0 = bez zmian): ", 0, WpisDziennika::SKALA_MAX);
                        if (st != 0) w.setPoziomStresu(st);
                        if (pobierzInt("Zmienić objawy? (1 = tak, 0 = nie): ", 0, 1) == 1) w.setObjawy(wybierzObjawy());
                        w.setNastroj(pobierzLinie("Nowy nastrój (Enter = bez zmian): ", w.getNastroj()));
                        w.setNotatka(pobierzLinie("Nowa notatka (Enter = bez zmian): ", w.getNotatka()));
                        cout << "Zapisano zmiany: " << w << "\n";
                        zmieniono = true;
                    }
                    break;
                }
                case 6: {
                    Kalendarz data = pobierzDate("Data wpisu do usunięcia (Enter = dziś): ", Kalendarz::dzisiaj());
                    const Cykl& cykl = historia.znajdzCyklDlaDaty(data);
                    historia.usunWpis(cykl.getNumerCyklu(), cykl.numerDniaDla(data));
                    cout << "Usunięto wpis.\n";
                    zmieniono = true;
                    break;
                }
                case 7: {
                    Kalendarz dzis = Kalendarz::dzisiaj();
                    Kalendarz data = pobierzDate("Data początku nowego cyklu (Enter = dziś): ", dzis);
                    if (dzis < data) throw invalid_argument("Data początku nie może być z przyszłości");
                    historia.rozpocznijNowyCykl(data); // długość poprzedniego cyklu liczy się z różnicy dat
                    const Cykl& poprzedni = historia.getCykle()[historia.getCykle().size() - 2];
                    cout << "Cykl " << poprzedni.getNumerCyklu() << " trwał " << poprzedni.getDlugoscTrwania()
                         << " dni. Model uczy się teraz z " << historia.liczbaZakonczonychCykli()
                         << " zakończonych cykli. Rozpoczęto cykl " << historia.biezacyCykl().getNumerCyklu() << ".\n";
                    zmieniono = true;
                    break;
                }
                case 8: {
                    Kalendarz dzis = Kalendarz::dzisiaj();
                    Kalendarz data = pobierzDate("Poprawna data początku bieżącego cyklu (DD-MM-RRRR): ", dzis);
                    if (dzis < data) throw invalid_argument("Data początku nie może być z przyszłości");
                    historia.ustawDateStartuBiezacego(data);
                    cout << "Zmieniono datę początku bieżącego cyklu na " << data << ".\n";
                    zmieniono = true;
                    break;
                }
                case 9:
                    if (pobierzInt("To zastąpi całą obecną historię danymi demo. Kontynuować? (1 = tak, 0 = nie): ", 0, 1) == 1) {
                        wczytajDaneDemo(historia);
                        cout << "Wczytano 3 zakończone cykle o różnej długości i czwarty, trwający. Zobacz opcje 3 i 2.\n";
                        zmieniono = true;
                    }
                    break;
                case 0:
                    dziala = false;
                    break;
            }
            if (zmieniono) historia.zapisz(PLIK);
        } catch (const ios_base::failure&) {
            cout << "\nKoniec wejścia - zamykam program.\n";
            dziala = false;
        } catch (const exception& e) {
            cout << "Błąd: " << e.what() << "\n";
        }
    }

    try {
        historia.zapisz(PLIK);
        cout << "Zapisano dane. Do zobaczenia!\n";
    } catch (const exception& e) {
        cout << "Nie udało się zapisać danych: " << e.what() << "\n";
    }
    return 0;
}