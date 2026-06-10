#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QLineEdit>
#include "cargadorDatosUI.h"
#include "VentasView.h"
#include "SistemaFarmacia.h"
#include "RecursosUI.h"

namespace FarmaSystem {

    VentasView::VentasView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent), sistema(sSistema) {

        construirUI();
        actualizarVista();
    }

    void VentasView::construirUI() {

        RecursosUI ui;
        ui.aplicarEstiloVentana(this);

        // Layout principal vertical de la ventana
        QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);

        QLabel* titulo = new QLabel("Modulo de Ventas");
        titulo->setAlignment(Qt::AlignCenter);
        ui.aplicarTituloNeon(titulo);
        layoutPrincipal->addWidget(titulo); // El titulo va directo arriba

        // Cuadricula para el formulario
        QGridLayout* layoutFormulario = new QGridLayout();

        // Componentes creados y estilizados en una sola linea mediante RecursosUI
        comboClientesVenta = ui.crearCombo();
        comboMedicamentosVenta = ui.crearCombo();

        spinCantidadVenta = ui.crearSpin(0, 1000, "");
        spinCantidadVenta->installEventFilter(this);

        checkRecetaVenta = ui.crearCheck("Presento Receta Medica");

        QLabel* lblTxtCliente = new QLabel("Cliente:");
        QLabel* lblTxtMedicamento = new QLabel("Medicamento:");
        QLabel* lblTxtCantidad = new QLabel("Cantidad:");
        ui.aplicarLabelInfo(lblTxtCliente);
        ui.aplicarLabelInfo(lblTxtMedicamento);
        ui.aplicarLabelInfo(lblTxtCantidad);

        layoutFormulario->addWidget(lblTxtCliente, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
        layoutFormulario->addWidget(comboClientesVenta, 0, 1);
        layoutFormulario->addWidget(lblTxtMedicamento, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
        layoutFormulario->addWidget(comboMedicamentosVenta, 1, 1);
        layoutFormulario->addWidget(lblTxtCantidad, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
        layoutFormulario->addWidget(spinCantidadVenta, 2, 1);
        layoutFormulario->addWidget(checkRecetaVenta, 3, 1);

        QHBoxLayout* layoutPrecios = new QHBoxLayout();
        layoutPrecios->setContentsMargins(0, 5, 0, 5);
        layoutPrecios->setSpacing(20);

        lblSubtotalVenta = new QLabel("Subtotal: CRC 0.00");
        lblTotalVenta = new QLabel("Total: CRC 0.00");
        lblSubtotalVenta->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        lblTotalVenta->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        ui.aplicarLabelSubtotal(lblSubtotalVenta);
        ui.aplicarLabelTotal(lblTotalVenta);

        layoutPrecios->addWidget(lblSubtotalVenta);
        layoutPrecios->addWidget(lblTotalVenta);

        QPushButton* btnVender = new QPushButton("Procesar Venta");
        btnVender->setFixedWidth(285);
        ui.aplicarEstiloBoton(btnVender);

        QHBoxLayout* layoutFiltro = new QHBoxLayout();
        comboFiltroClientes = ui.crearCombo();
        comboFiltroClientes->setFixedWidth(280);

        lblTotalFiltro = new QLabel("Total Ventas: CRC 0.00");
        lblTotalFiltro->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.aplicarLabelGanancias(lblTotalFiltro);

        QLabel* lblTxtFiltro = new QLabel("Ventas por Cliente:");
        ui.aplicarLabelInfo(lblTxtFiltro);

        layoutFiltro->addWidget(lblTxtFiltro);
        layoutFiltro->addWidget(comboFiltroClientes, 0, Qt::AlignLeft);
        layoutFiltro->addStretch();

        // Inicializacion y configuracion limpia de la tabla
        tablaVentas = new QTableWidget(this);
        tablaVentas->setColumnCount(6);
        tablaVentas->setHorizontalHeaderLabels({ "ID", "Fecha", "IdCliente", "Medicamento", "Cantidad", "Total" });
        tablaVentas->verticalHeader()->setVisible(false);
        tablaVentas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tablaVentas->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaVentas->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaVentas->setSelectionMode(QAbstractItemView::SingleSelection);
        tablaVentas->setFocusPolicy(Qt::StrongFocus);
        tablaVentas->setTabKeyNavigation(false);
        tablaVentas->setShowGrid(true);
        tablaVentas->installEventFilter(this);
        ui.aplicarEstiloTabla(tablaVentas);

        QPushButton* botonVolver = new QPushButton("Volver");
        botonVolver->setFixedWidth(285);
        ui.aplicarEstiloBoton(botonVolver);

        QHBoxLayout* layoutInferior = new QHBoxLayout();
        layoutInferior->addWidget(lblTotalFiltro); // Se clava a la izquierda
        layoutInferior->addStretch();              // Resorte que empuja los componentes a los lados
        layoutInferior->addWidget(botonVolver);    // Se clava a la derecha

        layoutPrincipal->addLayout(layoutFormulario);
        layoutPrincipal->addLayout(layoutPrecios, Qt::AlignRight);
        layoutPrincipal->addWidget(btnVender, 0, Qt::AlignRight);
        layoutPrincipal->addSpacing(10);
        layoutPrincipal->addLayout(layoutFiltro);
        layoutPrincipal->addWidget(tablaVentas);
        layoutPrincipal->addLayout(layoutInferior); // Añadimos la barra con el total y el boton volver

        // Conexiones de eventos
        connect(btnVender, &QPushButton::clicked, this, &VentasView::procesarVentaUI);
        connect(botonVolver, &QPushButton::clicked, this, &VentasView::volverAlMenu);
        connect(comboClientesVenta, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(comboMedicamentosVenta, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(spinCantidadVenta, QOverload<int>::of(&QSpinBox::valueChanged), this, &VentasView::actualizarVistaPrecioVenta);

        QLineEdit* spinEdit = spinCantidadVenta->findChild<QLineEdit*>();
        if (spinEdit) {
            connect(spinEdit, &QLineEdit::textChanged, this, &VentasView::actualizarVistaPrecioVenta);
            spinEdit->installEventFilter(this);
        }
        connect(comboFiltroClientes, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarFiltroVentas);
    }

    void VentasView::actualizarVista() {

        cargarCombosVentas();
        limpiarFormularioVenta();
        actualizarTablaVentas();
        actualizarFiltroVentas();
    }

    void VentasView::cargarCombosVentas() {

        // Listas paralelas de clientes
        QStringList nombresClientes;
        QList<int> idsClientes;

        for (int i = 0; i < sistema->getCantClientes(); i++) {
            Cliente* c = sistema->getClientePorIndice(i);
            if (c != nullptr) {
                nombresClientes.append(QString::fromStdString(c->getNombre()));
                idsClientes.append(c->getID());
            }
        }

        // Listas paralelas de medicamentos
        QStringList nombresMedicamentos;
        QList<int> idsMedicamentos;

        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {
            Medicamento* m = sistema->getMedicamentoPorIndice(i);
            if (m != nullptr) {
                nombresMedicamentos.append(QString::fromStdString(m->getNombre()));
                idsMedicamentos.append(m->getID());
            }
        }

        // Invocacin explicita y ultra segura con la etiqueta anidada EIF201
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(comboMedicamentosVenta, nombresMedicamentos, idsMedicamentos, "Ninguno...");
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(comboClientesVenta, nombresClientes, idsClientes, "Ninguno...");
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(comboFiltroClientes, nombresClientes, idsClientes, "");

        comboFiltroClientes->insertItem(0, "Todos", -1);
        comboFiltroClientes->setCurrentIndex(0);
    }

    void VentasView::actualizarTablaVentas() {

        tablaVentas->setRowCount(0);

        for (int i = 0; i < sistema->getCantVentas(); i++) {

            Venta* venta = sistema->getVentaPorIndice(i);

            if (venta != nullptr) {

                int fila = tablaVentas->rowCount();

                tablaVentas->insertRow(fila);
                tablaVentas->setItem(fila, 0, new QTableWidgetItem(QString::number(venta->getId())));
                tablaVentas->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(venta->getFecha())));
                tablaVentas->setItem(fila, 2, new QTableWidgetItem(QString::number(venta->getIdCliente())));
                tablaVentas->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(venta->getMedicamentoVendido()->getNombre())));
                tablaVentas->setItem(fila, 4, new QTableWidgetItem(QString::number(venta->getCantidad())));
                tablaVentas->setItem(fila, 5, new QTableWidgetItem("CRC " + QString::number(venta->getPrecioFinal(), 'f', 2)));
            }
        }
    }

    void VentasView::actualizarVistaPrecioVenta() {

        // Si el ID de los datos actuales es 0 o menor, es "Ninguno"
        int idCliente = comboClientesVenta->currentData().toInt();
        int idMedicamento = comboMedicamentosVenta->currentData().toInt();

        if (idCliente <= 0 || idMedicamento <= 0) {
            lblSubtotalVenta->setText("Subtotal: CRC 0.00");
            lblTotalVenta->setText("Total: CRC 0.00");
            return; // Sale pacificamente sin calcular precios
        }

        QLineEdit* spinEdit = spinCantidadVenta->findChild<QLineEdit*>();
        QString textoEnPantalla = (spinEdit) ? spinEdit->text().trimmed() : "";

        if (textoEnPantalla.isEmpty() || spinCantidadVenta->value() <= 0) {
            lblSubtotalVenta->setText("Subtotal: CRC 0.00");
            lblTotalVenta->setText("Total: CRC 0.00");
            return;
        }

        Cliente* cliente = sistema->buscarClientePorID(idCliente);
        Medicamento* medicamento = sistema->buscarMedicamentoPorID(idMedicamento);

        if (cliente != nullptr && medicamento != nullptr) {
            int cantidad = spinCantidadVenta->value();
            double subtotal = medicamento->calcularPrecioFinal(cantidad);
            double total = sistema->calcularTotalVenta(cliente, medicamento, cantidad);

            lblSubtotalVenta->setText("Subtotal: CRC " + QString::number(subtotal, 'f', 2));
            lblTotalVenta->setText("Total: CRC " + QString::number(total, 'f', 2));
        }
    }

    void VentasView::procesarVentaUI() {

        Cliente* cliente = sistema->buscarClientePorID(comboClientesVenta->currentData().toInt());
        Medicamento* medicamento = sistema->buscarMedicamentoPorID(comboMedicamentosVenta->currentData().toInt());

        if (cliente == nullptr || medicamento == nullptr) {
            QMessageBox::warning(this, "FarmaSystem", "Seleccione cliente y medicamento.");
            return;
        }

        QString fecha = QDate::currentDate().toString("dd/MM/yyyy");

        // Evaluamos si la respuesta del usuario es estrictamente QMessageBox::Yes
        bool confirmado = false;

        if ((QMessageBox::question(this, "FarmaSystem", "Esta seguro de que desea procesar esta venta?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)) {

			confirmado = true;
        }

        // Pasamos confirmado directamente como parametro a la controladora
        int resultado = sistema->registrarVenta(cliente->getCedula(), medicamento->getID(), spinCantidadVenta->value(),
            checkRecetaVenta->isChecked(), fecha.toStdString(), confirmado); 
        

        if (resultado == 0) {
            QMessageBox::information(this, "FarmaSystem", "Venta registrada correctamente.");
            actualizarVista();
            emit datosActualizados();
        }
        else {
            QString mensaje = "Error al registrar venta.";

            if (resultado == 1) { mensaje = "Cliente no existe."; }
            else if (resultado == 2) { mensaje = "Medicamento no existe."; }
            else if (resultado == 3) { mensaje = "Cantidad invalida."; }
            else if (resultado == 4) { mensaje = "Fecha invalida."; }
            else if (resultado == 5) { mensaje = "El medicamento requiere receta."; }
            else if (resultado == 6) { mensaje = "Stock insuficiente."; }
            else if (resultado == 7) { mensaje = "La cantidad excede la dosis permitida."; }
            else if (resultado == 8) { mensaje = "Venta cancelada."; }

            QMessageBox::warning(this, "FarmaSystem", mensaje);
        }
    }

    void VentasView::actualizarFiltroVentas() {

        int idCliente = comboFiltroClientes->currentData().toInt();
        double total = 0;

        tablaVentas->setRowCount(0);

        for (int i = 0; i < sistema->getCantVentas(); i++) {

            Venta* venta = sistema->getVentaPorIndice(i);

            if (venta != nullptr) {

                bool mostrar = false;

                if (idCliente == -1) { mostrar = true; }
                else if (venta->getIdCliente() == idCliente) { mostrar = true; }

                if (mostrar) {

                    total += venta->getPrecioFinal();

                    int fila = tablaVentas->rowCount();

                    tablaVentas->insertRow(fila);
                    tablaVentas->setItem(fila, 0, new QTableWidgetItem(QString::number(venta->getId())));
                    tablaVentas->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(venta->getFecha())));
                    tablaVentas->setItem(fila, 2, new QTableWidgetItem(QString::number(venta->getIdCliente())));
                    tablaVentas->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(venta->getMedicamentoVendido()->getNombre())));
                    tablaVentas->setItem(fila, 4, new QTableWidgetItem(QString::number(venta->getCantidad())));
                    tablaVentas->setItem(fila, 5, new QTableWidgetItem("CRC " + QString::number(venta->getPrecioFinal(), 'f', 2)));
                }
            }
        }
        lblTotalFiltro->setText("Total Ventas: CRC " + QString::number(total, 'f', 2));
    }

    void VentasView::manejarCambioTextoCantidad(const QString&) { actualizarVistaPrecioVenta(); }

    void VentasView::resetEstadoReceta() {

        checkRecetaVenta->setChecked(false);
        checkRecetaVenta->setEnabled(false);
    }

    bool VentasView::eventFilter(QObject* obj, QEvent* event) {

        RecursosUI ui;
        QLineEdit* spinEdit = spinCantidadVenta->findChild<QLineEdit*>();

        if ((obj == spinCantidadVenta || obj == spinEdit) && event->type() == QEvent::FocusOut) {
            if (spinEdit && spinEdit->text().trimmed().isEmpty()) {
                spinCantidadVenta->blockSignals(true);
                spinCantidadVenta->setValue(0);
                spinEdit->setText("0");
                spinCantidadVenta->blockSignals(false);
                actualizarVistaPrecioVenta();
            }
        }

        if (obj == tablaVentas) {

            if (event->type() == QEvent::FocusIn) {
                ui.cambiarColorGrid(tablaVentas, true);
            }
            else if (event->type() == QEvent::FocusOut) {
                tablaVentas->clearSelection();
                tablaVentas->setCurrentCell(-1, -1);
                ui.cambiarColorGrid(tablaVentas, false);
            }
        }
        return QWidget::eventFilter(obj, event);
    }

    void VentasView::limpiarFormularioVenta() {

        comboClientesVenta->setCurrentIndex(0);
        comboMedicamentosVenta->setCurrentIndex(0);

        checkRecetaVenta->setChecked(false);
        spinCantidadVenta->setValue(0);
        lblSubtotalVenta->setText("Subtotal: CRC 0.00");
        lblTotalVenta->setText("Total: CRC 0.00");
    }
} // namespace FarmaSystem