#pragma once

#include <string>
#include <vector>
#include "ShipType.h"
#include "Worker.h"
#include "Time.h"

/// Status statku w trakcie symulacji
enum class ShipStatus {
    AtPort,            // Statek stoi w porcie
    Sailing,           // Statek jest w drodze ("na morzu")
    WaitingForTerminal,// Statek czeka w porcie na wolny terminal
    Sunk               // Statek zaton¹³
};

class Ship {
public:
    /// Konstruktor:
    ///   name         – nazwa statku
    ///   type         – typ (³añcuch, np. "Towarowy")
    ///   capacity     – ³adownoœæ
    ///   cargo_type   – rodzaj ³adunku (np. "Container", "Liquid", "Powder")
    Ship(const std::string& name,
        const std::string& type,
        double capacity,
        const std::string& cargo_type);

    /// Zwraca nazwê statku
    const std::string& getName() const { return name_; }

    /// Zwraca nazwê typu statku (np. "Towarowy")
    std::string getType() const { return type_.getType(); }
    /// Zwraca ³adownoœæ (zdefiniowan¹ w ShipType)
    double getCapacity() const { return type_.getCapacity(); }
    /// Zwraca rodzaj ³adunku (zdefiniowan¹ w ShipType)
    std::string getCargoType() const { return type_.getCargoType(); }

    /// Pobiera prêdkoœæ [km/min]
    double getSpeed() const { return speed_; }
    /// Ustala prêdkoœæ [km/min]
    void setSpeed(double speed) { speed_ = speed; }

    /// Pobiera koszt podró¿y (na razie nieu¿ywane, ale zadeklarowane)
    double getTravelCosts() const { return travel_costs_; }
    /// Ustala koszt podró¿y
    void setTravelCosts(double costs) { travel_costs_ = costs; }

    /// Pobiera wektor pracowników (kopiuje vector)
    std::vector<Worker> getCrew() const { return crew_; }
    /// Dodaje jednego pracownika do za³ogi
    void addCrewMember(const Worker& worker) { crew_.push_back(worker); }

    /// Pobiera aktualny status statku
    ShipStatus getStatus() const { return status_; }
    /// Ustala status statku
    void setStatus(ShipStatus s) { status_ = s; }

    /// Jeœli statek jest w podró¿y, to ta metoda zwraca nazwê portu docelowego
    const std::string& getTargetPort() const { return targetPort_; }
    /// Ustala port docelowy („kierunek pl¹dowania”)
    void setTargetPort(const std::string& p) { targetPort_ = p; }

    /// Pozosta³y czas (w minutach) do dop³yniêcia, gdy statek jest w statusie Sailing
    int getRemainingTravelTime() const { return remainingTravelTime_; }
    /// Ustala, ile minut zosta³o do dop³yniêcia
    void setRemainingTravelTime(int mins) { remainingTravelTime_ = mins; }

    /// Jeœli statek czeka w porcie na terminal, to getWaitingTime() zwraca czas (w minutach), 
    /// który up³yn¹³ od momentu oczekiwania
    int getWaitingTime() const { return waitingTime_; }
    /// Ustala czas oczekiwania (w minutach) – resetuje licznikiem 0, kiedy statek zaczyna czekaæ
    void setWaitingTime(int wt) { waitingTime_ = wt; }

    /// Zapisuje stan statku do prostego pliku tekstowego.
    /// Zwraca true, jeœli uda³o siê otworzyæ plik i zapisaæ.
    bool saveToTextFile(const std::string& filename) const;

    /// Wczytuje stan statku z pliku tekstowego.
    /// Zwraca true, jeœli uda³o siê otworzyæ i wczytaæ dane.
    bool loadFromTextFile(const std::string& filename);

private:
    std::string name_;
    ShipType   type_;
    std::vector<Worker> crew_;
    double travel_costs_;
    double speed_;

    // Dodatkowe pola do symulacji:
    ShipStatus status_;
    std::string targetPort_;
    int remainingTravelTime_; // w minutach
    int waitingTime_;         // w minutach
};
