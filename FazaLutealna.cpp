#include "FazaLutealna.hpp"

std::string FazaLutealna::getNazwaFazy() const {
    return "🌘 Faza Lutealna | Księżyc Ubywający (wyciszenie)";
}

std::string FazaLutealna::getRekomendowanaDieta() const {
    return "Ciepłe, sycące posiłki bogate w złożone węglowodany oraz magnez (pełnoziarniste produkty, korzeniowe warzywa), które stabilizują poziom glukozy we krwi i zapobiegają nagłym zachciankom na słodycze.";
}

std::string FazaLutealna::getRekomendowanyTrening() const {
    return "Stopniowe zwalnianianie tempa. Zamiast intensywnego cardio wybieraj umiarkowany ruch: spacery na świeżym powietrzu, pilates, spokojna joga lub lżejszy trening siłowy w miarę spadku energii, który jest normalny w tej fazie cyklu.";
}

std::string FazaLutealna::getOpisHormonalny() const {
    return "Wzrost i późniejszy gwałtowny spadek progesteronu. Możliwe wahania nastroju, spadek energii fizycznej, zatrzymywanie wody w organizmie oraz objawy PMS pod koniec fazy.";
}

std::string FazaLutealna::getProfilProduktywnosci() const {
    return "Faza doskonała do domykania otwartych projektów, uporządkowania zadań, pracy w samotności i działań wymagających skrupulatności, ale bez forsowania się na siłę.";
}

std::string FazaLutealna::getZiola() const {
    return "Napary z liści malin, melisy lub rumianku, które koją układ nerwowy i łagodzą napięcie przedmiesiączkowe.";
}

std::string FazaLutealna::getStylRegeneracji() const {
    return "Świadome zwolnienie tempa, dbałość o komfort psychiczny i redukcja nadmiaru bodźców zewnętrznych.";
}

std::string FazaLutealna::getHolistyczneWsparcie() const {
    return "Budowanie wewnętrznej wyrozumiałości i dbanie o komfort psychiczny. Unikanie zimnych, surowych pokarmów na rzecz ciepłych, pieczonych i gęstych dań, które uziemiają i dają poczucie wewnętrznego bezpieczeństwa.";
}
std::string FazaLutealna::getWsparcieWyciszajace(const WpisDziennika& wpis) const {
    if (wpis.getPoziomStresu() >= 4 || wpis.getPoziomEnergii() <= 2) {
        return "Faza lutealna bywa trudna, a Twój dzisiejszy wpis wskazuje na duże obciążenie. Ogranicz kofeinę, zaparz melisę, zrób sobie wieczór bez ekranów i odpuść wszystko, co może poczekać.";
    }
    return "Zadbaj dzisiaj o łagodne tempo. Ciepła kąpiel, spacery na świeżym powietrzu i unikanie dodatkowego stresu pomogą Ci utrzymać równowagę emocjonalną";
}