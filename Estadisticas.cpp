#include "Estadisticas.h"

namespace FarmaSystem {

    Estadisticas::Estadisticas() {}

    Medicamento* Estadisticas::obtenerMenorStock(ListaMedicamentos& medicamentos) {
        return medicamentos.obtenerMenorStock();
    }

    Medicamento* Estadisticas::obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas) {
        return ventas.obtenerMedicamentoMasVendido(medicamentos);
    }

    Cliente* Estadisticas::obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas) {
        return clientes.obtenerClienteVIP(ventas);
    }

    double Estadisticas::obtenerIngresosTotales(ListaVentas& ventas) {
        return ventas.calcularIngresosTotales();
    }

    int Estadisticas::contarPorCategoria(ListaMedicamentos& medicamentos, std::string categoria) {
        return medicamentos.contarPorCategoria(categoria);
    }

    Estadisticas::~Estadisticas() {
        // La clase estadistica no es duena de los objetos, ella solo recibe referencias y retorna punteros
    }

}
