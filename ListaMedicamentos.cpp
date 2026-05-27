#include "ListaMedicamentos.h"

namespace FarmaSystem {

    ListaMedicamentos::ListaMedicamentos()
        : cabeza(nullptr), tamano(0) {
    }

    ListaMedicamentos::~ListaMedicamentos() {
        limpiar();
    }

    void ListaMedicamentos::agregar(Medicamento* med) {

        NodoMedicamento* nuevo = new NodoMedicamento(med);

        // Caso lista vacia
        if (cabeza == nullptr) {

            cabeza = nuevo;
        }
        else {

            NodoMedicamento* actual = cabeza;

            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }

            actual->siguiente = nuevo;
        }

        tamano++;
    }

    NodoMedicamento* ListaMedicamentos::getCabeza() const {
        return cabeza;
    }

    Medicamento* ListaMedicamentos::obtener(int indice) {

        if (indice < 0 || indice >= tamano) {
            return nullptr;
        }

        NodoMedicamento* actual = cabeza;

        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        return actual->dato;
    }

    Medicamento* ListaMedicamentos::buscarPorId(int id) {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr &&
                actual->dato->getID() == id) {

                return actual->dato;
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

    Medicamento* ListaMedicamentos::buscarPorNombre(std::string texto) {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr) {

                std::string nombre = actual->dato->getNombre();

                // Busqueda parcial
                if (nombre.find(texto) != std::string::npos) {

                    return actual->dato;
                }
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

    bool ListaMedicamentos::eliminar(int id) {
        if (cabeza == nullptr) {
            return false;
        }

        NodoMedicamento* actual = cabeza;
        NodoMedicamento* anterior = nullptr;

        while (actual != nullptr) {

            if (actual->dato != nullptr &&
                actual->dato->getID() == id) {

                // Si es el primero
                if (anterior == nullptr) {

                    cabeza = actual->siguiente;
                }
                else {

                    anterior->siguiente = actual->siguiente;
                }

                delete actual->dato;
                delete actual;

                tamano--;

                return true;
            }

            anterior = actual;
            actual = actual->siguiente;
        }

        return false;
    }

    int ListaMedicamentos::cantidad() const {
        return tamano;
    }

    void ListaMedicamentos::limpiar() {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            NodoMedicamento* temp = actual;

            actual = actual->siguiente;

            delete temp->dato;
            delete temp;
        }

        cabeza = nullptr;
        tamano = 0;
    }

}