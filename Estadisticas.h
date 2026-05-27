#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <string>

#include "ListaMedicamentos.h"
#include "ListaClientes.h"
#include "ListaVentas.h"

namespace FarmaSystem {

    class Estadisticas {

    public:
        Estadisticas();
        ~Estadisticas();

        Medicamento* obtenerMenorStock(ListaMedicamentos& medicamentos);
        Medicamento* obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas);
        Cliente* obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas);

        double obtenerIngresosTotales(ListaVentas& ventas);
        int contarPorCategoria(ListaMedicamentos& medicamentos, std::string categoria);
    };

}

#endif

