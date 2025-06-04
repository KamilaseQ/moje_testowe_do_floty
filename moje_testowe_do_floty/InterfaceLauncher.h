#pragma once
#include <string>  // konieczne dla std::string

/// Odpowiada za uruchomienie wybranego interfejsu.
class InterfaceLauncher {
public:
    /// type np. "console"
    static int launch(const std::string& type);
};