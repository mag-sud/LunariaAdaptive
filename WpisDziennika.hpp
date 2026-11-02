#ifndef WPIS_DZIENNIKA_HPP
#define WPIS_DZIENNIKA_HPP
#include <iostream>
#include <string>

class WpisDziennika {
private:
    int numerDniaCyklu;
    int poziomEnergii;
    int poziomStresu;
    std::string nastroj;
    std::string objawy;
    std::string notatka;

public:
    // Konstruktor domyślny i parametryczny
    WpisDziennika();
    WpisDziennika(int dzien, int energia, int stres, std::string nastroj, std::string objawy, std::string notatka);

    // pobieranie danych
    int getNumerDniaCyklu() const;
    int getPoziomEnergii() const;
    int getPoziomStresu() const;
    std::string getNastroj() const;
    std::string getObjawy() const;
    std::string getNotatka() const;

    // ustawianie danych
    void setNumerDniaCyklu(int dzien);
    void setPoziomEnergii(int energia);
    void setPoziomStresu(int stres);
    void setNastroj(std::string nastroj);
    void setObjawy(std::string objawy);
    void setNotatka(std::string notatka);

    // przeładowany operator do wypisywania wpisu strumieniowo
    friend std::ostream& operator<<(std::ostream& os, const WpisDziennika& wpis);
};

#endif