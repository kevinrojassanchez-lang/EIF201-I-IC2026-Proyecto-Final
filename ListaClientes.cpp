#include "ListaClientes.h"
#include <iostream>


namespace FarmaSystem {

    ListaClientes::ListaClientes()
        : cabeza(nullptr), cola(nullptr), tamano(0) {
    }

    ListaClientes::~ListaClientes() {
        limpiar();
    }

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

    NodoCliente* ListaClientes::getCabeza() const {

        return cabeza;
    }

    int ListaClientes::cantidad() const {

        return tamano;
    }

    Cliente* ListaClientes::obtener(int indice) {

        if (indice < 0 || indice >= tamano) {

            return nullptr;
        }

        NodoCliente* actual = cabeza;

        for (int i = 0; i < indice; i++) {

            actual = actual->siguiente;
        }

        return actual->dato;
    }

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

    Cliente* ListaClientes::obtenerClienteVIP(ListaVentas& ventas) {

        if (cabeza == nullptr || ventas.cantidad() == 0) {
            return nullptr;
        }

        Cliente* vip = nullptr;
        double mayorGasto = -1.0;

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            Cliente* cli = actual->dato;

            if (cli != nullptr) {

                double acumulado =
                    ventas.calcularAcumuladoPorCliente(cli->getID());

                if (acumulado > mayorGasto) {

                    mayorGasto = acumulado;
                    vip = cli;
                }
            }

            actual = actual->siguiente;
        }

        return vip;
    }

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

    void ListaClientes::mostrarTodos() {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr) {
                actual->dato->mostrar();
            }

            actual = actual->siguiente;
        }
    }

    void ListaClientes::guardarEnArchivo(std::ofstream& archivo) {

        NodoCliente* actual = cabeza;

        while (actual != nullptr) {

            Cliente* cliente = actual->dato;

            if (cliente != nullptr) {

                archivo << cliente->getID() << "|"
                    << cliente->getNombre() << "|"
                    << cliente->getCedula() << "|"
                    << (cliente->getTarjeta() ? "1" : "0")
                    << "\n";
            }

            actual = actual->siguiente;
        }
    }

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