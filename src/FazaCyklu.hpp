#ifndef FAZA_CYKLU_HPP
#define FAZA_CYKLU_HPP
#include <string>
#include "WpisDziennika.hpp"

// interfejs polimorficzny dla faz cyklu
class FazaCyklu {
private:
    std::string nazwaFazy;
protected:              // dostęp tylko dla klas dziedziczących po FazaCyklu
    explicit FazaCyklu(std::string nazwa);
public:
    // wirtualny destruktor - kluczowy przy polimorfizmie i dziedziczeniu; gwarantuje, że najpierw wykona się destruktor klasy pochodnej, a potem klasy bazowej
    virtual ~FazaCyklu() = default;

    // wirtualne metody (każda konkretna faza musi je zaimplementować)
    virtual std::string getNazwaFazy() const; // zwykła metoda wirtualna: musi mieć implementację w klasie bazowej; klasy pochodne mogą ją przesłonić (override)
    virtual std::string getRekomendowanaDieta() const = 0; // czysto wirtualna metoda: klasa bazowa nie daje domyślnej implementacji; musi override
    virtual std::string getRekomendowanyTrening() const = 0;
    virtual std::string getOpisHormonalny() const = 0;
    virtual std::string getProfilProduktywnosci() const = 0;
    virtual std::string getZiola() const = 0;
    virtual std::string getStylRegeneracji() const = 0;
    virtual std::string getHolistyczneWsparcie() const = 0;
    // przyjmuje dzisiejszy wpis z dziennika, dzięki czemu porada reaguje na bieżący stres i energię
    virtual std::string getWsparcieWyciszajace(const WpisDziennika& wpis) const = 0;
    // const, żeby metody tylko odczytywały stan obiektu, a nie modyfikowały go - bezpieczne wywoływanie na obiektach i referencjach const
};

#endif