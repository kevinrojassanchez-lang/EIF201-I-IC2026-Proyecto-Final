#include "ListaProveedores.h"

namespace FarmaSystem {

	// Constructor para inicializar la lista vacia
    ListaProveedores::ListaProveedores()
        : cabeza(nullptr), cola(nullptr), tamano(0) {
    }

	// Destructor para limpiar la lista y liberar memoria
    ListaProveedores::~ListaProveedores() {
        
        limpiar();
    }

	// Metodo para agregar un proveedor al final de la lista
    void ListaProveedores::agregar(Proveedor* prov) {

        NodoProveedor* nuevo = new NodoProveedor(prov);

        if (cabeza == nullptr) {

            cabeza = nuevo;
            cola = nuevo;
        }
        else {

            nuevo->anterior = cola;
            cola->siguiente = nuevo;
            cola = nuevo;
        }

        tamano++;
    }

	// Metodo para obtener la cabeza de la lista
    NodoProveedor* ListaProveedores::getCabeza() const { return cabeza; }

	// Metodo para obtener la cantidad total de proveedores en la lista
    int ListaProveedores::cantidad() const { return tamano; }

	// Metodo para obtener un proveedor por su posicion en la lista
    Proveedor* ListaProveedores::obtener(int indice) {

        if (indice < 0 || indice >= tamano) {
            return nullptr;
        }

        NodoProveedor* actual = nullptr;

        if (indice < tamano / 2) {

            actual = cabeza;

            int posicion = 0;

            while (posicion < indice) {

                actual = actual->siguiente;
                posicion++;
            }
        }
        else {

            actual = cola;

            int posicion = tamano - 1;

            while (posicion > indice) {

                actual = actual->anterior;
                posicion--;
            }
        }

        return actual->dato;
    }

	// Metodo para buscar un proveedor por su nombre
    Proveedor* ListaProveedores::buscarPorNombre(std::string nombre) {

        NodoProveedor* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato->getNombre() == nombre) {
                return actual->dato;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

	// Metodo para buscar un proveedor por su ID
    Proveedor* ListaProveedores::buscarPorId(int id) {

        NodoProveedor* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato->getID() == id) {
                return actual->dato;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

	// Metodo para eliminar un proveedor por su ID
    bool ListaProveedores::eliminar(int id, bool tieneMedicamentosAsociados) {

        if (tieneMedicamentosAsociados) {
            return false;
        }

        NodoProveedor* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato->getID() == id) {

                if (actual->anterior != nullptr) {
                    actual->anterior->siguiente = actual->siguiente;
                }
                else {
                    cabeza = actual->siguiente;
                }

                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                }
                else {
                    cola = actual->anterior;
                }

                delete actual->dato;
                delete actual;

                tamano--;

                return true;
            }

            actual = actual->siguiente;
        }

        return false;
    }

	// Metodo para guardar todos los proveedores en un archivo de texto
    void ListaProveedores::guardarEnArchivo(std::ofstream& archivo) {

        NodoProveedor* actual = cabeza;

        while (actual != nullptr) {

            Proveedor* proveedor = actual->dato;

            if (proveedor != nullptr) {

				archivo << proveedor->toString() << "\n";
            }

            actual = actual->siguiente;
        }
    }

	// Metodo para limpiar la lista y liberar toda la memoria asociada
    void ListaProveedores::limpiar() {

        NodoProveedor* actual = cabeza;

        while (actual != nullptr) {

            NodoProveedor* temp = actual->siguiente;

            delete actual->dato;
            delete actual;

            actual = temp;
        }

        cabeza = nullptr;
        cola = nullptr;
        tamano = 0;
    }

} // namespace FarmaSystem