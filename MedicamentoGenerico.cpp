#include "MedicamentoGenerico.h"

namespace FarmaSystem {

    MedicamentoGenerico::MedicamentoGenerico(int id, std::string nombre, double precio, bool receta, int stock, std::string principio)
        : Medicamento(id, nombre, precio, receta, stock), principioActivo(principio) {}

    std::string MedicamentoGenerico::getCategoria() const 
    { return "Generico"; }

    double MedicamentoGenerico::calcularPrecioFinal(int cantidad) const
    { return (getPrecio() * cantidad) * 0.90; }

    std::string MedicamentoGenerico::getAdvertencia() const 
    { return "Medicamento generico: misma eficacia a menor costo."; }

    std::string MedicamentoGenerico::getInfoPrecio() const 
    { return "Descuento fijo: 10%"; }

    std::string MedicamentoGenerico::mostrar() const
    { return Medicamento::mostrar() + " | Principio Activo: " + principioActivo; }

    std::string MedicamentoGenerico::getInfoExtra() const 
    { return "Principio activo: " + principioActivo; }

    MedicamentoGenerico::~MedicamentoGenerico() {}
}