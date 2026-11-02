#ifndef FAZA_CYKLU_HPP
#define FAZA_CYKLU_HPP
using namespace std;
#include <string>

// interfejs polimorficzny dla faz cyklu
class FazaCyklu {
public:
    // Wirtualny destruktor - kluczowy przy polimorfizmie i dziedziczeniu
    virtual ~FazaCyklu() = default;

    // wirtualne metody (każda konkretna faza musi je zaimplementować)
    virtual string getNazwaFazy() const = 0;
    virtual string getRekomendowanaDieta() const = 0;
    virtual string getRekomendowanyTrening() const = 0;
    virtual string getOpisHormonalny() const = 0;
    virtual string getProfilProduktywnosci() const = 0;
    virtual string getZiola() const = 0;
    virtual string getStylRegeneracji() const = 0;
    virtual string getHolistyczneWsparcie() const = 0;
};

#endif