#ifndef TESTE_H
#define TESTE_H

#include "Vehicul.h"
#include <cassert>
#include <iostream>

// --- CERINTA OBLIGATORIE: TESTE UNITARE ---
void ruleazaTesteUnitare() {
    std::cout << "[TEST] Initiere teste unitare...\n";

    // Test 1: Validare deplasare vehicul
    Masina m(999, VEST, 5, 5);
    m.deplaseaza();
    assert(m.getX() == 6 && m.getY() == 5); // Trebuie sa se miste la dreapta pe axa X

    // Test 2: Validare comportament polimorfism viteza
    Motocicleta moto(888, NORD, 0, 0);
    Camion cam(777, NORD, 0, 0);
    assert(moto.getViteza() > cam.getViteza());

    std::cout << "[TEST] Toate testele unitare au trecut cu succes! (assert-uri valide)\n\n";
}

#endif