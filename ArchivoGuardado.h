#ifndef ARCHIVOGUARDADO_H
#define ARCHIVOGUARDADO_H

#include <string>
#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class ArchivoGuardado {
    private:
        std::string rutaProveedores_;
        std::string rutaMedicamentos_;
        std::string rutaClientes_;
        std::string rutaVentas_;

    public:
        ArchivoGuardado();
        void guardarTodo(SistemaFarmacia& sistema);

    private:
        void guardarProveedores(SistemaFarmacia& sistema);
        void guardarMedicamentos(SistemaFarmacia& sistema);
        void guardarClientes(SistemaFarmacia& sistema);
        void guardarVentas(SistemaFarmacia& sistema);
    };

} // namespace FarmaSystem

#endif // ARCHIVOGUARDADO_H
