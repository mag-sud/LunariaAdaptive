#ifndef FAZA_CYKLU_HPP
#define FAZA_CYKLU_HPP
#include <string>
#include "WpisDziennika.hpp"

// interfejs polimorficzny dla faz cyklu
class FazaCyklu {
public:
    // wirtualny destruktor - kluczowy przy polimorfizmie i dziedziczeniu
    virtual ~FazaCyklu() = default;

    // wirtualne metody (każda konkretna faza musi je zaimplementować)
    virtual std::string getNazwaFazy() const = 0;
    virtual std::string getRekomendowanaDieta() const = 0;
    virtual std::string getRekomendowanyTrening() const = 0;
    virtual std::string getOpisHormonalny() const = 0;
    virtual std::string getProfilProduktywnosci() const = 0;
    virtual std::string getZiola() const = 0;
    virtual std::string getStylRegeneracji() const = 0;
    virtual std::string getHolistyczneWsparcie() const = 0;
    virtual std::string getWsparcieWyciszajace() const = 0;

    // wersja adaptacyjna – przyjmuje dzisiejszy wpis z dziennika, dzięki czemu porada reaguje na bieżący stres i energię
    virtual std::string getWsparcieWyciszajace(const WpisDziennika& wpis) const = 0;

};

#endif