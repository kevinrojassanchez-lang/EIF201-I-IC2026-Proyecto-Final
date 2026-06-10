#include "MedicamentoGenerico.h"

namespace FarmaSystem {

    MedicamentoGenerico::MedicamentoGenerico(int id, std::string nombre, double precio, bool receta, int stock,
        std::string principio, int idProveedor)
        : Medicamento(id, nombre, precio, receta, stock, idProveedor), principioActivo(principio) {
    }

    std::string MedicamentoGenerico::getCategoria() const { return "Generico"; }

    double MedicamentoGenerico::calcularPrecioFinal(int cantidad) const {
        
        return (getPrecio() * cantidad) * 0.90;
    }

    std::string MedicamentoGenerico::getAdvertencia() const { 
        
        return "Medicamento Generico: misma eficacia a menor costo.";
    }

    std::string MedicamentoGenerico::getInfoPrecio() const  { return "Descuento Fijo: 10%"; }

    std::string MedicamentoGenerico::toString() const { 
        
        return Medicamento::toString() + "|" + principioActivo;
    }

    std::string MedicamentoGenerico::getInfoExtra() const { return "Principio activo: " + principioActivo; }

    std::string MedicamentoGenerico::getPrincipioActivo() const { return principioActivo; }

    bool MedicamentoGenerico::setPrincipioActivo(const std::string& principio) {

        if (principio == "") { return false; }
        principioActivo = principio;
        return true;
    }

    MedicamentoGenerico::~MedicamentoGenerico() {}
}