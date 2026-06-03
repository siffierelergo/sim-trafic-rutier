#ifndef COMPONENTE_H
#define COMPONENTE_H

#include "Vehicul.h"
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>

// --- FACULTATIV: PATTERN OBSERVER PENTRU LOGGING ---
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
        // Headere CSV dacă e gol
        std::ifstream test("statistici_trafic.csv");
        if (test.peek() == std::ifstream::traits_type::eof()) {
            fisierCSV << "Eveniment,Mesaj\n";
        }
    }
    ~LoggerTrafic() {
        if (fisierLog.is_open()) fisierLog.close();
        if (fisierCSV.is_open()) fisierCSV.close();
    }
    void peEveniment(const std::string& eveniment) override {
        std::cout << eveniment << "\n"; // Afisare consola
        if (fisierLog.is_open()) fisierLog << eveniment << "\n"; // Salvare text
        if (fisierCSV.is_open()) fisierCSV << "Eveniment,\"" << eveniment << "\"\n"; // Export CSV
    }
};

// --- COMPOZITIE: INTERSECTIE ---
class Intersectie {
private:
    bool semafoare[4]; // N, E, S, V

public:
    Intersectie() {
        semafoare[NORD] = true;  semafoare[SUD] = true;
        semafoare[EST] = false; semafoare[VEST] = false;
    }

    void schimbaSemafoare() {
        semafoare[NORD] = !semafoare[NORD];
        semafoare[SUD] = !semafoare[SUD];
        semafoare[EST] = !semafoare[EST];
        semafoare[VEST] = !semafoare[VEST];
    }

    bool esteVerde(Directie dir) const { return semafoare[dir]; }
};

// --- COMPOZITIE: STRADA ---
class Strada {
private:
    std::string nume;
    std::vector<std::shared_ptr<Vehicul>> vehicule;

public:
    Strada(std::string nume) : nume(nume) {}
    void adaugaVehicul(std::shared_ptr<Vehicul> v) { vehicule.push_back(v); }
    std::vector<std::shared_ptr<Vehicul>>& getVehicule() { return vehicule; }
    std::string getNume() const { return nume; }
};

// --- COMPOZITIE: RETEA RUTIERA (Clasa principala) ---
class ReteaRutiera {
private:
    std::vector<Strada> strazi;
    Intersectie intersectie;
    std::vector<IObserver*> observeri;

public:
    ReteaRutiera() {
        strazi.push_back(Strada("Axa Nord-Sud"));
        strazi.push_back(Strada("Axa Est-Vest"));
    }

    void ataseazaObserver(IObserver* obs) { observeri.push_back(obs); }
    
    void notifica(const std::string& mesaj) {
        for (auto obs : observeri) obs->peEveniment(mesaj);
    }

    Intersectie& getIntersectie() { return intersectie; }
    std::vector<Strada>& getStrazi() { return strazi; }

    void actualizeazaSimulare() {
        // Verificare coliziuni si deplasare
        std::vector<std::shared_ptr<Vehicul>> toate;
        for (auto& strada : strazi) {
            for (auto& v : strada.getVehicule()) toate.push_back(v);
        }

        // Verificare coliziuni (cerinta obligatorie logare)
        for (size_t i = 0; i < toate.size(); i++) {
            for (size_t j = i + 1; j < toate.size(); j++) {
                if (toate[i]->getX() == toate[j]->getX() && toate[i]->getY() == toate[j]->getY()) {
                    notifica("[COLIZIUNE CRITICA] Vehiculul " + std::to_string(toate[i]->getId()) + 
                             " s-a ciocnit cu " + std::to_string(toate[j]->getId()) + "!");
                }
            }
        }

        // Deplasare logica cu respectarea regulilor de circulatie
        for (auto& v : toate) {
            bool poateInainta = true;
            
            // Simulam zona de intersectie la coordonatele (10, 10)
            if (v->getX() == 10 && v->getY() == 9 && v->getDirectie() == SUD && !intersectie.esteVerde(SUD)) poateInainta = false;
            if (v->getX() == 10 && v->getY() == 11 && v->getDirectie() == NORD && !intersectie.esteVerde(NORD)) poateInainta = false;
            if (v->getX() == 9 && v->getY() == 10 && v->getDirectie() == EST && !intersectie.esteVerde(EST)) poateInainta = false;
            if (v->getX() == 11 && v->getY() == 10 && v->getDirectie() == VEST && !intersectie.esteVerde(VEST)) poateInainta = false;

            if (poateInainta) {
                int vechiX = v->getX(), vechiY = v->getY();
                v->deplaseaza();
                // Logare intrare/iesire din intersectie
                if (v->getX() == 10 && v->getY() == 10) {
                    notifica("[INTRARE INTERSECTIE] " + v->getTip() + " " + std::to_string(v->getId()) + " a intrat in intersectie.");
                } else if (vechiX == 10 && vechiY == 10) {
                    notifica("[IESIRE INTERSECTIE] " + v->getTip() + " " + std::to_string(v->getId()) + " a parasit intersectie.");
                }
            } else {
                v->reactioneazaLaIntersectie(false);
            }
        }
    }
};

#endif