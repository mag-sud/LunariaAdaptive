#include <iostream>
#include "WpisDziennika.hpp"
using namespace std;

int main() {
    system("chcp 65001 > nul"); // polskie znaki
    cout << "=== TEST KLASY WPIS DZIENNIKA ===" << endl;
    WpisDziennika dzisiejszyWpis(5, 4, 2, "energiczny", "brak", "Bardzo udany dzień!");
    cout << dzisiejszyWpis << endl;

    return 0;
}