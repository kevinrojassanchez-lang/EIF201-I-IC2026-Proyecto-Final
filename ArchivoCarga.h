#ifndef ARCHIVOCARGA_H
#define ARCHIVOCARGA_H

#include <string>
#include <sstream>

#include "SistemaFarmacia.h"
#include "Medicamento.h"

namespace FarmaSystem {

    class ArchivoCarga {
    private:

        std::string rutaProveedores_;
        std::string rutaMedicamentos_;
        std::string rutaClientes_;
        std::string rutaVentas_;

        int validas_;
        int descartadas_;

    public:

        ArchivoCarga();

        void cargarTodo(SistemaFarmacia& sistema);

        int getValidas() const;
        int getDescartadas() const;

    private:

        void cargarProveedores(SistemaFarmacia& sistema);
        void cargarMedicamentos(SistemaFarmacia& sistema);
        void cargarClientes(SistemaFarmacia& sistema);
        void cargarVentas(SistemaFarmacia& sistema);
        Medicamento* crearMedicamentoDesdeTexto(std::stringstream& ss, const std::string& categoria, int id,
            const std::string& nombre, double precio, bool receta, int stock, int idProveedor);
    };
} // namespace FarmaSystem

#endif // ARCHIVOCARGA_H