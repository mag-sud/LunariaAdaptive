#ifndef FAZA_OWULACYJNA_HPP
#define FAZA_OWULACYJNA_HPP
#include "FazaCyklu.hpp"
using namespace std;

class FazaOwulacyjna : public FazaCyklu {
public:
    string getNazwaFazy() const override;
    string getRekomendowanaDieta() const override;
    string getRekomendowanyTrening() const override;
    string getOpisHormonalny() const override;
    string getProfilProduktywnosci() const override;
    string getZiola() const override;
    string getStylRegeneracji() const override;
    string getHolistyczneWsparcie() const override;
};

#endif