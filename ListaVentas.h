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

        // Requerimiento: agregar ventas
        void agregar(Venta* v);
		Medicamento* obtenerMedicamentoMasVendido(ListaMedicamentos& medicamentos);
        // Utilidades
		int calcularUnidadesPorMedicamento(int idMedicamento);
        int cantidad() const;
        Venta* obtener(int indice);
        void guardarEnArchivo(std::ofstream& archivo);

        // Requerimiento: calcular acumulados
        double calcularIngresosTotales();
        double calcularAcumuladoPorCliente(int idCliente);
		bool existeVentaDeMedicamento(int idMedicamento);
		bool existeVentaDeCliente(int idCliente);
        // Requerimiento: recorrer historial continuo
        void imprimirHistorial() const;

        void limpiar();
    };

}
