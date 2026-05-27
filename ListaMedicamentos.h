#pragma once
#include "NodoMedicamento.h"

namespace FarmaSystem {

    class ListaMedicamentos {

    private:

        NodoMedicamento* cabeza;
        int tamano;

    public:

        ListaMedicamentos();

        ~ListaMedicamentos();

        void agregar(Medicamento* med);

        NodoMedicamento* getCabeza() const;

        Medicamento* obtener(int indice);

        Medicamento* buscarPorId(int id);

        Medicamento* buscarPorNombre(std::string texto);

        bool eliminar(int id);

        int cantidad() const;

        void limpiar();
    };

}