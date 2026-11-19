#include "HistoriaUzytkowniczki.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    // zamieniamy tabulatory i znaki nowej linii na spację, żeby nie psuły formatu pliku
    std::string oczyscTekst(std::string tekst) {
        for (char& c : tekst) {
            if (c == '\t' || c == '\n' || c == '\r') {
                c = ' ';
            }
        }
        return tekst;
    }

    std::vector<std::string> podzielPoTabulatorze(const std::string& linia) {
        std::vector<std::string> pola;
        std::size_t start = 0;
        std::size_t pozycja = linia.find('\t', start);

        while (pozycja != std::string::npos) { // npos = no position = nie znaleziono
            pola.push_back(linia.substr(start, pozycja - start));
            start = pozycja + 1;
            pozycja = linia.find('\t', start);
        }
        pola.push_back(linia.substr(start));
        return pola;
    }
}

HistoriaUzytkowniczki::HistoriaUzytkowniczki() {
    cykle.emplace_back(1); // tworzy nowy obiekt wew. wektora, przekazując argumenty od razu do kontstruktora tego obiektu
}
const std::vector<Cykl>& HistoriaUzytkowniczki::getCykle() const {
    return cykle;
}
const Cykl& HistoriaUzytkowniczki::biezacyCykl() const {
    return cykle.back();
}
const Cykl& HistoriaUzytkowniczki::znajdzCykl(int numer) const {
    for (const auto& c : cykle) {
        if (c.getNumerCyklu() == numer) {
            return c;
        }
    }
    throw std::out_of_range("Nie ma cyklu o numerze " + std::to_string(numer));
}
const Cykl& HistoriaUzytkowniczki::znajdzCyklDlaDaty(const Kalendarz& data) const {
    for (const auto& c : cykle) {
        if (c.zawieraDate(data)) {
            return c;
        }
    }
    throw std::out_of_range("Żaden cykl w historii nie obejmuje daty " + data.doTekstu());
}

int HistoriaUzytkowniczki::liczbaZakonczonychCykli() const {
    int licznik = 0;
    for (const auto& c : cykle) {
        if (c.czyZakonczony()) {
            licznik++;
        }
    }
    return licznik;
}

void HistoriaUzytkowniczki::dodajWpis(const WpisDziennika& wpis) {
    for (auto& c : cykle) {
        if (c.getNumerCyklu() == wpis.getNumerCyklu()) {
            c.dodajWpis(wpis);
            return;
        }
    }
    throw std::out_of_range("Nie ma cyklu o numerze " + std::to_string(wpis.getNumerCyklu()));
}

WpisDziennika& HistoriaUzytkowniczki::znajdzWpis(int cykl, int dzien) {
    for (auto& c : cykle) {
        if (c.getNumerCyklu() == cykl) {
            WpisDziennika* wpis = c.znajdzWpis(dzien);
            if (wpis != nullptr) {
                return *wpis;
            }
            throw std::out_of_range(
                "Brak wpisu dla dnia " + std::to_string(dzien) + " w cyklu " + std::to_string(cykl)
            );
        }
    }
    throw std::out_of_range("Nie ma cyklu o numerze " + std::to_string(cykl));
}

void HistoriaUzytkowniczki::usunWpis(int cykl, int dzien) {
    for (auto& c : cykle) {
        if (c.getNumerCyklu() == cykl) {
            if (!c.usunWpis(dzien)) {
                throw std::out_of_range(
                    "Brak wpisu dla dnia " + std::to_string(dzien) + " w cyklu " + std::to_string(cykl)
                );
            }
            return;
        }
    }
    throw std::out_of_range("Nie ma cyklu o numerze " + std::to_string(cykl));
}

void HistoriaUzytkowniczki::ustawDateStartuBiezacego(const Kalendarz& data) {
    Cykl& biezacy = cykle.back();
    biezacy.ustawDateStartu(data);

    if (cykle.size() >= 2) {
        Cykl& poprzedni = cykle[cykle.size() - 2];
        if (poprzedni.czyMaDateStartu()) {
            poprzedni.zakoncz(data - poprzedni.getDataStartu());
        }
    }
}

void HistoriaUzytkowniczki::rozpocznijNowyCykl(const Kalendarz& dataStartu) {
    Cykl& biezacy = cykle.back();
    if (!biezacy.czyMaDateStartu()) {
        throw std::invalid_argument("Najpierw ustaw datę początku bieżącego cyklu");
    }
    biezacy.zakoncz(dataStartu - biezacy.getDataStartu());
    int nastepnyNumer = biezacy.getNumerCyklu() + 1;
    cykle.emplace_back(nastepnyNumer, dataStartu);
}

void HistoriaUzytkowniczki::wyczysc() {
    cykle.clear();
    cykle.emplace_back(1);
}

void HistoriaUzytkowniczki::zapisz(const std::string& sciezka) const {
    std::ofstream plik(sciezka);
    if (!plik) {
        throw std::runtime_error("Nie można otworzyć pliku do zapisu: " + sciezka);
    }

    for (const auto& c : cykle) {
        std::string startTekst = c.czyMaDateStartu() ? c.getDataStartu().doTekstu() : "-";
        plik << "C\t" << c.getNumerCyklu() << '\t' << c.getDlugoscTrwania() << '\t' << startTekst << '\n';

        for (const auto& w : c.getWpisy()) {
            plik << "W\t" << w.getNumerCyklu() << '\t' << w.getNumerDniaCyklu() << '\t'
                 << w.getPoziomEnergii() << '\t' << w.getPoziomStresu() << '\t'
                 << oczyscTekst(w.getNastroj()) << '\t'
                 << oczyscTekst(w.getObjawy()) << '\t'
                 << oczyscTekst(w.getNotatka()) << '\n';
        }
    }

    if (!plik) {
        throw std::runtime_error("Błąd podczas zapisu pliku: " + sciezka);
    }
}

void HistoriaUzytkowniczki::wczytaj(const std::string& sciezka) {
    std::ifstream plik(sciezka);
    if (!plik) {
        throw std::runtime_error("Nie znaleziono pliku: " + sciezka);
    }

    std::vector<Cykl> wczytaneCykle;
    std::string linia;
    int numerLinii = 0;

    while (std::getline(plik, linia)) {
        numerLinii++;
        if (!linia.empty() && linia.back() == '\r') {
            linia.pop_back();
        }
        if (linia.empty()) {
            continue;
        }

        std::vector<std::string> pola = podzielPoTabulatorze(linia);

        try {
            if (pola[0] == "C" && (pola.size() == 3 || pola.size() == 4)) {
                Kalendarz start;
                if (pola.size() == 4 && pola[3] != "-") {
                    start = Kalendarz::zTekstu(pola[3]);
                }
                Cykl cykl(std::stoi(pola[1]), start);
                int dlugosc = std::stoi(pola[2]);
                if (dlugosc > 0) {
                    cykl.zakoncz(dlugosc);
                }
                wczytaneCykle.push_back(cykl);
            } else if (pola[0] == "W" && pola.size() == 8 && !wczytaneCykle.empty()) {
                WpisDziennika wpis(
                    std::stoi(pola[1]),
                    std::stoi(pola[2]),
                    std::stoi(pola[3]),
                    std::stoi(pola[4]),
                    pola[5],
                    pola[6],
                    pola[7]
                );
                wczytaneCykle.back().dodajWpis(wpis);
            } else {
                throw std::runtime_error("Niepoprawny format w linii " + std::to_string(numerLinii));
            }
        } catch (const std::logic_error& e) {
            throw std::runtime_error("Błąd w linii " + std::to_string(numerLinii) + ": " + e.what());
        }
    }

    if (wczytaneCykle.empty() || wczytaneCykle.back().czyZakonczony()) {
        int nowyNumer = wczytaneCykle.empty() ? 1 : wczytaneCykle.back().getNumerCyklu() + 1;
        wczytaneCykle.emplace_back(nowyNumer);
    }

    cykle = wczytaneCykle;
}