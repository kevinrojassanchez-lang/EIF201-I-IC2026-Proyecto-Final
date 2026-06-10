#include "ListaClientes.h"

namespace FarmaSystem {

	// Constructor para inicializar la lista vacia
    ListaClientes::ListaClientes()
        : cabeza(nullptr), cola(nullptr), tamano(0) {
    }

	// Destructor para limpiar la lista y liberar memoria
    ListaClientes::~ListaClientes() { 

        limpiar();
    }

	// Metodo para agregar un cliente al final de la lista
    void ListaClientes::agregar(Cliente* cli) {

        NodoCliente* nuevo = new NodoCliente(cli);

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
    NodoCliente* ListaClientes::getCabeza() const { return cabeza; }

	// Metodo para obtener la cantidad total de clientes en la lista
    int ListaClientes::cantidad() const { return tamano; }

	// Metodo para obtener un cliente por su posicion en la lista
    Cliente* ListaClientes::obtener(int indice) {

        if (indice < 0 || indice >= tamano) {
            return nullptr;
        }

        NodoCliente* actual = nullptr;

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

	// Metodo para buscar un cliente por su ID
    Cliente* ListaClientes::buscarPorId(int id) {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato->getID() == id) {

                return actual->dato;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

	// Metodo para buscar un cliente por su cedula
    Cliente* ListaClientes::buscarPorCedula(std::string cedula) {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato->getCedula() == cedula) {

                return actual->dato;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

	// Metodo para eliminar un cliente por su ID
    bool ListaClientes::eliminar(int id) {

        NodoCliente* actual = cabeza;

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

                actual = nullptr;

                tamano--;

                return true;
            }

            actual = actual->siguiente;
        }

        return false;
    }

	// Metodo para mostrar todos los clientes en el frontend
    void ListaClientes::mostrarTodos() {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr) {

                actual->dato->toString();
            }

            actual = actual->siguiente;
        }
    }

	// Metodo para guardar todos los clientes en un archivo de texto
    void ListaClientes::guardarEnArchivo(std::ofstream& archivo) {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            Cliente* cliente = actual->dato;

            if (cliente != nullptr) {

                archivo << cliente->toString() << "\n";
            }

            actual = actual->siguiente;
        }
    }

	// Metodo para limpiar la lista y liberar memoria
    void ListaClientes::limpiar() {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            NodoCliente* temp = actual->siguiente;

            delete actual->dato;
            delete actual;

            actual = temp;
        }

        cabeza = nullptr;
        cola = nullptr;
        tamano = 0;
    }

}