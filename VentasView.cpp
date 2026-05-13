#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include "RecursosUI.h"

#include "SistemaFarmacia.h"
#include "VentasView.h"

namespace FarmaSystem {

    VentasView::VentasView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent), sistema(sSistema) {

        construirUI();
    }

    void VentasView::construirUI()
    {
        RecursosUI ui;

        QGridLayout* layout = new QGridLayout(this);
        ui.aplicarEstiloVentana(this);

        layout->setContentsMargins(30, 20, 30, 30);
        layout->setSpacing(15);

        // Titulo
        QLabel* titulo = new QLabel("Modulo de Ventas");
        titulo->setAlignment(Qt::AlignCenter);
        ui.aplicarTituloNeon(titulo);

		// Controles de seleccion y entrada
        comboClientesVenta = new QComboBox(this);
        comboMedicamentosVenta = new QComboBox(this);

        spinCantidadVenta = new QSpinBox(this);
        spinCantidadVenta->setRange(0, 1000);
        spinCantidadVenta->setValue(0);
        spinCantidadVenta->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);

       

        ui.aplicarBordePulsante(comboClientesVenta);
        ui.aplicarBordePulsante(comboMedicamentosVenta);
        ui.aplicarBordePulsante(spinCantidadVenta);

		// Labels de subtotal y total con efecto neon
        lblSubtotalVenta = new QLabel("Subtotal: CRC 0.00");
        lblTotalVenta = new QLabel("Total: CRC 0.00");

        lblSubtotalVenta->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        lblTotalVenta->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        ui.aplicarLabelSubtotal(lblSubtotalVenta);
        ui.aplicarLabelTotal(lblTotalVenta);
        ui.aplicarLabelNeon(lblSubtotalVenta, "#39FF14", 20);
        ui.aplicarLabelNeon(lblTotalVenta, "#F9FF00", 20);

		// Otros controles
        checkRecetaVenta = new QCheckBox("El cliente presento receta medica");

        ui.aplicarCheckboxReceta(checkRecetaVenta);

        QPushButton* botonVender = new QPushButton("Procesar Venta");
        QPushButton* botonMenu = new QPushButton("Volver");

        ui.aplicarEstiloBoton(botonVender);
        ui.aplicarEstiloBoton(botonMenu);

        tablaVentas = new QTableWidget(this);
        tablaVentas->setFixedHeight(100);
        tablaVentas->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaVentas->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablaVentas->setSelectionMode(QAbstractItemView::SingleSelection);
        tablaVentas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tablaVentas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        ui.aplicarEstiloTabla(tablaVentas);

		// Labels para los combos
        QLabel* lblCliente = new QLabel("Cliente:");
        QLabel* lblMedicamento = new QLabel("Medicamento:");
        QLabel* lblCantidad = new QLabel("Cantidad:");

        ui.aplicarLabelInfo(lblCliente);
        ui.aplicarLabelInfo(lblMedicamento);
        ui.aplicarLabelInfo(lblCantidad);

        // Lyout
        layout->addWidget(titulo, 0, 0, 1, 2);
        layout->addWidget(lblCliente, 1, 0, Qt::AlignRight);
        layout->addWidget(comboClientesVenta, 1, 1);
        layout->addWidget(lblMedicamento, 2, 0, Qt::AlignRight);
        layout->addWidget(comboMedicamentosVenta, 2, 1);
        layout->addWidget(lblCantidad, 3, 0, Qt::AlignRight);
        layout->addWidget(spinCantidadVenta, 3, 1);
        layout->addWidget(checkRecetaVenta, 4, 1);
        layout->addWidget(lblSubtotalVenta, 5, 0);
        layout->addWidget(lblTotalVenta, 5, 1);
        layout->addWidget(botonVender, 6, 0, 1, 2);
        layout->addWidget(tablaVentas, 7, 0, 1, 2);
        layout->addWidget(botonMenu, 8, 0, 1, 2);

		// Conexiones
        connect(comboClientesVenta, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VentasView::actualizarVistaPrecioVenta);

        connect(comboMedicamentosVenta, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VentasView::actualizarVistaPrecioVenta);

        connect(spinCantidadVenta, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &VentasView::actualizarVistaPrecioVenta);

        connect(checkRecetaVenta, &QCheckBox::checkStateChanged,
            this, &VentasView::actualizarVistaPrecioVenta);

        connect(botonVender, &QPushButton::clicked,
            this, &VentasView::procesarVentaUI);

        connect(botonMenu, &QPushButton::clicked,
            this, &VentasView::volverAlMenu);

		// conectamos el cambio de texto del QLineEdit dentro del QSpinBox para manejar cambios manuales
        if (QLineEdit* editorCantidad = spinCantidadVenta->findChild<QLineEdit*>()) {
            connect(editorCantidad, &QLineEdit::textChanged,
                this, &VentasView::manejarCambioTextoCantidad);
        }

        resetEstadoReceta();
    }

    void VentasView::actualizarVista() {
        cargarCombosVentas();
        actualizarTablaVentas();
    }

    void VentasView::actualizarTablaVentas() {
        tablaVentas->setRowCount(0);
        QStringList titulos = { "ID Venta", "Fecha", "Cliente", "Medicamento", "Cantidad", "Total" };
        tablaVentas->setColumnCount(titulos.size());
        tablaVentas->setHorizontalHeaderLabels(titulos);
        tablaVentas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        for (int i = 0; i < sistema->getCantVentas(); i++) {
            Venta* venta = sistema->getVentaPorIndice(i);

            if (venta != nullptr) {

                int fila = tablaVentas->rowCount();
                tablaVentas->insertRow(fila);

                tablaVentas->setItem(fila, 0, new QTableWidgetItem(QString::number(venta->getId())));
                tablaVentas->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(venta->getFecha())));
                tablaVentas->setItem(fila, 2, new QTableWidgetItem(QString::number(venta->getIdCliente())));
                tablaVentas->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(venta->getNombreMedicamento())));
                tablaVentas->setItem(fila, 4, new QTableWidgetItem(QString::number(venta->getCantidad())));
                tablaVentas->setItem(fila, 5, new QTableWidgetItem("₡" + QString::number(venta->getPrecioFinal(), 'f', 2)));
            }
        }
    }

    void VentasView::cargarCombosVentas() {
        comboClientesVenta->blockSignals(true);
        comboMedicamentosVenta->blockSignals(true);

        comboClientesVenta->clear();
        comboMedicamentosVenta->clear();

        for (int i = 0; i < sistema->getCantClientes(); i++) {
            Cliente* cliente = sistema->getClientePorIndice(i);
            if (cliente != nullptr) {
                comboClientesVenta->addItem(QString::fromStdString(cliente->getCedula() + " - " + cliente->getNombre()), cliente->getID());
            }
        }

        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {
            Medicamento* medicamento = sistema->getMedicamentoPorIndice(i);
            if (medicamento != nullptr) {
                comboMedicamentosVenta->addItem(QString::fromStdString(medicamento->getNombre()), medicamento->getID());
            }
        }

        // nada este seleccionado al inicio
        comboClientesVenta->setCurrentIndex(-1);
        comboMedicamentosVenta->setCurrentIndex(-1);
        comboClientesVenta->blockSignals(false);
        comboMedicamentosVenta->blockSignals(false);

        // limpiamos los labels de precio 
        lblSubtotalVenta->setText("Subtotal: CRC 0.00");
        lblTotalVenta->setText("Total: CRC 0.00");
    }

    void VentasView::actualizarVistaPrecioVenta() {
 
        if (comboClientesVenta->currentIndex() != -1 && comboMedicamentosVenta->currentIndex() != -1) {

            Cliente* cliente = sistema->buscarClientePorID(comboClientesVenta->currentData().toInt());
            Medicamento* medicamento = sistema->buscarMedicamentoPorID(comboMedicamentosVenta->currentData().toInt());

            if (cliente != nullptr && medicamento != nullptr) {

                int cantidadActual = spinCantidadVenta->value();
                double subtotal = medicamento->calcularPrecioFinal(cantidadActual);
                double total = sistema->calcularTotalVenta(cliente, medicamento, cantidadActual);

                lblSubtotalVenta->setText("Subtotal: CRC " + QString::number(subtotal, 'f', 2));
                lblTotalVenta->setText("Total: CRC " + QString::number(total, 'f', 2));

                // Logica de receta ,solo se habilita si el medicamento la requiere
                checkRecetaVenta->setEnabled(medicamento->getReceta());
                if (medicamento->getReceta() == false) {
                    checkRecetaVenta->setChecked(false);
                }
            }
        }
        else {

            lblSubtotalVenta->setText("Subtotal: CRC 0.00");
            lblTotalVenta->setText("Total: CRC 0.00");
			resetEstadoReceta();
        }
    }

    void VentasView::procesarVentaUI() {
        if (comboClientesVenta->currentIndex() == -1 || comboMedicamentosVenta->currentIndex() == -1) {
            QMessageBox::warning(this, "FarmaSystem", "Seleccione cliente y medicamento.");
            return;
        }

        Cliente* cliente = sistema->buscarClientePorID(comboClientesVenta->currentData().toInt());
        Medicamento* medicamento = sistema->buscarMedicamentoPorID(comboMedicamentosVenta->currentData().toInt());

        if (!cliente || !medicamento) return;

        QString fechaHoy = QDate::currentDate().toString("dd/MM/yyyy");
        int resultado = sistema->registrarVenta(cliente->getCedula(), medicamento->getID(), spinCantidadVenta->value(),
            checkRecetaVenta->isChecked(), fechaHoy.toStdString());

        if (resultado == 0) {
            QMessageBox::information(this, "FarmaSystem", "Venta registrada.");

            spinCantidadVenta->setValue(0);
            actualizarVista();
            emit datosActualizados();
            resetEstadoReceta();
        }
        else {
            QString mensajeSistema;
            switch (resultado) {
            case 1: mensajeSistema = "Cliente no existe."; break;
            case 2: mensajeSistema = "Medicamento no existe."; break;
            case 3: mensajeSistema = "Cantidad invalida."; break;
            case 4: mensajeSistema = "Fecha vacia."; break;
            case 5: mensajeSistema = "Requiere receta."; break;
            case 6: mensajeSistema = "Stock insuficiente."; break;
            case 7: mensajeSistema = "Excede dosis."; break;
            default: mensajeSistema = "Error desconocido."; break;
            }
            QMessageBox::critical(this, "FarmaSystem", mensajeSistema);
        }
    }

    void VentasView::manejarCambioTextoCantidad(const QString& texto) {
        if (texto.isEmpty()) {
    
            lblSubtotalVenta->setText("Subtotal: CRC 0.00");
            lblTotalVenta->setText("Total: CRC 0.00");
        }
        else {
            actualizarVistaPrecioVenta();
        }
    }

    void VentasView::resetEstadoReceta()
    {
        checkRecetaVenta->setChecked(false);
        checkRecetaVenta->setEnabled(false);
    }

}