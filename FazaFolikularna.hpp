#ifndef FAZA_FOLIKULARNA_HPP
#define FAZA_FOLIKULARNA_HPP
#include "FazaCyklu.hpp"
#include <string>

class FazaFolikularna : public FazaCyklu {
public:
    std::string getNazwaFazy() const override;
    std::string getRekomendowanaDieta() const override;
    std::string getRekomendowanyTrening() const override;
    std::string getOpisHormonalny() const override;
    std::string getProfilProduktywnosci() const override;
    std::string getZiola() const override;
    std::string getStylRegeneracji() const override;
    std::string getHolistyczneWsparcie() const override;
};

#endif // FAZA_FOLIKULARNA_HPP