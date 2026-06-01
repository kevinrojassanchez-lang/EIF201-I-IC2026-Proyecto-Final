#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

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

		// Auxiliares para calcular acumulados y obtener resultados
        static void acumularMedicamentos( ListaVentas& ventas, AcumuladoMedicamento* acumulados, int& usados);
        static int obtenerIdMasVendido(AcumuladoMedicamento* acumulados, int usados);
        static void acumularClientes(ListaVentas& ventas, AcumuladoCliente* acumulados, int& usados);
        static int obtenerIdVIP(AcumuladoCliente* acumulados, int usados);

    public:

        static Medicamento* obtenerMenorStock(ListaMedicamentos& medicamentos);

        static Medicamento* obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas);

        static Cliente* obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas);

        static double obtenerIngresosTotales(ListaVentas& ventas);

        static int contarPorCategoria(ListaMedicamentos& medicamentos, const std::string& categoria);
    };

}

#endif

