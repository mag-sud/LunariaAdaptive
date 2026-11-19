#ifndef WPIS_DZIENNIKA_HPP
#define WPIS_DZIENNIKA_HPP
#include <iostream>
#include <string>

class WpisDziennika {
private:
    int numerCyklu;
    int numerDniaCyklu;
    int poziomEnergii;
    int poziomStresu;
    std::string nastroj;
    std::string objawy;
    std::string notatka;

public:
    // static bo składowa należy do całej klasy, a nie do pojedynczego obiektu
    static const int SKALA_MIN = 1;
    static const int SKALA_MAX = 5;
    // Konstruktor domyślny i parametryczny
    WpisDziennika();
    WpisDziennika(int cykl, int dzien, int energia, int stres, std::string nastroj, std::string objawy, std::string notatka);

    // pobieranie danych
    int getNumerCyklu() const;
    int getNumerDniaCyklu() const;
    int getPoziomEnergii() const;
    int getPoziomStresu() const;
    std::string getNastroj() const;
    std::string getObjawy() const;
    std::string getNotatka() const;

    bool objawyMenstruacyjne() const;

    // ustawianie danych
    void setNumerCyklu(int cykl);
    void setNumerDniaCyklu(int dzien);
    void setPoziomEnergii(int energia);
    void setPoziomStresu(int stres);
    void setNastroj(std::string nastroj);
    void setObjawy(std::string objawy);
    void setNotatka(std::string notatka);

    // żeby program nie pozwolił dodać dwóch wpisów na ten sam dzień tego samego cyklu
    bool operator==(const WpisDziennika& inny) const;
    // kolejność chronologiczna (cykl, potem dzień) - używana przez std::sort
    bool operator<(const WpisDziennika& inny) const;

    // przeładowany operator do wypisywania wpisu strumieniowo
    friend std::ostream& operator<<(std::ostream& os, const WpisDziennika& wpis);
};

#endif