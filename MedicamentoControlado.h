#pragma once
#include "Medicamento.h"

namespace FarmaSystem {

    class MedicamentoControlado : public Medicamento {
    private:
        int nivelControl;       
        double dosisMaxima;
        double dosisPorUnidad;

    public:
        MedicamentoControlado(int id, std::string nombre, double precio, int stock, int nivel, double dosis);
        std::string getCategoria() const override;
        bool getReceta() const override;
        double calcularPrecioFinal(int cantidad) const override;
        std::string getAdvertencia() const override;
        std::string getInfoExtra() const override;
        bool excedeDosis(int cantidad) const override;
		std::string getInfoPrecio() const override;

        // Sobreescribimos mostrar para que incluya los datos de control
        std::string mostrar() const override;

        ~MedicamentoControlado();
    };
}

