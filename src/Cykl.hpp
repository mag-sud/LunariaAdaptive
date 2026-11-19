#ifndef CYKL_HPP
#define CYKL_HPP
#include <vector>
#include "WpisDziennika.hpp"
#include "Kalendarz.hpp"

class Cykl {
    private:
        int numerCyklu;
        int dlugoscTrwania; // w dniach; 0 = cykl wciąż trwa
        Kalendarz dataStartu;
        std::vector<WpisDziennika> wpisyWCyklu; // posortowane chronologicznie
    public:
        Cykl();
        explicit Cykl(int numer, const Kalendarz& start = Kalendarz()); // explicit dla konstruktora jednoargumentowego, żeby blokować niejawne konwersje z int na Cykl; const dla wydajności
        int getNumerCyklu() const;
        int getDlugoscTrwania() const;
        bool czyZakonczony() const;
        const Kalendarz& getDataStartu() const;
        bool czyMaDateStartu() const;
        int numerDniaDla(const Kalendarz& data) const;  // przelicza datę z kalendarza (data) na numer dnia liczonego od początku cyklu (dataStartu)
        bool zawieraDate(const Kalendarz& data) const;
        void ustawDateStartu(const Kalendarz& nowa);       
        const std::vector<WpisDziennika>& getWpisy() const;
        int ostatniDzien() const;

        void zakoncz(int dlugosc);                     
        void dodajWpis(const WpisDziennika& wpis);     
        bool usunWpis(int dzien);
        WpisDziennika* znajdzWpis(int dzien);
        const WpisDziennika* znajdzWpis(int dzien) const;
};

#endif