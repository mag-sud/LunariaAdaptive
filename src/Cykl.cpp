#include "Cykl.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>

Cykl::Cykl() // tworzy domyślny cykl nr 1
    : numerCyklu(1), dlugoscTrwania(0), dataStartu(Kalendarz()), wpisyWCyklu{} {}

Cykl::Cykl(int numer, const Kalendarz& start) // tworzy cykl, przyjmując konkretny numer cyklu i datę początkową
    : numerCyklu(numer), dlugoscTrwania(0), dataStartu(start), wpisyWCyklu{} {
    if (numerCyklu < 1) {
        throw std::invalid_argument("Numer cyklu musi być dodatni");
    }
}

// wszędzie const, żeby nie psuć danych
int Cykl::getNumerCyklu() const {
    return numerCyklu;
}
int Cykl::getDlugoscTrwania() const {
    return dlugoscTrwania;
}
bool Cykl::czyZakonczony() const {
    return dlugoscTrwania > 0;
}
const Kalendarz& Cykl::getDataStartu() const {
    return dataStartu;
}
bool Cykl::czyMaDateStartu() const {
    return dataStartu.czyUstawiona();
}
int Cykl::numerDniaDla(const Kalendarz& data) const {
    if (!czyMaDateStartu()) {
        throw std::invalid_argument("Ten cykl nie ma jeszcze ustawionej daty początku");
    }
    return (data - dataStartu) + 1;
}
bool Cykl::zawieraDate(const Kalendarz& data) const {
    if (!czyMaDateStartu()) {
        return false;
    }
    int dzien = numerDniaDla(data);
    return dzien >= 1 && (!czyZakonczony() || dzien <= dlugoscTrwania);
}

void Cykl::ustawDateStartu(const Kalendarz& nowa) {
    if (!nowa.czyUstawiona()) {
        throw std::invalid_argument("Data początku cyklu musi być ustawiona");
    }
    if (czyMaDateStartu() && !wpisyWCyklu.empty()) {
        int przesuniecie = dataStartu - nowa;
        for (const auto& w : wpisyWCyklu) {
            int nowyDzien = w.getNumerDniaCyklu() + przesuniecie;
            if (nowyDzien < 1 || (czyZakonczony() && nowyDzien > dlugoscTrwania)) {
                throw std::invalid_argument(
                    "Przy tej dacie startu wpis z dnia " + std::to_string(w.getNumerDniaCyklu()) +
                    " wypadłby poza cykl - najpierw usuń lub popraw ten wpis"
                );
            }
        }
        for (auto& w : wpisyWCyklu) {
            w.setNumerDniaCyklu(w.getNumerDniaCyklu() + przesuniecie);
        }
    }
    dataStartu = nowa;
}

const std::vector<WpisDziennika>& Cykl::getWpisy() const {
    return wpisyWCyklu;
}
int Cykl::ostatniDzien() const {
    return wpisyWCyklu.empty() ? 0 : wpisyWCyklu.back().getNumerDniaCyklu();
}
void Cykl::zakoncz(int dlugosc) {
    if (dlugosc < 1) {
        throw std::invalid_argument("Długość cyklu musi być dodatnia");
    }
    if (dlugosc < ostatniDzien()) {
        throw std::invalid_argument(
            "Długość cyklu (" + std::to_string(dlugosc) +
            ") nie może być mniejsza niż ostatni dzień z wpisem (" +
            std::to_string(ostatniDzien()) + ")"
        );
    }
    dlugoscTrwania = dlugosc;
}

void Cykl::dodajWpis(const WpisDziennika& wpis) {
    if (wpis.getNumerCyklu() != numerCyklu) {
        throw std::invalid_argument("Wpis należy do innego cyklu");
    }
    if (wpis.getNumerDniaCyklu() < 1) {
        throw std::invalid_argument("Numer dnia cyklu we wpisie musi być dodatni");
    }
    if (czyZakonczony() && wpis.getNumerDniaCyklu() > dlugoscTrwania) {
        throw std::invalid_argument(
            "Dzień " + std::to_string(wpis.getNumerDniaCyklu()) +
            " wykracza poza długość zakończonego cyklu (" +
            std::to_string(dlugoscTrwania) + ")"
        );
    }

    // Najpierw  nowy wpis na koniec wektora przez .push_back, potem sortowanie wektora od begin() do end(), 
    // żeby zachować chronologiczne ułożenie wpisów
    if (std::find(wpisyWCyklu.begin(), wpisyWCyklu.end(), wpis) != wpisyWCyklu.end()) {
        throw std::invalid_argument("Wpis dla tego dnia już istnieje - użyj edycji");
    }
    wpisyWCyklu.push_back(wpis);
    std::sort(wpisyWCyklu.begin(), wpisyWCyklu.end());
}

bool Cykl::usunWpis(int dzien) {
    auto it = std::find_if(
        wpisyWCyklu.begin(), wpisyWCyklu.end(),
        [dzien](const WpisDziennika& w) { return w.getNumerDniaCyklu() == dzien; }
    );
    if (it == wpisyWCyklu.end()) {
        return false;
    }
    wpisyWCyklu.erase(it);
    return true;
}

WpisDziennika* Cykl::znajdzWpis(int dzien) {
    for (auto& w : wpisyWCyklu) {
        if (w.getNumerDniaCyklu() == dzien) {
            return &w;
        }
    }
    return nullptr;
}

const WpisDziennika* Cykl::znajdzWpis(int dzien) const {
    for (const auto& w : wpisyWCyklu) {
        if (w.getNumerDniaCyklu() == dzien) {
            return &w;
        }
    }
    return nullptr;
}