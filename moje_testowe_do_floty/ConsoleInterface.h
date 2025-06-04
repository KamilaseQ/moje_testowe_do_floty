#pragma once
#include "IInterface.h"

/// Konsolowa implementacja interfejsu symulatora.
class ConsoleInterface : public IInterface {
public:
    int run() override;
};

