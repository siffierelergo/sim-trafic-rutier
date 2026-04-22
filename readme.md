# 🚦 Simularea de Trafic Rutier (Intersecție)

O aplicație consolă dezvoltată în **C++** care simulează fluxul de vehicule într-o intersecție cu patru sensuri (**Nord, Sud, Est, Vest**), respectând regulile de semaforizare și prioritățile de circulație.

## 📝 Descriere
Acest proiect gestionează dinamica unei intersecții prin implementarea unui sistem de cozi pentru vehicule. Programul colectează date de la utilizator despre numărul de mașini și poziția acestora, apoi calculează ordinea de trecere bazată pe alternarea semafoarelor și regulile de prioritate.

### Caracteristici Principale:
* **Sistem de Cozi (FIFO):** Mașinile sunt procesate în ordinea sosirii pe banda respectivă.
* **Semaforizare pe Axe:** Simulează realitatea prin activarea culorii verzi pentru axele complementare (Nord-Sud sau Est-Vest).
* **Regula de Dreapta:** Logica de verificare a priorității între benzile active.
* **Interfață Interactivă:** Utilizatorul configurează numărul de mașini și locația lor în timp real.

## 🛠️ Tehnologii Utilizate
* **Limbaj:** C++11 / C++17
* **Biblioteci Standard:** `<vector>`, `<queue>`, `<string>`, `<iostream>`, `<thread>`
* **Model AI:** Dezvoltat în colaborare cu **Gemini 3 Flash (Paid Tier)** — versiunea curentă (2026), optimizată pentru generare de cod robust și arhitectură logică.

## 🚀 Cum funcționează
1.  **Input:** Utilizatorul introduce numărul total de vehicule.
2.  **Configurare:** Pentru fiecare mașină, se alege direcția de proveniență (0: Nord, 1: Est, 2: Sud, 3: Vest).
3.  **Simulare:**
    * Semaforul începe cu axa **Nord-Sud**. Toate mașinile din aceste direcții sunt procesate.
    * Semaforul se schimbă pentru axa **Est-Vest**.
    * Procesul continuă ciclic până când toate benzile sunt goale.
4.  **Output:** Programul afișează un jurnal al plecărilor și o listă finală cu ordinea exactă în care mașinile au părăsit intersecția.

## 💻 Instalare și Rulare
1.  Asigurați-vă că aveți un compilator de C++ instalat (GCC, Clang sau MSVC).
2.  Salvați codul într-un fișier numit `main.cpp`.
3.  Compilați codul folosind terminalul:
    ```bash
    g++ -o simulare_trafic main.cpp
    ```
4.  Rulați aplicația:
    ```bash
    ./simulare_trafic
    ```

## 📊 Exemplu de Logica de Execuție
Dacă avem următoarea configurare:
* Mașina 1 la **Nord (0)**
* Mașina 2 la **Est (1)**
* Mașina 3 la **Nord (0)**

**Rezultatul simulării:**
1.  `[SEMAFOR] VERDE NORD-SUD`: Mașina 1 trece, apoi Mașina 3 trece.
2.  `[SEMAFOR] VERDE EST-VEST`: Mașina 2 trece.
3.  **Ordinea finală:** 1 -> 3 -> 2.

---
*Proiect realizat ca exercițiu de algoritmică, structuri de date și simulare logică.*