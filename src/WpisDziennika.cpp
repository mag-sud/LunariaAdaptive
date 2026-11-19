#include "WpisDziennika.hpp"
#include <stdexcept>
using namespace std;

// funkcja pomocnicza w anonimowej przestrzeni nazw (widoczna tylko w tym pliku)
namespace {
    void sprawdzSkale(int wartosc, const string& nazwa) {
        if (wartosc < WpisDziennika::SKALA_MIN || wartosc > WpisDziennika::SKALA_MAX) {
            throw invalid_argument(nazwa + " musi mieścić się w skali " + to_string(WpisDziennika::SKALA_MIN) + "-" + to_string(WpisDziennika::SKALA_MAX) + " (podano: " + to_string(wartosc) + ")");
        }
    }
}

// konstruktor domyślny (tworzy obiekt w bezpiecznym stanie zerowym)
WpisDziennika::WpisDziennika() 
    : numerCyklu(0), numerDniaCyklu(0), poziomEnergii(0), poziomStresu(0), nastroj("brak"), objawy("brak"), notatka("brak") {
}

// konstruktor parametryczny (wykorzystujący settery dla pełnej walidacji od samego startu)
WpisDziennika::WpisDziennika(int cykl, int dzien, int energia, int stres, string nastroj, string objawy, string notatka) {
    setNumerCyklu(cykl);
    setNumerDniaCyklu(dzien);
    setPoziomEnergii(energia);
    setPoziomStresu(stres);
    setNastroj(nastroj);
    setObjawy(objawy);
    setNotatka(notatka);
}

int WpisDziennika::getNumerCyklu() const { return numerCyklu; }
int WpisDziennika::getNumerDniaCyklu() const { return numerDniaCyklu; }
int WpisDziennika::getPoziomEnergii() const { return poziomEnergii; }
int WpisDziennika::getPoziomStresu() const { return poziomStresu; }
string WpisDziennika::getNastroj() const { return nastroj; }
string WpisDziennika::getObjawy() const { return objawy; }
string WpisDziennika::getNotatka() const { return notatka; }

bool WpisDziennika::objawyMenstruacyjne() const {
    return objawy == "skurcze" || objawy == "krwawienie"; 
}

void WpisDziennika::setNumerCyklu(int cykl) {
    if (cykl < 1) throw invalid_argument("Numer cyklu musi być dodatni");
    numerCyklu = cykl;
}

void WpisDziennika::setNumerDniaCyklu(int dzien) {
    if (dzien < 1) throw invalid_argument("Dzień cyklu musi być dodatni");
    numerDniaCyklu = dzien;
}

void WpisDziennika::setPoziomEnergii(int energia) { 
    sprawdzSkale(energia, "Energia"); 
    poziomEnergii = energia; 
}

void WpisDziennika::setPoziomStresu(int stres) { 
    sprawdzSkale(stres, "Stres"); 
    poziomStresu = stres; 
}

void WpisDziennika::setNastroj(string n) { nastroj = n; }
void WpisDziennika::setObjawy(string o) { objawy = o; }
void WpisDziennika::setNotatka(string notatki) { notatka = notatki; }
 
bool WpisDziennika::operator==(const WpisDziennika& inny) const {
    return numerCyklu == inny.numerCyklu && numerDniaCyklu == inny.numerDniaCyklu;
}

bool WpisDziennika::operator<(const WpisDziennika& inny) const {
    if (numerCyklu != inny.numerCyklu) return numerCyklu < inny.numerCyklu;
    return numerDniaCyklu < inny.numerDniaCyklu;
}

ostream& operator<<(ostream& os, const WpisDziennika& wpis) {
    os << "[Cykl: " << wpis.numerCyklu << " | Dzień cyklu: " << wpis.numerDniaCyklu << "] | "
       << "Energia: " << wpis.poziomEnergii << "/5 | "
       << "Stres: " << wpis.poziomStresu << "/5 | "
       << "Nastrój: " << wpis.nastroj << " | "
       << "Objawy: " << wpis.objawy << " | "
       << "Notatka: " << wpis.notatka;
    return os;
}