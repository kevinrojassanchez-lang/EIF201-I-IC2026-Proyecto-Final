#pragma once

#include "NodoCliente.h"
#include <string>

namespace FarmaSystem {

    class ListaClientes {

    private:
        NodoCliente* cabeza;
        NodoCliente* cola;
        int tamano;

    public:
        ListaClientes();
        ~ListaClientes();
        NodoCliente* getCabeza() const;
        void agregar(Cliente* cli);

        int cantidad() const;

        Cliente* obtener(int indice);

        Cliente* buscarPorId(int id);

        Cliente* buscarPorCedula(std::string cedula);

        bool eliminar(int id);

        void limpiar();
    };

}
