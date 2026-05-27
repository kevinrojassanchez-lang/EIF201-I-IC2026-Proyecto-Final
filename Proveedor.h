#pragma once
#include <string>

namespace FarmaSystem {

    class Proveedor {
    private:
        int id;
        std::string nombre;
        std::string telefono;
        std::string email;
        std::string pais;

    public:
        Proveedor(int id, std::string nombre, std::string telefono, std::string email, std::string pais);

        int getID() const;
        std::string getNombre() const;
        std::string getTelefono() const;
        std::string getEmail() const;
        std::string getPais() const;

        void setNombre(const std::string& nuevoNom);
        void setTelefono(const std::string& nuevoTel);
        void setEmail(const std::string& nuevoEmail);
        void setPais(const std::string& nuevoPais);

        std::string mostrar() const;
        ~Proveedor();
    };
}
