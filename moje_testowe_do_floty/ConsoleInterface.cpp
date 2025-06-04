#include "ConsoleInterface.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <fstream>

#include <nlohmann/json.hpp>
#include "JSONHelpers.h"   // <- zakladamy, ze ten plik istnieje i zawiera funkcje to_json/from_json
using json = nlohmann::json;

#include "Port.h"
#include "Ship.h"
#include "Worker.h"
#include "Location.h"
#include "Terminal.h"
#include "CargoType.h"
#include "Time.h"
#include "SimulationEngine.h"

using namespace std;

namespace {

    struct SavedData {
        map<int, Simulation> savedSims;
        int nextKey = 1;
    };

    SavedData data_;  // przechowuje wszystkie zapisane symulacje

    class Menu {
    public:
        Menu() {
            loadAll();
        }
        ~Menu() {
            saveAll();
        }

        int start() {
            while (true) {
                cout << "\n=== Flota Morska Symulator ===\n"
                    << "1) Uruchom symulacje\n"
                    << "2) Kreator symulacji\n"
                    << "3) Wczytaj symulacje\n"
                    << "4) Edytor symulacji\n"
                    << "0) Wyjscie\n"
                    << "Wybierz opcje: ";
                int opcja = readInt();
                switch (opcja) {
                case 1: runSimulation();    break;
                case 2: createSimulation(); break;
                case 3: loadSimulation();   break;
                case 4: editSimulation();   break;
                case 0: return 0;
                default: cout << "Nieprawidlowy wybor.\n";
                }
            }
        }

    private:
        bool hasLoaded_ = false;
        Simulation currentSim_;
        SimulationEngine& engine_ = SimulationEngine::instance();

        // =========================================================
        // 1) FUNKCJE ZAPISUJACE / LADUJACE WSZYSTKIE SYMULACJE JSON
        // =========================================================
        void saveAll() {
            json j_all = json::object();
            for (auto& pair : data_.savedSims) {
                int key = pair.first;
                j_all[std::to_string(key)] = pair.second;
            }
            ofstream fout("saved_sims.json");
            fout << j_all.dump(4);
        }

        void loadAll() {
            ifstream fin("saved_sims.json");
            if (!fin.is_open()) return;
            json j_all;
            fin >> j_all;
            data_.savedSims.clear();
            data_.nextKey = 1;
            for (auto& el : j_all.items()) {
                int key = stoi(el.key());
                Simulation sim = el.value().get<Simulation>();
                data_.savedSims[key] = sim;
                if (key >= data_.nextKey) data_.nextKey = key + 1;
            }
        }

        // =========================================================
        // 2) GLÓWNA PETLA SYMULACJI (runSimulation)
        //    - nie wyswietlamy pelnego menu / czasu w kazdej iteracji
        //    - tylko krotka zachêta, pelne sterowanie po 'h'
        // =========================================================
        void runSimulation() {
            if (!hasLoaded_) {
                cout << "Brak wczytanej symulacji. Wczytaj ja najpierw (opcja 3).\n";
                return;
            }
            // Zaladuj obiekt do silnika
            engine_.loadSimulation(currentSim_);

            cout << "\nWejscie do trybu symulacji \"" << currentSim_.name
                << "\". Wcisnij 'h', aby wyswietlic dostepne polecenia.\n";

            // Petla sterowania symulacja:
            while (true) {
                cout << "\n> ";
                char ch = readChar();

                switch (ch) {
                case 'h':
                    // Wyswietlenie pelnej listy klawiszy
                    showHelp();
                    break;

                case 'p':
                    // Wyswietl stan portow i statkow
                    showPortsAndShips();
                    break;

                case 'i':
                    // Inicjalizuj podroz statku (wyplyniec)
                    initiateVoyage();
                    break;

                case 't':
                    // Przesun czas o 15 minut
                    advanceTime();
                    break;

                case 'e':
                    // Wyswietl zdarzenia z ostatniej tury
                    showEvents();
                    break;

                case 'c':
                    // Wyswietl tylko czas
                    cout << "Aktualny czas: " << engine_.getCurrentTime() << "\n";
                    break;

                case '0':
                    // Zakonczenie symulacji i powrot do menu
                    cout << "Koncze symulacje i wracam do menu glownego.\n";
                    return;

                default:
                    cout << "Nieznany klawisz. Wcisnij 'h', aby zobaczyc liste polecen.\n";
                    break;
                }
            }
        }

        // =========================================================
        // 3) POKAZ POMOC (tylko na zadanie 'h')
        // =========================================================
        void showHelp() {
            cout << "\n--- Pomoc (sterowanie symulacja) ---\n"
                << " h) Wyswietl te pomoc\n"
                << " p) Pokaz stan portow i statki\n"
                << " i) Inicjalizuj podroz statku (wyplyn z portu do portu)\n"
                << " t) Przesun czas o 15 minut (krok symulacji)\n"
                << " e) Pokaz zdarzenia z ostatniej tury\n"
                << " c) Pokaz aktualny czas (Day, HH:MM)\n"
                << " 0) Zakoncz symulacje i wroc do menu glownego\n";
        }

        // =========================================================
        // 4) POKAZ STANY PORTÓW I STATKÓW
        // =========================================================
        void showPortsAndShips() {
            auto& sim = engine_.getSimulation();
            cout << "\n--- Porty i statki ---\n";
            for (auto& p : sim.ports) {
                cout << "* Port: " << p.getName() << "\n  Statki:\n";
                bool any = false;
                for (auto& s : sim.ships) {
                    auto it = sim.shipLocations.find(s.getName());
                    if (it != sim.shipLocations.end() && it->second == p.getName()) {
                        cout << "    - " << s.getName()
                            << " (status: " << statusToString(s.getStatus())
                            << ", speed: " << s.getSpeed() << ")\n";
                        any = true;
                    }
                }
                if (!any) cout << "    <brak statkow>\n";

                // Pokaz stan terminali w porcie:
                int idx = 1;
                for (auto& t : p.getTerminals()) {
                    cout << "    Terminal " << idx++
                        << " (typ: " << t.getCargoType() << ") -> "
                        << (t.isFree() ? "wolny" : "zajety przez " + t.currentShip())
                        << ", kolejka: " << t.queueSize() << "\n";
                }
            }
        }

        static std::string statusToString(ShipStatus st) {
            switch (st) {
            case ShipStatus::AtPort:             return "AtPort";
            case ShipStatus::Sailing:            return "Sailing";
            case ShipStatus::WaitingForTerminal: return "Waiting";
            case ShipStatus::Sunk:               return "Sunk";
            default: return "Unknown";
            }
        }

        // =========================================================
        // 5) INICJALIZACJA PODRÓ¯Y STATKU (‘i’)
        // =========================================================
        void initiateVoyage() {
            auto& sim = engine_.getSimulation();
            if (sim.ports.empty() || sim.ships.empty()) {
                cout << "Brak portow lub statkow w tej symulacji.\n";
                return;
            }

            // Wybór portu poczatkowego
            cout << "\nWybierz port poczatkowy:\n";
            for (size_t i = 0; i < sim.ports.size(); ++i) {
                cout << "  " << i + 1 << ") " << sim.ports[i].getName() << "\n";
            }
            int idxFrom = readInt();
            if (idxFrom < 1 || idxFrom > static_cast<int>(sim.ports.size())) {
                cout << "Nieprawidlowy numer portu.\n";
                return;
            }
            std::string portFrom = sim.ports[idxFrom - 1].getName();

            // Lista statkow w tym porcie
            vector<string> shipsHere;
            for (auto& s : sim.ships) {
                auto it = sim.shipLocations.find(s.getName());
                if (it != sim.shipLocations.end()
                    && it->second == portFrom
                    && s.getStatus() == ShipStatus::AtPort)
                {
                    shipsHere.push_back(s.getName());
                }
            }
            if (shipsHere.empty()) {
                cout << "Brak dostepnych statkow w porcie " << portFrom << "\n";
                return;
            }
            cout << "Wybierz statek:\n";
            for (size_t i = 0; i < shipsHere.size(); ++i) {
                cout << "  " << i + 1 << ") " << shipsHere[i] << "\n";
            }
            int idxShip = readInt();
            if (idxShip < 1 || idxShip > static_cast<int>(shipsHere.size())) {
                cout << "Nieprawidlowy numer statku.\n";
                return;
            }
            std::string shipName = shipsHere[idxShip - 1];

            // Wybór portu docelowego
            cout << "Wybierz port docelowy:\n";
            for (size_t i = 0; i < sim.ports.size(); ++i) {
                if (sim.ports[i].getName() == portFrom) continue;
                cout << "  " << i + 1 << ") " << sim.ports[i].getName() << "\n";
            }
            int idxTo = readInt();
            if (idxTo < 1 || idxTo > static_cast<int>(sim.ports.size())
                || sim.ports[idxTo - 1].getName() == portFrom)
            {
                cout << "Nieprawidlowy numer portu docelowego.\n";
                return;
            }
            std::string portTo = sim.ports[idxTo - 1].getName();

            // Wywolanie w SimulationEngine
            bool ok = engine_.initiateVoyage(shipName, portFrom, portTo);
            if (!ok) {
                cout << "Nie mozna zainicjowac podrozy. Sprawdz status statku/predkosc.\n";
            }
        }

        // =========================================================
        // 6) PRZESUNIECIE CZASU (‘t’)
        // =========================================================
        void advanceTime() {
            engine_.advanceTime(15);  // krok 15 minut
            cout << "Przesunieto czas o 15 minut.\n";
            // UWAGA: nie wyswietlamy automatycznie czasu –
            // uzytkownik moze go zobaczyc klawiszem 'c'
        }

        // =========================================================
        // 7) POKAZ ZDARZENIA Z OSTATNIEJ TURY (‘e’)
        // =========================================================
        void showEvents() {
            auto evs = engine_.getEvents();
            if (evs.empty()) {
                cout << "Brak zdarzen w tej turze.\n";
            }
            else {
                cout << "\n--- Zdarzenia z tej tury ---\n";
                for (auto& e : evs) {
                    cout << " * " << e.message << "\n";
                }
            }
        }

        // =========================================================
        // 8) KREATOR SYMULACJI
        // =========================================================
        void createSimulation() {
            Simulation sim;
            sim.name.clear();
            sim.ports.clear();
            sim.ships.clear();
            sim.workers.clear();
            sim.shipLocations.clear();

            cout << "\n--- Kreator nowej symulacji ---\n";
            while (true) {
                cout << "1) Dodaj port\n"
                    << "2) Dodaj terminal do portu\n"
                    << "3) Dodaj statek\n"
                    << "4) Dodaj pracownika\n"
                    << "5) Wyswietl aktualne dane\n"
                    << "0) Zakoncz tworzenie i zapisz symulacje\n"
                    << "Wybierz opcje: ";
                int opcja = readInt();
                switch (opcja) {
                case 1: createPort(sim);       break;
                case 2: createTerminal(sim);   break;
                case 3: createShip(sim);       break;
                case 4: createWorker(sim);     break;
                case 5: listAll(sim);          break;
                case 0:
                    if (sim.ports.empty() || sim.ships.empty()) {
                        cout << "Musisz miec co najmniej 1 port i 1 statek, aby zapisac.\n";
                    }
                    else {
                        cout << "Podaj nazwe symulacji do zapisu: ";
                        string sname;
                        getline(cin >> ws, sname);
                        sim.name = sname;
                        data_.savedSims[data_.nextKey] = sim;
                        cout << "Zapisano symulacje pod numerem: " << data_.nextKey << "\n";
                        data_.nextKey++;
                        return;
                    }
                    break;
                default:
                    cout << "Nieprawidlowy wybor.\n";
                }
            }
        }

        // =========================================================
        // 9) FUNKCJE POMOCNICZE DLA KREATORA / EDYTORA
        // =========================================================
        void createPort(Simulation& sim) {
            string name, cont, country;
            double lat, lon;
            cout << "Nazwa portu: ";       getline(cin >> ws, name);
            cout << "Szerokosc geogr.: "; cin >> lat;
            cout << "Dlugosc geogr.: ";   cin >> lon;
            cout << "Kontynent: ";         getline(cin >> ws, cont);
            cout << "Kraj: ";              getline(cin >> ws, country);
            Location loc(lat, lon, cont, country);
            sim.ports.emplace_back(name, loc);
            cout << "Port dodany: " << name << "\n";
        }

        void createTerminal(Simulation& sim) {
            if (sim.ports.empty()) {
                cout << "Brak portow. Dodaj najpierw port.\n";
                return;
            }
            cout << "Wybierz port, by dodac terminal:\n";
            for (size_t i = 0; i < sim.ports.size(); ++i) {
                cout << "  " << i + 1 << ") " << sim.ports[i].getName() << "\n";
            }
            int idx = readInt();
            if (idx < 1 || idx > static_cast<int>(sim.ports.size())) {
                cout << "Nieprawidlowy numer portu.\n";
                return;
            }
            cout << "Typ ladunku (0=Container,1=Liquid,2=Powder): ";
            int t; cin >> t;
            CargoType ct = static_cast<CargoType>(t);
            Terminal tmp(ct);
            sim.ports[idx - 1].addTerminal(tmp);
            cout << "Terminal (typ: " << ct << ") dodany do "
                << sim.ports[idx - 1].getName() << "\n";
        }

        void createShip(Simulation& sim) {
            if (sim.ports.empty()) {
                cout << "Dodaj najpierw port.\n";
                return;
            }
            string name, type, cargoType;
            double cap, speed;
            cout << "Nazwa statku: ";     getline(cin >> ws, name);
            cout << "Typ statku: ";       getline(cin >> ws, type);
            cout << "Ladownosc: ";        cin >> cap;
            cout << "Rodzaj ladunku: ";   getline(cin >> ws, cargoType);
            cout << "Predkosc (km/min): "; cin >> speed;

            Ship ship(name, type, cap, cargoType);
            ship.setSpeed(speed);
            sim.ships.push_back(ship);

            cout << "Przypisz statek do portu:\n";
            for (size_t i = 0; i < sim.ports.size(); ++i) {
                cout << "  " << i + 1 << ") " << sim.ports[i].getName() << "\n";
            }
            int idx = readInt();
            if (idx < 1 || idx > static_cast<int>(sim.ports.size())) {
                cout << "Nieprawidlowy numer portu. Statek nie zostal przypisany.\n";
            }
            else {
                string pName = sim.ports[idx - 1].getName();
                sim.shipLocations[name] = pName;
                cout << "Statek " << name
                    << " przypisano do portu " << pName << "\n";
            }
        }

        void createWorker(Simulation& sim) {
            string fn, ln;
            cout << "Imie: ";     getline(cin >> ws, fn);
            cout << "Nazwisko: "; getline(cin >> ws, ln);
            sim.workers.emplace_back(fn, ln);
            cout << "Pracownik dodany: " << fn << " " << ln << "\n";
        }

        void listAll(const Simulation& sim) {
            cout << "\n--- Porty ---\n";
            for (auto& p : sim.ports) {
                cout << "  - " << p.getName() << "\n";
            }
            cout << "\n--- Statki i lokalizacje ---\n";
            for (auto& s : sim.ships) {
                string loc = "[nieprzypisany]";
                auto it = sim.shipLocations.find(s.getName());
                if (it != sim.shipLocations.end())
                    loc = it->second;
                cout << "  - " << s.getName() << " -> " << loc << "\n";
            }
            cout << "\n--- Pracownicy ---\n";
            for (auto& w : sim.workers) {
                cout << "  - " << w.getName() << "\n";
            }
            cout << "\n--- Terminale w portach ---\n";
            for (auto& p : sim.ports) {
                cout << "* " << p.getName() << "\n";
                int idx = 1;
                for (auto& t : p.getTerminals()) {
                    cout << "    Terminal " << idx++
                        << " (typ: " << t.getCargoType() << ") -> "
                        << (t.isFree() ? "wolny" : "zajety przez " + t.currentShip())
                        << ", kolejka: " << t.queueSize() << "\n";
                }
            }
        }

        void editSimulation() {
            if (data_.savedSims.empty()) {
                cout << "Brak zapisanych symulacji.\n";
                return;
            }
            cout << "\n--- Edytor symulacji ---\n";
            for (auto& pair : data_.savedSims) {
                cout << " " << pair.first << ") " << pair.second.name << "\n";
            }
            cout << "Podaj numer symulacji do edycji: ";
            int key = readInt();
            auto it = data_.savedSims.find(key);
            if (it == data_.savedSims.end()) {
                cout << "Nie ma takiej symulacji.\n";
                return;
            }
            Simulation& simToEdit = it->second;
            cout << "Edytujesz: " << simToEdit.name << "\n";

            while (true) {
                cout << "\n--- Edytor (" << simToEdit.name << ") ---\n"
                    << "1) Dodaj port\n"
                    << "2) Dodaj terminal do portu\n"
                    << "3) Dodaj statek\n"
                    << "4) Dodaj pracownika\n"
                    << "5) Wyswietl dane\n"
                    << "0) Zakoncz edycje\n"
                    << "Wybierz opcje: ";
                int opcja = readInt();
                switch (opcja) {
                case 1: createPort(simToEdit);     break;
                case 2: createTerminal(simToEdit); break;
                case 3: createShip(simToEdit);     break;
                case 4: createWorker(simToEdit);   break;
                case 5: listAll(simToEdit);        break;
                case 0: cout << "Koniec edycji.\n"; return;
                default: cout << "Nieprawidlowy wybor.\n";
                }
            }
        }

        void loadSimulation() {
            if (data_.savedSims.empty()) {
                cout << "Brak zapisanych symulacji.\n";
                return;
            }
            cout << "\n--- Dostepne symulacje ---\n";
            for (auto& pair : data_.savedSims) {
                cout << " " << pair.first << ") " << pair.second.name << "\n";
            }
            cout << "Podaj numer symulacji do wczytania: ";
            int key = readInt();
            auto it = data_.savedSims.find(key);
            if (it == data_.savedSims.end()) {
                cout << "Nie ma takiej.\n";
                return;
            }
            currentSim_ = it->second;
            hasLoaded_ = true;
            cout << "Wczytano symulacje: " << currentSim_.name << "\n";
        }

        // =========================================================
        // 10) POMOCNICZE POBIERANIE WEJSCIA
        // =========================================================
        int readInt() {
            int x;
            while (!(cin >> x)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Podaj liczbe calkowita: ";
            }
            return x;
        }

        char readChar() {
            char c;
            cin >> ws >> c;
            return c;
        }
    };

} // koniec anonymous namespace

// =============================================================
// Definicja metody run() klasy ConsoleInterface (poza namespace)
// =============================================================
int ConsoleInterface::run() {
    Menu menu;
    return menu.start();
}
