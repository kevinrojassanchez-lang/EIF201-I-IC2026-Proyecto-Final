#include "Cliente.h"

namespace FarmaSystem { // Namespace

    // Constructor al estilo moderno que usa el profe
    Cliente::Cliente(int id, std::string nombre, std::string cedula) : id
        (id), nombre(nombre), cedula(cedula), tarjetaFidelidad(false) {
       
    }
    // Getters
    int Cliente::getID() const { return id; }
    std::string Cliente::getNombre() const { return nombre; }
    std::string Cliente::getCedula() const { return cedula; }
    bool Cliente::getTarjeta() const { return tarjetaFidelidad; }
    // Setter
    void Cliente::setTarjeta(bool estado) { this->tarjetaFidelidad = estado; }
	// Metodo mostrar() para la rubrica (sin cout)
    std::string Cliente::mostrar() const {
      return " ID: " + std::to_string(id) + " | Nombre: " + nombre + " | Cedula: " + cedula + " | Tarjeta: " + (tarjetaFidelidad ? "Activa" : "Inactiva");
    }
    Cliente::~Cliente() {
        // No requiere delete manual para los strings
    }

} // Fin FarmaSystem
