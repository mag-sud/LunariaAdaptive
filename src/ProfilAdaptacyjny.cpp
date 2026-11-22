#include "ProfilAdaptacyjny.hpp"
#include "FazaMenstruacyjna.hpp"
#include "FazaFolikularna.hpp"
#include "FazaOwulacyjna.hpp"
#include "FazaLutealna.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace {
    // tylko jeśli użytkowniczka nie ma jeszcze wystarczającej historii
    constexpr double DOMYSLNA_DLUGOSC_CYKLU = 28.0;
    constexpr double PROG_MENSTRUACJI = 0.14;
    constexpr double PROG_POCZATKU_OWULACJI = 0.45;
    constexpr double PROG_KONCA_OWULACJI = 0.62;
    constexpr int OKNO_DNI_STRESU = 1;
    constexpr double ROZNICA_WYSOKIEGO_STRESU = 1.0;
    constexpr int MIN_TYPOWA_DLUGOSC_CYKLU = 10;

    bool czyCyklAnomalny(int dlugosc, double dotychczasowaSrednia, int liczbaTypowych) {
        if (dlugosc < MIN_TYPOWA_DLUGOSC_CYKLU) {
            return true;
        }
        if (liczbaTypowych >= 2 && dotychczasowaSrednia > 0.0) {
            if (dlugosc < dotychczasowaSrednia * 0.5 || dlugosc > dotychczasowaSrednia * 1.6) {
                return true;
            }
        }
        return false;
    }

    // szuka dnia z najwyższym wynikiem (energia - stres) w zakończonym cyklu
    int znajdzDzienSzczytu(const Cykl& cykl) {
        const auto& wpisy = cykl.getWpisy();
        if (wpisy.empty()) {
            return -1;
        }

        int najlepszyDzien = -1;
        bool ustawiony = false;
        int najlepszyWynik = 0;

        for (const auto& w : wpisy) {
            int wynik = w.getPoziomEnergii() - w.getPoziomStresu();
            if (!ustawiony || wynik > najlepszyWynik) {
                najlepszyWynik = wynik;
                najlepszyDzien = w.getNumerDniaCyklu();
                ustawiony = true;
            }
        }
        return najlepszyDzien;
    }
}

ProfilAdaptacyjny::ProfilAdaptacyjny(const HistoriaUzytkowniczki& h) : historia(h) {
    przeliczModel();
}

void ProfilAdaptacyjny::przeliczModel() {
    liczbaCykliUczacych = 0;
    double sumaDlugosci = 0.0;
    int sumaSzczytow = 0;
    int liczbaSzczytow = 0;

    for (const auto& cykl : historia.getCykle()) {
        int dlugosc = cykl.getDlugoscTrwania();
        double biezacaSrednia = (liczbaCykliUczacych > 0) ? (sumaDlugosci / liczbaCykliUczacych) : 0.0;
        if (czyCyklAnomalny(dlugosc, biezacaSrednia, liczbaCykliUczacych)) {
            continue;
        }
        if (!cykl.czyZakonczony()) {
            continue;
        }
        liczbaCykliUczacych++;
        sumaDlugosci += cykl.getDlugoscTrwania();

        int szczyt = znajdzDzienSzczytu(cykl);
        if (szczyt > 0) {
            sumaSzczytow += szczyt;
            liczbaSzczytow++;
        }
    }

    if (liczbaCykliUczacych > 0 && sumaDlugosci > 0.0) {
        sredniaDlugoscCyklu = sumaDlugosci / liczbaCykliUczacych;
    } else {
        sredniaDlugoscCyklu = DOMYSLNA_DLUGOSC_CYKLU;
    }

    koniecMenstruacji = std::max(1.0, std::round(sredniaDlugoscCyklu * PROG_MENSTRUACJI));

    if (liczbaSzczytow > 0) {
        double sredniSzczyt = static_cast<double>(sumaSzczytow) / liczbaSzczytow;
        poczatekOwulacji = std::max(koniecMenstruacji + 1.0, std::round(sredniSzczyt - 1.0));
        koniecOwulacji = std::max(poczatekOwulacji + 1.0, std::round(sredniSzczyt + 1.0));
    } else {
        poczatekOwulacji = std::max(koniecMenstruacji + 1.0, std::round(sredniaDlugoscCyklu * PROG_POCZATKU_OWULACJI));
        koniecOwulacji = std::max(poczatekOwulacji + 1.0, std::round(sredniaDlugoscCyklu * PROG_KONCA_OWULACJI));
    }

    double sumaStresu = 0.0;
    int liczbaWpisow = 0;
    for (const auto& cykl : historia.getCykle()) {
        for (const auto& w : cykl.getWpisy()) {
            sumaStresu += w.getPoziomStresu();
            liczbaWpisow++;
        }
    }
    sredniaStresu = (liczbaWpisow > 0) ? (sumaStresu / liczbaWpisow) : 0.0;
}

TypFazy ProfilAdaptacyjny::pozycjaWgGranic(int dzienCyklu) const {
    if (dzienCyklu <= static_cast<int>(koniecMenstruacji)) {
        return TypFazy::Menstruacyjna;
    }
    if (dzienCyklu < static_cast<int>(poczatekOwulacji)) {
        return TypFazy::Folikularna;
    }
    if (dzienCyklu <= static_cast<int>(koniecOwulacji)) {
        return TypFazy::Owulacyjna;
    }
    return TypFazy::Lutealna;
}

TypFazy ProfilAdaptacyjny::wyznaczFaze(int dzienCyklu) const {
    TypFazy faza = pozycjaWgGranic(dzienCyklu);
    const WpisDziennika* wpis = historia.biezacyCykl().znajdzWpis(dzienCyklu);

    if (wpis == nullptr) {
        return faza;
    }

    // jeśli w pierwszej połowie cyklu pojawiają się objawy menstruacyjne, traktujemy to jako menstruację
    if (wpis->objawyMenstruacyjne() &&
        (faza == TypFazy::Menstruacyjna || faza == TypFazy::Folikularna)) {
        return TypFazy::Menstruacyjna;
    }

    return faza;
}

OcenaStresu ProfilAdaptacyjny::ocenStres(int dzienCyklu) const {
    OcenaStresu ocena;
    double sumaHistoryczna = 0.0;

    for (const auto& cykl : historia.getCykle()) {
        for (const auto& w : cykl.getWpisy()) {
            bool toDzisiejszyWpis = (&cykl == &historia.biezacyCykl() && w.getNumerDniaCyklu() == dzienCyklu);
            if (!toDzisiejszyWpis && std::abs(w.getNumerDniaCyklu() - dzienCyklu) <= OKNO_DNI_STRESU) {
                sumaHistoryczna += w.getPoziomStresu();
                ocena.liczbaProbek++;
            }
        }
    }

    if (ocena.liczbaProbek > 0) {
        ocena.sredniStresHistoryczny = sumaHistoryczna / ocena.liczbaProbek;
    }

    const WpisDziennika* dzis = historia.biezacyCykl().znajdzWpis(dzienCyklu);
    if (dzis != nullptr) {
        ocena.maWpisDzisiaj = true;
        ocena.stresDzisiaj = dzis->getPoziomStresu();
    }

    double punktOdniesienia = (ocena.liczbaProbek > 0) ? ocena.sredniStresHistoryczny : sredniaStresu;
    double wartoscDoPorownania = ocena.maWpisDzisiaj ? static_cast<double>(ocena.stresDzisiaj) : ocena.sredniStresHistoryczny;

    if (ocena.liczbaProbek > 0 || ocena.maWpisDzisiaj) {
        ocena.podwyzszony = (wartoscDoPorownania - punktOdniesienia) >= ROZNICA_WYSOKIEGO_STRESU;
    }

    return ocena;
}

int ProfilAdaptacyjny::getLiczbaCykliUczacych() const {
    return liczbaCykliUczacych;
}

double ProfilAdaptacyjny::getSredniaDlugoscCyklu() const {
    return sredniaDlugoscCyklu;
}

double ProfilAdaptacyjny::getSredniaStresu() const {
    return sredniaStresu;
}

std::string ProfilAdaptacyjny::opisModelu() const {
    std::ostringstream out;
    out << std::fixed << std::setprecision(1);
    out << "Zakończone cykle w modelu: " << liczbaCykliUczacych << "\n";
    out << "Średnia długość cyklu: " << sredniaDlugoscCyklu << " dni\n";
    out << "Koniec menstruacji: ok. dnia " << static_cast<int>(koniecMenstruacji) << "\n";
    out << "Okno owulacji: dni " << static_cast<int>(poczatekOwulacji)
        << "-" << static_cast<int>(koniecOwulacji) << "\n";
    out << "Średni poziom stresu: " << sredniaStresu << "\n";
    return out.str();
}

std::unique_ptr<FazaCyklu> utworzFaze(TypFazy typ) {
    switch (typ) {
        case TypFazy::Menstruacyjna:
            return std::make_unique<FazaMenstruacyjna>();
        case TypFazy::Folikularna:
            return std::make_unique<FazaFolikularna>();
        case TypFazy::Owulacyjna:
            return std::make_unique<FazaOwulacyjna>();
        case TypFazy::Lutealna:
        default:
            return std::make_unique<FazaLutealna>();
    }
}

std::unique_ptr<FazaCyklu> wyznaczFazeAdaptacyjnie(int dzienCyklu, const ProfilAdaptacyjny& profil) {
    return utworzFaze(profil.wyznaczFaze(dzienCyklu));
}