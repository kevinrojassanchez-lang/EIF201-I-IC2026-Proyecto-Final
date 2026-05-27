#pragma once
#include "Venta.h"

namespace FarmaSystem {

    struct NodoVenta {

        Venta* dato;
        NodoVenta* siguiente;

        NodoVenta(Venta* v)
            : dato(v), siguiente(nullptr) {
        }
        ~NodoVenta() {

        }
    };

}
