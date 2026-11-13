#include <iostream>
#include <vector> // odpowiednik tradycyjnych tablic, który potrafi sam zmieniać swój rozmiar w trakcie działania programu
#include <memory> // inteligentne wskaźniki, make_unique
#include <limits> // kontrola błędów w konsoli
#include "FazaCyklu.hpp"
#include "FazaMenstruacyjna.hpp"
#include "FazaFolikularna.hpp"
#include "FazaOwulacyjna.hpp"
#include "FazaLutealna.hpp"
#include "WpisDziennika.hpp"
using namespace std;

struct ArchiwumCyklu {
    int numerCyklu;
    int dlugoscTrwania; // w dniach
};
// funkcja do czyszczenia bufora wejścia
void wyczyscBufor() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}   
// Klasy pochodne dziedziczą po klasie bazowej FazaCyklu. Zwracamy je przez wskaźnik, dzięki czemu funkcja może dynamicznie zwrócić różne podklasy w zależności od dnia cyklu.
// unique_ptr pilnuje zasady wyłącznej własności - gdy obiekt fazy przestaje być potrzebny lub funkcja kończy działanie, pamięć jest automatycznie zwalniana przez system --> zapobiegamy wyciekom pamięci

// funkcja ma zadeklarowany typ zwracany jako wskaźnik do klasy bazowej
unique_ptr<FazaCyklu> wyznaczFazeAdaptacyjnie(int dzienCyklu, const vector<ArchiwumCyklu>& historiaCykli, const vector<WpisDziennika>& historiaWpisow) {
    
    // 1. bazowa średnia długość cyklu z historii; 28 dni TYLKO jeśli brak danych
    double sredniaDlugoscCyklu = 28.0; 
    if (!historiaCykli.empty()) {
        int sumaDni = 0;
        for (const auto& cykl : historiaCykli) {
            sumaDni += cykl.dlugoscTrwania;
        }
        sredniaDlugoscCyklu = static_cast<double>(sumaDni) / historiaCykli.size(); // static cast dla bardziej precyzyjnej średniej (nie ucina liczby zmiennoprzecinkowej przy dzieleniu)
    }

    // 2. analiza dla konkretnego dnia
    int sumaEnergiiDnia = 0;
    int sumaStresuDnia = 0;
    int liczbaWpisowDlaDnia = 0;
    int liczbaBoluLubKrwawien = 0;
    int liczbaSzczytowFormy = 0;
    int liczbaWysokiegoStresu = 0;
    
    for (const auto& wpis : historiaWpisow) {
        if (wpis.getNumerDniaCyklu() == dzienCyklu) {
            sumaEnergiiDnia += wpis.getPoziomEnergii();
            sumaStresuDnia += wpis.getPoziomStresu();
            liczbaWpisowDlaDnia++;

            string objaw = wpis.getObjawy();
            if (objaw == "skurcze" || objaw == "krwawienie") {
                liczbaBoluLubKrwawien++;}
            if (wpis.getPoziomEnergii() >= 4) liczbaSzczytowFormy++;
            if (wpis.getPoziomStresu() >= 4) liczbaWysokiegoStresu++;
        }
    }

    // 3. Jeśli brak wpisów dla tego dnia, prognozujemy fazę proporcjonalnie do indywidualnej średniej długości cyklu
    if (liczbaWpisowDlaDnia == 0) {
        double proporcjaDnia = static_cast<double>(dzienCyklu) / sredniaDlugoscCyklu;

        if (proporcjaDnia <= 0.14) {
            return make_unique<FazaMenstruacyjna>();
        } else if (proporcjaDnia < 0.48) {
            return make_unique<FazaFolikularna>();
        } else if (proporcjaDnia >= 0.48 && proporcjaDnia <= 0.62) {
            return make_unique<FazaOwulacyjna>();
        } else {
            return make_unique<FazaLutealna>();
        }
    }

    // 4. Jeśli mamy dane z historii, korygujemy prognozę
    // procentowe skalowanie progów faz w oparciu o unikalny rytm użytkowniczki
    double sredniaEnergiaHistoryczna = static_cast<double>(sumaEnergiiDnia) / liczbaWpisowDlaDnia;
    double sredniaStresHistoryczna = static_cast<double>(sumaStresuDnia) / liczbaWpisowDlaDnia;

    if (liczbaBoluLubKrwawien > 0 || sredniaEnergiaHistoryczna <= 2.0) {
        return make_unique<FazaMenstruacyjna>();
    } else if (liczbaSzczytowFormy > 0 && sredniaEnergiaHistoryczna >= 4.0 && sredniaStresHistoryczna < 3.0) {
        return make_unique<FazaOwulacyjna>();
    } else if (sredniaStresHistoryczna >= 4.0 || (static_cast<double>(dzienCyklu) / sredniaDlugoscCyklu) > 0.62) {
        return make_unique<FazaLutealna>();
    } else if (sredniaEnergiaHistoryczna >= 3.0) {
        return make_unique<FazaFolikularna>();
    } else {
        return make_unique<FazaLutealna>();
    }
}