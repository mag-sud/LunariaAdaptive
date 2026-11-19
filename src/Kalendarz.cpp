#include "Kalendarz.hpp"
#include <ctime> // pobieranie daty i czasu z systemu operacyjnego komputera
#include <iomanip>
#include <sstream> // do formatowania daty na tekst
#include <stdexcept>
using namespace std;

// funkcje pomocnicze potrzebne tylko tutaj, więc anonymous namespace, żeby były widoczne tylko w tym pliku
namespace {
    // Część logiki kalendarzowej (algorytm przeliczania dni) bazuje na rozwiązaniach stworzonych przez Howarda Hinnanta.
    // natychmiastowe przeliczanie daty na pojedynczą liczbę całkowitą (liczbę dni) oraz z powrotem, z pełnym uwzględnieniem lat przestępnych i nieregularności miesięcy
    // * The MIT License (MIT)
    // * Copyright (c) 2015, 2016, 2017 Howard Hinnant
    // * Permission is hereby granted, free of charge, to any person obtaining a copy
    // * of this software and associated documentation files (the "Software"), to deal
    // * in the Software without restriction, including without limitation the rights
    // * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    // * copies of the Software, and to permit persons to whom the Software is
    // * furnished to do so, subject to the following conditions:
    // * The above copyright notice and this permission notice shall be included in all
    // * copies or substantial portions of the Software.
    
        long dniOdEpoki(int r, int m, int d) {
        r -= (m <= 2) ? 1 : 0;
        const long era = (r >= 0 ? r : r - 399) / 400;
        const long yoe = r - era * 400;
        const long doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
        const long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
        return era * 146097 + doe - 719468;
    }
    // odwrotnie: z liczby dni odtwarza dzień, miesiąc, rok
    void zDni(long z, int& d, int& m, int& r) {
        z += 719468;
        const long era = (z >= 0 ? z : z - 146096) / 146097;
        const long doe = z - era * 146097;
        const long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
        const long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
        const long mp = (5 * doy + 2) / 153;
        d = static_cast<int>(doy - (153 * mp + 2) / 5 + 1);
        m = static_cast<int>(mp < 10 ? mp + 3 : mp - 9);
        r = static_cast<int>(yoe + era * 400 + (m <= 2 ? 1 : 0));
    }
}

Kalendarz::Kalendarz() : dzien(0), miesiac(0), rok(0), ustawiona(false) {}
// DD-MM-RRRR
Kalendarz::Kalendarz(int d, int m, int r) : dzien(d), miesiac(m), rok(r), ustawiona(true) {
    bool poprawna = (r >= 1900 && r <= 2200 && m >= 1 && m <= 12 && d >= 1 && d <= 31);
    if (poprawna) {
        // sprawdzenie w obie strony: nieistniejąca data (np. 30 lutego) zmieniłaby się po przeliczeniu
        int d2, m2, r2;
        zDni(dniOdEpoki(r, m, d), d2, m2, r2);
        poprawna = (d2 == d && m2 == m && r2 == r);
    }
    if (!poprawna) throw invalid_argument("Nieprawidłowa data (oczekiwany format DD-MM-RRRR)");
}
long Kalendarz::numerDnia() const { return dniOdEpoki(rok, miesiac, dzien); }

Kalendarz Kalendarz::dzisiaj() {
    time_t t = time(nullptr); // liczba sekund jaka upłynęła od epoki Unixa; nullptr bo ten wskaźnik nie wskazuje na żaden obiekt
    tm teraz{};
#ifdef _WIN32
    localtime_s(&teraz, &t);
#else
    localtime_r(&t, &teraz);
#endif
    return Kalendarz(teraz.tm_mday, teraz.tm_mon + 1, teraz.tm_year + 1900);
}

Kalendarz Kalendarz::zTekstu(const string& t) {
    const string blad = "Nieprawidłowa data (oczekiwany format DD-MM-RRRR)";
    if (t.size() != 10 || t[2] != '-' || t[5] != '-') throw invalid_argument(blad);
    for (size_t i = 0; i < t.size(); i++) {
        if (i != 2 && i != 5 && !isdigit(static_cast<unsigned char>(t[i]))) throw invalid_argument(blad);
    }
    // Parsujemy: DD (0-2), MM (3-5), RRRR (6-10)
    int d = stoi(t.substr(0, 2));
    int m = stoi(t.substr(3, 2));
    int r = stoi(t.substr(6, 4));
    return Kalendarz(d, m, r);
}

bool Kalendarz::czyUstawiona() const { return ustawiona; }

string Kalendarz::doTekstu() const {
    if (!ustawiona) return "-";
    ostringstream os;
    os << setfill('0') << setw(2) << dzien << '-' << setw(2) << miesiac << '-' << setw(4) << rok;
    return os.str();
}

Kalendarz Kalendarz::dodajDni(int ile) const {
    if (!ustawiona) throw invalid_argument("Nie można przesuwać nieustawionej daty");
    int d, m, r;
    zDni(numerDnia() + ile, d, m, r);
    return Kalendarz(d, m, r);
}

int Kalendarz::operator-(const Kalendarz& inna) const {
    if (!ustawiona || !inna.ustawiona) throw invalid_argument("Nie można odejmować nieustawionych dat");
    return static_cast<int>(numerDnia() - inna.numerDnia());
}

bool Kalendarz::operator==(const Kalendarz& inna) const {
    if (!ustawiona || !inna.ustawiona) return ustawiona == inna.ustawiona;
    return dzien == inna.dzien && miesiac == inna.miesiac && rok == inna.rok;
}

bool Kalendarz::operator<(const Kalendarz& inna) const {
    if (!ustawiona || !inna.ustawiona) throw invalid_argument("Nie można porównywać nieustawionych dat");
    return numerDnia() < inna.numerDnia();
}

ostream& operator<<(ostream& os, const Kalendarz& k) {
    return os << k.doTekstu();
}