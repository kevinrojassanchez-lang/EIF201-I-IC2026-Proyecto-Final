#include "MedicamentoMarca.h"

namespace FarmaSystem {

    MedicamentoMarca::MedicamentoMarca(int id, std::string nombre, double precio, bool receta, int stock, std::string pais, bool promo)
        : Medicamento(id, nombre, precio, receta, stock), paisOrigen(pais), estadoEnPromocion(promo) {}

    std::string MedicamentoMarca::getCategoria() const { 
        return "Marca"; }

    double MedicamentoMarca::calcularPrecioFinal(int cantidad) const {
        double subtotal = getPrecio() * cantidad;
        if (estadoEnPromocion == true) { return subtotal * 0.85; }
        return subtotal; }

    std::string MedicamentoMarca::getAdvertencia() const {
        std::string mensaje = "Producto Importado de Laboratorios Internacionales.";
        return mensaje; }

    std::string MedicamentoMarca::getInfoPrecio() const {
        return std::string("Descuento 15%: ") + (estadoEnPromocion ? "Si" : "No"); }

    std::string MedicamentoMarca::mostrar() const { 
        return Medicamento::mostrar() + " | Origen: " + paisOrigen + " | Promo: " + (estadoEnPromocion ? "Si" : "No"); }

    std::string MedicamentoMarca::getInfoExtra() const {
        return "Origen: " + paisOrigen + " | Promocion: " + (estadoEnPromocion ? "Si" : "No"); }

    MedicamentoMarca::~MedicamentoMarca() {}
}
