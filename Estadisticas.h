#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <string>
#include "ListaMedicamentos.h"
#include "ListaClientes.h"
#include "ListaVentas.h"

namespace FarmaSystem {

    class Estadisticas {

    private:

        // Auxiliares privados para manejo de operaciones mediante arreglos paralelos
        static void acumularMedicamentos(ListaVentas& ventas, int* idsMedicamentos, int* unidadesVendidas,
            int maxCapacidad, int& usados);

        static int obtenerIdMasVendido(int* idsMedicamentos, int* unidadesVendidas, int usados);

        static void acumularClientes(ListaVentas& ventas, int* idsClientes,double* gastosClientes,
            int maxCapacidad, int& usados);

        static int obtenerIdVIP(int* idsClientes, double* gastosClientes, int usados);

    public:

        // Retorna el medicamento con el stock mss bajo (Lista Simple)
        static Medicamento* obtenerMenorStock(ListaMedicamentos& medicamentos);

        // Retorna el medicamento mas vendido cruzando datos (Lista Simple + Lista Circular)
        static Medicamento* obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas);

        // Retorna el cliente con mayor monto acumulado (Lista Doble + Lista Circular)
        static Cliente* obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas);

        // Calcula la sumatoria de todas las ventas (Lista Circular)
        static double obtenerIngresosTotales(ListaVentas& ventas);

        // Cuenta medicamentos filtrados por su categoria (Lista Simple)
        static int contarPorCategoria(ListaMedicamentos& medicamentos, const std::string& categoria);
    };
}

#endif // ESTADISTICAS_H

