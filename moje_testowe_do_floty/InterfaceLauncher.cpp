#include "InterfaceLauncher.h"
#include "InterfaceFactory.h"

int InterfaceLauncher::launch(const std::string& type) {
    auto ui = InterfaceFactory::create(type);
    return ui->run();
}
