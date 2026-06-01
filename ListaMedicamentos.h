#pragma once
#include <fstream>
#include "NodoMedicamento.h"

namespace FarmaSystem {

    class ListaMedicamentos {

    private:

        NodoMedicamento* cabeza;
		NodoMedicamento* cola;
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
        bool tieneProveedor(int idProveedor);
        int cantidad() const;
        void mostrarTodos();
        void guardarEnArchivo(std::ofstream& archivo);
        void limpiar();
    };

}