#include "Proveedor.h"
#include <sstream>

namespace FarmaSystem {

    Proveedor::Proveedor(int id, std::string nombre, std::string telefono, std::string email, std::string pais)
        : id(id), nombre(nombre), telefono(telefono), email(email), pais(pais) {
    }

    int Proveedor::getID() const { return id; }
    std::string Proveedor::getNombre() const { return nombre; }
    std::string Proveedor::getTelefono() const { return telefono; }
    std::string Proveedor::getEmail() const { return email; }
    std::string Proveedor::getPais() const { return pais; }

    void Proveedor::setNombre(const std::string& nuevoNom) { nombre = nuevoNom; }
    void Proveedor::setTelefono(const std::string& nuevoTel) { telefono = nuevoTel; }
    void Proveedor::setEmail(const std::string& nuevoEmail) { email = nuevoEmail; }
    void Proveedor::setPais(const std::string& nuevoPais) { pais = nuevoPais; }

    std::string Proveedor::toString() const {

        std::ostringstream oss;

        oss << id << "|" << nombre << "|" << telefono << "|" << email << "|" << pais;

        return oss.str();
    }

    Proveedor::~Proveedor() {}
}