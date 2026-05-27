#pragma once
#include "Medicamento.h"

namespace FarmaSystem {

    struct NodoMedicamento {

        Medicamento* dato;
        NodoMedicamento* siguiente;

        NodoMedicamento(Medicamento* med)
            : dato(med), siguiente(nullptr) {
        }

        ~NodoMedicamento() {
           
        }
    };

}
