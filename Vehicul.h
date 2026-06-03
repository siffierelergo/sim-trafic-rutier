#ifndef VEHICUL_H
#define VEHICUL_H

#include <string>
#include <iostream>

enum Directie { NORD = 0, EST = 1, SUD = 2, VEST = 3 };

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

    virtual std::string getTip() const = 0;
    virtual void reactioneazaLaIntersectie(bool esteVerde) = 0;

    // Gettere
    int getId() const { return id; }
    Directie getDirectie() const { return directie; }
    int getX() const { return pozitieX; }
    int getY() const { return pozitieY; }
    int getViteza() const { return vitezaMaxima; } // <-- Aceasta era linia lipsa!
    
    // Logica de miscare cadru cu cadru
    void deplaseaza() {
        if (directie == NORD) pozitieY++;      // Vine de sus, coboara
        else if (directie == SUD) pozitieY--;  // Vine de jos, urca
        else if (directie == EST) pozitieX--;  // Vine din dreapta, merge stanga
        else if (directie == VEST) pozitieX++; // Vine din stanga, merge dreapta
    }
};

class Masina : public Vehicul {
public:
    Masina(int id, Directie dir, int x, int y) : Vehicul(id, 50, dir, x, y) {}
    std::string getTip() const override { return "Masina"; }
    void reactioneazaLaIntersectie(bool ok) override {}
};

class Motocicleta : public Vehicul {
public:
    Motocicleta(int id, Directie dir, int x, int y) : Vehicul(id, 70, dir, x, y) {}
    std::string getTip() const override { return "Motocicleta"; }
    void reactioneazaLaIntersectie(bool ok) override {}
};

class Camion : public Vehicul {
public:
    Camion(int id, Directie dir, int x, int y) : Vehicul(id, 30, dir, x, y) {}
    std::string getTip() const override { return "Camion"; }
    void reactioneazaLaIntersectie(bool ok) override {}
};

#endif