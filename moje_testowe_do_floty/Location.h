#pragma once
#include <string>
#include <cmath>

class Location {
public:
    Location(double latitude, double longtitude, const std::string& continent, const std::string& country);

    double getLatitude() const;
    double getLongtitude() const;
    const std::string& getContinent() const;
    const std::string& getCountry() const;
    double getDistance(const Location& other) const;

private:
    double latitude_;
    double longtitude_;
    std::string continent_;
    std::string country_;
};