#include "ListaVentas.h"
#include <iostream>

namespace FarmaSystem {

    ListaVentas::ListaVentas()
        : cola(nullptr), cantidadVentas(0) {
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

        if (cola == nullptr) {

            return 0;
        }

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

                total += actual->dato->getPrecioFinal();
            }

            actual = actual->siguiente;

        } while (actual != cabeza);

        return total;
    }

    void ListaVentas::imprimirHistorial() const {

        if (cola == nullptr) {

            return;
        }

        NodoVenta* cabeza = getCabeza();
        NodoVenta* actual = cabeza;

        do {

            std::cout << actual->dato->getInfoVenta() << std::endl;

            actual = actual->siguiente;

        } while (actual != cabeza);
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

}