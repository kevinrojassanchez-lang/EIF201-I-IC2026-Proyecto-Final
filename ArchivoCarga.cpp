#include <fstream>
#include "ArchivoCarga.h"
#include "MedicamentoGenerico.h"
#include "MedicamentoMarca.h"
#include "MedicamentoControlado.h"

namespace FarmaSystem {

    ArchivoCarga::ArchivoCarga() {

        rutaProveedores_ = "datos/proveedores.txt";
        rutaMedicamentos_ = "datos/medicamentos.txt";
        rutaClientes_ = "datos/clientes.txt";
        rutaVentas_ = "datos/ventas.txt";
        validas_ = 0;
        descartadas_ = 0;
    }

    // Funciones explicitas de validacion con aritmetica de caracteres
    bool esTextoEnteroValido(const std::string& texto) {

        if (texto.empty()) {
            return false;
        }

        for (size_t i = 0; i < texto.length(); i++) {

            char caracter = texto[i];

            if (caracter < '0' || caracter > '9') {
                return false;
            }
        }

        return true;
    }

    bool esTextoDecimalValido(const std::string& texto) {

        if (texto.empty()) {
            return false;
        }

        int contadorPuntos = 0;

        for (size_t i = 0; i < texto.length(); i++) {

            char caracter = texto[i];

            if (caracter == '.') {

                contadorPuntos++;

                if (contadorPuntos > 1) {
                    return false;
                }
            }
            else if (caracter < '0' || caracter > '9') {

                return false;
            }
        }

        return true;
    }

    Medicamento* ArchivoCarga::crearMedicamentoDesdeTexto(
        std::stringstream& ss,
        const std::string& categoria,
        int id,
        const std::string& nombre,
        double precio,
        bool receta,
        int stock,
        int idProveedor)
    {
        if (categoria == "Generico") {

            std::string principio;

            if (std::getline(ss, principio, '|')) {

                return new MedicamentoGenerico(
                    id,
                    nombre,
                    precio,
                    receta,
                    stock,
                    principio,
                    idProveedor
                );
            }
        }
        else if (categoria == "Marca") {

            std::string paisOrigen;
            std::string sPromo;

            if (std::getline(ss, paisOrigen, '|') &&
                std::getline(ss, sPromo, '|')) {

                bool promo = (sPromo == "1");

                return new MedicamentoMarca(
                    id,
                    nombre,
                    precio,
                    receta,
                    stock,
                    paisOrigen,
                    promo,
                    idProveedor
                );
            }
        }
        else if (categoria == "Controlado") {

            std::string sNivel;
            std::string sDosis;

            if (std::getline(ss, sNivel, '|') &&
                std::getline(ss, sDosis, '|')) {

                bool nivelValido = esTextoEnteroValido(sNivel);
                bool dosisValida = esTextoDecimalValido(sDosis);

                if (nivelValido && dosisValida) {

                    int nivel = 0;
                    double dosis = 0.0;

                    std::stringstream(sNivel) >> nivel;
                    std::stringstream(sDosis) >> dosis;

                    return new MedicamentoControlado(
                        id,
                        nombre,
                        precio,
                        stock,
                        nivel,
                        dosis,
                        idProveedor
                    );
                }
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

            if (!linea.empty()) {

                std::stringstream ss(linea);

                std::string sId;
                std::string nombre;
                std::string telefono;
                std::string email;
                std::string pais;

                if (std::getline(ss, sId, '|') &&
                    std::getline(ss, nombre, '|') &&
                    std::getline(ss, telefono, '|') &&
                    std::getline(ss, email, '|') &&
                    std::getline(ss, pais, '|')) {

                    if (esTextoEnteroValido(sId)) {

                        int id = 0;

                        std::stringstream(sId) >> id;

                        sistema.cargarProveedorDesdeArchivo(
                            id,
                            nombre,
                            telefono,
                            email,
                            pais
                        );

                        validas_++;
                    }
                    else {

                        descartadas_++;
                    }
                }
                else {

                    descartadas_++;
                }
            }
            else {

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

            if (!linea.empty()) {

                std::stringstream ss(linea);

                std::string sCategoria;
                std::string sId;
                std::string nombre;
                std::string sPrecio;
                std::string sStock;
                std::string sReceta;
                std::string sIdProveedor;

                if (std::getline(ss, sCategoria, '|') &&
                    std::getline(ss, sId, '|') &&
                    std::getline(ss, nombre, '|') &&
                    std::getline(ss, sPrecio, '|') &&
                    std::getline(ss, sStock, '|') &&
                    std::getline(ss, sReceta, '|') &&
                    std::getline(ss, sIdProveedor, '|')) {

                    bool datosValidos = true;

                    if (!esTextoEnteroValido(sId)) {
                        datosValidos = false;
                    }

                    if (!esTextoDecimalValido(sPrecio)) {
                        datosValidos = false;
                    }

                    if (!esTextoEnteroValido(sStock)) {
                        datosValidos = false;
                    }

                    if (!esTextoEnteroValido(sIdProveedor)) {
                        datosValidos = false;
                    }

                    if (sReceta != "0" && sReceta != "1") {
                        datosValidos = false;
                    }

                    if (datosValidos) {

                        int id = 0;
                        int stock = 0;
                        int idProveedor = 0;

                        double precio = 0.0;

                        std::stringstream(sId) >> id;
                        std::stringstream(sStock) >> stock;
                        std::stringstream(sIdProveedor) >> idProveedor;
                        std::stringstream(sPrecio) >> precio;

                        bool receta = (sReceta == "1");

                        Medicamento* medicamento =
                            crearMedicamentoDesdeTexto(
                                ss,
                                sCategoria,
                                id,
                                nombre,
                                precio,
                                receta,
                                stock,
                                idProveedor
                            );

                        if (medicamento != nullptr) {

                            sistema.cargarMedicamentoDesdeArchivo(medicamento);

                            validas_++;
                        }
                        else {

                            descartadas_++;
                        }
                    }
                    else {

                        descartadas_++;
                    }
                }
                else {

                    descartadas_++;
                }
            }
            else {

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

            if (!linea.empty()) {

                std::stringstream ss(linea);

                std::string sId;
                std::string nombre;
                std::string cedula;
                std::string sTarjeta;

                if (std::getline(ss, sId, '|') &&
                    std::getline(ss, nombre, '|') &&
                    std::getline(ss, cedula, '|') &&
                    std::getline(ss, sTarjeta, '|')) {

                    bool datosValidos = true;

                    if (!esTextoEnteroValido(sId)) {
                        datosValidos = false;
                    }

                    if (sTarjeta != "0" && sTarjeta != "1") {
                        datosValidos = false;
                    }

                    if (datosValidos) {

                        int id = 0;

                        std::stringstream(sId) >> id;

                        sistema.cargarClienteDesdeArchivo(
                            id,
                            nombre,
                            cedula,
                            (sTarjeta == "1")
                        );

                        validas_++;
                    }
                    else {

                        descartadas_++;
                    }
                }
                else {

                    descartadas_++;
                }
            }
            else {

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

            if (!linea.empty()) {

                std::stringstream ss(linea);

                std::string sId;
                std::string sIdCliente;
                std::string sIdMedicamento;
                std::string sCantidad;
                std::string sTotal;
                std::string fecha;

                if (std::getline(ss, sId, '|') &&
                    std::getline(ss, sIdCliente, '|') &&
                    std::getline(ss, sIdMedicamento, '|') &&
                    std::getline(ss, sCantidad, '|') &&
                    std::getline(ss, sTotal, '|') &&
                    std::getline(ss, fecha, '|')) {

                    bool ventaValida = true;

                    if (!esTextoEnteroValido(sId)) {
                        ventaValida = false;
                    }

                    if (!esTextoEnteroValido(sIdCliente)) {
                        ventaValida = false;
                    }

                    if (!esTextoEnteroValido(sIdMedicamento)) {
                        ventaValida = false;
                    }

                    if (!esTextoEnteroValido(sCantidad)) {
                        ventaValida = false;
                    }

                    if (!esTextoDecimalValido(sTotal)) {
                        ventaValida = false;
                    }

                    if (ventaValida) {

                        int idVenta = 0;
                        int idCliente = 0;
                        int idMedicamento = 0;
                        int cantidad = 0;

                        double total = 0.0;

                        std::stringstream(sId) >> idVenta;
                        std::stringstream(sIdCliente) >> idCliente;
                        std::stringstream(sIdMedicamento) >> idMedicamento;
                        std::stringstream(sCantidad) >> cantidad;
                        std::stringstream(sTotal) >> total;

                        Medicamento* medicamentoAsociado =
                            sistema.buscarMedicamentoPorID(idMedicamento);

                        if (medicamentoAsociado != nullptr) {

                            sistema.cargarVentaDesdeArchivo(
                                idVenta,
                                idCliente,
                                idMedicamento,
                                cantidad,
                                total,
                                fecha
                            );

                            validas_++;
                        }
                        else {

                            descartadas_++;
                        }
                    }
                    else {

                        descartadas_++;
                    }
                }
                else {

                    descartadas_++;
                }
            }
            else {

                descartadas_++;
            }
        }

        archivo.close();
    }

    int ArchivoCarga::getValidas() const {
        return validas_;
    }

    int ArchivoCarga::getDescartadas() const {
        return descartadas_;
    }

} // namespace FarmaSystem