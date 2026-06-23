#include "SistemaFarmacia.h"
#include "ArchivoCarga.h"
#include "MedicamentoGenerico.h"
#include "MedicamentoMarca.h"
#include "MedicamentoControlado.h"
#include "Estadisticas.h"
#include "ValidadorCentral.h"


namespace FarmaSystem {

    SistemaFarmacia::SistemaFarmacia() : nextIdMedicamento(1), nextIdProveedor(1), nextIdCliente(1),
        nextIdVenta(1), datosCargados(false), medicamentosAceptados(0), medicamentosDescartados(0),
        ventasAceptadas(0), ventasDescartadas(0), proveedoresAceptados(0), proveedoresDescartados(0),
        clientesAceptados(0), clientesDescartados(0) {
    
		cargarDatos(); // Carga inicial de datos al crear el sistema
    }

    void SistemaFarmacia::cargarDatos() {

		listaProveedores.limpiar();
		listaMedicamentos.limpiar();
		listaClientes.limpiar();
		listaVentas.limpiar();

        cargarProveedores();
        cargarMedicamentos();
        cargarClientes();
        cargarVentas();

        datosCargados = true;
    }

    void SistemaFarmacia::guardarDatos() {

        try {
            std::ofstream proveedores("datos/proveedores.txt", std::ios::out | std::ios::trunc);
            std::ofstream medicamentos("datos/medicamentos.txt", std::ios::out | std::ios::trunc);
            std::ofstream clientes("datos/clientes.txt", std::ios::out | std::ios::trunc);
            std::ofstream ventas("datos/ventas.txt", std::ios::out | std::ios::trunc);

            proveedores.exceptions(std::ofstream::failbit | std::ofstream::badbit);
            medicamentos.exceptions(std::ofstream::failbit | std::ofstream::badbit);
            clientes.exceptions(std::ofstream::failbit | std::ofstream::badbit);
            ventas.exceptions(std::ofstream::failbit | std::ofstream::badbit);

            if (!proveedores.is_open() || !medicamentos.is_open() || !clientes.is_open() || !ventas.is_open()) {
                throw std::runtime_error("No se pudieron abrir los archivos de guardado");
            }

            listaProveedores.guardarEnArchivo(proveedores);
            listaMedicamentos.guardarEnArchivo(medicamentos);
            listaClientes.guardarEnArchivo(clientes);
            listaVentas.guardarEnArchivo(ventas);
        }
        catch (...) {}
    }

    bool SistemaFarmacia::getDatosCargados() const { return datosCargados; }
    void SistemaFarmacia::setDatosCargados(bool estado) { datosCargados = estado; }

    void SistemaFarmacia::cargarProveedores() {

        std::ifstream archivo("datos/proveedores.txt");

        if (!archivo.is_open()) { return; }

        proveedoresAceptados = 0;
        proveedoresDescartados = 0;

        std::string linea = "";

        while (std::getline(archivo, linea)) {

            if (linea == "") { proveedoresDescartados++; }

            else {

                Proveedor* proveedor = ArchivoCarga::crearProveedor(linea);

                if (proveedor != nullptr) {

                    cargarProveedorDesdeArchivo(proveedor);
                    proveedoresAceptados++;
                }

                else { proveedoresDescartados++; }
            }
        }
        archivo.close();
    }

    void SistemaFarmacia::cargarMedicamentos() {

        std::ifstream archivo("datos/medicamentos.txt");

        if (!archivo.is_open()) { return; }

        medicamentosAceptados = 0;
        medicamentosDescartados = 0;

        std::string linea = "";

        while (std::getline(archivo, linea)) {

            if (linea == "") { medicamentosDescartados++; }

            else {

                Medicamento* medicamento = ArchivoCarga::crearMedicamento(linea);

                if (medicamento != nullptr) {

                    cargarMedicamentoDesdeArchivo(medicamento);
                    medicamentosAceptados++;
                }

                else { medicamentosDescartados++; }
            }
        }
        archivo.close();
    }

    void SistemaFarmacia::cargarClientes() {

        std::ifstream archivo("datos/clientes.txt");

        if (!archivo.is_open()) { return; }

        clientesAceptados = 0;
        clientesDescartados = 0;

        std::string linea;

        while (std::getline(archivo, linea)) {

            if (linea.empty()) { clientesDescartados++; }

            else {

                Cliente* cliente = ArchivoCarga::crearCliente(linea);

                if (cliente != nullptr) {

                    cargarClienteDesdeArchivo(cliente);
                    clientesAceptados++;
                }

                else { clientesDescartados++; }
            }
        }
        archivo.close();
    }

    void SistemaFarmacia::cargarVentas() {

        std::ifstream archivo("datos/ventas.txt");

        if (!archivo.is_open()) { return; }

        ventasAceptadas = 0;
        ventasDescartadas = 0;

        std::string linea = "";

        while (std::getline(archivo, linea)) {

            if (linea == "") { ventasDescartadas++; }

            else {

                try {
               
                    std::stringstream ss(linea);
                    std::string col1, col2, idMedStr;

                    std::getline(ss, col1, '|');      // Salta ID Venta
                    std::getline(ss, col2, '|');      // Salta ID Cliente
                    std::getline(ss, idMedStr, '|');  // Captura ID Medicamento

                    int idMedicamento = ValidadorCentral::validarYConvertirEntero(idMedStr);

                    // Buscamos el objeto en la ram
                    Medicamento* medicamento = buscarMedicamentoPorID(idMedicamento);

                    if (medicamento == nullptr) { ventasDescartadas++; }

                    else {
                    
                        Venta* venta = ArchivoCarga::crearVenta(linea, medicamento);

                        if (venta != nullptr) {

                            cargarVentaDesdeArchivo(venta);
                            ventasAceptadas++;
                        }

                        else { ventasDescartadas++; }
                    }
                }
                catch (...) { ventasDescartadas++; }
            }
        }
        archivo.close();
    }

    // Proveedores
    void SistemaFarmacia::cargarProveedorDesdeArchivo(Proveedor* extraido) {

        if (extraido == nullptr) { return; }

        listaProveedores.agregar(extraido);

        if (extraido->getID() >= nextIdProveedor) { nextIdProveedor = extraido->getID() + 1; }
    }

    int SistemaFarmacia::registrarProveedor(const std::string& nombre, const std::string& telefono, const std::string& email,
        const std::string& pais) {

        if (nombre == "" || telefono == "" || email == "" || pais == "") { return 1; }
        if (buscarProveedorPorNombre(nombre) != nullptr) { return 2; }

        Proveedor* nuevo = new Proveedor(nextIdProveedor++, nombre, telefono, email, pais);
        listaProveedores.agregar(nuevo);

        guardarDatos();
        return 0;
    }

    bool SistemaFarmacia::eliminarProveedor(int id) {

        bool tiene = proveedorTieneMedicamentos(id);
        bool eliminado = listaProveedores.eliminar(id, tiene);

        if (!eliminado) { return false; }

		guardarDatos(); // Guarda si se elimino correctamente
        return true;
    }

    bool SistemaFarmacia::proveedorTieneMedicamentos(int idProveedor) {
        return listaMedicamentos.tieneProveedor(idProveedor);
    }

    Proveedor* SistemaFarmacia::buscarProveedorPorNombre(const std::string& nombre) {
        return listaProveedores.buscarPorNombre(nombre);
    }

    Proveedor* SistemaFarmacia::buscarProveedorPorID(int id) {
        return listaProveedores.buscarPorId(id);
    }

    bool SistemaFarmacia::editarProveedor(int id, const std::string& nombre, const std::string& telefono,
        const std::string& email, const std::string& pais) {

        Proveedor* proveedor = buscarProveedorPorID(id);

        if (proveedor == nullptr) { return false;}

        proveedor->setNombre(nombre);
        proveedor->setTelefono(telefono);
        proveedor->setEmail(email);
        proveedor->setPais(pais);

        guardarDatos(); // Guarda los cambios en el text automaticamente
        return true;
    }
   
	// Medicamentos
    void SistemaFarmacia::cargarMedicamentoDesdeArchivo(Medicamento* extraido) {

        listaMedicamentos.agregar(extraido);

        if (extraido->getID() >= nextIdMedicamento) { nextIdMedicamento = extraido->getID() + 1; }
    }

    int SistemaFarmacia::registrarGenerico(const std::string& nombre, double precio, int stock,
        const std::string& principio, int idProveedor, bool receta) {

        if (nombre == "" || principio == "") { return 1; }
        if (precio <= 0 || stock < 0) { return 2; }
        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 3; }

        Medicamento* nuevo = new MedicamentoGenerico(nextIdMedicamento++, nombre, precio, receta, stock,
            principio, idProveedor);

        listaMedicamentos.agregar(nuevo);
        guardarDatos();
        return 0;
    }

    int SistemaFarmacia::registrarMarca(const std::string& nombre, double precio, int stock,
        const std::string& pais, bool promo, int idProveedor, bool receta) {

        if (nombre == "" || pais == "") { return 1; }
        if (precio <= 0 || stock < 0) { return 2; }
        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 3; }

        Medicamento* nuevo = new MedicamentoMarca(nextIdMedicamento++, nombre, precio, receta, stock, pais,
            promo, idProveedor);

        listaMedicamentos.agregar(nuevo);
        guardarDatos();
        return 0;
    }

    int SistemaFarmacia::registrarControlado(const std::string& nombre, double precio, int stock,
        int nivel, double dosis, int idProveedor, bool receta) {

        if (nombre == "") { return 1; }
        if (precio <= 0 || stock < 0 || dosis <= 0) { return 2; }
        if (nivel < 1 || nivel > 4) { return 3; }
        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 4; }

        Medicamento* nuevo = new MedicamentoControlado(nextIdMedicamento++, nombre, precio, receta, stock,
            nivel, dosis, idProveedor);

        listaMedicamentos.agregar(nuevo);
        guardarDatos();
        return 0;
    }

    int SistemaFarmacia::editarGenerico(int id, const std::string& nombre, double precio, int stock,
        bool receta, const std::string& principio, int idProveedor) {

        Medicamento* actual = buscarMedicamentoPorID(id);

        if (actual == nullptr) { return 1; }
        if (nombre == "" || principio == "") { return 2; }
        if (precio <= 0 || stock < 0) { return 3; }
        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 4; }
        if (!actual->setNombre(nombre)) { return 5; }
        if (!actual->setPrecio(precio)) { return 6; }
        if (!actual->setIdProveedor(idProveedor)) { return 7; }
        if (!actual->setPrincipioActivo(principio)) { return 8; }

        actual->setStock(stock);
        actual->setReceta(receta);

        guardarDatos();
        return 0;
    }

    int SistemaFarmacia::editarMarca(int id, const std::string& nombre, double precio, int stock,
        bool receta, const std::string& pais, bool promo, int idProveedor) {

        Medicamento* actual = buscarMedicamentoPorID(id);

        if (actual == nullptr) { return 1; }
        if (nombre == "" || pais == "") { return 2; }
        if (precio <= 0 || stock < 0) { return 3; }
        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 4; }
        if (!actual->setNombre(nombre)) { return 5; }
        if (!actual->setPrecio(precio)) { return 6; }
        if (!actual->setIdProveedor(idProveedor)) { return 7; }
        if (!actual->setPaisOrigen(pais)) { return 8; }

        actual->setStock(stock);
        actual->setReceta(receta);
        actual->setPromocion(promo);

        guardarDatos();
        return 0;
    }

    int SistemaFarmacia::editarControlado(int id, const std::string& nombre, double precio, int stock,
        bool receta, int nivel, double dosis, int idProveedor) {

        Medicamento* actual = buscarMedicamentoPorID(id);

        if (actual == nullptr) { return 1; }
        if (nombre == "") { return 2; }
        if (precio <= 0 || stock < 0 || dosis <= 0) { return 3; }
        if (nivel < 1 || nivel > 4) { return 4; }
        if (listaProveedores.buscarPorId(idProveedor) == nullptr) { return 5; }
        if (!actual->setNombre(nombre)) { return 6; }
        if (!actual->setPrecio(precio)) { return 7; }
        if (!actual->setIdProveedor(idProveedor)) { return 8; }
        if (!actual->setNivelControl(nivel)) { return 9; }
        if (!actual->setDosisMaxima(dosis)) { return 10; }

        actual->setStock(stock);
        actual->setReceta(true); // Siempre receta en true

		guardarDatos(); // Guarda si pasa todas las validaciones
        return 0;
    }

    int SistemaFarmacia::contarPorCategoria(const std::string& categoria) {

        return Estadisticas::contarPorCategoria(listaMedicamentos, categoria);
    }

    void SistemaFarmacia::listarMedicamentos() { listaMedicamentos.mostrarTodos(); }

    bool SistemaFarmacia::eliminarMedicamento(int id) {

        Medicamento* actual = listaMedicamentos.buscarPorId(id);

        if (actual == nullptr) { return false; }
        if (listaVentas.existeVentaDeMedicamento(id)) { return false; }

        bool eliminado = listaMedicamentos.eliminar(id);

        if (eliminado) { guardarDatos();}

        return eliminado;
    }

    Medicamento* SistemaFarmacia::buscarMedicamentoPorID(int id) {
        return listaMedicamentos.buscarPorId(id);
    }

    Medicamento* SistemaFarmacia::buscarMedicamentoPorNombre(const std::string& nombre) {
        return listaMedicamentos.buscarPorNombre(nombre);
    }

    std::string SistemaFarmacia::obtenerReporteMedicamento(int id) {

        Medicamento* medicamento = buscarMedicamentoPorID(id);
        if (medicamento == nullptr) { return "Medicamento no encontrado."; }

        double base = medicamento->getPrecio();
        double final = medicamento->calcularPrecioFinal(1);

        std::ostringstream reporte;
        reporte << "<ADVERTENCIA>:\n\n" << medicamento->getAdvertencia() << "\n\n"<< "<LOGICA DE PRECIO>:\n\n"
            << "Precio Base de 1 unidad: CRC " << std::fixed << std::setprecision(2) << base << "\n"
            << "Precio Fin de  1 unidad: CRC " << final << "\n" << medicamento->getInfoPrecio();

        return reporte.str();
    }

    Medicamento* SistemaFarmacia::obtenerMenorStock() {
        return Estadisticas::obtenerMenorStock(listaMedicamentos);
    }

    Medicamento* SistemaFarmacia::obtenerMasVendido() {
        return Estadisticas::obtenerMasVendido(listaMedicamentos, listaVentas);
    }

    // Clientes
    void SistemaFarmacia::cargarClienteDesdeArchivo(Cliente* extraido) {

        if (extraido == nullptr) { return; }

        listaClientes.agregar(extraido);

        if (extraido->getID() >= nextIdCliente) { nextIdCliente =extraido->getID() + 1; }
    }

    int SistemaFarmacia::registrarCliente(const std::string& nombre, const std::string& cedula) {

        if (nombre == "" || cedula == "") { return 1; }
        if (listaClientes.buscarPorCedula(cedula) != nullptr) { return 2; }

        Cliente* nuevo = new Cliente(nextIdCliente++, nombre, cedula);

        listaClientes.agregar(nuevo);
        guardarDatos();
        return 0; 
    }

    void SistemaFarmacia::listarClientes() { listaClientes.mostrarTodos(); }

    bool SistemaFarmacia::eliminarCliente(int id) {

        Cliente* cliente = listaClientes.buscarPorId(id);

        if (cliente == nullptr) { return false; }
        if (listaVentas.existeVentaDeCliente(id)) { return false; }

		bool eliminado = listaClientes.eliminar(id);

		if (eliminado) { guardarDatos(); }

        return eliminado;
    }

    int SistemaFarmacia::editarCliente(int id, const std::string& nombre, const std::string& cedula,
        bool fidelidad) {

        Cliente* cliente = buscarClientePorID(id);

        if (cliente == nullptr) { return 1; }
        if (!cliente->setNombre(nombre)) { return 2; }
        if (!cliente->setCedula(cedula)) { return 3; }

        cliente->setTarjeta(fidelidad);

		guardarDatos();
        return 0;
    }

    Cliente* SistemaFarmacia::buscarClientePorID(int id) {
        return listaClientes.buscarPorId(id);
    }

    Cliente* SistemaFarmacia::buscarClientePorCedula(const std::string& cedula) {
        return listaClientes.buscarPorCedula(cedula);
    }

    Cliente* SistemaFarmacia::obtenerClienteVIP() {
        return Estadisticas::obtenerClienteVIP(listaClientes, listaVentas);
    }

	// Ventas
    void SistemaFarmacia::cargarVentaDesdeArchivo(Venta* extraido) {

        if (extraido == nullptr) { return; }

        listaVentas.agregar(extraido);

        if (extraido->getId() >= nextIdVenta) { nextIdVenta = extraido->getId() + 1; }
    }

    int SistemaFarmacia::registrarVenta(const std::string& cedula, int idMed, int cantidad, bool presentoReceta,
        const std::string& fecha, bool confirmado) {

        // Validaciones rapidas de estado de la UI primero que nada
        if (!confirmado) { return 8; }
        if (fecha.empty()) { return 4; }
        if (cantidad <= 0) { return 3; }

        Cliente* clienteComprador = buscarClientePorCedula(cedula);
        Medicamento* medicamento = buscarMedicamentoPorID(idMed);

        if (clienteComprador == nullptr) { return 1; }
        if (medicamento == nullptr) { return 2; }
        if (medicamento->getReceta() && !presentoReceta) { return 5; }
        if (medicamento->getStock() < cantidad) { return 6; }
        if (medicamento->excedeDosis(cantidad)) { return 7; }

        double totalFinal = calcularTotalVenta(clienteComprador, medicamento, cantidad);

        Venta* nuevaVenta = new Venta(nextIdVenta, clienteComprador->getID(), medicamento, cantidad, totalFinal, fecha);

        listaVentas.agregar(nuevaVenta);
        nextIdVenta++;

        medicamento->setStock(medicamento->getStock() - cantidad);

        guardarDatos();
        return 0; // Exito
    }

    double SistemaFarmacia::calcularTotalVenta(Cliente* cliente, Medicamento* medicamento, int cantidad) {

        double precioBase = medicamento->calcularPrecioFinal(cantidad);

        return precioBase * (cliente->getTarjeta() ? 0.95 : 1.0);
    }

    void SistemaFarmacia::listarVentas() {

        if (listaVentas.cantidad() == 0) { return; }

        listaVentas.imprimirHistorial();
    }

    double SistemaFarmacia::obtenerIngresosTotales() {
        return Estadisticas::obtenerIngresosTotales(listaVentas);
    }

	// Getters para estadisticas de carga
    int SistemaFarmacia::getCantProveedoresAceptados() const { return proveedoresAceptados; }
    int SistemaFarmacia::getCantProveedoresDescartados() const { return proveedoresDescartados; }
    int SistemaFarmacia::getCantClientesAceptados() const { return clientesAceptados; }
    int SistemaFarmacia::getCantClientesDescartados() const { return clientesDescartados; }
    int SistemaFarmacia::getCantMedicamentosAceptados() const { return medicamentosAceptados; }
    int SistemaFarmacia::getCantMedicamentosDescartados() const { return medicamentosDescartados; }
    int SistemaFarmacia::getCantVentasAceptadas() const { return ventasAceptadas; }
    int SistemaFarmacia::getCantVentasDescartadas() const { return ventasDescartadas; }

	// Getters para acceso a datos
    int SistemaFarmacia::getCantMedicamentos() const { return listaMedicamentos.cantidad(); }
    int SistemaFarmacia::getCantClientes() const { return listaClientes.cantidad(); }
    int SistemaFarmacia::getCantVentas() const { return listaVentas.cantidad(); }
    int SistemaFarmacia::getCantProveedores() const { return listaProveedores.cantidad(); }
    Medicamento* SistemaFarmacia::getMedicamentoPorIndice(int i) { return listaMedicamentos.obtener(i); }
    Cliente* SistemaFarmacia::getClientePorIndice(int i) { return listaClientes.obtener(i); }
    Venta* SistemaFarmacia::getVentaPorIndice(int i) { return listaVentas.obtener(i); }
    Proveedor* SistemaFarmacia::getProveedorPorIndice(int i) { return listaProveedores.obtener(i); }
	
	// Destructor
    SistemaFarmacia::~SistemaFarmacia() {}

        // Las listas enlazadas se encargan de liberar la memoria de sus nodos
    
} // namespaceFarmaSystem