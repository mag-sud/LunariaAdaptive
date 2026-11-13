#include "FazaOwulacyjna.hpp"

std::string FazaOwulacyjna::getNazwaFazy() const {
    return "🌕 Faza Owulacyjna | Pełnia Księżyca (szczyt magnetyzmu i ekspresji)";
}

std::string FazaOwulacyjna::getRekomendowanaDieta() const {
    return "Posiłki wspierające pracę wątroby w metaboliźmie estrogenów (warzywa krzyżowe), bogate w antyoksydanty owoce jagodowe chroniące komórki oraz błonnik. Lekkie i odżywcze posiłki.";
}

std::string FazaOwulacyjna::getRekomendowanyTrening() const {
    return "Maksimum siły i wytrzymałości! Idealny czas na mocny trening siłowy, interwały, dynamiczny taniec lub bieganie. Organizm jest u szczytu swoich możliwości fizycznych.";
}

std::string FazaOwulacyjna::getOpisHormonalny() const {
    return "Szczytowy poziom estrogenów oraz nagły skok hormonów LH i FSH. Maksymalna energia życiowa, wysoka pewność siebie i otwartość na kontakt z ludźmi.";
}

std::string FazaOwulacyjna::getProfilProduktywnosci() const {
    return "Najlepszy moment na wystąpienia publiczne, negocjacje, spotkania zespołu, burze mózgów i kluczowe prezentacje. Komunikacja i charyzma są na najwyższym poziomie.";
}

std::string FazaOwulacyjna::getZiola() const {
    return "Orzeźwiające napary z miętą, hibiskusem lub czarnym bzem.";
}

std::string FazaOwulacyjna::getStylRegeneracji() const {    
    return "Aktywne spędzanie czasu na zewnątrz i spotkania z ludźmi. Regeneracja przez ruch, taniec i ekspresję twórczą.";
}

std::string FazaOwulacyjna::getHolistyczneWsparcie() const {
    return "Faza maksymalnej ekspresji energii. Zadbaj o to, aby ta potężna fala energii znalazła ujście w działaniu, ale pamiętaj też o uziemieniu, by uniknąć przebodźcowania.";
}
std::string FazaOwulacyjna::getWsparcieWyciszajace(const WpisDziennika& wpis) const {
    if (wpis.getPoziomStresu() >= 4) {
        return "Mimo owulacyjnego szczytu energii, odczuwasz silny stres. Zadbaj o krótką przerwę na wyciszenie umysłu, oddychanie przeponowe i unikaj nadmiaru bodźców społecznych.";
    }
    return "Twoja energia jest na szczycie! Ciesz się lekkością ruchu, ale pamiętaj o chwili na świadomy oddech, by utrzymać wewnętrzny balans.";
}