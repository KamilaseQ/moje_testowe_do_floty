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

    // SprawdŸ, czy jest dostêpny choæ jeden wolny terminal
    bool hasFreeTerminal() const {
        for (auto& t : terminals_) {
            if (t.isFree()) return true;
        }
        return false;
    }

    // Przeka¿ statek do terminala (zwroci true, jeœli statek od razu trafi na terminal,
    // false, jeœli dopisany do kolejki czekaj¹cych)
    bool assignToTerminal(const std::string& shipName) {
        for (auto& t : terminals_) {
            if (t.isFree()) {
                t.occupy(shipName);
                return true;
            }
        }
        // Jeœli tu – brak wolnego terminala => wrzuæ do kolejki pierwszego terminala, który pasuje typem
        for (auto& t : terminals_) {
            if (t.getCargoType() == shipTypeOf(shipName)) {
                t.enqueue(shipName);
                return false;
            }
        }
        // Jeœli nie ma terminala obs³uguj¹cego ten typ ³adunku, te¿ zwracamy false
        return false;
    }

    // Zwolnij terminal obs³uguj¹cy statek (najpierw szukamy terminal, którego currentShip == shipName)
    void releaseTerminal(const std::string& shipName) {
        for (auto& t : terminals_) {
            if (t.currentShip() == shipName) {
                t.release();
                return;
            }
        }
    }

    // Pomocnicza funkcja: w oparciu o nazwê statku zwraca typ ³adunku
    // (zak³adamy, ¿e SimulationEngine przechowuje mapê nazw statków -> obiekt Ship)
    static CargoType shipTypeOf(const std::string& shipName);

    // Zwróæ referencjê do ca³ej listy terminali (np. do debugowania)
    const std::vector<Terminal>& getTerminals() const { return terminals_; }

private:
    std::string name_;
    Location location_;
    std::vector<Terminal> terminals_;
};
