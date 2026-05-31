#include "ArchivoGuardado.h"
#include <fstream>

namespace FarmaSystem {

    ArchivoGuardado::ArchivoGuardado() {

        rutaProveedores_ = "datos/proveedores.txt";

        rutaMedicamentos_ = "datos/medicamentos.txt";

        rutaClientes_ = "datos/clientes.txt";

        rutaVentas_ = "datos/ventas.txt";
    }

    void ArchivoGuardado::guardarTodo(SistemaFarmacia& sistema) {

        guardarProveedores(sistema);

        guardarMedicamentos(sistema);

        guardarClientes(sistema);

        guardarVentas(sistema);
    }

    void ArchivoGuardado::guardarProveedores(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaProveedores_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        sistema.getListaProveedores().guardarEnArchivo(archivo);

        archivo.close();
    }

    void ArchivoGuardado::guardarMedicamentos(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaMedicamentos_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        sistema.getListaMedicamentos().guardarEnArchivo(archivo);

        archivo.close();
    }

    void ArchivoGuardado::guardarClientes(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaClientes_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        sistema.getListaClientes().guardarEnArchivo(archivo);

        archivo.close();
    }

    void ArchivoGuardado::guardarVentas(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaVentas_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        sistema.getListaVentas().guardarEnArchivo(archivo);

        archivo.close();
    }

} // namespace FarmaSystem