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

        NodoProveedor* actual = sistema.getListaProveedores().getCabeza();
        while (actual != nullptr) {
            Proveedor* _proveedor = actual->dato;
            if (_proveedor != nullptr) {
                archivo << _proveedor->getID() << "|"
                    << _proveedor->getNombre() << "|"
                    << _proveedor->getTelefono() << "|"
                    << _proveedor->getEmail() << "|"
                    << _proveedor->getPais() << "\n";
            }
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void ArchivoGuardado::guardarMedicamentos(SistemaFarmacia& sistema) {
        std::ofstream archivo(rutaMedicamentos_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        NodoMedicamento* actual = sistema.getListaMedicamentos().getCabeza();

        while (actual != nullptr) {
            Medicamento* med = actual->dato;

            if (med != nullptr) {
                archivo << med->toFile() << "\n";
            }
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void ArchivoGuardado::guardarClientes(SistemaFarmacia& sistema) {
        std::ofstream archivo(rutaClientes_, std::ios::out | std::ios::trunc);

        if (!archivo.is_open()) { return; }

        NodoCliente* actual = sistema.getListaClientes().getCabeza();

        while (actual != nullptr) {
            Cliente* cliente = actual->dato;

            if (cliente != nullptr) {
                archivo << cliente->getID() << "|" << cliente->getNombre() << "|" << cliente->getCedula() << "|";
                if (cliente->getTarjeta() == true) {
                    archivo << "1\n";
                }
                else {
                    archivo << "0\n";
                }
            }
            actual = actual->siguiente;
        }
        archivo.close();
    }

    void ArchivoGuardado::guardarVentas(SistemaFarmacia& sistema) {
        std::ofstream archivo(rutaVentas_, std::ios::out | std::ios::trunc);
        if (!archivo.is_open()) { return; }

        NodoVenta* cabeza = sistema.getListaVentas().getCabeza();
        if (cabeza != nullptr) {
            NodoVenta* actual = cabeza;
            do {
                Venta* venta = actual->dato;
                if (venta != nullptr) {
                    int idMedicamento = 0;
                    if (venta->getMedicamentoVendido() != nullptr) {
                        idMedicamento = venta->getMedicamentoVendido()->getID();
                    }
                    archivo << venta->getId() << "|"
                        << venta->getIdCliente() << "|"
                        << idMedicamento << "|"
                        << venta->getCantidad() << "|"
                        << venta->getPrecioFinal() << "|"
                        << venta->getFecha() << "\n";
                }
                actual = actual->siguiente;
            } while (actual != cabeza);
        }
        archivo.close();
    }

} // namespace FarmaSystem