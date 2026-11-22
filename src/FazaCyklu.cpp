#include "FazaCyklu.hpp"
#include <utility>

// Baza przechowuje nazwę fazy, a każda pochodna przekazuje ją przez listę inicjalizacyjną.

FazaCyklu::FazaCyklu(std::string nazwa) : nazwaFazy(std::move(nazwa)) {}

std::string FazaCyklu::getNazwaFazy() const {
    return nazwaFazy;
}