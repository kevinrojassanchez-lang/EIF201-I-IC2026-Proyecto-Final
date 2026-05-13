#ifndef CATEGORIASVIEW_H
#define CATEGORIASVIEW_H

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class CategoriasView : public QWidget {
        Q_OBJECT

    public:
        explicit CategoriasView(SistemaFarmacia* sistema, QWidget* parent = nullptr);
        void actualizarVista();

    signals:
        void volverAlMenu();
		void datosActualizados();

    private:
        SistemaFarmacia* sistema;

        // UI Components
        QListWidget* listaCategorias;
        QListWidget* listaMedicamentosCategoria;
        QTextEdit* infoCategoria;
        QTimer* timerBorde;
        QStringList listaColores;
        int colorActualIdx = 0;
        void animarBorde();

        // Metodos de logica
        void construirUI();
        void mostrarCategoriaSeleccionada();
        void mostrarMedicamentoCategoria();
        void actualizarVistaCategorias();
    };

}

#endif
