#ifndef ARCHIVOCARGA_H
#define ARCHIVOCARGA_H

#include <string>

#include "Medicamento.h"
#include "Cliente.h"
#include "Proveedor.h"
#include "Venta.h"

namespace FarmaSystem {

    class ArchivoCarga {
    
    public:

        // Medicamentos
        static Medicamento* crearMedicamento(
            const std::string& linea);

        // Clientes
        static Cliente* crearCliente(
            const std::string& linea);

        // Proveedores
        static Proveedor* crearProveedor(
            const std::string& linea);

        // Ventas
        static Venta* crearVenta(
            const std::string& linea,
            Medicamento* medicamento);

    };

}

#endif
