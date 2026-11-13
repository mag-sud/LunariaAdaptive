#include "FazaMenstruacyjna.hpp"

std::string FazaMenstruacyjna::getNazwaFazy() const {
    return "🌑 Faza Menstruacyjna | Nów Księżyca (czas introspekcji i azylu)";
}

std::string FazaMenstruacyjna::getRekomendowanaDieta() const {
    return "Ciepłe, gotowane i łatwostrawne posiłki (np. buliony, zupy krem, kleiki, pieczone lub gotowane warzywa korzeniowe). Unikaj surowych sałatek, zimnych potraw i napojów, które wychładzają organizm od środka. Zadbaj o odpowiednie uzupełnienie żelaza.";
}

std::string FazaMenstruacyjna::getRekomendowanyTrening() const {
    return "Głęboki odpoczynek i regeneracja. Dopuszczalne są jedynie bardzo delikatne spacery, rozciąganie niskointensywne lub joga yin. Zrezygnuj z ciężkich treningów siłowych i intensywnego cardio.";
}

std::string FazaMenstruacyjna::getOpisHormonalny() const {
    return "Gwałtowny spadek poziomów estrogenów i progesteronu. Niski poziom energii na początku fazy, organizm naturalnie domaga się zwolnienia tempa.";
}

std::string FazaMenstruacyjna::getProfilProduktywnosci() const {
    return "Faza introspekcji, refleksji i intuicji. Doskonały czas na planowanie strategiczne, podsumowania, pisanie i spokojną pracę analityczną w odosobnieniu.";
}

std::string FazaMenstruacyjna::getZiola() const {
    return "Rozgrzewające napary z dodatkiem świeżego imbiru, cynamonu, liści malin, krwawnika lub melisy. Korzenne przyprawy wspierają krążenie krwi.";
}

std::string FazaMenstruacyjna::getStylRegeneracji() const {
    return "Wydłużony sen (minimum 8-9 godzin), techniki oddechowe relaksacyjne, unikanie stresu i termofor przykładany w okolice podbrzusza i/lub na wysokości odcinka krzyżowego (ogonowego).";
}

std::string FazaMenstruacyjna::getHolistyczneWsparcie() const {
    return "Ochrona dolnej partii pleców i brzucha przed wychłodzeniem. Wieczorne, ciepłe kąpiele stóp z dodatkiem imbiru lub soli, które pomagają rozproszyć zastój krwi, ogrzewają meridiany i ułatwiają zasypianie. Akceptacja potrzeby zwolnienia tempa i wsłuchanie się w sygnały ciała.";
}
std::string FazaMenstruacyjna::getWsparcieWyciszajace(const WpisDziennika& wpis) const {
    if (wpis.objawyMenstruacyjne() || wpis.getPoziomStresu() >= 4) {
        return "Masz objawy menstruacyjne i podwyższony stres. Zastosuj ciepły kompres, połóż się wygodnie, zrezygnuj z obowiązków niecierpiących zwłoki i sięgnij po napar rozgrzewający.";
    }
    return "Pamiętaj o łagodnym traktowaniu siebie. Ciepła kąpiel, herbata z imbirem i spokojna muzyka pomogą Ci przetrwać ten czas w komforcie.";
}