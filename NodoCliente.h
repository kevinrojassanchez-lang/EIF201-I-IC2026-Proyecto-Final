#pragma once

#include "Cliente.h"

namespace FarmaSystem {

    struct NodoCliente {

        Cliente* dato;
        NodoCliente* siguiente;
        NodoCliente* anterior;

        NodoCliente(Cliente* _cliente)
            : dato(_cliente), siguiente(nullptr), anterior(nullptr) {
        }

        ~NodoCliente() {
   
        }


    };

}
