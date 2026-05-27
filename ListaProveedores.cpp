#include "ListaProveedores.h"
#include <iostream>

namespace FarmaSystem {

    ListaProveedores::ListaProveedores()
        : cabeza(nullptr), cola(nullptr), tamano(0) {
    }

    ListaProveedores::~ListaProveedores() {
        limpiar();
        std::cout << "[ListaProveedores destruida]" << std::endl;
    }

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

    NodoProveedor* ListaProveedores::getCabeza() const {
        return cabeza;
    }

    int ListaProveedores::cantidad() const {
        return tamano;
    }

    Proveedor* ListaProveedores::obtener(int indice) {

        if (indice < 0 || indice >= tamano) {
            return nullptr;
        }

        NodoProveedor* actual = cabeza;

        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        return actual->dato;
    }

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

}