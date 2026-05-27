#include "Medicamento.h"

namespace FarmaSystem {

    // Se añade idProveedor a la lista de inicializacion
    Medicamento::Medicamento(int id, std::string nombre, double precio, bool receta, int stock, int idProveedor)
        : id(id), nombre(nombre), precio(precio), stock(stock), requiereReceta(receta), idProveedor(idProveedor) {
    }

    // Getters
    int Medicamento::getID() const { return id; }
    std::string Medicamento::getNombre() const { return nombre; }
    double Medicamento::getPrecio() const { return precio; }
    int Medicamento::getStock() const { return stock; }
    bool Medicamento::getReceta() const { return requiereReceta; }
    int Medicamento::getIdProveedor() const { return idProveedor; }

    // Setter
    void Medicamento::setStock(int cantidad) { this->stock = cantidad; }

    // Getters virtuales 
    std::string Medicamento::getPrincipioActivo() const { return ""; }
    std::string Medicamento::getPaisOrigen() const { return ""; }
    bool Medicamento::getPromocion() const { return false; }
    int Medicamento::getNivelControl() const { return 0; }
    double Medicamento::getDosisMaxima() const { return 0.0; }
    bool Medicamento::excedeDosis(int cantidad) const { return false; }

    std::string Medicamento::mostrar() const {
        std::ostringstream oss;
 
        oss << "ID: " << id << " | Med: " << nombre << " | Categoria: " << getCategoria()
            << " | Precio: " << std::fixed << std::setprecision(2) << precio << " CRC"
            << " | Stock: " << stock << " | Receta: " << (getReceta() ? "Si" : "No")
            << " | Prov ID: " << idProveedor;
        return oss.str();
    }

    Medicamento::~Medicamento() {
        // Destructor limpio
    }
} // Fin FarmaSystem