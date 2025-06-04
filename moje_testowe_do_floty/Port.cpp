#include "Port.h"
#include "SimulationEngine.h"  // by odczyta� typ �adunku statku

// Implementacja pomocniczej funkcji static
CargoType Port::shipTypeOf(const std::string& shipName) {
    // To jest hack: wyci�gamy typ �adunku z Engine na podstawie nazwy statku.
    // SimulationEngine::instance() to singleton, albo statyczna referencja. 
    // Za��my, �e SimulationEngine ma metod� getShipByName().
    auto sPtr = SimulationEngine::instance().getShipByName(shipName);
    if (sPtr) {
        return sPtr->getCargoType() == "Liquid" ? CargoType::Liquid
            : sPtr->getCargoType() == "Powder" ? CargoType::Powder
            : CargoType::Container;
    }
    // Domy�lnie Container, je�li nie znaleziono
    return CargoType::Container;
}
