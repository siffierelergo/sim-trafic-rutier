#include "Componente.h"
#include "Teste.h"
#include <thread>
#include <chrono>

void randeazaInterfataGraficaConsola(ReteaRutiera& retea, int nrMasini, const std::vector<int>& ordine) {
    // Curatare ecran standard
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif

    std::cout << "========================================================================\n";
    std::cout << "             C++ TRAFFIC SIMULATION & INTERSECTION CONTROL              \n";
    std::cout << "========================================================================\n";

    char harta[21][21];
    for(int i = 0; i <= 20; i++) {
        for(int j = 0; j <= 20; j++) {
            if (i == 10 || j == 10) harta[i][j] = ' '; // Zona de sosea
            else harta[i][j] = '.'; // Zona de decor
        }
    }

    // Pozitionam semafoarele pe harta cu litere simple
    harta[9][10] = retea.getIntersectie().esteVerde(NORD) ? 'V' : 'R';
    harta[11][10] = retea.getIntersectie().esteVerde(SUD) ? 'V' : 'R';
    harta[10][9] = retea.getIntersectie().esteVerde(VEST) ? 'V' : 'R';
    harta[10][11] = retea.getIntersectie().esteVerde(EST) ? 'V' : 'R';

    // Pozitionam vehiculele in matrice
    for (auto& strada : retea.getStrazi()) {
        for (auto& v : strada.getVehicule()) {
            if (v->getX() >= 0 && v->getX() <= 20 && v->getY() >= 0 && v->getY() <= 20) {
                if (v->getTip() == "Motocicleta") {
                    harta[v->getY()][v->getX()] = 'T';
                } else {
                    harta[v->getY()][v->getX()] = v->getTip()[0]; // 'M' sau 'C'
                }
            }
        }
    }

    // Afisare linie cu linie fara culori sau caractere speciale
    for(int i = 0; i <= 20; i++) {
        for(int j = 0; j <= 20; j++) {
            std::cout << harta[i][j] << " ";
        }
        
        // --- PANOUL LATERAL ULTRA-CURAT ---
        if (i == 1)  std::cout << "    STATUS: Simularea ruleaza in timp real...";
        if (i == 2)  std::cout << "    VEHICULE TOTAL IN RETEA: " << nrMasini;
        if (i == 4)  std::cout << "    STARE SEMAFOARE:";
        if (i == 5)  std::cout << "      Axa N-S: " << (retea.getIntersectie().esteVerde(NORD) ? "VERDE" : "ROSU");
        if (i == 6)  std::cout << "      Axa E-V: " << (retea.getIntersectie().esteVerde(EST) ? "VERDE" : "ROSU");
        if (i == 8)  std::cout << "    REGULA APLICATA: Prioritate de dreapta pe axe active.";
        if (i == 11) std::cout << "    DIAGRAMA FLUX (ORDINE TRECERE CHRONO):";
        if (i == 12) {
            std::cout << "      ";
            if (ordine.empty()) {
                std::cout << "...";
            } else {
                for(int id : ordine) std::cout << "[" << id << "] -> ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "========================================================================\n";
    std::cout << "LEGENDA: M = Masina | C = Camion | T = Motocicleta\n";
    std::cout << "SEMAFOARE: V = Verde | R = Rosu\n";
}

int main() {
    ruleazaTesteUnitare();

    ReteaRutiera retea;
    LoggerTrafic logger;
    retea.ataseazaObserver(&logger);

    int nrMasini;
    std::cout << "Introduceti numarul de masini pentru simulare: ";
    std::cin >> nrMasini;

    std::vector<int> ordineaTrecerii;

    for (int i = 0; i < nrMasini; i++) {
        int optDir, optTip;
        std::cout << "\nVehicul " << i + 1 << " -> Tip (0: Masina, 1: Motocicleta, 2: Camion): ";
        std::cin >> optTip;
        std::cout << "Directie de provenienta (0: Nord, 1: Est, 2: Sud, 3: Vest): ";
        std::cin >> optDir;

        Directie d = static_cast<Directie>(optDir);
        std::shared_ptr<Vehicul> v;

        int startX = 10, startY = 10;
        if (d == NORD) { startX = 10; startY = 4; }
        if (d == SUD)  { startX = 10; startY = 16; }
        if (d == EST)  { startX = 16; startY = 10; }
        if (d == VEST) { startX = 4;  startY = 10; }

        if (optTip == 0)      v = std::make_shared<Masina>(i + 1, d, startX, startY);
        else if (optTip == 1) v = std::make_shared<Motocicleta>(i + 1, d, startX, startY);
        else                  v = std::make_shared<Camion>(i + 1, d, startX, startY);

        if (d == NORD || d == SUD) retea.getStrazi()[0].adaugaVehicul(v);
        else retea.getStrazi()[1].adaugaVehicul(v);
    }

    int cadreSemafor = 0;
    for (int pas = 0; pas < 25; pas++) {
        retea.actualizeazaSimulare();
        
        for(auto& strada : retea.getStrazi()) {
            for(auto& v : strada.getVehicule()) {
                if(v->getX() == 10 && v->getY() == 10) {
                    if(std::find(ordineaTrecerii.begin(), ordineaTrecerii.end(), v->getId()) == ordineaTrecerii.end()) {
                        ordineaTrecerii.push_back(v->getId());
                    }
                }
            }
        }

        randeazaInterfataGraficaConsola(retea, nrMasini, ordineaTrecerii);

        cadreSemafor++;
        if (cadreSemafor >= 5) { 
            retea.getIntersectie().schimbaSemafoare();
            retea.notifica("[SEMAFOR] Schimbare de faza rutiere detectata automat.");
            cadreSemafor = 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "\n[SUCCESS] Simulare incheiata cu succes. Statistici exportate in CSV si TXT.\n";
    return 0;
}