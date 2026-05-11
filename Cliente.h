#pragma once
#include <string>

namespace FarmaSystem {

    class Cliente {
    private: // Atributos privados (Encapsulamiento)
        int id;
        std::string nombre;
        std::string cedula;
        bool tarjetaFidelidad;

    public: // Constructor
        Cliente(int id, std::string nombre, std::string cedula);
        
        // Getters/Setters
        int getID() const;
        std::string getNombre() const;
        std::string getCedula() const;
        bool getTarjeta() const;
        void setTarjeta(bool estado);
        // Metodo para la rubrica (sin cout)
        std::string mostrar() const;

        ~Cliente(); // Destructor
    };
}

