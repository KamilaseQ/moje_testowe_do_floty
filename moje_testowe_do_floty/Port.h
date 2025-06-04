#pragma once
#include <string>
#include <vector>
#include "Terminal.h"
#include "Location.h"

class Port {
public:
    Port(const std::string& name, const Location& location)
        : name_(name), location_(location)
    {
    }

    const std::string& getName() const { return name_; }
    const Location& getLocation() const { return location_; }

    // Dodawanie terminala
    void addTerminal(const Terminal& terminal) { terminals_.push_back(terminal); }

    // Sprawd�, czy jest dost�pny cho� jeden wolny terminal
    bool hasFreeTerminal() const {
        for (auto& t : terminals_) {
            if (t.isFree()) return true;
        }
        return false;
    }

    // Przeka� statek do terminala (zwroci true, je�li statek od razu trafi na terminal,
    // false, je�li dopisany do kolejki czekaj�cych)
    bool assignToTerminal(const std::string& shipName) {
        for (auto& t : terminals_) {
            if (t.isFree()) {
                t.occupy(shipName);
                return true;
            }
        }
        // Je�li tu � brak wolnego terminala => wrzu� do kolejki pierwszego terminala, kt�ry pasuje typem
        for (auto& t : terminals_) {
            if (t.getCargoType() == shipTypeOf(shipName)) {
                t.enqueue(shipName);
                return false;
            }
        }
        // Je�li nie ma terminala obs�uguj�cego ten typ �adunku, te� zwracamy false
        return false;
    }

    // Zwolnij terminal obs�uguj�cy statek (najpierw szukamy terminal, kt�rego currentShip == shipName)
    void releaseTerminal(const std::string& shipName) {
        for (auto& t : terminals_) {
            if (t.currentShip() == shipName) {
                t.release();
                return;
            }
        }
    }

    // Pomocnicza funkcja: w oparciu o nazw� statku zwraca typ �adunku
    // (zak�adamy, �e SimulationEngine przechowuje map� nazw statk�w -> obiekt Ship)
    static CargoType shipTypeOf(const std::string& shipName);

    // Zwr�� referencj� do ca�ej listy terminali (np. do debugowania)
    const std::vector<Terminal>& getTerminals() const { return terminals_; }

private:
    std::string name_;
    Location location_;
    std::vector<Terminal> terminals_;
};
