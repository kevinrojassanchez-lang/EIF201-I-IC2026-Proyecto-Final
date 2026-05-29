#pragma once

#include <fstream>
#include "ListaVentas.h"
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
        Cliente* obtenerClienteVIP(ListaVentas& ventas);
        bool eliminar(int id);
        void mostrarTodos();
        void guardarEnArchivo(std::ofstream& archivo);
        void limpiar();
    };

}
