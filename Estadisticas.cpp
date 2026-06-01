#include "Estadisticas.h"

namespace FarmaSystem {

    Medicamento* Estadisticas::obtenerMenorStock(ListaMedicamentos& medicamentos) {

        NodoMedicamento* actual = medicamentos.getCabeza();

        if (actual == nullptr) return nullptr;

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

            total += actual->dato->getPrecioFinal();

            actual = actual->siguiente;

        } while (actual != cabeza);

        return total;
    }

    void Estadisticas::acumularMedicamentos(ListaVentas& ventas, AcumuladoMedicamento* acumulados, int& usados)
    {
        NodoVenta* cabeza = ventas.getCabeza();

        NodoVenta* actual = cabeza;

        do {

            int id = actual->dato->getMedicamentoVendido()->getID();

            int cantidad = actual->dato->getCantidad();

            bool encontrado = false;

            for (int i = 0; i < usados; i++) {

                if (acumulados[i].idMedicamento == id) {

                    acumulados[i].unidadesVendidas += cantidad;

                    encontrado = true;

                    break;
                }
            }

            if (!encontrado) {

                acumulados[usados].idMedicamento = id;

                acumulados[usados].unidadesVendidas = cantidad;

                usados++;
            }

            actual = actual->siguiente;

        } while (actual != cabeza);
    }

    int Estadisticas::obtenerIdMasVendido(AcumuladoMedicamento* acumulados, int usados) {

        int id = -1;

        int max = -1;

        for (int i = 0; i < usados; i++) {

            if (acumulados[i].unidadesVendidas > max) {

                max = acumulados[i].unidadesVendidas;

                id = acumulados[i].idMedicamento;
            }
        }

        return id;
    }

    Medicamento* Estadisticas::obtenerMasVendido(ListaMedicamentos& medicamentos, ListaVentas& ventas) {

        if (ventas.cantidad() == 0) { return nullptr; }

        AcumuladoMedicamento* acumulados = new AcumuladoMedicamento[medicamentos.cantidad()];

        int usados = 0;

        acumularMedicamentos(ventas, acumulados, usados);

        int id = obtenerIdMasVendido(acumulados, usados);

        delete[] acumulados;

		acumulados = nullptr;

        return medicamentos.buscarPorId(id);
    }


    void Estadisticas::acumularClientes(ListaVentas& ventas, AcumuladoCliente* acumulados, int& usados) {

        NodoVenta* cabeza = ventas.getCabeza();

        NodoVenta* actual = cabeza;

        do {

            int id = actual->dato->getIdCliente();

            double monto = actual->dato->getPrecioFinal();

            bool encontrado = false;

            for (int i = 0; i < usados; i++) {

                if (acumulados[i].idCliente == id) {

                    acumulados[i].totalGastado += monto;

                    encontrado = true;

                    break;
                }
            }

            if (!encontrado) {

                acumulados[usados].idCliente = id;

                acumulados[usados].totalGastado = monto;

                usados++;
            }

            actual = actual->siguiente;

        } while (actual != cabeza);
    }

    int Estadisticas::obtenerIdVIP(AcumuladoCliente* acumulados, int usados) {
       
        int idVIP = -1;

        double max = -1;

        for (int i = 0; i < usados; i++) {

            if (acumulados[i].totalGastado > max) {

                max = acumulados[i].totalGastado; 

                idVIP = acumulados[i].idCliente;
            }
        }

        return idVIP;
    }

    Cliente* Estadisticas::obtenerClienteVIP(ListaClientes& clientes, ListaVentas& ventas) {

        if (clientes.cantidad() == 0 || ventas.cantidad() == 0) { return nullptr; }

        AcumuladoCliente* acumulados = new AcumuladoCliente[clientes.cantidad()];

        int usados = 0;

        acumularClientes(ventas, acumulados, usados);

        int idVIP = obtenerIdVIP(acumulados, usados);

        delete[] acumulados;

		acumulados = nullptr;

        return clientes.buscarPorId(idVIP);
    }
}