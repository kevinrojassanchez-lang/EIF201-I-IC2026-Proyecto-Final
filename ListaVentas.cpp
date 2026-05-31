#include "ListaVentas.h"
#include <iostream>

namespace FarmaSystem {

    ListaVentas::ListaVentas() : cola(nullptr), cantidadVentas(0) {

    }

    ListaVentas::~ListaVentas() {

        limpiar();
    }

    NodoVenta* ListaVentas::getCabeza() const {

        return (cola != nullptr) ? cola->siguiente : nullptr;
    }

    void ListaVentas::agregar(Venta* v) {

        NodoVenta* nuevo = new NodoVenta(v);

        if (cola == nullptr) {

            cola = nuevo;
            nuevo->siguiente = nuevo;
        }
        else {

            nuevo->siguiente = cola->siguiente;
            cola->siguiente = nuevo;

            cola = nuevo;
        }

        cantidadVentas++;
    }

    int ListaVentas::cantidad() const {

        return cantidadVentas;
    }

    Venta* ListaVentas::obtener(int indice) {

        if (indice < 0 || indice >= cantidadVentas || cola == nullptr) {

            return nullptr;
        }

        NodoVenta* actual = getCabeza();

        for (int i = 0; i < indice; i++) {

            actual = actual->siguiente;
        }

        return actual->dato;
    }

    double ListaVentas::calcularIngresosTotales() {

        if (cola == nullptr) { return 0; }

        double total = 0;

        NodoVenta* cabeza = getCabeza();
        NodoVenta* actual = cabeza;

        do {

            total += actual->dato->getPrecioFinal();

            actual = actual->siguiente;

        } while (actual != cabeza);

        return total;
    }

    double ListaVentas::calcularAcumuladoPorCliente(int idCliente) {

        if (cola == nullptr) {

            return 0;
        }

        double total = 0;

        NodoVenta* cabeza = getCabeza();
        NodoVenta* actual = cabeza;

        do {

            if (actual->dato->getIdCliente() == idCliente) {

				total += actual->dato->getPrecioFinal(); // getPrecioFinal ya incluye el 
            }                                             // calculo del precio unitario por cantidad

            actual = actual->siguiente;

        } while (actual != cabeza);

        return total;
    }

    void ListaVentas::imprimirHistorial() const {

        if (cola == nullptr) {
            return;
        }

        NodoVenta* cabeza = cola->siguiente;
        NodoVenta* actual = cabeza;

        do {

            if (actual->dato != nullptr) {
                actual->dato->toString();
            }

            actual = actual->siguiente;

        } while (actual != cabeza);
    }

    bool ListaVentas::existeVentaDeMedicamento(int idMedicamento) {

        if (cola == nullptr) return false;

        NodoVenta* cabeza = cola->siguiente;
        NodoVenta* actual = cabeza;

        do {

            if (actual->dato != nullptr &&
                actual->dato->getMedicamentoVendido() != nullptr &&
                actual->dato->getMedicamentoVendido()->getID() == idMedicamento) {
                return true;
            }

            actual = actual->siguiente;

        } while (actual != cabeza);

        return false;
    }

    bool ListaVentas::existeVentaDeCliente(int idCliente) {

        if (cola == nullptr) { return false; }

        NodoVenta* cabeza = cola->siguiente;
        NodoVenta* actual = cabeza;

        do {

            if (actual->dato != nullptr && actual->dato->getIdCliente() == idCliente) {
                return true;
            }

            actual = actual->siguiente;

        } while (actual != cabeza);

        return false;
    }

    Medicamento* ListaVentas::obtenerMedicamentoMasVendido(ListaMedicamentos& medicamentos) {

        Medicamento* medicamentoMasVendido = nullptr;
        int maxUnidades = -1;

        NodoMedicamento* actualMedicamento = medicamentos.getCabeza();

        while (actualMedicamento != nullptr) {

            Medicamento* medicamento = actualMedicamento->dato;

            if (medicamento != nullptr) {

				int idMedicamento = medicamento->getID();
                int unidades = calcularUnidadesPorMedicamento(idMedicamento);

                if (unidades > maxUnidades) {

                    maxUnidades = unidades;
                    medicamentoMasVendido = medicamento;
                }
            }

            actualMedicamento = actualMedicamento->siguiente;
        }

        return medicamentoMasVendido;
    }

    int ListaVentas::calcularUnidadesPorMedicamento(int idMedicamento) {

        int total = 0;

        NodoVenta* actual = cola ? cola->siguiente : nullptr;

        if (actual == nullptr) { return 0; }

        do {

            if (actual->dato != nullptr &&
                actual->dato->getMedicamentoVendido() != nullptr &&
                actual->dato->getMedicamentoVendido()->getID() == idMedicamento) {

                total += actual->dato->getCantidad();
            }

            actual = actual->siguiente;

        } while (actual != cola->siguiente);

        return total;
    }

    void ListaVentas::limpiar() {

        if (cola == nullptr) {

            return;
        }

        NodoVenta* cabeza = cola->siguiente;

        // Romper el ciclo
        cola->siguiente = nullptr;

        NodoVenta* actual = cabeza;

        while (actual != nullptr) {

            NodoVenta* temp = actual->siguiente;

            delete actual->dato;
            delete actual;

            actual = temp;
        }

        cola = nullptr;
        cantidadVentas = 0;
    }

    void ListaVentas::guardarEnArchivo(std::ofstream& archivo) {

        if (cola == nullptr) {
            return;
        }

        NodoVenta* cabeza = cola->siguiente;
        NodoVenta* actual = cabeza;

        do {

            Venta* venta = actual->dato;

            if (venta != nullptr) {

                archivo << venta->toString() << "\n";
            }

            actual = actual->siguiente;

        } while (actual != cabeza);
    }

}