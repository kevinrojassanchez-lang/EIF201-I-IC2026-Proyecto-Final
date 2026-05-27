#ifndef ESTADISTICASVIEW_H
#define ESTADISTICASVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class EstadisticasView : public QWidget {
        Q_OBJECT

    private:

        SistemaFarmacia* sistema;

        QLabel* lblMenorStock;
        QLabel* lblMasVendido;
        QLabel* lblClienteVIP;
        QLabel* lblIngresos;

        QLabel* barraGenericos;
        QLabel* barraMarca;
        QLabel* barraControlados;

        QLabel* valorGenericos;
        QLabel* valorMarca;
        QLabel* valorControlados;

        QPushButton* botonActualizar;
        QPushButton* botonVolver;

        void construirUI();
        void actualizarDatos();
        void actualizarGraficoCategorias();

    public:

        EstadisticasView(SistemaFarmacia* sistema,
            QWidget* parent = nullptr);

    signals:

        void volverAlMenu();

    };

}

#endif

