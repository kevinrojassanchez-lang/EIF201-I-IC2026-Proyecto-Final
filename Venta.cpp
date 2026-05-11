#include "Venta.h"
#include <sstream>
#include <iomanip>

namespace FarmaSystem {

    Venta::Venta(int id, int idCliente, Medicamento* medicamentoVendido, int cant, double precioFinal, std::string fecha)
        : id(id), idCliente(idCliente), medicamentoVendido(medicamentoVendido), cantidad(cant), fecha(fecha)
    {
        this->precioUnitario = medicamentoVendido->getPrecio();
        this->precioFinal = precioFinal;
    }

    int Venta::getId() const {
        return id;
    }

    int Venta::getIdCliente() const {
        return idCliente;
    }

    std::string Venta::getNombreMedicamento() const {
        if (medicamentoVendido != nullptr) { return medicamentoVendido->getNombre(); }
        return "Desconocido";
    }

    int Venta::getCantidad() const {
        return cantidad;
    }

    double Venta::getPrecioFinal() const {
        return precioFinal;
    }

    std::string Venta::getFecha() const {
        return fecha;
    }

    Medicamento* Venta::getMedicamentoVendido() const {
        return medicamentoVendido;
    }

    std::string Venta::getInfoVenta() const {
        std::ostringstream oss;

        oss << "=========== FACTURA ===========+" << "\n";
        oss << "Venta #: " << id << "\n";
        oss << "Cliente ID: " << idCliente << "\n";
        oss << "Fecha: " << fecha << "\n";
        oss << "--------------------------------\n";
        oss << "Medicamento: " << getNombreMedicamento() << "\n";
        oss << "Cantidad: " << cantidad << "\n";
        oss << "Precio Unitario: CRC" << std::fixed << std::setprecision(2) << precioUnitario << "\n";
        oss << "Total: CRC" << std::fixed << std::setprecision(2) << precioFinal << "\n";
        oss << "================================+\n";

        return oss.str();
    }

    Venta::~Venta() {}
        // NO eliminar medicamentoVendido
        // El sistema de inventario es el dueño de ese objeto
    

}
