#ifndef HISTORIA_UZYTKOWNICZKI_HPP
#define HISTORIA_UZYTKOWNICZKI_HPP

#include <string>
#include <vector>
#include "Cykl.hpp"

class HistoriaUzytkowniczki {
private:
    std::vector<Cykl> cykle; // zawsze co najmniej jeden cykl
public:
    HistoriaUzytkowniczki();
    const std::vector<Cykl>& getCykle() const;
    const Cykl& biezacyCykl() const;
    const Cykl& znajdzCykl(int numer) const;
    const Cykl& znajdzCyklDlaDaty(const Kalendarz& data) const;
    int liczbaZakonczonychCykli() const;

    void dodajWpis(const WpisDziennika& wpis);
    WpisDziennika& znajdzWpis(int cykl, int dzien);
    void usunWpis(int cykl, int dzien);
    void ustawDateStartuBiezacego(const Kalendarz& data);
    void rozpocznijNowyCykl(const Kalendarz& dataStartu);
    void wyczysc();

    void zapisz(const std::string& sciezka) const;
    void wczytaj(const std::string& sciezka);
};

#endif