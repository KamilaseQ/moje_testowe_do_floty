#pragma once
#include <string>
#include <queue>
#include "CargoType.h"

class Terminal {
public:
    explicit Terminal(CargoType type)
        : type_(type), busy_(false)
    {
    }

    // Sprawdza, czy terminal jest wolny
    bool isFree() const { return !busy_; }

    // Zajmuje terminal przez dany statek (nazwa statku)
    void occupy(const std::string& shipName) {
        busy_ = true;
        currentShip_ = shipName;
    }

    // Zwolnij terminal (statek opu�ci�)
    void release() {
        busy_ = false;
        currentShip_.clear();
        // Je�li w kolejce s� statki, to pierwszy w kolejce zajmuje terminal
        if (!waitingQueue_.empty()) {
            currentShip_ = waitingQueue_.front();
            waitingQueue_.pop();
            busy_ = true;
        }
    }

    // Dodaj statek do kolejki oczekuj�cych
    void enqueue(const std::string& shipName) {
        waitingQueue_.push(shipName);
    }

    // Pobierz nazw� statku aktualnie na terminalu
    const std::string& currentShip() const { return currentShip_; }

    // Coturn: je�li statek na terminalu zako�czy� operacj� (roz�adunek) ? release()
    // Mo�na tu doda� dodatkow� logik� np. czasu roz�adunku, ale to w [TODO].

    CargoType getCargoType() const { return type_; }

    // Sprawdza, czy kolejka jest pusta
    bool queueEmpty() const { return waitingQueue_.empty(); }

    // Ile statk�w czeka w kolejce
    int queueSize() const { return static_cast<int>(waitingQueue_.size()); }

private:
    CargoType type_;
    bool busy_;
    std::string currentShip_;
    std::queue<std::string> waitingQueue_;
};
