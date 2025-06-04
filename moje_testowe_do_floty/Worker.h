#pragma once

#include <string>

class Worker {
public:
    // Konstruktor przyjmuj�cy osobno imi� i nazwisko
    Worker(const std::string& first_name, const std::string& last_name);

    // NOWY: konstruktor przyjmuj�cy jeden string "Imi� Nazwisko"
    Worker(const std::string& fullName);

    // Zwraca pe�ne imi� i nazwisko
    std::string getName(void) const;

private:
    std::string first_name_;
    std::string last_name_;
};
