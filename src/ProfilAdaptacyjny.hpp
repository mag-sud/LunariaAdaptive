#ifndef PROFIL_ADAPTACYJNY_HPP
#define PROFIL_ADAPTACYJNY_HPP
#include <memory>
#include <string>
#include "FazaCyklu.hpp"
#include "HistoriaUzytkowniczki.hpp"

enum class TypFazy {
    Menstruacyjna,
    Folikularna,
    Owulacyjna,
    Lutealna
};

struct OcenaStresu {
    double sredniStresHistoryczny = 0.0;
    int liczbaProbek = 0;
    bool maWpisDzisiaj = false;
    int stresDzisiaj = 0;
    bool podwyzszony = false;
};

class ProfilAdaptacyjny {
private:
    const HistoriaUzytkowniczki& historia;

    int liczbaCykliUczacych = 0;
    double sredniaDlugoscCyklu = 0.0;
    double koniecMenstruacji = 0.0;
    double poczatekOwulacji = 0.0;
    double koniecOwulacji = 0.0;
    double sredniaStresu = 0.0;

    void przeliczModel();

public:
    explicit ProfilAdaptacyjny(const HistoriaUzytkowniczki& h);

    TypFazy pozycjaWgGranic(int dzienCyklu) const;
    TypFazy wyznaczFaze(int dzienCyklu) const;
    OcenaStresu ocenStres(int dzienCyklu) const;

    int getLiczbaCykliUczacych() const;
    double getSredniaDlugoscCyklu() const;
    double getSredniaStresu() const;
    std::string opisModelu() const;
};

std::unique_ptr<FazaCyklu> utworzFaze(TypFazy typ);
std::unique_ptr<FazaCyklu> wyznaczFazeAdaptacyjnie(int dzienCyklu, const ProfilAdaptacyjny& profil);

#endif