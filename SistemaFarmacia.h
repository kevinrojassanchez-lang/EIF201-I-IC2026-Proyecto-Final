#pragma once

#include <string>

#include "Medicamento.h"
#include "ListaMedicamentos.h"
#include "ListaProveedores.h"
#include "ListaClientes.h"
#include "ListaVentas.h"
#include "Estadisticas.h"

namespace FarmaSystem { // Namespace

    class SistemaFarmacia {

    private:
   
		ListaMedicamentos listaMedicamentos; // Reemplaza al arreglo dinamico de medicamentos
        ListaProveedores listaProveedores;   // Nuevo proveedores
		ListaClientes listaClientes;         // Reemplaza al arreglo dinamico de clientes
		ListaVentas listaVentas;             // Reemplaza al arreglo dinamico de ventas
        Estadisticas estadisticas;
        bool datosCargados;
        int nextIdMedicamento, nextIdProveedor, nextIdCliente, nextIdVenta;

    public:
        SistemaFarmacia();

        ListaProveedores& getListaProveedores();
        ListaMedicamentos& getListaMedicamentos();
        ListaClientes& getListaClientes();
        ListaVentas& getListaVentas();

        // Gestion de Persistencia
        void cargarDatos();
        void guardarDatos();
        void cargarProveedorDesdeArchivo(int id, std::string nombre, std::string telefono, std::string email, std::string pais);
        void cargarMedicamentoDesdeArchivo(Medicamento* med);
        void cargarClienteDesdeArchivo(int id, std::string nombre, std::string cedula, bool tarjeta);
        void cargarVentaDesdeArchivo(int id, int idCliente, int idMedicamento, int cantidad, double precioFinal, std::string fecha);
        bool getDatosCargados() const;
        void setDatosCargados(bool estado);

        // Gestion de Medicamentos
        int registrarGenerico(std::string nombre, double precio, int stock, std::string principio, int idProveedor = 0);
        int registrarMarca(std::string nombre, double precio, int stock, std::string pais, bool promo, int idProveedor = 0);
        int registrarControlado(std::string nombre, double precio, int stock, int nivel, double dosis, int idProveedor = 0);
        int actualizarStock(int id, int cantidad);
        void listarMedicamentos();
        bool eliminarMedicamento(int id);
        Medicamento* buscarMedicamentoPorID(int id);
        Medicamento* buscarMedicamentoPorNombre(std::string nombre);

        // Gestion de Clientes 
        int registrarCliente(std::string nombre, std::string cedula);
        void listarClientes();
        void toggleFidelidad(int id);
        bool eliminarCliente(int id);
        Cliente* buscarClientePorID(int id);
        Cliente* buscarClientePorCedula(std::string cedula);
      
        // Gestion de Ventas 
        int registrarVenta(std::string cedula, int idMed, int cant, bool presentoReceta, std::string fecha);
		void listarVentas();

		// Gestion de Proveedores
        int registrarProveedor(std::string nombre, std::string telefono, std::string email, std::string pais);
        bool eliminarProveedor(int id);
        bool proveedorTieneMedicamentos(int id);
        Proveedor* buscarProveedorPorNombre(std::string nombre);

		// Getters
        int getCantMedicamentos() const;
        int getCantClientes() const;
        int getCantVentas() const;
        Medicamento* getMedicamentoPorIndice(int indice);
        Cliente* getClientePorIndice(int indice);
        Venta* getVentaPorIndice(int indice);

        double calcularTotalVenta(Cliente* cliente, Medicamento* medicamento, int cant);
        Medicamento* obtenerMenorStock();
        Medicamento* obtenerMasVendido();
        Cliente* obtenerClienteVIP();
        double obtenerIngresosTotales();
        int contarPorCategoria(std::string categoria);

        ~SistemaFarmacia(); // Destructor para liberacion limpia
    };

} // Fin FarmaSystem