#include "Ship.h"

#include <fstream>
#include <sstream>
#include <limits>

// -------------------------------------------------------------
// 1) IMPLEMENTACJA KONSTRUKTORA (sygnatura musi byæ identyczna
//    z deklaracj¹ w Ship.h)
// -------------------------------------------------------------
Ship::Ship(const std::string& name,
    const std::string& type,
    double capacity,
    const std::string& cargo_type)
    : name_(name),
    type_(type, capacity, cargo_type),
    travel_costs_(0.0),
    speed_(0.0),
    status_(ShipStatus::AtPort),
    targetPort_(""),
    remainingTravelTime_(0),
    waitingTime_(0)
{
    // Dodatkow¹ inicjalizacjê mo¿na wstawiæ tutaj, jeœli potrzeba
}

// -------------------------------------------------------------
// 2) Implementacja saveToTextFile
// -------------------------------------------------------------
bool Ship::saveToTextFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }

    // Zapis: kolejno po linijkach:
    // 1) nazwa statku
    out << name_ << '\n';

    // 2) typ statku (z obiektu ShipType)
    out << type_.getType() << '\n';

    // 3) ³adownoœæ
    out << type_.getCapacity() << '\n';

    // 4) rodzaj ³adunku
    out << type_.getCargoType() << '\n';

    // 5) travel_costs_, speed_
    out << travel_costs_ << '\n';
    out << speed_ << '\n';

    // 6) liczba pracowników w za³odze, a potem ich imiona
    out << crew_.size() << '\n';
    for (const auto& w : crew_) {
        out << w.getName() << '\n';
    }

    // 7) status statku (jako int)
    out << static_cast<int>(status_) << '\n';

    // 8) targetPort_ (nazwa portu docelowego lub pusty string)
    out << targetPort_ << '\n';

    // 9) remainingTravelTime_
    out << remainingTravelTime_ << '\n';

    // 10) waitingTime_
    out << waitingTime_ << '\n';

    return true;
}

// -------------------------------------------------------------
// 3) Implementacja loadFromTextFile
// -------------------------------------------------------------
bool Ship::loadFromTextFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    std::string type_str;
    std::string cargo_type_str;
    double capacity;

    // 1) Wczytaj nazwê
    std::getline(in, name_);

    // 2) Wczytaj typ statku
    std::getline(in, type_str);

    // 3) Wczytaj ³adownoœæ
    in >> capacity;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 4) Wczytaj rodzaj ³adunku
    std::getline(in, cargo_type_str);

    // Odtworzenie obiektu ShipType
    type_ = ShipType(type_str, capacity, cargo_type_str);

    // 5) Wczytaj travel_costs_ oraz speed_
    in >> travel_costs_ >> speed_;

    // 6) Wczytaj liczbê pracowników w za³odze:
    size_t crew_size;
    in >> crew_size;
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    crew_.clear();
    for (size_t i = 0; i < crew_size; ++i) {
        std::string worker_name;
        std::getline(in, worker_name);
        crew_.emplace_back(worker_name); // tu wywo³a siê Worker(const std::string&)
    }

    // 7) Wczytaj status statku (jako int), potem skonwertuj na ShipStatus
    int st;
    in >> st;
    status_ = static_cast<ShipStatus>(st);
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 8) Wczytaj targetPort_
    std::getline(in, targetPort_);

    // 9) Wczytaj remainingTravelTime_
    in >> remainingTravelTime_;

    // 10) Wczytaj waitingTime_
    in >> waitingTime_;

    return true;
}
