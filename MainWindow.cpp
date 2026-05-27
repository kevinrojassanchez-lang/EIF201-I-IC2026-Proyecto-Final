#include "MainWindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QCloseEvent>

namespace FarmaSystem {

    MainWindow::MainWindow(SistemaFarmacia* sSistema, QWidget* parent) : QMainWindow(parent), sistema(sSistema)
    {
        if (!sistema->getDatosCargados()) { sistema->cargarDatos(); }

        vistas = new QStackedWidget(this);
        setCentralWidget(vistas);

        // Menu Principal
        vistas->addWidget(crearMenuPrincipal());

        // Inventario
        vistaInventario = new InventarioView(sistema);
        vistas->addWidget(vistaInventario);
        connect(vistaInventario, &InventarioView::datosActualizados, this, &MainWindow::actualizarBarraEstado);
        connect(vistaInventario, &InventarioView::volverAlMenu, this, &MainWindow::mostrarVistaMenuPrincipal);

        // Clientes
        vistaClientes = new ClientesView(sistema);
        vistas->addWidget(vistaClientes);
        connect(vistaClientes, &ClientesView::datosActualizados, this, &MainWindow::actualizarBarraEstado);
        connect(vistaClientes, &ClientesView::volverAlMenu, this, &MainWindow::mostrarVistaMenuPrincipal);

        // Proveedores
        vistaProveedores = new ProveedoresView(sistema);
        vistas->addWidget(vistaProveedores);
        connect(vistaProveedores, &ProveedoresView::volverAlMenu, this, &MainWindow::mostrarVistaMenuPrincipal);

        // Categorias
        vistaCategorias = new CategoriasView(sistema);
        vistas->addWidget(vistaCategorias);
        connect(vistaCategorias, &CategoriasView::volverAlMenu, this, &MainWindow::mostrarVistaMenuPrincipal);

        // Ventas
        vistaVentas = new VentasView(sistema);
        vistas->addWidget(vistaVentas);
        connect(vistaVentas, &VentasView::datosActualizados, this, &MainWindow::actualizarBarraEstado);
        connect(vistaVentas, &VentasView::volverAlMenu, this, &MainWindow::mostrarVistaMenuPrincipal);

        // Estadisticas                                               / nuevo /
        vistaEstadisticas = new EstadisticasView(sistema);
        vistas->addWidget(vistaEstadisticas);
        connect(vistaEstadisticas, &EstadisticasView::volverAlMenu,this, &MainWindow::mostrarVistaMenuPrincipal);

        // Mostrar menu principal
        vistas->setCurrentIndex(0);
        actualizarBarraEstado();
    }

    QWidget* MainWindow::crearMenuPrincipal() {

        QWidget* widget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout(widget);

        QLabel* titulo = new QLabel("<h1>FarmaSystem</h1>");
        titulo->setAlignment(Qt::AlignCenter);

        QPushButton* botonInventario = new QPushButton("Inventario");
        QPushButton* botonClientes = new QPushButton("Clientes");
        QPushButton* botonProveedores = new QPushButton("Proveedores");
        QPushButton* botonCategorias = new QPushButton("Categorias");
        QPushButton* botonVentas = new QPushButton("Ventas");
        QPushButton* botonEstadisticas = new QPushButton("Estadisticas");
        QPushButton* botonSalir = new QPushButton("Salir");

		// widgets al layout
        layout->addWidget(titulo);
        layout->addStretch();
        layout->addWidget(botonInventario);
        layout->addWidget(botonClientes);
        layout->addWidget(botonProveedores);
        layout->addWidget(botonCategorias);
        layout->addWidget(botonVentas);
        layout->addWidget(botonEstadisticas);
        layout->addStretch();
        layout->addWidget(botonSalir);

        // Conexiones a los metodos de navegacion
        connect(botonInventario, &QPushButton::clicked, this, &MainWindow::mostrarVistaInventario);
        connect(botonClientes, &QPushButton::clicked, this, &MainWindow::mostrarVistaClientes);
        connect(botonProveedores, &QPushButton::clicked, this, &MainWindow::mostrarVistaProveedores);
        connect(botonCategorias, &QPushButton::clicked, this, &MainWindow::mostrarVistaCategorias);
        connect(botonVentas, &QPushButton::clicked, this, &MainWindow::mostrarVistaVentas);
        connect(botonEstadisticas, &QPushButton::clicked, this, &MainWindow::mostrarVistaEstadisticas);
        connect(botonSalir, &QPushButton::clicked, qApp, &QApplication::quit);

        return widget;
    }

    void MainWindow::mostrarVistaVentas() {
        vistaVentas->actualizarVista();
        vistas->setCurrentWidget(vistaVentas);
    }

    void MainWindow::mostrarVistaCategorias() {
        vistaCategorias->actualizarVista();
        vistas->setCurrentWidget(vistaCategorias);
    }

    void MainWindow::mostrarVistaInventario() {
        vistas->setCurrentWidget(vistaInventario);
    }

    void MainWindow::mostrarVistaClientes() {
        vistas->setCurrentWidget(vistaClientes);
    }

    void MainWindow::mostrarVistaProveedores() {
        vistas->setCurrentWidget(vistaProveedores);
    }

    void MainWindow::mostrarVistaEstadisticas() {
        vistas->setCurrentWidget(vistaEstadisticas);
    }

    void MainWindow::mostrarVistaMenuPrincipal() {
        actualizarBarraEstado();
        vistas->setCurrentIndex(0);
    }

    void MainWindow::actualizarBarraEstado() {
        QString mensaje = QString("Medicamentos: %1 | Clientes: %2").arg(sistema->getCantMedicamentos())
            .arg(sistema->getCantClientes());
        this->statusBar()->showMessage(mensaje);
    }

    MainWindow::~MainWindow() {
        // Qt se encarga de borrar los widgets hijos, no necesitamos hacer delete a los punteros de las vistas
    }

    void MainWindow::closeEvent(QCloseEvent* event) {

        sistema->guardarDatos();
        event->accept();
    }
}