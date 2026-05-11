#include "SistemaFarmacia.h"
#include <iostream>
#include <string>
#include <cctype>

namespace FarmaSystem {

    SistemaFarmacia::SistemaFarmacia(int cantidadInicial) : cantidadClientes
    (0), cantidadMedicamentos(0), cantidadVentas(0), capacidadClientes(cantidadInicial), capacidadMedicamentos(cantidadInicial),
        capacidadVentas(cantidadInicial), nextIdMedicamento(1), nextIdCliente(1), nextIdVenta(1)
    {
        listaClientes = new Cliente * [capacidadClientes];
        listaMedicamentos = new Medicamento * [capacidadMedicamentos];
        listaVentas = new Venta * [capacidadVentas]; // ventas
    }
   
    int SistemaFarmacia::registrarGenerico(std::string nombre, double precio, int stock, std::string principio) {
        if (nombre.empty() || principio.empty()) return 1;
        if (precio <= 0 || stock < 0) return 2;

        if (cantidadMedicamentos == capacidadMedicamentos) { redimensionarMedicamentos(); }

        listaMedicamentos[cantidadMedicamentos] = new MedicamentoGenerico(nextIdMedicamento++, nombre, precio, false, stock, principio);
        cantidadMedicamentos++;
        return 0;
    }

    int SistemaFarmacia::registrarMarca(std::string nombre, double precio, int stock, std::string pais, bool promo) {
        if (nombre.empty() || pais.empty()) return 1;
        if (precio <= 0 || stock < 0) return 2;

        if (cantidadMedicamentos == capacidadMedicamentos) { redimensionarMedicamentos(); }

        listaMedicamentos[cantidadMedicamentos] = new MedicamentoMarca(nextIdMedicamento++, nombre, precio, false, stock, pais, promo);
        cantidadMedicamentos++;
        return 0;
    }

    int SistemaFarmacia::registrarControlado(std::string nombre, double precio, int stock, int nivel, double dosis) {
        if (nombre.empty()) return 1;
        if (precio <= 0 || stock < 0 || dosis <= 0) return 2;
        if (nivel < 1 || nivel > 4) return 3;

        if (cantidadMedicamentos == capacidadMedicamentos) { redimensionarMedicamentos(); }

        listaMedicamentos[cantidadMedicamentos] = new MedicamentoControlado(nextIdMedicamento++, nombre, precio, stock, nivel, dosis);
        cantidadMedicamentos++;
        return 0;
    }

    void SistemaFarmacia::listarMedicamentos() {
        if (cantidadMedicamentos == 0) {
            return;
		}
        for (int i = 0; i < cantidadMedicamentos; i++) {
            listaMedicamentos[i]->mostrar();
        }
    }
       
    int SistemaFarmacia::actualizarStock(int id, int cantidad) {
        for (int i = 0; i < cantidadMedicamentos; i++) {
            if (listaMedicamentos[i] != nullptr && listaMedicamentos[i]->getID() == id) {
                int nuevoStock = listaMedicamentos[i]->getStock() + cantidad;
                if (nuevoStock < 0) { return 2; } // Stock negativo

                listaMedicamentos[i]->setStock(nuevoStock);
                return 0; // exito
            }
        }
        return 1; // ID no encontrado
    }
   
    bool SistemaFarmacia::eliminarMedicamento(int id) {
        int posicion = -1;
        for (int i = 0; i < cantidadMedicamentos; i++) {
            if (listaMedicamentos[i]->getID() == id) {
                posicion = i;
                break;
            }
        }

        if (posicion == -1) {
            return false;
        }

        for (int i = 0; i < cantidadVentas; i++) {
            if (listaVentas[i]->getMedicamentoVendido() == listaMedicamentos[posicion]) {
                return false;
            }
        }

        delete listaMedicamentos[posicion];
        listaMedicamentos[posicion] = nullptr;

        for (int j = posicion; j < cantidadMedicamentos - 1; j++) {
            listaMedicamentos[j] = listaMedicamentos[j + 1];
        }

        listaMedicamentos[cantidadMedicamentos - 1] = nullptr;
        cantidadMedicamentos--;

        return true;
    }
    // Clientes
    int SistemaFarmacia::registrarCliente(std::string nombre, std::string cedula) {
        if (nombre.empty() || cedula.empty()) return 1;

        for (int i = 0; i < cantidadClientes; i++) {
            if (listaClientes[i]->getCedula() == cedula) {
                return 2;
            }
        }

        if (cantidadClientes == capacidadClientes) { redimensionarClientes(); }

        listaClientes[cantidadClientes] = new Cliente(nextIdCliente++, nombre, cedula);
        cantidadClientes++;
        return 0; // exito
    }

    void SistemaFarmacia::listarClientes() {
        if (cantidadClientes == 0) { return; }
        for (int i = 0; i < cantidadClientes; i++) {
            listaClientes[i]->mostrar();
        }
    }

    void SistemaFarmacia::toggleFidelidad(int id) {
        for (int i = 0; i < cantidadClientes; i++) {
            if (listaClientes[i]->getID() == id) {
               bool estadoActual = listaClientes[i]->getTarjeta();
               listaClientes[i]->setTarjeta(!estadoActual);
               return;
            }
        }
    }

    bool SistemaFarmacia::eliminarCliente(int id) {
        int indice = -1;
        for (int i = 0; i < cantidadClientes; i++) {
            if (listaClientes[i]->getID() == id) {
                indice = i;
                break;
            }
        }

        if (indice == -1) {
            return false;
        }

        std::string nombreCliente = listaClientes[indice]->getNombre();

        delete listaClientes[indice];
        listaClientes[indice] = nullptr;

        for (int j = indice; j < cantidadClientes - 1; j++) {
            listaClientes[j] = listaClientes[j + 1];
        }

        listaClientes[cantidadClientes - 1] = nullptr;
        cantidadClientes--;

        return true;
    }

    int SistemaFarmacia::registrarVenta(std::string cedula, int idMed, int cantidad, bool presentoReceta, std::string fecha) {

        Cliente* clienteComprador = buscarClientePorCedula(cedula);
        if (clienteComprador == nullptr) return 1; // Cliente no existe

        Medicamento* medicamento = buscarMedicamentoPorID(idMed);
		if (medicamento == nullptr) return 2; // Medicamento no existe

        if (cantidad <= 0) return 3;  
        if (fecha.empty()) return 4;
        if (medicamento->excedeDosis(cantidad)) return 7;
        if (medicamento->getReceta() && !presentoReceta) return 5;
        if (medicamento->getStock() < cantidad) return 6;

        double totalFinal = calcularTotalVenta(clienteComprador, medicamento, cantidad);

        if (cantidadVentas == capacidadVentas) { redimensionarVentas(); }

        listaVentas[cantidadVentas] = new Venta(nextIdVenta++, clienteComprador->getID(), medicamento, cantidad, totalFinal, fecha);
        cantidadVentas++;

        medicamento->setStock(medicamento->getStock() - cantidad);

        return 0; // exito
    }

    void SistemaFarmacia::listarVentas() {
        if (cantidadVentas == 0) {
            return;
        }

        for (int i = 0; i < cantidadVentas; i++) {
            listaVentas[i]->getInfoVenta();
        }
    }

    SistemaFarmacia::~SistemaFarmacia() {
        for (int i = 0; i < cantidadMedicamentos; i++) {
            delete listaMedicamentos[i];
            listaMedicamentos[i] = nullptr;
        }

        delete[] listaMedicamentos;
	    listaMedicamentos = nullptr;

        for (int i = 0; i < cantidadClientes; i++) {
            delete listaClientes[i];
            listaClientes[i] = nullptr;
        }

        delete[] listaClientes;
	    listaClientes = nullptr;

        for (int i = 0; i < cantidadVentas; i++) {
            delete listaVentas[i];
        }
        delete[] listaVentas;
        listaVentas = nullptr;
    }

    // Implementacion de Getters para Qt
    int SistemaFarmacia::getCantMedicamentos() const {
        return cantidadMedicamentos;
    }

    int SistemaFarmacia::getCantClientes() const {
        return cantidadClientes;
    }

    int SistemaFarmacia::getCantVentas() const {
        return cantidadVentas;
    }

    Medicamento* SistemaFarmacia::getMedicamentoPorIndice(int i) {
        if (i >= 0 && i < cantidadMedicamentos) {
            return listaMedicamentos[i];
        }
        return nullptr;
    }

    Cliente* SistemaFarmacia::getClientePorIndice(int i) {
        if (i >= 0 && i < cantidadClientes) {
            return listaClientes[i];
        }
        return nullptr;
    }

    Venta* SistemaFarmacia::getVentaPorIndice(int i) {
        if (i >= 0 && i < cantidadVentas) {
            return listaVentas[i];
        }
        return nullptr;
    }

	// Nuevos metodos sepadados por criterio
    Cliente* SistemaFarmacia::buscarClientePorCedula(std::string cedula) {
        for (int i = 0; i < cantidadClientes; i++) {
            if (listaClientes[i]->getCedula() == cedula) {
                return listaClientes[i];
            }
        }
        return nullptr;
	}

    Cliente* SistemaFarmacia::buscarClientePorID(int id) {
        for (int i = 0; i < cantidadClientes; i++) {
            if (listaClientes[i]->getID() == id) {
                return listaClientes[i];
            }
        }
        return nullptr;
	}

    Medicamento* SistemaFarmacia::buscarMedicamentoPorID(int id) {
        for (int i = 0; i < cantidadMedicamentos; i++) {
            if (listaMedicamentos[i]->getID() == id) {
                return listaMedicamentos[i];
            }
        }
        return nullptr;
	}

    double SistemaFarmacia::calcularTotalVenta(Cliente* cliente, Medicamento* medicamento, int cantidad) {
        double precioBase = medicamento->calcularPrecioFinal(cantidad);
        return precioBase * (cliente->getTarjeta() ? 0.95 : 1.0);
	}

	// Metodos de redimensionamiento separados por tipo de dato
    void SistemaFarmacia::redimensionarMedicamentos() {
        int nuevaCapacidad = capacidadMedicamentos * 2;
        Medicamento** nuevaLista = new Medicamento * [nuevaCapacidad];
        for (int i = 0; i < cantidadMedicamentos; i++) {
            *(nuevaLista + i) = *(listaMedicamentos + i);
        }

        for (int i = cantidadMedicamentos; i < nuevaCapacidad; i++) {
            *(nuevaLista + i) = nullptr;
        }
        delete[] listaMedicamentos;
        listaMedicamentos = nuevaLista;
        capacidadMedicamentos = nuevaCapacidad;
    }

    void SistemaFarmacia::redimensionarClientes() {
        int nuevaCapacidad = capacidadClientes * 2;
        Cliente** nuevaLista = new Cliente * [nuevaCapacidad];
        for (int i = 0; i < cantidadClientes; i++) {
            *(nuevaLista + i) = *(listaClientes + i);
        }

        for (int i = cantidadClientes; i < nuevaCapacidad; i++) {
            *(nuevaLista + i) = nullptr;
        }
        delete[] listaClientes;
        listaClientes = nuevaLista;
        capacidadClientes = nuevaCapacidad;
    }

    void SistemaFarmacia::redimensionarVentas() {
        int nuevaCapacidad = capacidadVentas * 2;
        Venta** nuevaLista = new Venta * [nuevaCapacidad];
        for (int i = 0; i < cantidadVentas; i++) {
            *(nuevaLista + i) = *(listaVentas + i);
        }

        for (int i = cantidadVentas; i < nuevaCapacidad; i++) {
            *(nuevaLista + i) = nullptr;
        }
        delete[] listaVentas;
        listaVentas = nuevaLista;
        capacidadVentas = nuevaCapacidad;
	}

} // namespace

