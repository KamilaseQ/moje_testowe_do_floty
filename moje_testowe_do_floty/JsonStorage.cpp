#include "JsonStorage.h"
#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

class JsonStorage {
public:
    template<typename T>
    static void saveToFile(const std::string& filename, const T& obj) {
        std::ofstream file(filename);
        if (file.is_open()) {
            nlohmann::json j = obj;
            file << j.dump(4); // pretty print with 4-space indent
            file.close();
        }
        else {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
    }

    template<typename T>
    static void loadFromFile(const std::string& filename, T& obj) {
        std::ifstream file(filename);
        if (file.is_open()) {
            nlohmann::json j;
            file >> j;
            obj = j.get<T>();
            file.close();
        }
        else {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }
    }
};
