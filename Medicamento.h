#pragma once
#include <string>
#include <sstream>
#include <iomanip>

namespace FarmaSystem {

    class Medicamento {
    protected:
        int id;
        std::string nombre;
        double precio;
        int stock;
        bool requiereReceta;

    public:
        Medicamento(int id, std::string nombre, double precio, bool receta, int stock);
        int getID() const;
        std::string getNombre() const;
        double getPrecio() const;
        int getStock() const;
        virtual bool getReceta() const;
        void setStock(int cantidad);
		virtual bool excedeDosis(int cantidad) const; // Controlado tiene dosis maxima, los otros no
        virtual std::string getCategoria() const = 0;
        virtual double calcularPrecioFinal(int cantidad) const = 0;
        virtual std::string getAdvertencia() const = 0;
        virtual std::string getInfoExtra() const = 0;
        virtual std::string getInfoPrecio() const = 0;
        virtual std::string mostrar() const;
        virtual ~Medicamento();
    };
}

