#include "Componente.h"
#include "Teste.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>

// --- CODURI ANSI ---
#define RESET   "\033[0m"
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define BG_GRAY "\033[48;5;235m" 

struct DateInitialeVehicul {
    int id;
    int tip;
    int dir;
};

void randeazaCadru(ReteaRutiera& retea, int iteratie, int nrMasini, const std::vector<int>& ordine) {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif

    std::cout << CYAN << "========================================================================\n" << RESET;
    std::cout << CYAN << "          SIMULARE ANIMATA: ITERATIA [" << iteratie << "] IN DESFASURARE\n" << RESET;
    std::cout << CYAN << "========================================================================\n" << RESET;

    char harta[21][21];
    for(int i = 0; i <= 20; i++) {
        for(int j = 0; j <= 20; j++) {
            if (i == 9 || i == 10 || j == 9 || j == 10) harta[i][j] = ' '; 
            else harta[i][j] = '.'; 
        }
    }

    for (auto& strada : retea.getStrazi()) {
        for (auto& v : strada.getVehicule()) {
            if (v->getX() >= 0 && v->getX() <= 20 && v->getY() >= 0 && v->getY() <= 20) {
                if (v->getTip() == "Motocicleta") harta[v->getY()][v->getX()] = 'T';
                else if (v->getTip() == "Camion") harta[v->getY()][v->getX()] = 'C';
                else harta[v->getY()][v->getX()] = 'M';
            }
        }
    }

    for(int i = 0; i <= 20; i++) {
        for(int j = 0; j <= 20; j++) {
            if (harta[i][j] == 'M') std::cout << BLUE "M " RESET;
            else if (harta[i][j] == 'T') std::cout << MAGENTA "T " RESET;
            else if (harta[i][j] == 'C') std::cout << YELLOW "C " RESET;
            else if (i == 9 || i == 10 || j == 9 || j == 10) std::cout << BG_GRAY "  " RESET;
            else std::cout << ". ";
        }
        
        if (i == 2)  std::cout << "    " YELLOW "SEMNE RUTIERE CURENTE:" RESET;
        if (i == 3) {
            std::cout << "      Nord: ";
            auto s = retea.getStrazi()[0].getSemn();
            if (s == DRUM_CU_PRIORITATE) std::cout << GREEN "PRIORITATE" RESET;
            else if (s == CEDEAZA_TRECEREA) std::cout << YELLOW "CEDEAZA" RESET;
            else std::cout << RED "STOP" RESET;
        }
        if (i == 4) {
            std::cout << "      Est : ";
            auto s = retea.getStrazi()[1].getSemn();
            if (s == DRUM_CU_PRIORITATE) std::cout << GREEN "PRIORITATE" RESET;
            else if (s == CEDEAZA_TRECEREA) std::cout << YELLOW "CEDEAZA" RESET;
            else std::cout << RED "STOP" RESET;
        }
        if (i == 5) {
            std::cout << "      Sud : ";
            auto s = retea.getStrazi()[2].getSemn();
            if (s == DRUM_CU_PRIORITATE) std::cout << GREEN "PRIORITATE" RESET;
            else if (s == CEDEAZA_TRECEREA) std::cout << YELLOW "CEDEAZA" RESET;
            else std::cout << RED "STOP" RESET;
        }
        if (i == 6) {
            std::cout << "      Vest: ";
            auto s = retea.getStrazi()[3].getSemn();
            if (s == DRUM_CU_PRIORITATE) std::cout << GREEN "PRIORITATE" RESET;
            else if (s == CEDEAZA_TRECEREA) std::cout << YELLOW "CEDEAZA" RESET;
            else std::cout << RED "STOP" RESET;
        }
        if (i == 10) std::cout << "    " GREEN ">>> ORDINE TRECERE <<<" RESET;
        if (i == 11) {
            std::cout << "      ";
            if (ordine.empty()) std::cout << "...";
            for(size_t k = 0; k < ordine.size(); k++) {
                std::cout << CYAN "[" << ordine[k] << "]" RESET;
                if (k != ordine.size() - 1) std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
    // CRITIC: std::flush forteaza consola sa deseneze imaginea chiar in acel moment!
    std::cout << CYAN << "========================================================================\n" << RESET << std::flush;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    ruleazaTesteUnitare();

    ReteaRutiera retea;
    LoggerTrafic logger;
    retea.ataseazaObserver(&logger);

    int nrMasini;
    std::cout << "Introduceti numarul de vehicule pentru simulare: ";
    std::cin >> nrMasini;

    std::vector<DateInitialeVehicul> dateStart;
    for (int i = 0; i < nrMasini; i++) {
        int optDir, optTip;
        std::cout << "\nVehicul " << i + 1 << " -> Tip (0: Masina, 1: Motocicleta, 2: Camion): ";
        std::cin >> optTip;
        std::cout << "Directie de provenienta (0: Nord, 1: Est, 2: Sud, 3: Vest): ";
        std::cin >> optDir;
        dateStart.push_back({i + 1, optTip, optDir});
    }

    int nrIteratii;
    std::cout << "\nIntroduceti numarul de iteratii dorit: ";
    std::cin >> nrIteratii;
    
    // CRITIC: Curatam absolut tot bufferul de la tastatura inainte sa incepem
    while(std::cin.get() != '\n');

    for (int pas = 1; pas <= nrIteratii; pas++) {
        
        retea.golesteToateStrazile();
        int spawnCounter[4] = {0, 0, 0, 0}; 

        for (const auto& d : dateStart) {
            Directie dir = static_cast<Directie>(d.dir);
            std::shared_ptr<Vehicul> v;
            
            int x = 10, y = 10;
            if (dir == NORD) { x = 9;  y = 8 - spawnCounter[NORD]++; }
            if (dir == SUD)  { x = 10; y = 11 + spawnCounter[SUD]++; }
            if (dir == EST)  { x = 11 + spawnCounter[EST]++; y = 9; }
            if (dir == VEST) { x = 8 - spawnCounter[VEST]++; y = 10; }

            if (d.tip == 0)      v = std::make_shared<Masina>(d.id, dir, x, y);
            else if (d.tip == 1) v = std::make_shared<Motocicleta>(d.id, dir, x, y);
            else                 v = std::make_shared<Camion>(d.id, dir, x, y);

            retea.getStrazi()[d.dir].adaugaVehicul(v);
        }

        retea.genereazaSemneAleatorii();
        std::vector<int> ordineaTrecerii;

        while(retea.areVehiculeActive()) {
            
            randeazaCadru(retea, pas, nrMasini, ordineaTrecerii);
            
            std::cout << "\n>>> Apasa [ENTER] pentru a permite urmatorului vehicul sa treaca! <<<\n";
            std::string temp;
            std::getline(std::cin, temp);

            retea.actualizeazaCadru(pas, ordineaTrecerii, true);
            randeazaCadru(retea, pas, nrMasini, ordineaTrecerii);
            // Am scazut delay-ul la 100ms ca masina sa fie rapida si fluida
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            for(int k = 0; k < 11; k++) {
                retea.actualizeazaCadru(pas, ordineaTrecerii, false);
                randeazaCadru(retea, pas, nrMasini, ordineaTrecerii);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        randeazaCadru(retea, pas, nrMasini, ordineaTrecerii);
        std::cout << "\n>>> ITERATIA [" << pas << "] S-A INCHEIAT COMPLET.\n";
        std::cout << ">>> Apasati [ENTER] pentru a porni urmatoarea iteratie...\n";
        std::string temp;
        std::getline(std::cin, temp);
    }

    std::cout << "\n[SUCCES] Toate iteratiile au fost finalizate si salvate!\n";
    std::cout << "Apasati ENTER pentru a iesi din simulare.\n";
    std::string tempFinal;
    std::getline(std::cin, tempFinal);
    return 0;
}