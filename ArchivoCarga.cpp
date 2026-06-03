#include <fstream>
#include "ArchivoCarga.h"
#include "MedicamentoGenerico.h"
#include "MedicamentoMarca.h"
#include "MedicamentoControlado.h"
#include "ValidadorCentral.h"

namespace FarmaSystem {

    ArchivoCarga::ArchivoCarga() {

        rutaProveedores_ = "datos/proveedores.txt";
        rutaMedicamentos_ = "datos/medicamentos.txt";
        rutaClientes_ = "datos/clientes.txt";
        rutaVentas_ = "datos/ventas.txt";
        validas_ = 0;
        descartadas_ = 0;
    }

    Medicamento* ArchivoCarga::crearMedicamentoDesdeTexto(std::stringstream& ss, const std::string& categoria,
        int id, const std::string& nombre, double precio, bool receta, int stock, int idProveedor) {

        if (categoria == "Generico") {

            std::string principio;

            if (std::getline(ss, principio, '|')) {

                return new MedicamentoGenerico(id, nombre, precio, receta, stock, principio, idProveedor);
            }
        }
        else if (categoria == "Marca") {

            std::string paisOrigen;
            std::string sPromo;

            if (std::getline(ss, paisOrigen, '|') && std::getline(ss, sPromo, '|')) {

                // Usamos el validador central para la bandera binaria de la promocion
                bool promo = ValidadorCentral::validarBanderaBinaria(sPromo);

                return new MedicamentoMarca(id, nombre, precio, receta, stock, paisOrigen, promo, idProveedor);
            }
        }
        else if (categoria == "Controlado") {

            std::string sNivel;
            std::string sDosis;

            if (std::getline(ss, sNivel, '|') && std::getline(ss, sDosis, '|')) {

               // Si viene una letra, tira un throw que atrapara cargarMedicamentos
                int nivel = ValidadorCentral::validarYConvertirEntero(sNivel);
                double dosis = ValidadorCentral::validarYConvertirDecimal(sDosis);

                return new MedicamentoControlado(id, nombre, precio, stock, nivel, dosis, idProveedor);
            }
        }

        return nullptr;
    }

    void ArchivoCarga::cargarTodo(SistemaFarmacia& sistema) {

        validas_ = 0;
        descartadas_ = 0;

        cargarProveedores(sistema);
        cargarMedicamentos(sistema);
        cargarClientes(sistema);
        cargarVentas(sistema);
    }

    void ArchivoCarga::cargarProveedores(SistemaFarmacia& sistema) {

        std::ifstream archivo(rutaProveedores_, std::ios::in);

        if (!archivo.is_open()) {
            return;
        }

        std::string linea;

        while (std::getline(archivo, linea)) {

            if (linea.empty()) {
                descartadas_++;
                continue; 
            }

            try {
                std::stringstream ss(linea);

                std::string sId;
                std::string nombre;
                std::string telefono;
                std::string email;
                std::string pais;

                
                if (!(std::getline(ss, sId, '|') && std::getline(ss, nombre, '|') && 
                    std::getline(ss, telefono, '|') && std::getline(ss, email, '|') && 
                    std::getline(ss, pais, '|'))) {

                    throw std::runtime_error("Linea de proveedor incompleta");
                }

                int id = ValidadorCentral::validarYConvertirEntero(sId);

                sistema.cargarProveedorDesdeArchivo(id, nombre, telefono, email, pais);

                validas_++;
            }
            catch (const std::exception& e) {
                // Cualquier error cae en esta red de seguridad
                descartadas_++;
            }
        }

        archivo.close();
    }

    void ArchivoCarga::cargarMedicamentos(SistemaFarmacia& sistema) {

        std::ifstream archivo(rutaMedicamentos_, std::ios::in);

        if (!archivo.is_open()) {
            return;
        }

        std::string linea;

        while (std::getline(archivo, linea)) {

            if (linea.empty()) {
                descartadas_++;
                continue; 
            }

            try {
                std::stringstream ss(linea);

                std::string sCategoria;
                std::string sId;
                std::string nombre;
                std::string sPrecio;
                std::string sStock;
                std::string sReceta;
                std::string sIdProveedor;

                if (!(std::getline(ss, sCategoria, '|') &&
                    std::getline(ss, sId, '|') &&
                    std::getline(ss, nombre, '|') &&
                    std::getline(ss, sPrecio, '|') &&
                    std::getline(ss, sStock, '|') &&
                    std::getline(ss, sReceta, '|') &&
                    std::getline(ss, sIdProveedor, '|'))) {

                    throw std::runtime_error("Linea de medicamento incompleta");
                }

                // Si encuentra una letra donde va un numero, dispara un throw automatico
                int id = ValidadorCentral::validarYConvertirEntero(sId);
                double precio = ValidadorCentral::validarYConvertirDecimal(sPrecio);
                int stock = ValidadorCentral::validarYConvertirEntero(sStock);
                int idProveedor = ValidadorCentral::validarYConvertirEntero(sIdProveedor);
                bool receta = ValidadorCentral::validarBanderaBinaria(sReceta);

                Medicamento* medicamento = crearMedicamentoDesdeTexto(ss, sCategoria, id, nombre, precio,
                    receta, stock, idProveedor);

                if (medicamento == nullptr) {
                    throw std::runtime_error("Error al instanciar tipo de medicamento");
                }

                sistema.cargarMedicamentoDesdeArchivo(medicamento);
                validas_++;
            }
            catch (const std::exception& e) {
      
                descartadas_++;
            }
        }

        archivo.close();
    }

    void ArchivoCarga::cargarClientes(SistemaFarmacia& sistema) {

        std::ifstream archivo(rutaClientes_, std::ios::in);

        if (!archivo.is_open()) {
            return;
        }

        std::string linea;

        while (std::getline(archivo, linea)) {

            if (linea.empty()) {
                descartadas_++;
                continue;
            }

            try {
                std::stringstream ss(linea);

                std::string sId;
                std::string nombre;
                std::string cedula;
                std::string sTarjeta;

                if (!(std::getline(ss, sId, '|') &&
                    std::getline(ss, nombre, '|') &&
                    std::getline(ss, cedula, '|') &&
                    std::getline(ss, sTarjeta, '|'))) {

                    // Si faltan columnas en la linea, disparamos el descarte
                    throw std::runtime_error("Linea incompleta");
                }

                // Si algo esta mal, tira un throw automaticamente y el programa salta al catch
                int id = ValidadorCentral::validarYConvertirEntero(sId);
                bool tieneTarjeta = ValidadorCentral::validarBanderaBinaria(sTarjeta);

                // Si todo el control de calidad paso con exito, cargamos el cliente
                sistema.cargarClienteDesdeArchivo(id, nombre, cedula, tieneTarjeta);

                validas_++;
            }
            catch (const std::exception& e) {

                descartadas_++;
            }
        }

        archivo.close();
    }

    void ArchivoCarga::cargarVentas(SistemaFarmacia& sistema) {

        std::ifstream archivo(rutaVentas_, std::ios::in);

        if (!archivo.is_open()) {
            return;
        }

        std::string linea;

        while (std::getline(archivo, linea)) {

            if (linea.empty()) {
                descartadas_++;
                continue; 
            }

            try {
                std::stringstream ss(linea);

                std::string sId;
                std::string sIdCliente;
                std::string sIdMedicamento;
                std::string sCantidad;
                std::string sTotal;
                std::string fecha;

      
                if (!(std::getline(ss, sId, '|') && std::getline(ss, sIdCliente, '|') &&
                    std::getline(ss, sIdMedicamento, '|') && std::getline(ss, sCantidad, '|') &&
                    std::getline(ss, sTotal, '|') &&  std::getline(ss, fecha, '|'))) {

                    throw std::runtime_error("Linea de venta incompleta");
                }

                int idVenta = ValidadorCentral::validarYConvertirEntero(sId);
                int idCliente = ValidadorCentral::validarYConvertirEntero(sIdCliente);
                int idMedicamento = ValidadorCentral::validarYConvertirEntero(sIdMedicamento);
                int cantidad = ValidadorCentral::validarYConvertirEntero(sCantidad);
                double total = ValidadorCentral::validarYConvertirDecimal(sTotal);

                // Buscar si el medicamento comprado realmente existe
                Medicamento* medicamentoAsociado = sistema.buscarMedicamentoPorID(idMedicamento);

                if (medicamentoAsociado == nullptr) {

                    throw std::runtime_error("El medicamento de la venta no existe en el sistema");
                }

                // Si todas las conversiones y busquedas salieron perfectas, se registra la venta circular
                sistema.cargarVentaDesdeArchivo(idVenta, idCliente, idMedicamento, cantidad, total, fecha);

                validas_++;
            }
            catch (const std::exception& e) {

                descartadas_++;
            }
        }

        archivo.close();
    }

    int ArchivoCarga::getValidas() const { return validas_; }

    int ArchivoCarga::getDescartadas() const { return descartadas_; }

} // namespace FarmaSystem