#pragma once

#include "Proveedor.h"

namespace FarmaSystem {

    struct NodoProveedor {

        Proveedor* dato;
        NodoProveedor* siguiente;
        NodoProveedor* anterior;

        NodoProveedor(Proveedor* prov)
            : dato(prov), siguiente(nullptr), anterior(nullptr) {
        }

        ~NodoProveedor() {

        }

    };

}
