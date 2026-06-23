#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QStatusBar>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTimer>
#include <QMovie>

#include "ClientesView.h"
#include "InventarioView.h"
#include "CategoriasView.h"
#include "VentasView.h"
#include "ProveedoresView.h"
#include "EstadisticasView.h"
#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class MainWindow : public QMainWindow {
        Q_OBJECT

    private slots:
        // SLOTS ASOCIADAS A EVENTOS Y NAVEGACION NATIVA DE QT
        void actualizarFotogramaFondo();
        void mostrarVistaInventario();
        void mostrarVistaClientes();
        void mostrarVistaVentas();
        void mostrarVistaCategorias();
        void mostrarVistaProveedores();
        void mostrarVistaEstadisticas();
        void mostrarVistaMenuPrincipal();
        void actualizarBarraEstado();

    private:
        // VARIABLES Y METODOS INTERNOS DE CONTROL
        SistemaFarmacia* sistema;
        QTextEdit* consolaDebug;
        QLabel* labelFondo;
        QTimer* timerAnimacion;
        QMovie* animacionFondo;

        // El contador numerico del fotograma actual Mantiene el consumo de RAM en cero
        int fotogramaActual;

        void imprimirLogsIniciales();
        void logConsola(const QString& mensaje, const QString& color = "#00FF00");
        QWidget* crearMenuPrincipal();

        // CONTENEDORES DE INTERFAZ Y VISTAS DE LAS PANTALLAS
        QStackedWidget* vistas;
        InventarioView* vistaInventario;
        ClientesView* vistaClientes;
        ProveedoresView* vistaProveedores;
        CategoriasView* vistaCategorias;
        VentasView* vistaVentas;
        EstadisticasView* vistaEstadisticas;

    protected:
        // EVENTOS NATIVOS SOBREESCRITOS DE LA VENTANA
        void closeEvent(QCloseEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;

    public:
        MainWindow(SistemaFarmacia* sistema, QWidget* parent = nullptr);
        ~MainWindow();
    };

} // namespace FarmaSystem

#endif // MAINWINDOW_H