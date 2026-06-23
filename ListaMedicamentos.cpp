#include "ListaMedicamentos.h"

namespace FarmaSystem {

	// Constructor para inicializar la lista vacia
    ListaMedicamentos::ListaMedicamentos()
      : cabeza(nullptr), cola(nullptr), tamano(0) {
    }

	// Destructor para limpiar la lista y liberar memoria
    ListaMedicamentos::~ListaMedicamentos() {
       
        limpiar();
    }

	// Metodo para agregar un medicamento al final de la lista
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

	// Metodo para obtener la cabeza de la lista
    NodoMedicamento* ListaMedicamentos::getCabeza() const {  return cabeza; }

	// Metodo para obtener un medicamento por su posicion en la lista
    Medicamento* ListaMedicamentos::obtener(int indice) {

        if (indice < 0 || indice >= tamano) {
            return nullptr;
        }

        NodoMedicamento* actual = cabeza;

        int posicion = 0;

        while (posicion < indice) {

            actual = actual->siguiente;
            posicion++;
        }

        return actual->dato;
    }

	// Metodo para buscar un medicamento por su ID
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

	// Metodo para buscar un medicamento por su nombre (busqueda parcial)
    Medicamento* ListaMedicamentos::buscarPorNombre(std::string texto) {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr) {

                std::string nombre = actual->dato->getNombre();

                if (nombre.find(texto) != std::string::npos) { // Busqueda parcial

                    return actual->dato;
                }
            }

            actual = actual->siguiente;
        }

        return nullptr;
    }

	// Metodo para eliminar un medicamento por su ID
    bool ListaMedicamentos::eliminar(int id) {

        if (cabeza == nullptr) {
            return false;
        }

        NodoMedicamento* actual = cabeza;
        NodoMedicamento* anterior = nullptr;

        while (actual != nullptr) {

            if (actual->dato != nullptr &&
                actual->dato->getID() == id) {

                if (anterior == nullptr) { // si es el primero

                    cabeza = actual->siguiente;

                    if (cabeza == nullptr) { // si era el unico nodo
                        cola = nullptr;
                    }
                }
                else {

                    anterior->siguiente = actual->siguiente;

                    if (actual == cola) { // si era el ultimo nodo
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

	// Metodo para verificar si existe un medicamento asociado a un proveedor
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

	// Metodo para obtener la cantidad de medicamentos en la lista
    int ListaMedicamentos::cantidad() const { return tamano; }

	// Metodo para mostrar todos los medicamentos en el frontend
    void ListaMedicamentos::mostrarTodos() {

        NodoMedicamento* actual = cabeza;

        while (actual != nullptr) {

            if (actual->dato != nullptr) {
                actual->dato->toString();
            }

            actual = actual->siguiente;
        }
    }

	// Metodo para guardar todos los medicamentos en un archivo de texto
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

	// Metodo para limpiar la lista y liberar memoria
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

}// Fin namespace FarmaSystem