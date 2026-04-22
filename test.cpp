#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <ctime>   // Pentru time()
#include <cstdlib> // Pentru rand() si srand()

using namespace std;

enum Directie { NORD = 0, EST = 1, SUD = 2, VEST = 3 };

struct Masina {
    int id;
    string numeDirectie;
};

// Functie pentru a genera semne rutiere aleatorii
void genereazaSemneRutiere() {
    string semne[] = {
        "Circulatie Normala", 
        "Drum in Lucru (Viteza Redusa)", 
        "Atentie: Polei!", 
        "Prioritate pentru Transport Public"
    };
    string directii[] = {"Nord", "Est", "Sud", "Vest"};

    cout << "=== CONDITII RUTIERE GENERATE ===\n";
    for(int i = 0; i < 4; i++) {
        int r = rand() % 4; // Alege un semn la intamplare din cele 4 de mai sus
        cout << "Directia " << directii[i] << ": " << semne[r] << endl;
    }
    cout << "=================================\n\n";
}

int main() {
    // Initializam generatorul de numere aleatorii cu timpul curent
    srand(static_cast<unsigned int>(time(0)));

    // Pasul 1: Afisam semnele rutiere inainte de orice
    genereazaSemneRutiere();

    // Pasul 2: Introducere date de la utilizator
    int nrMasini;
    vector<queue<Masina>> benzi(4);
    vector<int> ordineaTrecerii;

    cout << "Introduceti numarul total de masini: ";
    cin >> nrMasini;

    for (int i = 0; i < nrMasini; i++) {
        int opt;
        cout << "Masina " << i + 1 << " vine de la (0:Nord, 1:Est, 2:Sud, 3:Vest): ";
        cin >> opt;
        
        if (opt < 0 || opt > 3) {
            cout << "Directie invalida! Reincercati.\n";
            i--;
            continue;
        }

        string numeDir = (opt == 0) ? "Nord" : (opt == 1) ? "Est" : (opt == 2) ? "Sud" : "Vest";
        benzi[opt].push({i + 1, numeDir});
    }

    // Pasul 3: Logica de semaforizare (N-S apoi E-V)
    int faza = 0; 
    while (!(benzi[0].empty() && benzi[1].empty() && benzi[2].empty() && benzi[3].empty())) {
        if (faza == 0) {
            cout << "\n[SEMAFOR] VERDE: NORD si SUD\n";
            int axe[] = {0, 2};
            for (int d : axe) {
                while(!benzi[d].empty()){
                    cout << " >> Masina " << benzi[d].front().id << " a plecat din " << benzi[d].front().numeDirectie << endl;
                    ordineaTrecerii.push_back(benzi[d].front().id);
                    benzi[d].pop();
                }
            }
            faza = 1;
        } else {
            cout << "\n[SEMAFOR] VERDE: EST si VEST\n";
            int axe[] = {1, 3};
            for (int d : axe) {
                while(!benzi[d].empty()){
                    cout << " >> Masina " << benzi[d].front().id << " a plecat din " << benzi[d].front().numeDirectie << endl;
                    ordineaTrecerii.push_back(benzi[d].front().id);
                    benzi[d].pop();
                }
            }
            faza = 0;
        }
    }

    cout << "\nORDINE FINALA: ";
    for (size_t i = 0; i < ordineaTrecerii.size(); i++) {
        cout << ordineaTrecerii[i] << (i == ordineaTrecerii.size() - 1 ? "" : " -> ");
    }
    cout << endl;

    return 0;
}