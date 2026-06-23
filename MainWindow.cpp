#include "MainWindow.h"
#include "RecursosUI.h"  
#include <QVBoxLayout>
#include <QApplication>
#include <QCloseEvent>
#include <QTime>       
#include <QStatusBar>  
#include <QTimer>      
#include <QDir>        

namespace FarmaSystem {

    MainWindow::MainWindow(SistemaFarmacia* sSistema, QWidget* parent) : QMainWindow(parent), sistema(sSistema)
    {
        // Aplicar el fondo oscuro general a la ventana principal
        RecursosUI ui;
        ui.aplicarEstiloVentana(this);

        consolaDebug = new QTextEdit();

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

        // Estadisticas                                               
        vistaEstadisticas = new EstadisticasView(sistema);
        vistas->addWidget(vistaEstadisticas);
        connect(vistaEstadisticas, &EstadisticasView::volverAlMenu, this, &MainWindow::mostrarVistaMenuPrincipal);

        QTimer::singleShot(50, this, [=]() { this->imprimirLogsIniciales(); });

        // Mostrar menu principal
        vistas->setCurrentIndex(0);
        actualizarBarraEstado();
    }

    QWidget* MainWindow::crearMenuPrincipal() {

        QWidget* widget = new QWidget;
        RecursosUI ui;

        labelFondo = new QLabel(widget);
        labelFondo->lower();

        animacionFondo = new QMovie("datos/fondo_sistema.gif", QByteArray(), this);
        labelFondo->setMovie(animacionFondo);
        animacionFondo->start();

        widget->setAttribute(Qt::WA_TranslucentBackground, false);

        // LAYOUT PRINCIPAL Mantiene la estructura de la ventana
        QVBoxLayout* layoutPrincipal = new QVBoxLayout(widget);

        QLabel* titulo = new QLabel("FarmaSystem");
        titulo->setAlignment(Qt::AlignCenter);
        ui.aplicarTituloNeon(titulo);

        QPushButton* botonInventario = new QPushButton("Inventario");
        QPushButton* botonClientes = new QPushButton("Clientes");
        QPushButton* botonProveedores = new QPushButton("Proveedores");
        QPushButton* botonCategorias = new QPushButton("Categorias");
        QPushButton* botonVentas = new QPushButton("Ventas");
        QPushButton* botonEstadisticas = new QPushButton("Estadisticas");
        QPushButton* botonSalir = new QPushButton("Salir");
        QPushButton* botonCargar = new QPushButton("Cargar Datos");
        QPushButton* botonGuardar = new QPushButton("Guardar Datos");

        botonCargar->setFixedWidth(180);
        botonGuardar->setFixedWidth(180);

        ui.aplicarEstiloBoton(botonCargar);
        ui.aplicarEstiloBoton(botonGuardar);

        int anchoBotonesMenu = 150;
        botonInventario->setFixedWidth(anchoBotonesMenu);
        botonClientes->setFixedWidth(anchoBotonesMenu);
        botonProveedores->setFixedWidth(anchoBotonesMenu);
        botonCategorias->setFixedWidth(anchoBotonesMenu);
        botonVentas->setFixedWidth(anchoBotonesMenu);
        botonEstadisticas->setFixedWidth(anchoBotonesMenu);

        ui.aplicarEstiloBotonMenuPrincipal(botonInventario);
        ui.aplicarEstiloBotonMenuPrincipal(botonClientes);
        ui.aplicarEstiloBotonMenuPrincipal(botonProveedores);
        ui.aplicarEstiloBotonMenuPrincipal(botonCategorias);
        ui.aplicarEstiloBotonMenuPrincipal(botonVentas);
        ui.aplicarEstiloBotonMenuPrincipal(botonEstadisticas);
        ui.aplicarEstiloBoton(botonSalir);

        consolaDebug->setReadOnly(true);
        consolaDebug->setParent(widget);
        consolaDebug->setMaximumHeight(110);
        consolaDebug->setFixedWidth(400);
        ui.aplicarEstiloTexto(consolaDebug);

        QHBoxLayout* contenedorGruposLayout = new QHBoxLayout();

        QVBoxLayout* columnaIzquierdaLayout = new QVBoxLayout();
        columnaIzquierdaLayout->setSpacing(15);
        columnaIzquierdaLayout->addWidget(botonCategorias);
        columnaIzquierdaLayout->addWidget(botonInventario);
        columnaIzquierdaLayout->addWidget(botonProveedores);

        QVBoxLayout* columnaDerechaLayout = new QVBoxLayout();
        columnaDerechaLayout->setSpacing(15);
        columnaDerechaLayout->addWidget(botonEstadisticas);
        columnaDerechaLayout->addWidget(botonVentas);
        columnaDerechaLayout->addWidget(botonClientes);

        contenedorGruposLayout->addLayout(columnaIzquierdaLayout);
        contenedorGruposLayout->addStretch();
        contenedorGruposLayout->addLayout(columnaDerechaLayout);

        layoutPrincipal->addWidget(titulo);
        layoutPrincipal->addStretch();
        layoutPrincipal->addLayout(contenedorGruposLayout);
        layoutPrincipal->addStretch();

        botonSalir->setFixedWidth(180);

        QHBoxLayout* filaInferiorLayout = new QHBoxLayout();

        QVBoxLayout* columnaTerminalLayout = new QVBoxLayout();
        QLabel* lblTerminalTxt = new QLabel("Consistencia de Datos:");
        ui.aplicarLabelInfo(lblTerminalTxt);

        columnaTerminalLayout->addWidget(lblTerminalTxt);
        columnaTerminalLayout->addWidget(consolaDebug);

        filaInferiorLayout->addLayout(columnaTerminalLayout, 0);
        filaInferiorLayout->addStretch();

        QVBoxLayout* columnaSistemaDerecha = new QVBoxLayout();
        columnaSistemaDerecha->setSpacing(10);
        columnaSistemaDerecha->addWidget(botonCargar);
        columnaSistemaDerecha->addWidget(botonGuardar);
        columnaSistemaDerecha->addWidget(botonSalir);

        filaInferiorLayout->addLayout(columnaSistemaDerecha);

        layoutPrincipal->addLayout(filaInferiorLayout);

        connect(botonInventario, &QPushButton::clicked, this, &MainWindow::mostrarVistaInventario);
        connect(botonClientes, &QPushButton::clicked, this, &MainWindow::mostrarVistaClientes);
        connect(botonProveedores, &QPushButton::clicked, this, &MainWindow::mostrarVistaProveedores);
        connect(botonCategorias, &QPushButton::clicked, this, &MainWindow::mostrarVistaCategorias);
        connect(botonVentas, &QPushButton::clicked, this, &MainWindow::mostrarVistaVentas);
        connect(botonEstadisticas, &QPushButton::clicked, this, &MainWindow::mostrarVistaEstadisticas);
        connect(botonSalir, &QPushButton::clicked, qApp, &QApplication::quit);

        connect(botonGuardar, &QPushButton::clicked, this, [&]() {

            sistema->guardarDatos();
            logConsola("Persistencia confirmada. Datos guardados exitosamente.", "#00FF00");
        });
        connect(botonCargar, &QPushButton::clicked, this, [&]() {

            sistema->cargarDatos();
            consolaDebug->clear();
            imprimirLogsIniciales();
        });

        actualizarFotogramaFondo();

        return widget;
    }

    void MainWindow::actualizarFotogramaFondo() {
        if (labelFondo == nullptr || animacionFondo == nullptr) return;

        // Fijo a la izquierda con un ancho personalizado de 400px
        labelFondo->setGeometry(100, 100, 400, 400);

        // ajuste de la escala de reproduccion del GIF al tamano del QLabel
        animacionFondo->setScaledSize(QSize(400, 400));
    }

    void MainWindow::logConsola(const QString& mensaje, const QString& color) {
        consolaDebug->append(QString("<span style='color: %1;'>[%2] %3</span>").arg(color)
            .arg(QTime::currentTime().toString("hh:mm:ss")).arg(mensaje));
    }
    void MainWindow::imprimirLogsIniciales() {
        logConsola("Iniciando FarmaSystem ...", "#00FFFF");

        logConsola("Analizando consistencia de archivos...", "#888888");

        logConsola(QString("Proveedores cargados: %1 [Omitidos: %2]") .arg(sistema->getCantProveedoresAceptados())
            .arg(sistema->getCantProveedoresDescartados()), "#00FF00");

        logConsola(QString("Medicamentos cargados: %1 [Omitidos: %2]").arg(sistema->getCantMedicamentosAceptados())
            .arg(sistema->getCantMedicamentosDescartados()), "#00FF00");

        logConsola(QString("Clientes cargados: %1 [Omitidos: %2]").arg(sistema->getCantClientesAceptados())
            .arg(sistema->getCantClientesDescartados()), "#00FF00");

        logConsola(QString("Ventas cargadas: %1 [Omitidas: %2]").arg(sistema->getCantVentasAceptadas())
            .arg(sistema->getCantVentasDescartadas()), "#00FF00");

        logConsola("Sistema Farmacia listo.", "#00FFFF");
    }

    void MainWindow::mostrarVistaVentas() {

        if (animacionFondo) animacionFondo->stop();

        vistaVentas->actualizarVista();
        vistas->setCurrentWidget(vistaVentas);
    }

    void MainWindow::mostrarVistaCategorias() {

        if (animacionFondo) animacionFondo->stop();

        vistaCategorias->actualizarVista();
        vistas->setCurrentWidget(vistaCategorias);
    }

    void MainWindow::mostrarVistaInventario() {

        if (animacionFondo) animacionFondo->stop();

        vistas->setCurrentWidget(vistaInventario);
    }

    void MainWindow::mostrarVistaClientes() {

        if (animacionFondo) animacionFondo->stop();

        vistas->setCurrentWidget(vistaClientes);
    }

    void MainWindow::mostrarVistaProveedores() {

        if (animacionFondo) animacionFondo->stop();

        vistas->setCurrentWidget(vistaProveedores);
    }

    void MainWindow::mostrarVistaEstadisticas() {
        if (animacionFondo) animacionFondo->stop();

        vistas->setCurrentWidget(vistaEstadisticas);
    }

    void MainWindow::mostrarVistaMenuPrincipal() {

        actualizarBarraEstado();
        vistas->setCurrentIndex(0);

        // reproduccion automatica al regresar al menu principal
        if (animacionFondo) {
            animacionFondo->start();
            actualizarFotogramaFondo();
        }
    }

    void MainWindow::actualizarBarraEstado() {

        QString mensaje = QString("Medicamentos: %1 | Clientes: %2").arg(sistema->getCantMedicamentos())
            .arg(sistema->getCantClientes());

        this->statusBar()->showMessage(mensaje);
    }

    void MainWindow::closeEvent(QCloseEvent* event) {

        sistema->guardarDatos();
        event->accept();
    }

    void MainWindow::resizeEvent(QResizeEvent* event) {

        QMainWindow::resizeEvent(event);

        if (labelFondo && animacionFondo) {
            labelFondo->setGeometry(100, 100, 400, 400);
            animacionFondo->setScaledSize(QSize(400, 400));
        }
    }

    MainWindow::~MainWindow() {
        // Al heredar de QMainWindow, Qt se encarga del ciclo de vida del GIF automaticamente
    }

} // namespace FarmaSystem