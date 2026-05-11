#include "VentasView.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>


#include "SistemaFarmacia.h"

namespace FarmaSystem {

    VentasView::VentasView(SistemaFarmacia* s, QWidget* parent) : QWidget(parent), sistema(s) {

        setupUI();
    }

    void VentasView::setupUI() {
        QGridLayout* layout = new QGridLayout(this); // Cambiamos a Grid para mejor orden
        this->setStyleSheet("background-color: #2b2b2b; color: #e0e0e0;");
        layout->setContentsMargins(30, 20, 30, 30);
        layout->setSpacing(15);

        // Titulo
        QLabel* titulo = new QLabel("Modulo de Ventas");
        titulo->setStyleSheet("color: white; font-weight: bold; font-size: 24px;");

        comboClientesVenta = new QComboBox();
        comboMedicamentosVenta = new QComboBox();
        
        spinCantidadVenta = new QSpinBox();
        spinCantidadVenta->setRange(0, 1000);
        spinCantidadVenta->setValue(0);
        spinCantidadVenta->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        QLineEdit* editorCantidad = spinCantidadVenta->findChild<QLineEdit*>();

        // Estilo para combos y spinbox
        QString estiloWidgets = "QComboBox, QSpinBox { background-color: #1a1a1a; color: white; border: 2px solid #39FF14; border-radius: 5px; padding: 5px; }";
        comboClientesVenta->setStyleSheet(estiloWidgets);
        comboMedicamentosVenta->setStyleSheet(estiloWidgets);
        spinCantidadVenta->setStyleSheet(estiloWidgets);

        lblSubtotalVenta = new QLabel("Subtotal: CRC 0.00");
        lblTotalVenta = new QLabel("Total: CRC 0.00");
        lblTotalVenta->setStyleSheet("font-weight: bold; color: #39FF14; font-size: 16px;");

        checkRecetaVenta = new QCheckBox("El cliente presento receta medica");

        QPushButton* botonVender = new QPushButton("Procesar Venta");
        QPushButton* botonMenu = new QPushButton("Volver");

        // Estilo del boton
        QString estiloBoton = "QPushButton { background-color: #444; color: white; border-radius: 5px; padding: 10px; font-weight: bold; }"
            "QPushButton:hover { background-color: #39FF14; color: black; }";
        botonVender->setStyleSheet(estiloBoton);
        botonMenu->setStyleSheet(estiloBoton);

        tablaVentas = new QTableWidget();
        tablaVentas->setStyleSheet("QTableWidget { background-color: #1a1a1a; color: white; gridline-color: #39FF14; border: 1px solid #444; }"
            "QHeaderView::section { background-color: #333; color: white; padding: 5px; border: 1px solid #111; }");

        // Acomodo en el grid
        layout->addWidget(titulo, 0, 0, 1, 2);

        layout->addWidget(new QLabel("<b>Cliente:</b>"), 1, 0);
        layout->addWidget(comboClientesVenta, 1, 1);

        layout->addWidget(new QLabel("<b>Medicamento:</b>"), 2, 0);
        layout->addWidget(comboMedicamentosVenta, 2, 1);

        layout->addWidget(new QLabel("<b>Cantidad:</b>"), 3, 0);
        layout->addWidget(spinCantidadVenta, 3, 1);

        layout->addWidget(checkRecetaVenta, 4, 1);

        layout->addWidget(lblSubtotalVenta, 5, 0);
        layout->addWidget(lblTotalVenta, 5, 1);

        layout->addWidget(botonVender, 6, 0, 1, 2);
        layout->addWidget(tablaVentas, 7, 0, 1, 2);
        layout->addWidget(botonMenu, 8, 0, 1, 2);

        // Conexiones
        connect(comboClientesVenta, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(comboMedicamentosVenta, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(spinCantidadVenta, QOverload<int>::of(&QSpinBox::valueChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(checkRecetaVenta, &QCheckBox::checkStateChanged, this, &VentasView::actualizarVistaPrecioVenta);
        connect(botonVender, &QPushButton::clicked, this, &VentasView::procesarVentaUI);
        connect(botonMenu, &QPushButton::clicked, this, &VentasView::volverAlMenu);
       
        if (editorCantidad != nullptr) { 
        connect(editorCantidad, &QLineEdit::textChanged,this, &VentasView::manejarCambioTextoCantidad);
        }

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

                // Logica de receta (solo se habilita si el medicamento la requiere)
                checkRecetaVenta->setEnabled(medicamento->getReceta());
                if (medicamento->getReceta() == false) {
                    checkRecetaVenta->setChecked(false);
                }
            }
        }
        else {

            lblSubtotalVenta->setText("Subtotal: CRC 0.00");
            lblTotalVenta->setText("Total: CRC 0.00");
            checkRecetaVenta->setChecked(false);
            checkRecetaVenta->setEnabled(false);
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

}