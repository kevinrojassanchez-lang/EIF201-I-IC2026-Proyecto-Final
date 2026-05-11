#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ClientesView.h"
#include "InventarioView.h"
#include "CategoriasView.h"
#include "VentasView.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QStatusBar>
#include <QPushButton>
#include <QLabel>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class MainWindow : public QMainWindow {
        Q_OBJECT

     private:
        // SISTEMA
        SistemaFarmacia* sistema;

        // Contenedor de pantallas
        QStackedWidget* vistas;

        // Punteros a las vistas (Clases modularizadas)
        InventarioView* vistaInventario;
        ClientesView* vistaClientes;
        CategoriasView* vistaCategorias;
        VentasView* vistaVentas;

        // Metodos de creacion y navegacion
        QWidget* crearMenuPrincipal();
        void mostrarVistaInventario();
        void mostrarVistaClientes();
        void mostrarVistaVentas();
        void mostrarVistaCategorias();

        // UI General
        void actualizarBarraEstado();
        void mostrarVistaMenuPrincipal();

     public:
        MainWindow(SistemaFarmacia* sistema, QWidget* parent = nullptr);
        ~MainWindow();

    
    };
}

#endif