#pragma once
#include <string>
#include "Cliente.h"
#include "Medicamento.h"
#include "MedicamentoGenerico.h"
#include "MedicamentoMarca.h"
#include "MedicamentoControlado.h"
#include "Venta.h"

namespace FarmaSystem { // Namespace

    class SistemaFarmacia {

    private:
        // Arreglos dinamicos de punteros (Doble puntero)
        Medicamento** listaMedicamentos;
        Cliente** listaClientes;
        Venta** listaVentas;

        int cantidadClientes, cantidadMedicamentos, cantidadVentas;
        int capacidadClientes, capacidadMedicamentos, capacidadVentas;
        int nextIdMedicamento, nextIdCliente, nextIdVenta;

        // Metodos de redimensionamiento (Aritmetica de punteros interna)
		void redimensionarMedicamentos();
        void redimensionarClientes();
		void redimensionarVentas();

    public:
        SistemaFarmacia(int cantidadInicial = 3);

        //     GESTION DE MEDICAMENTOS
        int registrarGenerico(std::string nombre, double precio, int stock, std::string principio);
        int registrarMarca(std::string nombre, double precio, int stock, std::string pais, bool promo);
        int registrarControlado(std::string nombre, double precio, int stock, int nivel, double dosis);
        int actualizarStock(int id, int cantidad);
        void listarMedicamentos();
        bool eliminarMedicamento(int id);
        Medicamento* buscarMedicamentoPorID(int id);

        //     GESTION DE CLIENTES 
        int registrarCliente(std::string nombre, std::string cedula);
        void listarClientes();
        void toggleFidelidad(int id);
        bool eliminarCliente(int id);
        Cliente* buscarClientePorID(int id);
        Cliente* buscarClientePorCedula(std::string cedula);
      
        //     GESTION DE VENTA 
        int registrarVenta(std::string cedula, int idMed, int cant, bool presentoReceta, std::string fecha);
		void listarVentas();

		//	   GETTERS PARA LA GUI
        int getCantMedicamentos() const;
        int getCantClientes() const;
        int getCantVentas() const;
        Medicamento* getMedicamentoPorIndice(int indice);
        Cliente* getClientePorIndice(int indice);
        Venta* getVentaPorIndice(int indice);

        double calcularTotalVenta(Cliente* cliente, Medicamento* medicamento, int cant);
        ~SistemaFarmacia(); // Destructor para liberacion limpia
    };

} // Fin FarmaSystem