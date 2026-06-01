#include "ListaMedicamentos.h"

namespace FarmaSystem {

    ListaMedicamentos::ListaMedicamentos()
      : cabeza(nullptr), cola(nullptr), tamano(0) {
    }

    ListaMedicamentos::~ListaMedicamentos() { limpiar(); }

    void ListaMedicamentos::agregar(Medicamento* med) {

        NodoMedicamento* nuevo = new NodoMedicamento(med);

        if (cabeza == nullptr) {

            cabeza = nuevo;
            cola = nuevo;
        }

        else {

            cola->siguiente = nuevo;
            cola = nuevo;
        }

        tamano++;
    }

    NodoMedicamento* ListaMedicamentos::getCabeza() const {  return cabeza; }

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

                // Caso: es el primero
                if (anterior == nullptr) {

                    cabeza = actual->siguiente;

                    // Si era el unico nodo
                    if (cabeza == nullptr) {
                        cola = nullptr;
                    }

                }
                else {

                    anterior->siguiente = actual->siguiente;

                    // Si era el último nodo
                    if (actual == cola) {
                        cola = anterior;
                    }
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

    bool ListaMedicamentos::tieneProveedor(int idProveedor) {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr &&
                actual->dato->getIdProveedor() == idProveedor) {

                return true;
            }

            actual = actual->siguiente;
        }

        return false;
    }

    int ListaMedicamentos::cantidad() const { return tamano; }

    void ListaMedicamentos::guardarEnArchivo(std::ofstream& archivo) {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            Medicamento* medicamento = actual->dato;

            if (medicamento != nullptr) {

                archivo << medicamento->toString() << "\n";
            }

            actual = actual->siguiente;
        }
    }

    void ListaMedicamentos::mostrarTodos() {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr) {
                actual->dato->toString();
            }

            actual = actual->siguiente;
        }
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
        cola = nullptr;
        tamano = 0;
    }

}