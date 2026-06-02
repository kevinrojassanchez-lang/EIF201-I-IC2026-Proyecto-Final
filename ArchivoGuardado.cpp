#include "ArchivoGuardado.h"
#include <stdexcept> // uso de std::exception
#include <fstream>

namespace FarmaSystem {

    // Constructor para inicializar las rutas fisicas de archivos de texto
    ArchivoGuardado::ArchivoGuardado() {

        rutaProveedores_ = "datos/proveedores.txt";
        rutaMedicamentos_ = "datos/medicamentos.txt";
        rutaClientes_ = "datos/clientes.txt";
        rutaVentas_ = "datos/ventas.txt";
    }

    // funcion central de guardado protegida contra fallos mecanicos o de permisos
    void ArchivoGuardado::guardarTodo(SistemaFarmacia& sistema) {
        try {
            guardarProveedores(sistema);
            guardarMedicamentos(sistema);
            guardarClientes(sistema);
            guardarVentas(sistema);
        }
        catch (const std::exception& e) {
            // Si el disco se llena o se desconecta una USB, el error cae aqui,
            // evitamos que Qt se caiga y el programa sigue vivo.
        }
    }


    void ArchivoGuardado::guardarProveedores(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaProveedores_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        archivo.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        sistema.getListaProveedores().guardarEnArchivo(archivo);

        archivo.close();
    }

    void ArchivoGuardado::guardarMedicamentos(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaMedicamentos_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        archivo.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        sistema.getListaMedicamentos().guardarEnArchivo(archivo);

        archivo.close();
    }

    void ArchivoGuardado::guardarClientes(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaClientes_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        archivo.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        sistema.getListaClientes().guardarEnArchivo(archivo);

        archivo.close();
    }

    void ArchivoGuardado::guardarVentas(SistemaFarmacia& sistema) {

        std::ofstream archivo(rutaVentas_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        archivo.exceptions(std::ofstream::failbit | std::ofstream::badbit);

        sistema.getListaVentas().guardarEnArchivo(archivo);

        archivo.close();
    }
} // namespace FarmaSystem