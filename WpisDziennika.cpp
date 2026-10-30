#include "WpisDziennika.hpp"
using namespace std;

// konstruktor domyślny (tworzy obiekt w bezpiecznym stanie zerowym)
WpisDziennika::WpisDziennika() 
    : numerDniaCyklu(0), poziomEnergii(0), poziomStresu(0), nastroj("brak"), objawy("brak"), notatka("brak") {
}
// onstruktor parametryczny (dla konkretnych, pełnych danych)
WpisDziennika::WpisDziennika(int dzien, int energia, int stres, string nastroj, string objawy, string notatka) 
    : numerDniaCyklu(dzien), poziomEnergii(energia), poziomStresu(stres), nastroj(nastroj), objawy(objawy), notatka(notatka) {
}

int WpisDziennika::getNumerDniaCyklu() const {return numerDniaCyklu;}
int WpisDziennika::getPoziomEnergii() const {return poziomEnergii;}
int WpisDziennika::getPoziomStresu() const {return poziomStresu;}
string WpisDziennika::getNastroj() const {return nastroj;}
string WpisDziennika::getObjawy() const {return objawy;}
string WpisDziennika::getNotatka() const {return notatka;}

void WpisDziennika::setNumerDniaCyklu(int dzien) {numerDniaCyklu = dzien;}
void WpisDziennika::setPoziomEnergii(int energia) {poziomEnergii = energia;}
void WpisDziennika::setPoziomStresu(int stres) {poziomStresu = stres;}
void WpisDziennika::setNastroj(string n) {nastroj = n;}
void WpisDziennika::setObjawy(string o) {objawy = o;}
void WpisDziennika::setNotatka(string notatki) {notatka = notatki;}

ostream& operator<<(ostream& os, const WpisDziennika& wpis) {
    os << "[Dzień cyklu: " << wpis.numerDniaCyklu << "] | "
       << "Energia: " << wpis.poziomEnergii << "/5 | "
       << "Stres: " << wpis.poziomStresu << "/5 |"
       << "Nastrój: " << wpis.nastroj << " | "
       << "Objawy: " << wpis.objawy << " | "
       << "Notatka: " << wpis.notatka;
    return os;
}