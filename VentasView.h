#ifndef VENTASVIEW_H
#define VENTASVIEW_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class VentasView : public QWidget {
        Q_OBJECT

    public:
        VentasView(SistemaFarmacia* sSistema, QWidget* parent = nullptr);
        void actualizarVista();

    signals:
        void datosActualizados();
        void volverAlMenu();

    protected:
        bool eventFilter(QObject* obj, QEvent* event) override;

    private:
        // Estructura para las líneas del carrito (C++ Tradicional)
        struct ItemCarrito {
            Medicamento* medicamento;
            int cantidad;
            bool presentoReceta;
            double totalItem;
        };

        SistemaFarmacia* sistema;
        QList<ItemCarrito> carritoTemporal; // El carrito en la RAM de la interfaz
        QPushButton* btnQuitarProducto; 

        void quitarProductoUI();
        double totalAcumuladoCarrito;

        // Componentes visuales de Qt
        QComboBox* comboClientesVenta;
        QComboBox* comboMedicamentosVenta;
        QComboBox* comboFiltroClientes;
        QSpinBox* spinCantidadVenta;
        QCheckBox* checkRecetaVenta;
        QTableWidget* tablaVentas;

        QLabel* lblSubtotalVenta;
        QLabel* lblTotalVenta;
        QLabel* lblTotalFiltro;

        QPushButton* btnFinalizarFactura; // Nuevo botón para el supermercado

        // Métodos de control
        void construirUI();
        void cargarCombosVentas();
        void limpiarFormularioVenta();
        void resetearLabelsPrecio();
        void resetearCarrito();
        void actualizarTablaVisualCarrito();

    private slots:
        void agregarAlCarritoUI();   // Reemplaza a procesarVentaUI
        void finalizarFacturaUI();   // Procesa todo el carrito de golpe
        void actualizarVistaPrecioVenta();
        void actualizarFiltroVentas();
        void manejarCambioTextoCantidad(const QString&);
        void resetextoReceta();
        void resetEstadoReceta();
    };
}

#endif // VENTASVIEW_H