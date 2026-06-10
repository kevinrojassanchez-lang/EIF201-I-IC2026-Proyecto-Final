#include "ValidadorCentral.h"
#include <sstream>
#include <stdexcept> // Biblioteca estandar basica para usar std::runtime_error

namespace FarmaSystem {

    bool ValidadorCentral::esTextoEnteroValido(const std::string& texto) {

        if (texto.empty()) { return false; }

        for (size_t i = 0; i < texto.length(); i++) {

            char caracter = texto[i];

            if (caracter < '0' || caracter > '9') {
                return false;
            }
        }
        return true;
    }

    bool ValidadorCentral::esTextoDecimalValido(const std::string& texto) {

        if (texto.empty()) { return false; }

        int contadorPuntos = 0;

        for (size_t i = 0; i < texto.length(); i++) {

            char caracter = texto[i];

            if (caracter == '.') {

                contadorPuntos++;

                if (contadorPuntos > 1) {
                    return false;
                }
            }
            else if (caracter < '0' || caracter > '9') {
                return false;
            }
        }
        return true;
    }

  
    int ValidadorCentral::validarYConvertirEntero(const std::string& texto) {
        // Ejecuta filtro de caracteres. Si falla, tira la alarma hacia el catch
        if (!esTextoEnteroValido(texto)) {
            throw std::runtime_error("Dato entero invalido");
        }

        int resultado = 0;
        std::stringstream(texto) >> resultado;
        return resultado;
    }

    double ValidadorCentral::validarYConvertirDecimal(const std::string& texto) {
     
        if (!esTextoDecimalValido(texto)) {
            throw std::runtime_error("Dato decimal invalido");
        }

        double resultado = 0.0;
        std::stringstream(texto) >> resultado;
        return resultado;
    }

    bool ValidadorCentral::validarBanderaBinaria(const std::string& texto) {

        if (texto != "0" && texto != "1") {
            throw std::runtime_error("Bandera binaria invalida");
        }

        return (texto == "1");
    }

} // namespace FarmaSystem