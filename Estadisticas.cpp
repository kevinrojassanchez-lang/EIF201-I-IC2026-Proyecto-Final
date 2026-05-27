#include "Estadisticas.h"

namespace FarmaSystem {

    Estadisticas::Estadisticas() {}

    Medicamento* Estadisticas::obtenerMenorStock(ListaMedicamentos& medicamentos) {

        NodoMedicamento* actual = medicamentos.getCabeza();

        if (actual == nullptr) {
            return nullptr;
        }

        Medicamento* menor = actual->dato;

        while (actual != nullptr) {

            Medicamento* medicamento = actual->dato;

            if (medicamento != nullptr) {

                if (medicamento->getStock() < menor->getStock()) {
                    menor = medicamento;
                }
            }
            actual = actual->siguiente;
        }

        return menor;
    }

    Medicamento* Estadisticas::obtenerMasVendido(
        ListaMedicamentos& medicamentos,
        ListaVentas& ventas) {

        if (medicamentos.cantidad() == 0 || ventas.cantidad() == 0) {
           
            return nullptr;
        }

        Medicamento* masVendido = nullptr;
        int maxUnidades = -1;

        NodoMedicamento* actualMed = medicamentos.getCabeza();

        while (actualMed != nullptr) {

            Medicamento* medicamento = actualMed->dato;

            if (medicamento != nullptr) {

                int unidades = 0;

                NodoVenta* cabezaVentas = ventas.getCabeza();

                if (cabezaVentas != nullptr) {

                    NodoVenta* actualVenta = cabezaVentas;

                    do {

                        Venta* venta = actualVenta->dato;

                        if (venta != nullptr &&
                            venta->getMedicamentoVendido() != nullptr) {

                            if (venta->getMedicamentoVendido()->getID() == medicamento->getID()) {

                                unidades += venta->getCantidad();
                            }
                        }

                        actualVenta = actualVenta->siguiente;

                    } while (actualVenta != cabezaVentas);
                }

                if (unidades > maxUnidades) {

                    maxUnidades = unidades;
                    masVendido = medicamento;
                }
            }

            actualMed = actualMed->siguiente;
        }

        return masVendido;
    }

    Cliente* Estadisticas::obtenerClienteVIP(
        ListaClientes& clientes,
        ListaVentas& ventas) {

        if (clientes.cantidad() == 0 || ventas.cantidad() == 0) {

            return nullptr;
        }

        Cliente* vip = nullptr;
        double mayorGasto = -1.0;

        NodoCliente* actual = clientes.getCabeza();

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

    double Estadisticas::obtenerIngresosTotales(ListaVentas& ventas) {

        return ventas.calcularIngresosTotales();
    }

    int Estadisticas::contarPorCategoria(ListaMedicamentos& medicamentos, std::string categoria) {
        int contador = 0;

        NodoMedicamento* actual = medicamentos.getCabeza();

        while (actual != nullptr) {

            Medicamento* medicamento = actual->dato;

            if (medicamento != nullptr) {

                if (medicamento->getCategoria() == categoria) {
                    contador++;
                }
            }

            actual = actual->siguiente;
        }

        return contador;
    }

    Estadisticas::~Estadisticas() {
        // La clase estadistica no es duena de los objetos, ella solo recibe referencias y retorna punteros
    }

}
