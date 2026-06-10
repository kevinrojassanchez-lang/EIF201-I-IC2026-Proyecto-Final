#include "ListaVentas.h"

namespace FarmaSystem {

	// Constructor para inicializar la lista vacia
    ListaVentas::ListaVentas() : cola(nullptr), cantidadVentas(0) {
		// La lista circular se representa con un puntero a la cola, que apunta al ultimo nodo
    }

	// Destructor para limpiar la lista y liberar memoria
    ListaVentas::~ListaVentas() {

        limpiar();
    }

	// Metodo para obtener la cabeza de la lista circular 
    NodoVenta* ListaVentas::getCabeza() const {

        return (cola != nullptr) ? cola->siguiente : nullptr;
    }

	// Metodo para agregar una venta al final de la lista circular
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

	// Metodo para obtener la cantidad total de ventas en la lista
    int ListaVentas::cantidad() const {

        return cantidadVentas;
    }

	// Metodo para obtener una venta por su posicion en la lista
    Venta* ListaVentas::obtener(int indice) {

        if (indice < 0 || indice >= cantidadVentas || cola == nullptr) {
            return nullptr;
        }

        NodoVenta* actual = getCabeza();

        int posicion = 0;

        while (posicion < indice) {

            actual = actual->siguiente;
            posicion++;
        }

        return actual->dato;
    }

	// Metodo para verificar si existe una venta asociada a un medicamento
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

	// Metodo para verificar si existe una venta asociada a un cliente
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

	// Metodo para imprimir el historial de ventas en el frontend
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

	// Metodo para guardar el historial de ventas en un archivo de texto
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

	// Metodo para limpiar la lista y liberar memoria
    void ListaVentas::limpiar() {

        if (cola == nullptr) {

            return;
        }

        NodoVenta* cabeza = cola->siguiente;

        cola->siguiente = nullptr; // Romper el ciclo

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

} // Fin namespace FarmaSystem