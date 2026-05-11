#pragma once
#include <string>
#include "Medicamento.h"

namespace FarmaSystem {

    class Venta {
    private:
        int id;
        int idCliente;        
        Medicamento* medicamentoVendido;
        int cantidad;
        double precioUnitario;
        double precioFinal;
        std::string fecha; // formato dd/mm/aaaa

    public:
        Venta(int id, int idCliente, Medicamento* medicamentoVendido, int cant, double precioFinal, std::string fecha);
        // Getters para la GUI
        int getId() const;
        int getIdCliente() const;
        std::string getNombreMedicamento() const;
        int getCantidad() const;
        double getPrecioFinal() const;
        std::string getFecha() const;
        Medicamento* getMedicamentoVendido() const;
        std::string getInfoVenta() const; // Factura

        ~Venta();
    };
}

