#pragma once
#include <string>
#include <iostream>

enum class CargoType {
    Container,
    Liquid,
    Powder
};

inline std::string toString(CargoType type) {
    switch (type) {
    case CargoType::Container: return "Container";
    case CargoType::Liquid: return "Liquid";
    case CargoType::Powder: return "Powder";
    default: return "Other";
    }
}

inline std::ostream& operator<<(std::ostream& os, CargoType type) {
    return os << toString(type);
}