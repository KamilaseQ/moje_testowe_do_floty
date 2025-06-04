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
    Sunk               // Statek zaton��
};

class Ship {
public:
    /// Konstruktor:
    ///   name         � nazwa statku
    ///   type         � typ (�a�cuch, np. "Towarowy")
    ///   capacity     � �adowno��
    ///   cargo_type   � rodzaj �adunku (np. "Container", "Liquid", "Powder")
    Ship(const std::string& name,
        const std::string& type,
        double capacity,
        const std::string& cargo_type);

    /// Zwraca nazw� statku
    const std::string& getName() const { return name_; }

    /// Zwraca nazw� typu statku (np. "Towarowy")
    std::string getType() const { return type_.getType(); }
    /// Zwraca �adowno�� (zdefiniowan� w ShipType)
    double getCapacity() const { return type_.getCapacity(); }
    /// Zwraca rodzaj �adunku (zdefiniowan� w ShipType)
    std::string getCargoType() const { return type_.getCargoType(); }

    /// Pobiera pr�dko�� [km/min]
    double getSpeed() const { return speed_; }
    /// Ustala pr�dko�� [km/min]
    void setSpeed(double speed) { speed_ = speed; }

    /// Pobiera koszt podr�y (na razie nieu�ywane, ale zadeklarowane)
    double getTravelCosts() const { return travel_costs_; }
    /// Ustala koszt podr�y
    void setTravelCosts(double costs) { travel_costs_ = costs; }

    /// Pobiera wektor pracownik�w (kopiuje vector)
    std::vector<Worker> getCrew() const { return crew_; }
    /// Dodaje jednego pracownika do za�ogi
    void addCrewMember(const Worker& worker) { crew_.push_back(worker); }

    /// Pobiera aktualny status statku
    ShipStatus getStatus() const { return status_; }
    /// Ustala status statku
    void setStatus(ShipStatus s) { status_ = s; }

    /// Je�li statek jest w podr�y, to ta metoda zwraca nazw� portu docelowego
    const std::string& getTargetPort() const { return targetPort_; }
    /// Ustala port docelowy (�kierunek pl�dowania�)
    void setTargetPort(const std::string& p) { targetPort_ = p; }

    /// Pozosta�y czas (w minutach) do dop�yni�cia, gdy statek jest w statusie Sailing
    int getRemainingTravelTime() const { return remainingTravelTime_; }
    /// Ustala, ile minut zosta�o do dop�yni�cia
    void setRemainingTravelTime(int mins) { remainingTravelTime_ = mins; }

    /// Je�li statek czeka w porcie na terminal, to getWaitingTime() zwraca czas (w minutach), 
    /// kt�ry up�yn�� od momentu oczekiwania
    int getWaitingTime() const { return waitingTime_; }
    /// Ustala czas oczekiwania (w minutach) � resetuje licznikiem 0, kiedy statek zaczyna czeka�
    void setWaitingTime(int wt) { waitingTime_ = wt; }

    /// Zapisuje stan statku do prostego pliku tekstowego.
    /// Zwraca true, je�li uda�o si� otworzy� plik i zapisa�.
    bool saveToTextFile(const std::string& filename) const;

    /// Wczytuje stan statku z pliku tekstowego.
    /// Zwraca true, je�li uda�o si� otworzy� i wczyta� dane.
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
