#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QStatusBar>
#include <QPushButton>
#include <QLabel>

#include "ArchivoCarga.h"
#include "ArchivoGuardado.h"
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

     private:
        SistemaFarmacia* sistema;

        // Atributos de persistencia
        ArchivoCarga cargador;
        ArchivoGuardado guardador;

        // Vistas
        QStackedWidget* vistas;
        InventarioView* vistaInventario;
        ClientesView* vistaClientes;
        ProveedoresView* vistaProveedores;
        CategoriasView* vistaCategorias;
        VentasView* vistaVentas;
        EstadisticasView* vistaEstadisticas; // nuevo

        // Metodos de creacion y navegacion
        QWidget* crearMenuPrincipal();
        void mostrarVistaInventario();
        void mostrarVistaClientes();
        void mostrarVistaVentas();
        void mostrarVistaCategorias();
        void mostrarVistaProveedores();
        void mostrarVistaEstadisticas();

        // UI General
        void actualizarBarraEstado();
        void mostrarVistaMenuPrincipal();

    protected:
        // Sobrescribimos el evento de cierre de Qt para atrapar la X que cierra la ventana
        void closeEvent(QCloseEvent* event) override;

     public:
        MainWindow(SistemaFarmacia* sistema, QWidget* parent = nullptr);
        ~MainWindow();

    
    };
}

#endif