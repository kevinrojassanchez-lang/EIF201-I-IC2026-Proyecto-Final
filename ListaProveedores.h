#ifndef LISTAPROVEEDORES_H
#define LISTAPROVEEDORES_H

#include <fstream>
#include "NodoProveedor.h"
#include <string>

namespace FarmaSystem {

    class ListaProveedores {

    private:

        NodoProveedor* cabeza;
        NodoProveedor* cola;
        int tamano;

    public:

        ListaProveedores();
        ~ListaProveedores();

        void agregar(Proveedor* prov);

        int cantidad() const;

        Proveedor* obtener(int indice);
        NodoProveedor* getCabeza() const ;
        Proveedor* buscarPorId(int id);
        Proveedor* buscarPorNombre(std::string nombre);

        bool eliminar(int id, bool tieneMedicamentosAsociados);
        void guardarEnArchivo(std::ofstream& archivo);
        void limpiar();
    };

}

#endif
