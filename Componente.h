#ifndef COMPONENTE_H
#define COMPONENTE_H

#include "Vehicul.h"
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <cstdlib>

enum SemnRutier { DRUM_CU_PRIORITATE = 0, CEDEAZA_TRECEREA = 1, STOP = 2 };

class IObserver {
public:
    virtual void peEveniment(const std::string& eveniment) = 0;
    virtual ~IObserver() {}
};

class LoggerTrafic : public IObserver {
private:
    std::ofstream fisierLog;
    std::ofstream fisierCSV;
public:
    LoggerTrafic() {
        fisierLog.open("trafic_log.txt", std::ios::app);
        fisierCSV.open("statistici_trafic.csv", std::ios::app);
        std::ifstream test("statistici_trafic.csv");
        if (test.peek() == std::ifstream::traits_type::eof()) {
            fisierCSV << "Iteratie,Eveniment,Mesaj\n";
        }
    }
    ~LoggerTrafic() {
        if (fisierLog.is_open()) fisierLog.close();
        if (fisierCSV.is_open()) fisierCSV.close();
    }
    void peEveniment(const std::string& eveniment) override {
        if (fisierLog.is_open()) fisierLog << eveniment << "\n"; 
        if (fisierCSV.is_open()) fisierCSV << eveniment << "\n"; 
    }
};

class Strada {
private:
    std::string nume;
    std::vector<std::shared_ptr<Vehicul>> vehicule;
    SemnRutier semnCurent;

public:
    Strada(std::string nume) : nume(nume), semnCurent(CEDEAZA_TRECEREA) {}
    void adaugaVehicul(std::shared_ptr<Vehicul> v) { vehicule.push_back(v); }
    std::vector<std::shared_ptr<Vehicul>>& getVehicule() { return vehicule; }
    void goleste() { vehicule.clear(); }
    
    void setSemn(SemnRutier s) { semnCurent = s; }
    SemnRutier getSemn() const { return semnCurent; }
    
    std::string getSemnString() const {
        if (semnCurent == DRUM_CU_PRIORITATE) return "PRIORITATE";
        if (semnCurent == CEDEAZA_TRECEREA) return "CEDEAZA";
        return "STOP";
    }
};

class ReteaRutiera {
private:
    std::vector<Strada> strazi; 
    std::vector<IObserver*> observeri;

public:
    ReteaRutiera() {
        strazi.push_back(Strada("Nord"));
        strazi.push_back(Strada("Est"));
        strazi.push_back(Strada("Sud"));
        strazi.push_back(Strada("Vest"));
    }

    void ataseazaObserver(IObserver* obs) { observeri.push_back(obs); }
    void notifica(int iteratie, const std::string& actiune, const std::string& detalii) {
        std::string csvFormat = std::to_string(iteratie) + ",\"" + actiune + "\",\"" + detalii + "\"";
        for (auto obs : observeri) obs->peEveniment(csvFormat);
    }

    std::vector<Strada>& getStrazi() { return strazi; }
    void golesteToateStrazile() { for (auto& s : strazi) s.goleste(); }

    void genereazaSemneAleatorii() {
        for (int i = 0; i < 4; i++) {
            SemnRutier s = static_cast<SemnRutier>(rand() % 3);
            strazi[i].setSemn(s);
        }
    }

    bool areVehiculeActive() {
        for (auto& s : strazi) if (!s.getVehicule().empty()) return true;
        return false;
    }

    // Verifica daca in acest moment coordonata (x,y) este blocata de alta masina
    bool pozitieOcupata(int x, int y) {
        for (auto& s : strazi) {
            for (auto& v : s.getVehicule()) {
                if (v->getX() == x && v->getY() == y) return true;
            }
        }
        return false;
    }

    void actualizeazaCadru(int iteratie, std::vector<int>& ordineaTrecerii, bool permiteTrecereNoua) {
        std::vector<std::shared_ptr<Vehicul>> masiniLaStop;
        
        for (auto& s : strazi) {
            for (auto& v : s.getVehicule()) {
                if ((v->getX() == 9  && v->getY() == 8  && v->getDirectie() == NORD) ||
                    (v->getX() == 10 && v->getY() == 11 && v->getDirectie() == SUD) ||
                    (v->getX() == 11 && v->getY() == 9  && v->getDirectie() == EST) ||
                    (v->getX() == 8  && v->getY() == 10 && v->getDirectie() == VEST)) {
                    masiniLaStop.push_back(v);
                }
            }
        }

        std::shared_ptr<Vehicul> celCareTrece = nullptr;
        bool intersectieOcupata = pozitieOcupata(9, 9) || pozitieOcupata(9, 10) || pozitieOcupata(10, 9) || pozitieOcupata(10, 10);
        if (permiteTrecereNoua && !masiniLaStop.empty() && !intersectieOcupata) {
            for (auto& v : masiniLaStop) {
                if (strazi[v->getDirectie()].getSemn() == DRUM_CU_PRIORITATE) {
                    celCareTrece = v;
                    break;
                }
            }
            if (celCareTrece == nullptr) celCareTrece = masiniLaStop.front(); 
        }

        for (int i = 0; i < 4; i++) {
            auto& vehicule = strazi[i].getVehicule();
            for (auto it = vehicule.begin(); it != vehicule.end(); ) {
                auto& v = *it;
                int nextX = v->getX();
                int nextY = v->getY();

                if (v->getDirectie() == NORD) nextY++;
                else if (v->getDirectie() == SUD) nextY--;
                else if (v->getDirectie() == EST) nextX--;
                else if (v->getDirectie() == VEST) nextX++;

                bool eLaLiniaDeStop = (v->getX() == 9  && v->getY() == 8  && v->getDirectie() == NORD) ||
                                      (v->getX() == 10 && v->getY() == 11 && v->getDirectie() == SUD) ||
                                      (v->getX() == 11 && v->getY() == 9  && v->getDirectie() == EST) ||
                                      (v->getX() == 8  && v->getY() == 10 && v->getDirectie() == VEST);

                bool poateInainta = true;

                if (eLaLiniaDeStop) {
                    if (v != celCareTrece) {
                        poateInainta = false; 
                    } else {
                        ordineaTrecerii.push_back(v->getId());
                        notifica(iteratie, "TRECERE", "Vehicul " + std::to_string(v->getId()) + " a patruns in intersectie.");
                    }
                } else if (pozitieOcupata(nextX, nextY)) {
                    poateInainta = false; // Sistem de coada perfect: masina asteapta sa plece cea din fata ei
                }

                if (poateInainta) v->deplaseaza();

                // NOUA REGULA DE STERGERE: Se sterge STRICT cand iese de pe ecran prin partea opusa
                bool aIesit = false;
                if (v->getDirectie() == NORD && v->getY() > 20) aIesit = true;
                if (v->getDirectie() == SUD && v->getY() < 0) aIesit = true;
                if (v->getDirectie() == EST && v->getX() < 0) aIesit = true;
                if (v->getDirectie() == VEST && v->getX() > 20) aIesit = true;

                if (aIesit) {
                    it = vehicule.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
};

#endif