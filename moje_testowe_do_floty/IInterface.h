#pragma once

/// Abstrakcyjny interfejs dla UI.
class IInterface {
public:
    virtual ~IInterface() = default;

    /// G��wna p�tla interfejsu; zwraca kod wyj�cia (0 = OK).
    virtual int run() = 0;
};
