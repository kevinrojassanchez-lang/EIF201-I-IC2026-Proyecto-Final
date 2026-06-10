#include "MainWindow.h"
#include "RecursosUI.h"  
#include <QVBoxLayout>
#include <QApplication>
#include <QCloseEvent>
#include <QTime>       
#include <QStatusBar>  
#include <QTimer>      
#include <QDir>        // Necesario para las rutas locales relativas de los fotogramas
#include "CargadorDatosUI.h" // Para la conexion explicita de la carga asincrona del fondo animado

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

        // Inicializar el contador de fotogramas en cero
        fotogramaActual = 0;

        // CONFIGURAR EL TEMPORIZADOR DE ANIMACION NATIVO 
        timerAnimacion = new QTimer(widget);
        connect(timerAnimacion, &QTimer::timeout, this, &MainWindow::actualizarFotogramaFondo);
        timerAnimacion->start(33);

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
        // =========================================================================

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

        // =========================================================================
        // ACOMODO VERTICAL: Cargar -> Guardar -> Salir en la esquina derecha
        // =========================================================================
        QVBoxLayout* columnaSistemaDerecha = new QVBoxLayout();
        columnaSistemaDerecha->setSpacing(10);
        columnaSistemaDerecha->addWidget(botonCargar);
        columnaSistemaDerecha->addWidget(botonGuardar);
        columnaSistemaDerecha->addWidget(botonSalir);

        filaInferiorLayout->addLayout(columnaSistemaDerecha);
        // =========================================================================

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

            // Limpiamos la consola para no amontonar texto y refrescamos los contadores reales
            consolaDebug->clear();
            imprimirLogsIniciales();
        });

        actualizarFotogramaFondo();

        return widget;
    }

    void MainWindow::actualizarFotogramaFondo() {
        if (labelFondo == nullptr) return;

        // Fijo a la izquierda con un ancho personalizado de 400px
        labelFondo->setGeometry(100, 100, 400, 400);

        EIF201::CargadorDatosUI::plasmarFotogramaFondo(labelFondo, fotogramaActual, 240);
        fotogramaActual++;
    }

    void MainWindow::logConsola(const QString& mensaje, const QString& color) {
        consolaDebug->append(QString("<span style='color: %1;'>[%2] %3</span>").arg(color)
            .arg(QTime::currentTime().toString("hh:mm:ss")).arg(mensaje));
    }

    void MainWindow::imprimirLogsIniciales() {
        logConsola("Iniciando FarmaSystem ...", "#00FFFF");
        logConsola("Analizando consistencia de archivos...", "#888888");

        logConsola(QString("Proveedores cargados: %1 [Omitidos: %2]")
            .arg(sistema->getCantProveedoresAceptados())
            .arg(sistema->getCantProveedoresDescartados()), "#00FF00");

        logConsola(QString("Medicamentos cargados: %1 [Omitidos: %2]")
            .arg(sistema->getCantMedicamentosAceptados())
            .arg(sistema->getCantMedicamentosDescartados()), "#00FF00");

        logConsola(QString("Clientes cargados: %1 [Omitidos: %2]")
            .arg(sistema->getCantClientesAceptados())
            .arg(sistema->getCantClientesDescartados()), "#00FF00");

        logConsola(QString("Ventas cargadas: %1 [Omitidas: %2]")
            .arg(sistema->getCantVentasAceptadas())
            .arg(sistema->getCantVentasDescartadas()), "#00FF00");

        logConsola("Sistema Farmacia listo.", "#00FFFF");
    }

    void MainWindow::mostrarVistaVentas() {
        // Pausar el temporizador del menu al salir de el para liberar memoria RAM
        if (timerAnimacion) timerAnimacion->stop();

        vistaVentas->actualizarVista();
        vistas->setCurrentWidget(vistaVentas);
    }

    void MainWindow::mostrarVistaCategorias() {
        if (timerAnimacion) timerAnimacion->stop();

        vistaCategorias->actualizarVista();
        vistas->setCurrentWidget(vistaCategorias);
    }

    void MainWindow::mostrarVistaInventario() {
        if (timerAnimacion) timerAnimacion->stop();
        vistas->setCurrentWidget(vistaInventario);
    }

    void MainWindow::mostrarVistaClientes() {
        if (timerAnimacion) timerAnimacion->stop();
        vistas->setCurrentWidget(vistaClientes);
    }

    void MainWindow::mostrarVistaProveedores() {
        if (timerAnimacion) timerAnimacion->stop();
        vistas->setCurrentWidget(vistaProveedores);
    }

    void MainWindow::mostrarVistaEstadisticas() {
        if (timerAnimacion) timerAnimacion->stop();
        vistas->setCurrentWidget(vistaEstadisticas);
    }

    void MainWindow::mostrarVistaMenuPrincipal() {
        actualizarBarraEstado();
        vistas->setCurrentIndex(0);

        // Reencender el temporizador de forma estable a 33ms al volver al menu
        if (timerAnimacion) {
            timerAnimacion->start(33);
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
        QMainWindow::resizeEvent(event); // Mantener comportamiento nativo

        if (labelFondo && vistas) {
            labelFondo->setGeometry(100, 100, 400, 400);

            EIF201::CargadorDatosUI::plasmarFotogramaFondo(labelFondo, fotogramaActual, 240);
        }
    }

    MainWindow::~MainWindow() {
        // Qt gestiona la memoria de los elementos hijos automaticamente
    }

} // namespace FarmaSystem