#pragma once

#include <nlohmann/json.hpp>
#include "SimulationEngine.h"
#include "Port.h"
#include "Terminal.h"
#include "Ship.h"
#include "Worker.h"
#include "Location.h"
#include "CargoType.h"

using nlohmann::json;

// Funkcje to_json i from_json musz¹ byæ w GLOBALNEJ przestrzeni nazw,
// aby ADL (argument?driven lookup) je znalaz³.

inline void to_json(json& j, const Simulation& sim) {
    j = json::object();
    j["name"] = sim.name;

    // =====================================================
    // 1) Porty (wraz z terminalami)
    // =====================================================
    j["ports"] = json::array();
    for (const auto& p : sim.ports) {
        json jp;
        jp["name"] = p.getName();
        jp["latitude"] = p.getLocation().getLatitude();
        jp["longitude"] = p.getLocation().getLongtitude(); // uwaga: w Waszej klasie getLongtitude(), a nie getLongitude()
        jp["continent"] = p.getLocation().getContinent();
        jp["country"] = p.getLocation().getCountry();

        // Zapisujemy terminale
        jp["terminals"] = json::array();
        for (const auto& t : p.getTerminals()) {
            json jt;
            jt["type"] = toString(t.getCargoType());
            jt["isFree"] = t.isFree();
            jt["currentShip"] = t.currentShip();
            jt["queueSize"] = t.queueSize();

            // Jeœli chcemy w przysz³oœci odtwarzaæ ca³¹ kolejkê, mo¿emy dodaæ:
            // jt["waitingList"] = json::array();
            // for (auto& nameInQ : t.getWaitingList()) {
            //     jt["waitingList"].push_back(nameInQ);
            // }

            jp["terminals"].push_back(jt);
        }
        j["ports"].push_back(jp);
    }

    // =====================================================
    // 2) Statki
    // =====================================================
    j["ships"] = json::array();
    for (const auto& s : sim.ships) {
        json js;
        js["name"] = s.getName();
        js["type"] = s.getType();
        js["capacity"] = s.getCapacity();
        js["cargoType"] = s.getCargoType();
        js["speed"] = s.getSpeed();
        js["status"] = static_cast<int>(s.getStatus());
        js["targetPort"] = s.getTargetPort();
        js["remainingTime"] = s.getRemainingTravelTime();
        js["waitingTime"] = s.getWaitingTime();
        j["ships"].push_back(js);
    }

    // =====================================================
    // 3) Pracownicy
    // =====================================================
    j["workers"] = json::array();
    for (const auto& w : sim.workers) {
        j["workers"].push_back(w.getName());
    }

    // =====================================================
    // 4) Mapa lokalizacji statków
    // =====================================================
    j["shipLocations"] = json::object();
    for (const auto& pair : sim.shipLocations) {
        j["shipLocations"][pair.first] = pair.second;
    }
}

// ========================
//  Funkcja from_json
// ========================
inline void from_json(const json& j, Simulation& sim) {
    // 1) Nazwa symulacji
    // Je¿eli brakuje klucza "name", zwracamy po prostu pusty string.
    sim.name = j.value("name", std::string{});

    // 2) Porty (wraz z terminalami)
    sim.ports.clear();
    if (j.contains("ports") && j["ports"].is_array()) {
        // Je¿eli klucz "ports" istnieje i jest tablic¹, wczytujemy elementy
        for (const auto& jp : j["ports"]) {
            // Dla ka¿dego obiektu portu najpierw sprawdzamy podstawowe pola:
            std::string pname = jp.value("name", std::string{});
            double plat = jp.value("latitude", 0.0);
            double plong = jp.value("longitude", 0.0);
            std::string pcont = jp.value("continent", std::string{});
            std::string pcountry = jp.value("country", std::string{});

            // Tworzymy obiekt Port (zak³adamy, ¿e konstruktor Port(name, Location) jest poprawny):
            Port port(pname, Location(plat, plong, pcont, pcountry));

            // Je¿eli s¹ terminale, wczytujemy je:
            if (jp.contains("terminals") && jp["terminals"].is_array()) {
                for (const auto& jt : jp["terminals"]) {
                    std::string typeStr = jt.value("type", std::string{});
                    int ttype = 0;
                    if (typeStr == "Container") ttype = 0;
                    else if (typeStr == "Liquid") ttype = 1;
                    else if (typeStr == "Powder") ttype = 2;
                    CargoType ct = static_cast<CargoType>(ttype);

                    Terminal term(ct);

                    bool isFree = jt.value("isFree", true);
                    if (!isFree) {
                        // Je¿eli w JSON-ie nie ma "currentShip", value(...) zwróci pusty string
                        std::string shipOn = jt.value("currentShip", std::string{});
                        if (!shipOn.empty()) {
                            term.occupy(shipOn);
                        }
                    }

                    // Je¿eli w JSON-ie jest tablica "waitingList", mo¿emy do³¹czyæ poszczególne nazwy:
                    // if (jt.contains("waitingList") && jt["waitingList"].is_array()) {
                    //     for (const auto& nameInQ : jt["waitingList"]) {
                    //         term.enqueue(nameInQ.get<std::string>());
                    //     }
                    // }

                    port.addTerminal(term);
                }
            }

            sim.ports.push_back(port);
        }
    }

    // 3) Statki
    sim.ships.clear();
    if (j.contains("ships") && j["ships"].is_array()) {
        for (const auto& js : j["ships"]) {
            std::string sname = js.value("name", std::string{});
            std::string stype = js.value("type", std::string{});
            double      scap = js.value("capacity", 0.0);
            std::string scargo = js.value("cargoType", std::string{});
            double      sspeed = js.value("speed", 0.0);

            Ship s(sname, stype, scap, scargo);
            s.setSpeed(sspeed);

            int st = js.value("status", static_cast<int>(ShipStatus::AtPort));
            s.setStatus(static_cast<ShipStatus>(st));

            s.setTargetPort(js.value("targetPort", std::string{}));
            s.setRemainingTravelTime(js.value("remainingTime", 0));
            s.setWaitingTime(js.value("waitingTime", 0));

            sim.ships.push_back(s);
        }
    }

    // 4) Pracownicy
    sim.workers.clear();
    if (j.contains("workers") && j["workers"].is_array()) {
        for (const auto& wname : j["workers"]) {
            // Je¿eli w tablicy pracowników jest nie?string, get<std::string>() rzuci wyj¹tek,
            // ale zak³adamy, ¿e to zawsze ³añcuch znaków.
            sim.workers.emplace_back(wname.get<std::string>());
        }
    }

    // 5) Mapa lokalizacji statków
    sim.shipLocations.clear();
    if (j.contains("shipLocations") && j["shipLocations"].is_object()) {
        for (auto& el : j["shipLocations"].items()) {
            // el.key() to string, el.value() to np. "Gdansk" ? get<std::string>()
            sim.shipLocations[el.key()] = el.value().get<std::string>();
        }
    }
}
