#include "MedicamentoMarca.h"

namespace FarmaSystem {

    MedicamentoMarca::MedicamentoMarca(int id, std::string nombre, double precio,
        bool receta, int stock, std::string pais, bool promo, int idProveedor) : Medicamento(id, nombre,
            precio, receta, stock, idProveedor), paisOrigen(pais), estadoEnPromocion(promo) {

    }

    std::string MedicamentoMarca::getCategoria() const {  return "Marca"; }
    std::string MedicamentoMarca::getPaisOrigen() const { return paisOrigen; }
    bool MedicamentoMarca::getPromocion() const { return estadoEnPromocion; }
    
    double MedicamentoMarca::calcularPrecioFinal(int cantidad) const { 

        double subtotal = getPrecio() * cantidad;

        if (estadoEnPromocion == true) { return subtotal * 0.85; }

        return subtotal; 
    }

    std::string MedicamentoMarca::getAdvertencia() const {

        std::string mensaje = "Producto Importado de Laboratorios Internacionales.";

        return mensaje;
    }

    std::string MedicamentoMarca::toString() const {

        return Medicamento::toString() + "|" + paisOrigen + "|" + (estadoEnPromocion ? "1" : "0");
    }

	std::string MedicamentoMarca::getInfoExtra() const { // Para mostrar en la GUI

        return "Origen: " + paisOrigen + " | Promocion: " + (estadoEnPromocion ? "Si" : "No");
    }

	std::string MedicamentoMarca::getInfoPrecio() const { // Para mostrar en la GUI

        return std::string("Descuento 15%: ") + (estadoEnPromocion ? "Si" : "No");
    }

    bool MedicamentoMarca::setPaisOrigen(const std::string& pais) {

        if (pais == "") { return false; }
        paisOrigen = pais;
        return true;
	}

	void MedicamentoMarca::setPromocion(bool estado) { this->estadoEnPromocion = estado; }


    MedicamentoMarca::~MedicamentoMarca() {}
}
