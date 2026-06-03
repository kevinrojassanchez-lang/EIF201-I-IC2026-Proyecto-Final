#include "Estadisticas.h"

namespace FarmaSystem {

    Medicamento* Estadisticas::obtenerMenorStock(ListaMedicamentos& medicamentos) {

        NodoMedicamento* actual = medicamentos.getCabeza();

        if (actual == nullptr) { return nullptr; }

        Medicamento* menor = actual->dato;

        while (actual != nullptr) {

            if (actual->dato != nullptr && actual->dato->getStock() < menor->getStock()) {

                menor = actual->dato;
            }
            actual = actual->siguiente;
        }
        return menor;
    }

    int Estadisticas::contarPorCategoria(ListaMedicamentos& medicamentos, const std::string& categoria) {

        int contador = 0;

        NodoMedicamento* actual = medicamentos.getCabeza();

        while (actual != nullptr) {

            if (actual->dato != nullptr && actual->dato->getCategoria() == categoria) {
                contador++;
            }
            actual = actual->siguiente;
        }
        return contador;
    }

    double Estadisticas::obtenerIngresosTotales(ListaVentas& ventas) {

        NodoVenta* cabeza = ventas.getCabeza();

        if (cabeza == nullptr) { return 0; }

        double total = 0;
        NodoVenta* actual = cabeza;

        do {
            if (actual->dato != nullptr) {

                total += actual->dato->getPrecioFinal();
            }
            actual = actual->siguiente;

        } while (actual != nullptr && actual != cabeza);

        return total;
    }

    void Estadisticas::acumularMedicamentos(ListaVentas& ventas, int* idsMedicamentos, int* unidadesVendidas,
        int maxCapacidad, int& usados) {

        NodoVenta* cabeza = ventas.getCabeza();

        if (cabeza == nullptr) { return; }

        NodoVenta* actual = cabeza;

        do {

            if (actual->dato != nullptr && actual->dato->getMedicamentoVendido() != nullptr) {

                int id = actual->dato->getMedicamentoVendido()->getID();
                int cantidad = actual->dato->getCantidad();

                bool encontrado = false;

                for (int i = 0; i < usados; i++) {

                    if (idsMedicamentos[i] == id) {

                        unidadesVendidas[i] += cantidad;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado && usados < maxCapacidad) {

                    idsMedicamentos[usados] = id;
                    unidadesVendidas[usados] = cantidad;
                    usados++;
                }
            }

            actual = actual->siguiente;

        } while (actual != nullptr && actual != cabeza);
    }

    int Estadisticas::obtenerIdMasVendido(int* idsMedicamentos, int* unidadesVendidas, int usados) {

        int id = -1;
        int maximo = -1;

        for (int i = 0; i < usados; i++) {

            if (unidadesVendidas[i] > maximo) {

                maximo = unidadesVendidas[i];
                id = idsMedicamentos[i];
            }
        }

        return id;
    }

    Medicamento* Estadisticas::obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas) {

        int capacidad = ventas.cantidad();

        if (capacidad == 0 || medicamentos.cantidad() == 0) {
            return nullptr;
        }

        int* idsMedicamentos = new int[capacidad];
        int* unidadesVendidas = new int[capacidad];

        for (int i = 0; i < capacidad; i++) {

            idsMedicamentos[i] = -1;
            unidadesVendidas[i] = 0;
        }

        int usados = 0;

        acumularMedicamentos(ventas, idsMedicamentos, unidadesVendidas, capacidad, usados);

        int id = obtenerIdMasVendido(idsMedicamentos, unidadesVendidas, usados);

        delete[] idsMedicamentos;
        delete[] unidadesVendidas;
		idsMedicamentos = nullptr;
		unidadesVendidas = nullptr;

        return medicamentos.buscarPorId(id);
    }

    void Estadisticas::acumularClientes(ListaVentas& ventas, int* idsClientes, double* gastosClientes,
        int maxCapacidad, int& usados) {

        NodoVenta* cabeza = ventas.getCabeza();

        if (cabeza == nullptr) { return; }

        NodoVenta* actual = cabeza;

        do {

            if (actual->dato != nullptr) {

                int id = actual->dato->getIdCliente();
                double monto = actual->dato->getPrecioFinal();

                bool encontrado = false;

                for (int i = 0; i < usados; i++) {

                    if (idsClientes[i] == id) {

                        gastosClientes[i] += monto;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado && usados < maxCapacidad) {

                    idsClientes[usados] = id;
                    gastosClientes[usados] = monto;
                    usados++;
                }
            }

            actual = actual->siguiente;

        } while (actual != nullptr && actual != cabeza);
    }

    int Estadisticas::obtenerIdVIP(int* idsClientes, double* gastosClientes, int usados) {

        int idVIP = -1;
        double max = -1;

        for (int i = 0; i < usados; i++) {

            if (gastosClientes[i] > max) {

                max = gastosClientes[i];
                idVIP = idsClientes[i];
            }
        }

        return idVIP;
    }

    Cliente* Estadisticas::obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas) {

        int capacidad = ventas.cantidad();

        if (capacidad == 0 || clientes.cantidad() == 0) {
            return nullptr;
        }

        int* idsClientes = new int[capacidad];
        double* gastosClientes = new double[capacidad];

        for (int i = 0; i < capacidad; i++) {

            idsClientes[i] = -1;
            gastosClientes[i] = 0.0;
        }

        int usados = 0;

        acumularClientes(ventas, idsClientes, gastosClientes, capacidad, usados);

        int idVIP = obtenerIdVIP(idsClientes, gastosClientes, usados);

        delete[] idsClientes;
        delete[] gastosClientes;
		idsClientes = nullptr;
		gastosClientes = nullptr;

        return clientes.buscarPorId(idVIP);
    }
} // namespace FarmaSystem 