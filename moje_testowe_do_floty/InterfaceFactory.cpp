#include "InterfaceFactory.h"
#include "ConsoleInterface.h"
#include <stdexcept>
// #include inny interfejs  // w przysz³oœci

std::unique_ptr<IInterface> InterfaceFactory::create(const std::string& type) {
    if (type == "console") {
        return std::make_unique<ConsoleInterface>();
    }
    throw std::runtime_error("Nieznany typ interfejsu: " + type);
}
