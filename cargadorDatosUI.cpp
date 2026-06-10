#include "CargadorDatosUI.h"
#include <QDir>
#include <QPixmap>

namespace FarmaSystem {
    namespace EIF201 {

        void CargadorDatosUI::plasmarDatosEnCombo(QComboBox* combo,
            const QStringList& nombres,
            const QList<int>& ids,
            const QString& textoInicial) {
            if (!combo) return;
            combo->clear();

            if (!textoInicial.isEmpty()) {
                combo->addItem(textoInicial, 0);
            }

            for (int i = 0; i < nombres.size(); i++) {
                combo->addItem(nombres.at(i), ids.at(i));
            }
        }

        void CargadorDatosUI::plasmarFotogramaFondo(QLabel* labelFondo, int fotogramaActual, int totalImagenes) {
            if (!labelFondo) return;

            int numeroImagen = (fotogramaActual % totalImagenes) + 1;
            QString nombreArchivo = QString("datos/fondo (%1).png").arg(numeroImagen);
            QString rutaAbsoluta = QDir::current().absoluteFilePath(nombreArchivo);

            QPixmap pixmap(rutaAbsoluta);
            if (!pixmap.isNull()) {
                labelFondo->setAlignment(Qt::AlignLeft | Qt::AlignTop);

                // Escalamos de forma limpia al tamano exacto que tenga el QLabel
                // Usamos KeepAspectRatio para que no se deforme y SmoothTransformation para maxima nitidez
                labelFondo->setPixmap(pixmap.scaled(
                    labelFondo->size(),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                ));
            }
        }


    } // namespace EIF201
} // namespace FarmaSystem