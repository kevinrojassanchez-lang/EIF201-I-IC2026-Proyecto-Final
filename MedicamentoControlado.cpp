#include "MedicamentoControlado.h"

namespace FarmaSystem {

    MedicamentoControlado::MedicamentoControlado(int id, std::string nombre, double precio, int stock, int nivel, double dosis)
		: Medicamento(id, nombre, precio, true, stock), dosisMaxima(dosis), dosisPorUnidad(100) // cada unidad equivale a 100 mg
    {
        if (nivel < 1) { nivelControl = 1; }
        else if (nivel > 4) { nivelControl = 4; }
        else { nivelControl = nivel; }
    }
    
    std::string MedicamentoControlado::getCategoria() const {
        return "Controlado";
    }

    double MedicamentoControlado::calcularPrecioFinal(int cantidad) const {
        // recargo del 5% 
        double subtotal = getPrecio() * cantidad;
        return subtotal * 1.05;
    }

    bool MedicamentoControlado::getReceta() const {
        return true;
    }

    std::string MedicamentoControlado::getAdvertencia() const {
        return "VENTA RESTRINGIDA.Requiere receta medica original y control de dosis.";
    }

    std::string MedicamentoControlado::mostrar() const {
        // llamamos al mostrar del padre y le pegamos lo nuevo
        return Medicamento::mostrar() + " | Nivel: " + std::to_string(nivelControl) + " | Dosis Max: " + std::to_string(dosisMaxima) + "mg";
    }

    bool MedicamentoControlado::excedeDosis(int cantidad) const {
        return (cantidad * dosisPorUnidad) > dosisMaxima;
    }

    std::string MedicamentoControlado::getInfoPrecio() const {
        std::ostringstream oss;

        oss << "Recargo por control: 5%";
        
        return oss.str();
    }

    std::string MedicamentoControlado::getInfoExtra() const {
        std::ostringstream oss;

        oss << "Nivel: " << nivelControl
            << " | Dosis Max: "
            << std::fixed << std::setprecision(2)
            << dosisMaxima << " mg";

        return oss.str();
    }

    MedicamentoControlado::~MedicamentoControlado() {}
}
