#include "Medicamento.h"

namespace FarmaSystem {

    // Se añade idProveedor a la lista de inicializacion
    Medicamento::Medicamento(int id, std::string nombre, double precio, bool receta, int stock,
        int idProveedor) : id(id), nombre(nombre), precio(precio), stock(stock), requiereReceta(receta),
        idProveedor(idProveedor) {
 
    }

    // Getters
    int Medicamento::getID() const { return id; }
    std::string Medicamento::getNombre() const { return nombre; }
    double Medicamento::getPrecio() const { return precio; }
    int Medicamento::getStock() const { return stock; }
    bool Medicamento::getReceta() const { return requiereReceta; }
    int Medicamento::getIdProveedor() const { return idProveedor; }

    // Setters
    void Medicamento::setStock(int cantidad) { if (cantidad >= 0) { this->stock = cantidad; } }
    bool Medicamento::setNombre(const std::string& nNombre) {

        if (nNombre == "") { return false; }
        nombre = nNombre;
        return true;
    }
    bool Medicamento::setPrecio(double nPrecio) {

        if (nPrecio <= 0) { return false; }
        precio = nPrecio;
        return true;
	}
    void Medicamento::setReceta(bool receta) { requiereReceta = receta; }
    bool Medicamento::setIdProveedor(int nIdProveedor) {

        if (nIdProveedor < 0) { return false; }
        idProveedor = nIdProveedor;
        return true;
	}

    bool Medicamento::setPrincipioActivo(const std::string&) { return false;}
    bool Medicamento::setPaisOrigen(const std::string&) { return false; }
    void Medicamento::setPromocion(bool) {} // No hace nada, solo lo implementan los medicamentos de marca 
    bool Medicamento::setNivelControl(int) { return false; }
    bool Medicamento::setDosisMaxima(double) { return false; }

    // Getters virtuales 
    std::string Medicamento::getPrincipioActivo() const { return ""; }
    std::string Medicamento::getPaisOrigen() const { return ""; }
    bool Medicamento::getPromocion() const { return false; }
    int Medicamento::getNivelControl() const { return 0; }
    double Medicamento::getDosisMaxima() const { return 0.0; }
    bool Medicamento::excedeDosis(int cantidad) const { return false; }

    std::string Medicamento::toString() const {
        std::ostringstream oss;
 
        oss << getCategoria() << "|" << id << "|" << nombre << "|"
            << std::fixed << std::setprecision(2) << precio  << "|" 
            << stock << "|" << (getReceta() ? "1" : "0") << "|" << idProveedor;

        return oss.str();
    }

    Medicamento::~Medicamento() {
        // Destructor limpio
    }
} // Fin FarmaSystem
