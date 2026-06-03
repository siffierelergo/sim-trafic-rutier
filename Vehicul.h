#ifndef VEHICUL_H
#define VEHICUL_H

#include <string>
#include <iostream>

enum Directie { NORD = 0, EST = 1, SUD = 2, VEST = 3 };

// --- CLASA ABSTRACTA ---
class Vehicul {
protected:
    int id;
    int vitezaMaxima;
    Directie directie;
    int pozitieX, pozitieY;

public:
    Vehicul(int id, int viteza, Directie dir, int x, int y) 
        : id(id), vitezaMaxima(viteza), directie(dir), pozitieX(x), pozitieY(y) {}
    
    virtual ~Vehicul() {}

    // Metode virtuale pure (Polimorfism)
    virtual std::string getTip() const = 0;
    virtual void reactioneazaLaIntersectie(bool esteVerde) = 0;

    // Gettere si Settere
    int getId() const { return id; }
    int getViteza() const { return vitezaMaxima; }
    Directie getDirectie() const { return directie; }
    int getX() const { return pozitieX; }
    int getY() const { return pozitieY; }
    
    void setPozitie(int x, int y) { pozitieX = x; pozitieY = y; }
    
    void deplaseaza() {
        if (directie == NORD) pozitieY--;
        else if (directie == SUD) pozitieY++;
        else if (directie == EST) pozitieX++;
        else if (directie == VEST) pozitieX--;
    }
};

// --- CLASE DERIVATE ---
class Masina : public Vehicul {
public:
    Masina(int id, Directie dir, int x, int y) : Vehicul(id, 50, dir, x, y) {}
    std::string getTip() const override { return "Masina"; }
    void reactioneazaLaIntersectie(bool esteVerde) override {
        if (!esteVerde) std::cout << "[LOG] Masina " << id << " franeaza lin la rosu.\n";
    }
};

class Motocicleta : public Vehicul {
public:
    Motocicleta(int id, Directie dir, int x, int y) : Vehicul(id, 70, dir, x, y) {}
    std::string getTip() const override { return "Motocicleta"; }
    void reactioneazaLaIntersectie(bool esteVerde) override {
        if (!esteVerde) std::cout << "[LOG] Motocicleta " << id << " se opreste brusc.\n";
    }
};

class Camion : public Vehicul {
public:
    Camion(int id, Directie dir, int x, int y) : Vehicul(id, 30, dir, x, y) {}
    std::string getTip() const override { return "Camion"; }
    void reactioneazaLaIntersectie(bool esteVerde) override {
        if (!esteVerde) std::cout << "[LOG] Camionul " << id << " are inertie mare! Opreste lent.\n";
    }
};

#endif