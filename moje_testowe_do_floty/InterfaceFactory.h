#pragma once
#include <memory>
#include <string>
#include "IInterface.h"

/// Fabryka tworz�ca implementacje IInterface.
class InterfaceFactory {
public:
    static std::unique_ptr<IInterface> create(const std::string& type);
};
