#include "Medicamento.h"

namespace FarmaSystem {

    // Constructor al estilo moderno que usa el profe
    Medicamento::Medicamento(int id, std::string nombre, double precio, bool receta, int stock) : id
        (id), nombre(nombre), precio(precio), stock(stock), requiereReceta(receta) {}

    // Getters
    int Medicamento::getID() const { return id; }
    std::string Medicamento::getNombre() const { return nombre; }
    double Medicamento::getPrecio() const { return precio; }
    int Medicamento::getStock() const { return stock; }
    bool Medicamento::getReceta() const { return requiereReceta; }

    // Setter
    void Medicamento::setStock(int cantidad) { this->stock = cantidad; }

	// Por defecto, la clase base no tiene una dosis maxima, asi que este metodo devuelve false.
    bool Medicamento::excedeDosis(int cantidad) const { return false; }

   // Metodo mostrar()
    std::string Medicamento::mostrar() const {
        std::ostringstream oss;

        oss << "ID: " << id << " | Med: " << nombre << " | Categoria: " << getCategoria()<< " | Precio: " << std::fixed << std::setprecision(2) << precio << " CRC"
            << " | Stock: " << stock << " | Receta: " << (getReceta() ? "Si" : "No");
         return oss.str();
    }
    Medicamento::~Medicamento() {
        // Destructor limpio
    }
} // Fin FarmaSystem