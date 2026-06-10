#ifndef SISTEMAFARMACIA_H
#define SISTEMAFARMACIA_H

#include <string>

#include "ListaMedicamentos.h"
#include "ListaProveedores.h"
#include "ListaClientes.h"
#include "ListaVentas.h"

namespace FarmaSystem {

    class SistemaFarmacia {

    private:

        // Estructuras principales del sistema
        ListaMedicamentos listaMedicamentos;
        ListaProveedores listaProveedores;
        ListaClientes listaClientes;
        ListaVentas listaVentas;

        // Estado de carga
        bool datosCargados;

        // Generadores de ID
        int nextIdMedicamento;
        int nextIdProveedor;
        int nextIdCliente;
        int nextIdVenta;

        // Estadisticas de carga
        int medicamentosAceptados;
        int medicamentosDescartados;
        int ventasAceptadas;
        int ventasDescartadas;
        int proveedoresAceptados;
        int proveedoresDescartados;
        int clientesAceptados;
        int clientesDescartados;

        // Metodos internos de carga
        void cargarProveedores();
        void cargarMedicamentos();
        void cargarClientes();
        void cargarVentas();

    public:

        SistemaFarmacia();

        // Persistencia
        void cargarDatos();
        void guardarDatos();
        bool getDatosCargados() const;
        void setDatosCargados(bool estado);

        // Proveedores
        void cargarProveedorDesdeArchivo(int id, const std::string& nombre,const std::string& telefono,
            const std::string& email, const std::string& pais);

        int registrarProveedor(const std::string& nombre, const std::string& telefono,
            const std::string& email, const std::string& pais);

        bool eliminarProveedor(int id);
        bool proveedorTieneMedicamentos(int id);

        Proveedor* buscarProveedorPorNombre(const std::string& nombre);
        Proveedor* buscarProveedorPorID(int id);

        bool editarProveedor(int id, const std::string& nombre, const std::string& telefono, const std::string& email,
            const std::string& pais);

        // Medicamentos
        void cargarMedicamentoDesdeArchivo(Medicamento* med);

        int registrarGenerico(const std::string& nombre, double precio,int stock, const std::string& principio, int idProveedor,
            bool receta);

        int registrarMarca(const std::string& nombre, double precio,int stock, const std::string& pais, bool promo, int idProveedor,
            bool receta);

        int registrarControlado(const std::string& nombre, double precio, int stock, int nivel, double dosis, int idProveedor,
            bool receta);

        int editarGenerico(int id, const std::string& nombre, double precio, int stock, bool receta,
            const std::string& principio, int idProveedor);

        int editarMarca(int id, const std::string& nombre, double precio, int stock, bool receta, const std::string& pais,
            bool promo, int idProveedor);

        int editarControlado(int id, const std::string& nombre, double precio, int stock, bool receta, int nivel,
            double dosis, int idProveedor);

        int contarPorCategoria(const std::string& categoria);

        void listarMedicamentos();
        bool eliminarMedicamento(int id);

        Medicamento* buscarMedicamentoPorID(int id);
        Medicamento* buscarMedicamentoPorNombre(const std::string& nombre);

        std::string obtenerReporteMedicamento(int id);

        Medicamento* obtenerMenorStock();
        Medicamento* obtenerMasVendido();

        // Clientes
        void cargarClienteDesdeArchivo(int id, const std::string& nombre, const std::string& cedula, bool tarjeta);

        int registrarCliente(const std::string& nombre, const std::string& cedula);

        void listarClientes();
        bool eliminarCliente(int id);

        int editarCliente(int id, const std::string& nombre, const std::string& cedula, bool fidelidad);

        Cliente* buscarClientePorID(int id);
        Cliente* buscarClientePorCedula(const std::string& cedula);

        Cliente* obtenerClienteVIP();

        // Ventas
        void cargarVentaDesdeArchivo(int id, int idCliente, int idMedicamento, int cantidad, double precioFinal,
            const std::string& fecha);

        int registrarVenta(const std::string& cedula, int idMed, int cant, bool presentoReceta, const std::string& fecha,
            bool confirmado);

        double calcularTotalVenta(Cliente* cliente, Medicamento* medicamento, int cant);

        void listarVentas();
        double obtenerIngresosTotales();

        // Estadisticas de carga
        int getCantProveedoresAceptados() const;
        int getCantProveedoresDescartados() const;
        int getCantClientesAceptados() const;
        int getCantClientesDescartados() const;
        int getCantMedicamentosAceptados() const;
        int getCantMedicamentosDescartados() const;
        int getCantVentasAceptadas() const;
        int getCantVentasDescartadas() const;

        // Acceso a datos
        int getCantMedicamentos() const;
        int getCantClientes() const;
        int getCantVentas() const;
        int getCantProveedores() const;

        Medicamento* getMedicamentoPorIndice(int indice);
        Cliente* getClientePorIndice(int indice);
        Venta* getVentaPorIndice(int indice);
        Proveedor* getProveedorPorIndice(int indice);

        ~SistemaFarmacia();
    };

} // namespace FarmaSystem

#endif // SISTEMAFARMACIA_H