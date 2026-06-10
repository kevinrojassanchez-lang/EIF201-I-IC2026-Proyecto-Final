#pragma once

#include <fstream>
#include "NodoVenta.h"
#include "ListaMedicamentos.h"

namespace FarmaSystem {

    class ListaVentas {

    private:

        NodoVenta* cola;
        int cantidadVentas;

    public:

        ListaVentas();
        ~ListaVentas();

        NodoVenta* getCabeza() const;

        void agregar(Venta* v);
        int cantidad() const;
        Venta* obtener(int indice);
		bool existeVentaDeMedicamento(int idMedicamento);
		bool existeVentaDeCliente(int idCliente);
        void imprimirHistorial() const;
        void guardarEnArchivo(std::ofstream& archivo);
        void limpiar();
    };

}
