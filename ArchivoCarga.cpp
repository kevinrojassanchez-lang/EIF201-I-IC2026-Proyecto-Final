#include "ArchivoCarga.h"

#include <sstream>

#include "MedicamentoGenerico.h"
#include "MedicamentoMarca.h"
#include "MedicamentoControlado.h"
#include "ValidadorCentral.h"

namespace FarmaSystem {

    // PROVEEDORES

    Proveedor* ArchivoCarga::crearProveedor(const std::string& linea) {

        try {

            std::stringstream ss(linea);

            std::string idStr;
            std::string nombre;
            std::string telefono;
            std::string email;
            std::string pais;

            std::getline(ss, idStr, '|');
            std::getline(ss, nombre, '|');
            std::getline(ss, telefono, '|');
            std::getline(ss, email, '|');
            std::getline(ss, pais, '|');

            int id = ValidadorCentral::validarYConvertirEntero(idStr);

            return new Proveedor(id, nombre, telefono, email, pais);
        }

        catch (...) { return nullptr; }
    }

    // CLIENTES

    Cliente* ArchivoCarga::crearCliente(const std::string& linea) {

        try {

            std::stringstream ss(linea);

            std::string idStr;
            std::string nombre;
            std::string cedula;
            std::string tarjetaStr;

            std::getline(ss, idStr, '|');
            std::getline(ss, nombre, '|');
            std::getline(ss, cedula, '|');
            std::getline(ss, tarjetaStr, '|');

            int id = ValidadorCentral::validarYConvertirEntero(idStr);
            bool tarjeta = ValidadorCentral::validarBanderaBinaria(tarjetaStr);

            Cliente* nuevo = new Cliente(id, nombre, cedula);

            nuevo->setTarjeta(tarjeta);

            return nuevo;
        }

        catch (...) { return nullptr; }
    }

    // MEDICAMENTOS

    Medicamento* ArchivoCarga::crearMedicamento(const std::string& linea) {

        try {

            std::stringstream ss(linea);

            std::string tipoTexto;
            std::string idStr;
            std::string nombre;
            std::string precioStr;
            std::string stockStr;
            std::string recetaStr;
            std::string idProvStr;
            std::string esp1;
            std::string esp2;

            std::getline(ss, tipoTexto, '|');
            std::getline(ss, idStr, '|');
            std::getline(ss, nombre, '|');
            std::getline(ss, precioStr, '|');
            std::getline(ss, stockStr, '|');
            std::getline(ss, recetaStr, '|');
            std::getline(ss, idProvStr, '|');

            int id = ValidadorCentral::validarYConvertirEntero(idStr);
            double precio = ValidadorCentral::validarYConvertirDecimal(precioStr);
            int stock = ValidadorCentral::validarYConvertirEntero(stockStr);
            bool receta = ValidadorCentral::validarBanderaBinaria(recetaStr);
            int idProveedor = ValidadorCentral::validarYConvertirEntero(idProvStr);

            if (tipoTexto == "Generico") {

                std::getline(ss, esp1, '|');

                return new MedicamentoGenerico(id, nombre, precio, receta, stock, esp1, idProveedor);
            }

            if (tipoTexto == "Marca") {

                std::getline(ss, esp1, '|');
                std::getline(ss, esp2, '|');

                bool promo = ValidadorCentral::validarBanderaBinaria(esp2);

                return new MedicamentoMarca(id, nombre, precio, receta, stock, esp1, promo, idProveedor);
            }

            if (tipoTexto == "Controlado") {

                std::getline(ss, esp1, '|');
                std::getline(ss, esp2, '|');

                int nivel =ValidadorCentral::validarYConvertirEntero(esp1);
                double dosis = ValidadorCentral::validarYConvertirDecimal(esp2);

                return new MedicamentoControlado(id, nombre, precio, receta, stock, nivel, dosis, idProveedor);
            }

            return nullptr;
        }

        catch (...) { return nullptr; }
    }

    // VENTAS

    Venta* ArchivoCarga::crearVenta(const std::string& linea, Medicamento* medicamento) {

        try {

            if (medicamento == nullptr) { return nullptr; }

            std::stringstream ss(linea);

            std::string idStr;
            std::string idClienteStr;
            std::string idMedStr; // La columna del ID del medicamento en el texto
            std::string cantidadStr;
            std::string totalStr;
            std::string fecha;

            std::getline(ss, idStr, '|');
            std::getline(ss, idClienteStr, '|');
            std::getline(ss, idMedStr, '|'); // Extraemos el texto
            std::getline(ss, cantidadStr, '|');
            std::getline(ss, totalStr, '|');
            std::getline(ss, fecha, '|');

            int id = ValidadorCentral::validarYConvertirEntero(idStr);
            int idCliente = ValidadorCentral::validarYConvertirEntero(idClienteStr);
            int idMedicamentoArchivo = ValidadorCentral::validarYConvertirEntero(idMedStr);
            int cantidad = ValidadorCentral::validarYConvertirEntero(cantidadStr);
            double precioFinal = ValidadorCentral::validarYConvertirDecimal(totalStr);

            // Validamos que el ID del archivo coincida con el medicamento de la ram
            if (idMedicamentoArchivo != medicamento->getID()) { return nullptr; }

            return new Venta(id, idCliente, medicamento, cantidad, precioFinal, fecha);
        }

        catch (...) { return nullptr; }
    }
} // namespace FarmaSystem