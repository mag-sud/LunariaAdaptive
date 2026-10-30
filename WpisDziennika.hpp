#ifndef WPIS_DZIENNIKA_HPP
#define WPIS_DZIENNIKA_HPP
#include <iostream>
#include <string>
using namespace std;

class WpisDziennika {
private:
    int numerDniaCyklu;
    int poziomEnergii;
    int poziomStresu;
    string nastroj;
    string objawy;
    string notatka;
public: // konstruktor domyślny, parametryczny
    WpisDziennika();
    WpisDziennika(int dzien, int energia, int stres, string nastroj, string objawy, string notatka);
    // pobieranie danych
    int getNumerDniaCyklu() const;
    int getPoziomEnergii() const;
    int getPoziomStresu() const;
    string getNastroj() const;
    string getObjawy() const;
    string getNotatka() const;
    // ustawianie danych
    void setNumerDniaCyklu(int dzien);
    void setPoziomEnergii(int energia);
    void setPoziomStresu(int stres);
    void setNastroj(string nastroj);
    void setObjawy(string objawy);
    void setNotatka(string notatka);

    friend ostream& operator<<(ostream& os, const WpisDziennika& wpis);
};
#endif