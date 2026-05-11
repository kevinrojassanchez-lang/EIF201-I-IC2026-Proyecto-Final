#pragma once
#include "Medicamento.h"

namespace FarmaSystem {

    class MedicamentoMarca : public Medicamento {

    private:

        std::string paisOrigen;
        bool estadoEnPromocion;

    public:
        MedicamentoMarca(int id, std::string nombre, double precio, bool receta, int stock, std::string pais, bool promo);

		// Metodos polimorficos sobrescritos
        std::string getCategoria() const override;
        double calcularPrecioFinal(int cantidad) const override;
        std::string getAdvertencia() const override;
        std::string mostrar() const override;
		std::string getInfoExtra() const override; 
		std::string getInfoPrecio() const override; 

        ~MedicamentoMarca();
    };
}

