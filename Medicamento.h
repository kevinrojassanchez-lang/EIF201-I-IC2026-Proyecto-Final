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
        int idProveedor;            // nuevo atributo

    public:
        Medicamento(int id, std::string nombre, double precio, bool receta, int stock, int idProveedor);
        int getID() const;
        std::string getNombre() const;
        double getPrecio() const;
        int getStock() const;
        virtual bool getReceta() const;
		int getIdProveedor() const;  // nuevo getter
        void setStock(int cantidad);
		
        virtual std::string getCategoria() const = 0;
        virtual double calcularPrecioFinal(int cantidad) const = 0;
        virtual std::string getAdvertencia() const = 0;
        virtual std::string getInfoExtra() const = 0;
        virtual std::string getInfoPrecio() const = 0;
        virtual std::string toFile() const = 0; // Nuevo metodo
        virtual std::string mostrar() const;

        // MedicamentoGenerico
        virtual std::string getPrincipioActivo() const;

        // MedicamentoMarca
        virtual std::string getPaisOrigen() const;
        virtual bool getPromocion() const;

        // MedicamentoControlado
        virtual bool excedeDosis(int cantidad) const; 
        virtual int getNivelControl() const;
        virtual double getDosisMaxima() const;

        virtual ~Medicamento();
    };
}

