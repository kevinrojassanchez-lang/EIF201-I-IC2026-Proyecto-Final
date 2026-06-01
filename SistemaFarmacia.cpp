#include "SistemaFarmacia.h"
#include "ArchivoCarga.h"
#include "ArchivoGuardado.h"
#include "MedicamentoGenerico.h"
#include "MedicamentoMarca.h"
#include "MedicamentoControlado.h"

namespace FarmaSystem {

    SistemaFarmacia::SistemaFarmacia() : nextIdMedicamento(1), nextIdProveedor(1), nextIdCliente(1),
        nextIdVenta(1), datosCargados(false) {

        // Las listas enlazadas se inicializan solas con sus constructores por defecto
    }

    void SistemaFarmacia::cargarDatos() {

        listaMedicamentos.limpiar();

        listaClientes.limpiar();

        listaProveedores.limpiar();

        listaVentas.limpiar();

        ArchivoCarga carga;

        carga.cargarTodo(*this);

        datosCargados = true;
    }

    void SistemaFarmacia::guardarDatos() {

        ArchivoGuardado guardado;

        guardado.guardarTodo(*this);
    }
   
    int SistemaFarmacia::registrarGenerico(std::string nombre, double precio, int stock, std::string principio,
        int idProveedor) {

        if (nombre.empty() || principio.empty()) { return 1; }

        if (precio <= 0 || stock < 0) { return 2; } 

        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 3;}

        Medicamento* nuevo = new MedicamentoGenerico(nextIdMedicamento++, nombre, precio, false, stock,
        principio, idProveedor);

        listaMedicamentos.agregar(nuevo);

        return 0;
    }

    int SistemaFarmacia::registrarMarca(std::string nombre, double precio, int stock, std::string pais,
        bool promo, int idProveedor) {

        if (nombre.empty() || pais.empty()) { return 1; }

        if (precio <= 0 || stock < 0) { return 2; }

        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 3; }

        Medicamento* nuevo = new MedicamentoMarca(nextIdMedicamento++, nombre, precio, false, stock,
            pais, promo, idProveedor);

        listaMedicamentos.agregar(nuevo);

        return 0;
    }

    int SistemaFarmacia::registrarControlado(std::string nombre, double precio, int stock, int nivel,
        double dosis, int idProveedor) {

        if (nombre.empty()) { return 1; }

        if (precio <= 0 || stock < 0 || dosis <= 0) { return 2; }

        if (nivel < 1 || nivel > 4) { return 3; }

        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 4; }

        Medicamento* nuevo = new MedicamentoControlado(nextIdMedicamento++, nombre, precio, stock, 
            nivel, dosis, idProveedor);

        listaMedicamentos.agregar(nuevo);

        return 0;
    }

    void SistemaFarmacia::listarMedicamentos() { listaMedicamentos.mostrarTodos(); }

    int SistemaFarmacia::actualizarStock(int id, int cantidad) {

        Medicamento* medicamento = listaMedicamentos.buscarPorId(id);

        if (medicamento == nullptr) { return 1; }

        int nuevoStock = medicamento->getStock() + cantidad;

        if (nuevoStock < 0) { return 2; }

        medicamento->setStock(nuevoStock);

        return 0;
    }

    bool SistemaFarmacia::eliminarMedicamento(int id) {

        Medicamento* medicamento = listaMedicamentos.buscarPorId(id);

        if (medicamento == nullptr) { return false; }

        if (listaVentas.existeVentaDeMedicamento(id)) { return false; }

        return listaMedicamentos.eliminar(id);
    }

	// Registro de Proveedores
    int SistemaFarmacia::registrarProveedor(std::string nombre, std::string telefono, std::string email,
        std::string pais) {

        if (nombre.empty() || telefono.empty() || email.empty() || pais.empty()) { return 1; }

        if (buscarProveedorPorNombre(nombre) != nullptr) { return 2;}

        Proveedor* nuevo = new Proveedor(nextIdProveedor++, nombre, telefono, email, pais);

        listaProveedores.agregar(nuevo);

        return 0;
    }

    void SistemaFarmacia::cargarProveedorDesdeArchivo(int id, std::string nombre, std::string telefono,
        std::string email, std::string pais) {

        Proveedor* nuevoProv = new Proveedor(id, nombre, telefono, email, pais);

        listaProveedores.agregar(nuevoProv);

        if (id >= nextIdProveedor) { nextIdProveedor = id + 1; }
    }

    bool SistemaFarmacia::eliminarProveedor(int id) {

        bool tiene = proveedorTieneMedicamentos(id);

        bool eliminado = listaProveedores.eliminar(id, tiene);

        if (!eliminado) { return false; }

        return true;
    }

    Proveedor* SistemaFarmacia::buscarProveedorPorNombre(std::string nombre) {
        return listaProveedores.buscarPorNombre(nombre);
    }

    // Clientes
    int SistemaFarmacia::registrarCliente(std::string nombre, std::string cedula) {

        if (nombre.empty() || cedula.empty()) { return 1; }

        if (listaClientes.buscarPorCedula(cedula) != nullptr) { return 2; }

        Cliente* nuevoCli = new Cliente(nextIdCliente++, nombre, cedula);

        listaClientes.agregar(nuevoCli);

        return 0; 
    }

    void SistemaFarmacia::toggleFidelidad(int id) {

        Cliente* cliente = listaClientes.buscarPorId(id);

        if (cliente != nullptr) {

            bool estadoActual = cliente->getTarjeta();

			cliente->setTarjeta(!estadoActual);
        }
    }

    bool SistemaFarmacia::eliminarCliente(int id) {

        Cliente* cliente = listaClientes.buscarPorId(id);

        if (cliente == nullptr) { return false; }

        if (listaVentas.existeVentaDeCliente(id)) { return false; }

        return listaClientes.eliminar(id);
    }

    int SistemaFarmacia::registrarVenta(std::string cedula, int idMed, int cantidad, bool presentoReceta,
        std::string fecha) {

        Cliente* clienteComprador = buscarClientePorCedula(cedula);

        if (clienteComprador == nullptr) { return 1; }

        Medicamento* medicamento = buscarMedicamentoPorID(idMed);

        if (medicamento == nullptr) { return 2; }

        if (cantidad <= 0) { return 3; }

        if (fecha.empty()) { return 4; }

        if (medicamento->excedeDosis(cantidad)) { return 7; }

        if (medicamento->getReceta() && !presentoReceta) { return 5; }

        if (medicamento->getStock() < cantidad) { return 6; }

        double totalFinal = calcularTotalVenta(clienteComprador, medicamento, cantidad);

        Venta* nuevaVenta = new Venta(nextIdVenta++, clienteComprador->getID(), medicamento, cantidad, 
            totalFinal, fecha);

        listaVentas.agregar(nuevaVenta);

        medicamento->setStock(medicamento->getStock() - cantidad);

        return 0; // Exito
    }

    void SistemaFarmacia::listarVentas() {

        if (listaVentas.cantidad() == 0) { return; }

        listaVentas.imprimirHistorial();
    }

    // Implementacion de Getters
    ListaProveedores& SistemaFarmacia::getListaProveedores() { return listaProveedores; }

    ListaMedicamentos& SistemaFarmacia::getListaMedicamentos() { return listaMedicamentos; }
    
    ListaClientes& SistemaFarmacia::getListaClientes() { return listaClientes; }

    ListaVentas& SistemaFarmacia::getListaVentas() { return listaVentas; }

    int SistemaFarmacia::getCantMedicamentos() const { return listaMedicamentos.cantidad(); }

    int SistemaFarmacia::getCantClientes() const { return listaClientes.cantidad(); }

    int SistemaFarmacia::getCantVentas() const { return listaVentas.cantidad(); }

    Medicamento* SistemaFarmacia::getMedicamentoPorIndice(int i) { return listaMedicamentos.obtener(i); }

    Cliente* SistemaFarmacia::getClientePorIndice(int i) { return listaClientes.obtener(i); }

    Venta* SistemaFarmacia::getVentaPorIndice(int i) { return listaVentas.obtener(i); }

    bool SistemaFarmacia::getDatosCargados() const { return datosCargados; }

    void SistemaFarmacia::setDatosCargados(bool estado) { datosCargados = estado; }

    void SistemaFarmacia::listarClientes() { listaClientes.mostrarTodos(); }

	// Nuevos metodos separados por criterio
    Cliente* SistemaFarmacia::buscarClientePorCedula(std::string cedula) {

        return listaClientes.buscarPorCedula(cedula);
    }

    Cliente* SistemaFarmacia::buscarClientePorID(int id) {

        return listaClientes.buscarPorId(id);
    }

    Medicamento* SistemaFarmacia::buscarMedicamentoPorID(int id) {

        return listaMedicamentos.buscarPorId(id);
    }

    Medicamento* SistemaFarmacia::buscarMedicamentoPorNombre(std::string nombre) {

       return listaMedicamentos.buscarPorNombre(nombre);
    }

    double SistemaFarmacia::calcularTotalVenta(Cliente* cliente, Medicamento* medicamento, int cantidad) {

        double precioBase = medicamento->calcularPrecioFinal(cantidad);

        return precioBase * (cliente->getTarjeta() ? 0.95 : 1.0);
	}

    void SistemaFarmacia::cargarMedicamentoDesdeArchivo(Medicamento* med) {

        listaMedicamentos.agregar(med);

        if (med->getID() >= nextIdMedicamento) { nextIdMedicamento = med->getID() + 1; }
    }

    void SistemaFarmacia::cargarClienteDesdeArchivo(int id, std::string nombre, std::string cedula, 
        bool tarjeta) {

        Cliente* nuevo = new Cliente(id, nombre, cedula);

        nuevo->setTarjeta(tarjeta);

        listaClientes.agregar(nuevo);

        if (id >= nextIdCliente) { nextIdCliente = id + 1; }
    }

    void SistemaFarmacia::cargarVentaDesdeArchivo(int id, int idCliente, int idMedicamento, int cantidad,
        double precioFinal, std::string fecha) {
        
        Medicamento* medicamento = listaMedicamentos.buscarPorId(idMedicamento);

		if (medicamento == nullptr) { return; } // Si el medicamento no existe, no se puede cargar la venta

        Venta* nuevaVenta = new Venta(id, idCliente, medicamento, cantidad, precioFinal, fecha);

        listaVentas.agregar(nuevaVenta);

        if (id >= nextIdVenta) { nextIdVenta = id + 1; }
    }

    Medicamento* SistemaFarmacia::obtenerMenorStock() {

        return Estadisticas::obtenerMenorStock(listaMedicamentos);
    }

    Medicamento* SistemaFarmacia::obtenerMasVendido() {

        return Estadisticas::obtenerMasVendido(listaMedicamentos, listaVentas);
    }

    Cliente* SistemaFarmacia::obtenerClienteVIP() {

        return Estadisticas::obtenerClienteVIP(listaClientes, listaVentas);
    }

    double SistemaFarmacia::obtenerIngresosTotales() {

        return Estadisticas::obtenerIngresosTotales(listaVentas);
    }

    int SistemaFarmacia::contarPorCategoria(std::string categoria) {

        return Estadisticas::contarPorCategoria(listaMedicamentos, categoria);
    }

    bool SistemaFarmacia::proveedorTieneMedicamentos(int idProveedor) {

        return listaMedicamentos.tieneProveedor(idProveedor);
    }

    SistemaFarmacia::~SistemaFarmacia() {

        // Las listas enlazadas se encargan de liberar la memoria de sus nodos
    }
} // namespaceFarmaSystem