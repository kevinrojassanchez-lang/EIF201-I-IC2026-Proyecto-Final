#ifndef VALIDADOR_CENTRAL_H
#define VALIDADOR_CENTRAL_H

#include <string>

namespace FarmaSystem {

    class ValidadorCentral {
    private:
        // funciones base con aritmetica de caracteres (Ocultas en private)
        static bool esTextoEnteroValido(const std::string& texto);
        static bool esTextoDecimalValido(const std::string& texto);

    public:
        // Metodos publicos de control para el try-catch de carga
        static int validarYConvertirEntero(const std::string& texto);
        static double validarYConvertirDecimal(const std::string& texto);
        static bool validarBanderaBinaria(const std::string& texto);
    };

}

#endif // VALIDADOR_CENTRAL_H

