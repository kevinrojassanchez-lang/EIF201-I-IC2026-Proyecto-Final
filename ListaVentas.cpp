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