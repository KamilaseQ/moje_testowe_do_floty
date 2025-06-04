#pragma once
#include <string>

class ShipType
{
public:
    ShipType(const std::string& type, double capacity, const std::string& cargo_type);

    std::string getType(void) const;
    double      getCapacity(void) const;
    std::string getCargoType(void) const;

private:
    std::string type_;
    double      capacity_;
    std::string cargo_type_;
};
