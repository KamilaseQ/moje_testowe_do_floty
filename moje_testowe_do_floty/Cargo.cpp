#include "Cargo.h"


Cargo::Cargo(const std::string& name, double weight, CargoType type)
{
    name_ = name;
    weight_ = weight;
    type_ = type;
}


std::string Cargo::getName(void)
{
    return name_;
}


double Cargo::getWeight(void)
{
    return weight_;
}


Cargo& Cargo::operator+=(double added_weight)
{
    weight_ += added_weight;
    if (weight_ < 0) weight_ = 0;
    return *this;
}


Cargo& Cargo::operator-=(double subtracted_weight)
{
    weight_ -= subtracted_weight;
    if (weight_ < 0) weight_ = 0;
    return *this;
}
