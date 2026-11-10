#ifndef KALENDARZ_HPP
#define KALENDARZ_HPP
#include <iostream>
#include <string>

class Kalendarz {
    private:
        int dzien;
        int miesiac;
        int rok;
        bool ustawiona; // czy data została podana (np. czy znamy początek cyklu)
        long numerDnia() const; // const bo metoda nie zmienia obiektu
    public:
        Kalendarz(); // konstruktor domyślny (data nieustawiona)
        Kalendarz(int dzien, int miesiac, int rok);

        static Kalendarz dzisiaj();
        static Kalendarz zTekstu(const std::string& tekst); //DD-MM-RRRR
        bool czyUstawiona() const;
        std::string doTekstu() const;
        Kalendarz dodajDni(int ile) const;

        int operator-(const Kalendarz& inna) const;  // różnica w dniach
        bool operator==(const Kalendarz& inna) const;
        bool operator<(const Kalendarz& inna) const;

        friend std::ostream& operator<<(std::ostream& os, const Kalendarz& k);
};

#endif
