#include "MedicamentoControlado.h"

namespace FarmaSystem {

    MedicamentoControlado::MedicamentoControlado(int id, std::string nombre, double precio, bool receta,
        int stock, int nivel, double dosis, int idProveedor): Medicamento(id, nombre, precio, receta, stock,
            idProveedor), dosisMaxima(dosis), dosisPorUnidad(100) {

        if (nivel < 1) {
            nivelControl = 1;
        }
        else if (nivel > 4) {
            nivelControl = 4;
        }
        else {
            nivelControl = nivel;
        }
    }
    
    std::string MedicamentoControlado::getCategoria() const { return "Controlado"; }

    bool MedicamentoControlado::getReceta() const { return true; } // Siempre requiere receta

    std::string MedicamentoControlado::getAdvertencia() const {

        return "VENTA RESTRINGIDA.Requiere receta medica original y control de dosis.";
    }

    double MedicamentoControlado::calcularPrecioFinal(int cantidad) const {

        double subtotal = getPrecio() * cantidad;

        return subtotal * 1.05;
    }

    std::string MedicamentoControlado::getInfoExtra() const { // Para mostrar en la GUI

        std::ostringstream oss;

        oss << "Nivel: " << nivelControl << " | Dosis Max: " << std::fixed << std::setprecision(2)
            << dosisMaxima << "mg";

        return oss.str();
    }

    bool MedicamentoControlado::excedeDosis(int cantidad) const { // Extras

        return (cantidad * dosisPorUnidad) > dosisMaxima;

    }

	std::string MedicamentoControlado::getInfoPrecio() const { // Para mostrar en la GUI

        std::ostringstream oss;

        oss << "Recargo por control: 5%";
        
        return oss.str();
    }

    int MedicamentoControlado::getNivelControl() const { return nivelControl; }

    double MedicamentoControlado::getDosisMaxima() const { return dosisMaxima; }
    bool MedicamentoControlado::setNivelControl(int nivel) {

        if (nivel < 1 || nivel > 4) { return false; }
        nivelControl = nivel;
        return true;
    }
    bool MedicamentoControlado::setDosisMaxima(double dosis) {

        if (dosis <= 0) { return false; }
        dosisMaxima = dosis;
        return true;
    }

    std::string MedicamentoControlado::toString() const {

        std::ostringstream oss;

        oss << Medicamento::toString() << "|" << nivelControl << "|" << dosisMaxima;

        return oss.str();
    }

	MedicamentoControlado::~MedicamentoControlado() {} // Destructor limpio
}
