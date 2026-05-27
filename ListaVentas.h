#pragma once
#include "NodoVenta.h"

namespace FarmaSystem {

    class ListaVentas {

    private:

        NodoVenta* cola;
        int cantidadVentas;

    public:

        ListaVentas();
        ~ListaVentas();

        NodoVenta* getCabeza() const;

        // Requerimiento: agregar ventas
        void agregar(Venta* v);

        // Utilidades
        int cantidad() const;
        Venta* obtener(int indice);

        // Requerimiento: calcular acumulados
        double calcularIngresosTotales();
        double calcularAcumuladoPorCliente(int idCliente);

        // Requerimiento: recorrer historial continuo
        void imprimirHistorial() const;

        void limpiar();
    };

}
