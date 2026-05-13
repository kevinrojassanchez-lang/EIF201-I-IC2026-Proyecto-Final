#ifndef VENTASVIEW_H
#define VENTASVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class VentasView : public QWidget {
        Q_OBJECT

    private:
        SistemaFarmacia* sistema;

        // UI Components
        bool bordeIluminado = true;
        QComboBox* comboClientesVenta;
        QComboBox* comboMedicamentosVenta;
        QSpinBox* spinCantidadVenta;
        QCheckBox* checkRecetaVenta;
        QLabel* lblSubtotalVenta;
        QLabel* lblTotalVenta;
        QTableWidget* tablaVentas;
        QTimer* timerBorde;

        // Metodos internos
        void construirUI();
        void cargarCombosVentas();
        void actualizarTablaVentas();
        void actualizarVistaPrecioVenta();
        void procesarVentaUI();
        void manejarCambioTextoCantidad(const QString& texto);
        void resetEstadoReceta();
        



    public:
        VentasView(SistemaFarmacia* sistema, QWidget* parent = nullptr);
        void actualizarVista(); // Se llama al entrar a la pantalla

    signals:
        void datosActualizados();
        void volverAlMenu();


    };
}

#endif
