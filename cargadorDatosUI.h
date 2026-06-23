#ifndef CARGADOR_DATOS_UI_H
#define CARGADOR_DATOS_UI_H

#include <QComboBox>
#include <QStringList>
#include <QList>
#include <QLabel>

namespace FarmaSystem {
    namespace EIF201 {

        class CargadorDatosUI {
        public:
            // Funcion para los combos indexados
            static void plasmarDatosEnCombo(QComboBox* combo,
                const QStringList& nombres,
                const QList<int>& ids,
                const QString& textoInicial = "");

        };

    } // namespace EIF201
} // namespace FarmaSystem

#endif // CARGADOR_DATOS_UI_H