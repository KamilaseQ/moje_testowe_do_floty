#include "Worker.h"
#include <sstream>  // do std::istringstream

// Istniej¹cy konstruktor dwuargumentowy
Worker::Worker(const std::string& first_name, const std::string& last_name)
    : first_name_(first_name), last_name_(last_name)
{
}

// NOWY: jednokargumentowy konstruktor rozdzielaj¹cy "Imiê Nazwisko"
Worker::Worker(const std::string& fullName) {
    std::istringstream iss(fullName);
    std::string first, rest;
    if (iss >> first) {
        // Odczytaj resztê linii jako 'rest'
        std::getline(iss, rest);
        // Usuñ pocz¹tkow¹ spacjê, jeœli istnieje
        if (!rest.empty() && rest.front() == ' ')
            rest.erase(0, 1);
    }
    first_name_ = first;
    last_name_ = rest;
}

// Metoda zwracaj¹ca pe³ne imiê i nazwisko
std::string Worker::getName(void) const {
    if (last_name_.empty())
        return first_name_;
    return first_name_ + " " + last_name_;
}
