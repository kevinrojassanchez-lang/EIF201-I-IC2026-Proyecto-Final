#include "MedicamentoMarca.h"

namespace FarmaSystem {

    MedicamentoMarca::MedicamentoMarca(int id, std::string nombre, double precio, bool receta, int stock, std::string pais, bool promo, int idProveedor)
        : Medicamento(id, nombre, precio, receta, stock, idProveedor), paisOrigen(pais), estadoEnPromocion(promo) {
    }
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

    std::string MedicamentoMarca::getPaisOrigen() const {
        return paisOrigen;
    }

    bool MedicamentoMarca::getPromocion() const {
        return estadoEnPromocion;
    }

    std::string MedicamentoMarca::toFile() const {

        return getCategoria() + "|" + std::to_string(getID()) + "|" + getNombre() + "|" + std::to_string(getPrecio()) + "|" +
            std::to_string(getStock()) + "|" + (getReceta() ? "1" : "0") + "|" + std::to_string(getIdProveedor())
            + "|" + paisOrigen + "|" + (getPromocion() ? "1" : "0");
    }

    MedicamentoMarca::~MedicamentoMarca() {}
}
