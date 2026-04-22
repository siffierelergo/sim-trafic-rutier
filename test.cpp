#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// Definim directiile pentru claritate
enum Directie { NORD = 0, EST = 1, SUD = 2, VEST = 3 };

struct Masina {
    int id;
    string numeDirectie;
    int codDirectie;
};

string getNumeDirectie(int optiune) {
    switch(optiune) {
        case 0: return "Nord";
        case 1: return "Est";
        case 2: return "Sud";
        case 3: return "Vest";
        default: return "Necunoscut";
    }
}

int main() {
    int nrMasini;
    vector<queue<Masina>> benzi(4);
    vector<int> ordineaTrecerii;

    cout << "=== SIMULARE INTERSECTIE C++ ===\n";
    cout << "Introduceti numarul total de masini: ";
    cin >> nrMasini;

    for (int i = 0; i < nrMasini; i++) {
        int opt;
        cout << "Masina " << i + 1 << " vine de la (0:Nord, 1:Est, 2:Sud, 3:Vest): ";
        cin >> opt;
        
        if (opt < 0 || opt > 3) {
            cout << "Directie invalida! Incercati din nou.\n";
            i--;
            continue;
        }

        Masina m = {i + 1, getNumeDirectie(opt), opt};
        benzi[opt].push(m);
    }

    cout << "\n--- PROCESARE TRAFIC (Regula de Dreapta & Semafoare) ---\n";
    
    // Simulam ciclurile semaforului pana cand toate benzile sunt goale
    // Ciclul 1: Nord si Sud au Verde | Ciclul 2: Est si Vest au Verde
    int fazaSemafor = 0; // 0 pentru N-S, 1 pentru E-V

    while (!(benzi[0].empty() && benzi[1].empty() && benzi[2].empty() && benzi[3].empty())) {
        bool aMiscatCineva = false;

        if (fazaSemafor == 0) {
            cout << "\n[SEMAFOR] VERDE pentru axa NORD-SUD\n";
            // Procesam Nord si Sud
            int axe[] = {0, 2}; // NORD si SUD
            for (int dir : axe) {
                while (!benzi[dir].empty()) {
                    // Verificam regula de dreapta: 
                    // Daca suntem la Nord (0), dreapta este Est (1). 
                    // Daca Estul are masina, Nordul asteapta (daca ar fi nesemaforizat), 
                    // dar aici semaforul Est-Vest este ROSU, deci Nord poate trece.
                    Masina m = benzi[dir].front();
                    cout << " >> Masina " << m.id << " a plecat de la " << m.numeDirectie << endl;
                    ordineaTrecerii.push_back(m.id);
                    benzi[dir].pop();
                    aMiscatCineva = true;
                }
            }
            fazaSemafor = 1; // Schimbam faza
        } else {
            cout << "\n[SEMAFOR] VERDE pentru axa EST-VEST\n";
            // Procesam Est si Vest
            int axe[] = {1, 3}; // EST si VEST
            for (int dir : axe) {
                while (!benzi[dir].empty()) {
                    Masina m = benzi[dir].front();
                    cout << " >> Masina " << m.id << " a plecat de la " << m.numeDirectie << endl;
                    ordineaTrecerii.push_back(m.id);
                    benzi[dir].pop();
                    aMiscatCineva = true;
                }
            }
            fazaSemafor = 0; // Schimbam faza
        }
    }

    // Rezultat Final
    cout << "\n====================================\n";
    cout << "ORDINEA FINALA A TRECERII: ";
    for (int i = 0; i < ordineaTrecerii.size(); i++) {
        cout << ordineaTrecerii[i] << (i == ordineaTrecerii.size() - 1 ? "" : " -> ");
    }
    cout << "\n====================================\n";

    return 0;
}