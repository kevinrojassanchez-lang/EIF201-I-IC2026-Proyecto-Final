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

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class VentasView : public QWidget {
        Q_OBJECT

    public:
        explicit VentasView(SistemaFarmacia* sistema, QWidget* parent = nullptr);
        void actualizarVista(); // Se llama al entrar a la pantalla

    signals:
        void datosActualizados();
        void volverAlMenu();

    private:
        SistemaFarmacia* sistema;

        // UI Components
        QComboBox* comboClientesVenta;
        QComboBox* comboMedicamentosVenta;
        QSpinBox* spinCantidadVenta;
        QCheckBox* checkRecetaVenta;
        QLabel* lblSubtotalVenta;
        QLabel* lblTotalVenta;
        QTableWidget* tablaVentas;

        // Metodos internos
        void setupUI();
        void cargarCombosVentas();
        void actualizarTablaVentas();
        void actualizarVistaPrecioVenta();
        void procesarVentaUI();
        void manejarCambioTextoCantidad(const QString& texto);
    };

}

#endif
