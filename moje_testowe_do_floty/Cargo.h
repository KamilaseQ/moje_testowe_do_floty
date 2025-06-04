#ifndef CARGO_H
#define CARGO_H

#include <string>
#include "CargoType.h"


class Cargo {

public:
    Cargo(const std::string& name, double weight, CargoType type);
    std::string getName(void);
    double getWeight(void);

    Cargo& operator+=(double added_weight);
    Cargo& operator-=(double subtracted_weight);

private:
    std::string name_;
    double weight_;
    CargoType type_;

};


#endif // CARGO_H
