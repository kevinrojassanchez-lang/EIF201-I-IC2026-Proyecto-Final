#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <string>
#include "ListaMedicamentos.h"
#include "ListaClientes.h"
#include "ListaVentas.h"

namespace FarmaSystem {

    class Estadisticas {

    private:

        struct AcumuladoCliente {
            int idCliente;
            double totalGastado;
        };

        struct AcumuladoMedicamento {
            int idMedicamento;
            int unidadesVendidas;
        };

        // Auxiliares privados: Se agrega maxCapacidad para proteger la memoria de los arreglos
        static void acumularMedicamentos(ListaVentas& ventas, AcumuladoMedicamento* acumulados, int maxCapacidad, int& usados);
        static int obtenerIdMasVendido(AcumuladoMedicamento* acumulados, int usados);
        static void acumularClientes(ListaVentas& ventas, AcumuladoCliente* acumulados, int maxCapacidad, int& usados);
        static int obtenerIdVIP(AcumuladoCliente* acumulados, int usados);

    public:

        // Retorna el medicamento con el stock más bajo (Lista Simple)
        static Medicamento* obtenerMenorStock(ListaMedicamentos& medicamentos);

        // Retorna el medicamento más vendido cruzando datos (Lista Simple + Lista Circular)
        static Medicamento* obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas);

        // Retorna el cliente con mayor monto acumulado (Lista Doble + Lista Circular)
        static Cliente* obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas);

        // Calcula la sumatoria de todas las ventas (Lista Circular)
        static double obtenerIngresosTotales(ListaVentas& ventas);

        // Cuenta medicamentos filtrados por su categoría (Lista Simple)
        static int contarPorCategoria(ListaMedicamentos& medicamentos, const std::string& categoria);
    };
}
#endif // ESTADISTICAS_H

