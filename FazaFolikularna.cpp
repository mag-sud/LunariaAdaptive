#include "FazaFolikularna.hpp"

std::string FazaFolikularna::getNazwaFazy() const {
    return "🌒 Faza Folikularna | Księżyc Przybywający (narodziny energii)";
}

std::string FazaFolikularna::getRekomendowanaDieta() const {
    return "Lżejsze, świeże posiłki bogate w białko, kiełki, warzywa liściaste oraz produkty wspierające odbudowę żelaza i mikroelementów po krwawieniu.";
}

std::string FazaFolikularna::getRekomendowanyTrening() const {
    return "Wzrost siły i energii! Doskonały moment na intensywniejszy trening siłowy, bieganie, naukę nowych umiejętności ruchowych czy zajęcia taneczne. Organizm świetnie buduje masę mięśniową.";
}

std::string FazaFolikularna::getOpisHormonalny() const {
    return "Stopniowy wzrost poziomu estrogenów. Poprawa nastroju, wzrost motywacji, jasność umysłu i kumulacja fizycznej energii.";
}

std::string FazaFolikularna::getProfilProduktywnosci() const {
    return "Czas wysokiej kreatywności, burz mózgów, planowania wielkich projektów i nauki nowych, złożonych tematów. Planowanie nowych celów, skupienie na rozwoju osobistym.";
}

std::string FazaFolikularna::getZiola() const {
    return "Świeże napary z miętą, zielona herbata lub pokrzywa wspierająca układ krwioobiegu.";
}

std::string FazaFolikularna::getStylRegeneracji() const {
    return "Standardowa, zrównoważona regeneracja. Energia rozpycha się w ciele, więc sen może być nieco krótszy, ale warto zadbać o relaks w ruchu (spacery, taniec).";
}

std::string FazaFolikularna::getHolistyczneWsparcie() const {
    return "Wzmacnianie energii oraz budowanie nowej 'Krwi' po okresie menstruacji. Czas otwierania się na świat, czy budowania relacji.";
}

// element adaptacyjny
std::string FazaFolikularna::getWsparcieWyciszajace(const WpisDziennika& wpis) const {
    if (wpis.getPoziomStresu() >= 4 || wpis.getPoziomEnergii() <= 2) {
        return "Mimo że trwa faza folikularna, Twój dzisiejszy stres lub zmęczenie są wysokie. Odpuść ambitne zadania, zrób przerwę od ekranu i postaw na głęboki oddech.";
    }
    // Standardowa porada dla tej fazy, gdy parametry są w normie:
    return "Masz teraz naturalny wzrost energii. To świetny moment na realizację zadań i lżejszą aktywność - ciesz się tym przypływem sił!";
}