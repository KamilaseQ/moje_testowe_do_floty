#include "ShipType.h"

// Tutaj definiujemy wszystkie funkcje:
ShipType::ShipType(const std::string& type, double capacity, const std::string& cargo_type)
    : type_(type), capacity_(capacity), cargo_type_(cargo_type)
{
}

std::string ShipType::getType(void) const {
    return type_;
}

double ShipType::getCapacity(void) const {
    return capacity_;
}

std::string ShipType::getCargoType(void) const {
    return cargo_type_;
}
