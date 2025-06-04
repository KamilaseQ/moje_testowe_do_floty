#pragma once

#include <string>
#include <vector>
#include <map>
#include "Port.h"
#include "Ship.h"
#include "Worker.h"
#include "Time.h"

struct Simulation {
    std::string name;
    std::vector<Port> ports;
    std::vector<Ship> ships;
    std::vector<Worker> workers;
    // Mapa: nazwa statku -> nazwa portu lub "<sea>"
    std::map<std::string, std::string> shipLocations;
};

struct ShipEvent {
    std::string message;  // np. "Statek Titanic dop³yn¹³ do Gdañska"
};

class SimulationEngine {
public:
    // Singleton
    static SimulationEngine& instance() {
        static SimulationEngine eng;
        return eng;
    }

    // Nie kopiujemy, nie przenosimy
    SimulationEngine(const SimulationEngine&) = delete;
    SimulationEngine& operator=(const SimulationEngine&) = delete;

    // Za³aduj symulacjê jako obiekt
    void loadSimulation(const Simulation& sim) {
        currentSim_ = sim;
        currentTime_ = Time(0, 0, 0);
        events_.clear();
        // Zaktualizuj mapê wskaŸników do statków
        shipMap_.clear();
        for (auto& s : currentSim_.ships) {
            shipMap_[s.getName()] = &s;
        }
    }

    // Pobierz referencjê do aktualnej symulacji
    Simulation& getSimulation() { return currentSim_; }

    // Pobierz wskaŸnik na Statek po nazwie (lub nullptr, jeœli nie ma)
    Ship* getShipByName(const std::string& name) {
        auto it = shipMap_.find(name);
        if (it == shipMap_.end()) return nullptr;
        return it->second;
    }

    // Inicjalizuje now¹ podró¿ statku:
    // statek musi mieæ status AtPort i byæ w podanym porcie
    // nowy status: Sailing, targetPort = portTo, remainingTravelTime = ceil(dystans/prêdkoœæ)
    bool initiateVoyage(const std::string& shipName,
        const std::string& portFrom,
        const std::string& portTo)
    {
        Ship* s = getShipByName(shipName);
        if (!s) return false;
        if (s->getStatus() != ShipStatus::AtPort) return false;
        auto locFrom = findPort(portFrom).getLocation();
        auto locTo = findPort(portTo).getLocation();
        double distance = locFrom.getDistance(locTo);
        double speed = s->getSpeed();  // km/min
        if (speed <= 0.0) return false;
        int travelMins = static_cast<int>(std::ceil(distance / speed));

        s->setTargetPort(portTo);
        s->setRemainingTravelTime(travelMins);
        s->setStatus(ShipStatus::Sailing);
        currentSim_.shipLocations[shipName] = "<sea>";

        events_.push_back({ "Statek " + shipName + " wyruszyl z " + portFrom + " do " + portTo });
        return true;
    }

    // Przesuwa czas o zadany licznik minut (zazwyczaj 15)
    void advanceTime(int minutes) {
        events_.clear();
        Time delta(0, 0, minutes);
        currentTime_ += delta;

        // 1) Aktualizuj statki w statusie Sailing
        for (auto& s : currentSim_.ships) {
            if (s.getStatus() == ShipStatus::Sailing) {
                int rem = s.getRemainingTravelTime() - minutes;
                if (rem > 0) {
                    s.setRemainingTravelTime(rem);
                    // Statek nadal p³ynie – mo¿na opcjonalnie dodaæ event
                }
                else {
                    // Statek doplynal
                    s.setStatus(ShipStatus::AtPort);
                    s.setRemainingTravelTime(0);
                    std::string dest = s.getTargetPort();
                    currentSim_.shipLocations[s.getName()] = dest;
                    events_.push_back({ "Statek " + s.getName() + " doplynal do " + dest });

                    // Teraz próbujemy od razu przydzieliæ terminal
                    Port& p = findPort(dest);
                    bool gotTerminal = p.assignToTerminal(s.getName());
                    if (gotTerminal) {
                        events_.push_back({ "Statek " + s.getName() + " zajal wolny terminal w " + dest });
                    }
                    else {
                        s.setStatus(ShipStatus::WaitingForTerminal);
                        s.setWaitingTime(0);
                        events_.push_back({ "Brak wolnego terminala. Statek " + s.getName() + " czeka w kolejce w " + dest });
                    }
                }
            }
        }

        // 2) Aktualizuj statki czekajace na terminal
        for (auto& s : currentSim_.ships) {
            if (s.getStatus() == ShipStatus::WaitingForTerminal) {
                int wait = s.getWaitingTime() + minutes;
                if (wait < 60) {
                    s.setWaitingTime(wait);
                    // wci¹¿ czeka
                }
                else {
                    // Statek zatopiony
                    std::string name = s.getName();
                    s.setStatus(ShipStatus::Sunk);
                    currentSim_.shipLocations.erase(name);
                    // usuwamy ze wszystkich terminali (byæ mo¿e jeszcze w kolejce)
                    for (auto& port : currentSim_.ports) {
                        port.releaseTerminal(name);
                    }
                    events_.push_back({ "Statek " + name + " zatopiony z powodu oczekiwania na terminal > 60 minut" });
                }
            }
        }

        // 3) Opcjonalnie: obs³uga zakoñczenia roz³adunku (jeœli chcielibyœmy)
        //    Na razie zostawiamy jako [TODO] – bo nie znamy dok³adnego czasu roz³adunku.
    }

    // Zwraca aktualny czas
    Time getCurrentTime() const { return currentTime_; }

    // Zwraca wszystkie eventy z ostatniej tury (advanceTime)
    const std::vector<ShipEvent>& getEvents() const { return events_; }

    // Zwraca referencjê do portu o danej nazwie
    Port& findPort(const std::string& portName) {
        for (auto& p : currentSim_.ports) {
            if (p.getName() == portName) return p;
        }
        // Je¿eli nie znaleziono – rzucamy (w zasadzie powinno siê unikaæ, ale na razie)
        throw std::runtime_error("Port " + portName + " nie istnieje w symulacji.");
    }

private:
    Simulation currentSim_;
    Time currentTime_;
    std::vector<ShipEvent> events_;
    std::map<std::string, Ship*> shipMap_; // mapa nazwa statku -> wskaŸnik

    SimulationEngine()
        : currentTime_(0, 0, 0)
    {
    }
};
