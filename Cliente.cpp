#include "Cliente.h"

namespace FarmaSystem { // Namespace

    // Constructor
    Cliente::Cliente(int id, std::string nombre, std::string cedula) : id
        (id), nombre(nombre), cedula(cedula), tarjetaFidelidad(false) {
       
    }

    // Getters
    int Cliente::getID() const { return id; }
    std::string Cliente::getNombre() const { return nombre; }
    std::string Cliente::getCedula() const { return cedula; }
    bool Cliente::getTarjeta() const { return tarjetaFidelidad; }

    // Setters
    void Cliente::setTarjeta(bool estado) { this->tarjetaFidelidad = estado; }
    bool Cliente::setNombre(const std::string& nNombre) {

        if (nNombre == "") { return false; }

        nombre = nNombre;

        return true;
    }

    bool Cliente::setCedula(const std::string& nCedula) {

        if (nCedula == "") { return false; }

        cedula = nCedula;

        return true;
    }

    std::string Cliente::toString() const {

      return std::to_string(id) + "|" + nombre + "|" + cedula + "|" + (tarjetaFidelidad ? "1" : "0");
    }

    Cliente::~Cliente() {
        // No requiere delete manual para los strings
    }
} // Fin FarmaSystem