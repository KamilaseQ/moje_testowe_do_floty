#include "Location.h"

Location::Location(double latitude, double longtitude, const std::string& continent, const std::string& country)
    : latitude_(latitude), longtitude_(longtitude), continent_(continent), country_(country) {
}

double Location::getLatitude() const { return latitude_; }
double Location::getLongtitude() const { return longtitude_; }
const std::string& Location::getContinent() const { return continent_; }
const std::string& Location::getCountry() const { return country_; }

double Location::getDistance(const Location& other) const {
    // locally its enough, worldwide need to account for distance other way around
    double distance_x = latitude_ - other.latitude_;
    double distance_y = longtitude_ - other.longtitude_;
    return std::sqrt(distance_x * distance_x + distance_y * distance_y);
}