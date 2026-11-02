#ifndef FAZA_LUTEALNA_HPP
#define FAZA_LUTEALNA_HPP
#include "FazaCyklu.hpp"
using namespace std;

class FazaLutealna : public FazaCyklu {
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