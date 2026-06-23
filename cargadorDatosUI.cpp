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
    } // namespace EIF201
} // namespace FarmaSystem