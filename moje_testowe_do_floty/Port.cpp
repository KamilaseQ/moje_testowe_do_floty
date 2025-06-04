#include "Port.h"
#include "SimulationEngine.h"  // by odczytaæ typ ³adunku statku

// Implementacja pomocniczej funkcji static
CargoType Port::shipTypeOf(const std::string& shipName) {
    // To jest hack: wyci¹gamy typ ³adunku z Engine na podstawie nazwy statku.
    // SimulationEngine::instance() to singleton, albo statyczna referencja. 
    // Za³ó¿my, ¿e SimulationEngine ma metodê getShipByName().
    auto sPtr = SimulationEngine::instance().getShipByName(shipName);
    if (sPtr) {
        return sPtr->getCargoType() == "Liquid" ? CargoType::Liquid
            : sPtr->getCargoType() == "Powder" ? CargoType::Powder
            : CargoType::Container;
    }
    // Domyœlnie Container, jeœli nie znaleziono
    return CargoType::Container;
}
