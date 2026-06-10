#pragma once
#include <string>

namespace FarmaSystem {

    class Cliente {
    private: 
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
        bool setNombre(const std::string& nNombre);
		bool setCedula(const std::string& nCedula);

		// Metodo para representar el cliente como string
        std::string toString() const;

        ~Cliente(); // Destructor
    };
}

