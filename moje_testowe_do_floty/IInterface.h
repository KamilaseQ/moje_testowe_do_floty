#pragma once

/// Abstrakcyjny interfejs dla UI.
class IInterface {
public:
    virtual ~IInterface() = default;

    /// G³ówna pêtla interfejsu; zwraca kod wyjœcia (0 = OK).
    virtual int run() = 0;
};
