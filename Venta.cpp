#include "Venta.h"
#include <sstream>
#include <iomanip>

namespace FarmaSystem {

    Venta::Venta(int id, int idCliente, Medicamento* medicamentoVendido, int cant, double precioFinal, 
        std::string fecha) : id(id), idCliente(idCliente), medicamentoVendido(medicamentoVendido), 
        cantidad(cant), fecha(fecha) {

        this->precioUnitario = medicamentoVendido->getPrecio();
        this->precioFinal = precioFinal;

    }

    int Venta::getId() const { return id; }
    int Venta::getIdCliente() const { return idCliente; }
    int Venta::getCantidad() const { return cantidad; }
    double Venta::getPrecioFinal() const { return precioFinal; }
    Medicamento* Venta::getMedicamentoVendido() const { return medicamentoVendido; }
    std::string Venta::getFecha() const { return fecha; }

    std::string Venta::getNombreMedicamento() const {
        if (medicamentoVendido != nullptr) { return medicamentoVendido->getNombre(); }
        return "Desconocido";
    }

    std::string Venta::toString() const {
        std::ostringstream oss;

        oss << id << "|" << idCliente << "|" << medicamentoVendido->getID() << "|" << cantidad << "|"
            << precioFinal << "|" << fecha;

        return oss.str();
    }

    Venta::~Venta() {}
        // NO eliminar medicamentoVendido
        // El sistema de inventario es el dueño de ese objeto
}
