#pragma once

#include <string>

class Worker {
public:
    // Konstruktor przyjmuj¹cy osobno imiê i nazwisko
    Worker(const std::string& first_name, const std::string& last_name);

    // NOWY: konstruktor przyjmuj¹cy jeden string "Imiê Nazwisko"
    Worker(const std::string& fullName);

    // Zwraca pe³ne imiê i nazwisko
    std::string getName(void) const;

private:
    std::string first_name_;
    std::string last_name_;
};
