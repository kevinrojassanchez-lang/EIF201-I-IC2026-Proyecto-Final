#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QLineEdit>

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

        QGridLayout* layout = new QGridLayout(this);

        QLabel* titulo = new QLabel("Modulo de Ventas");
        titulo->setAlignment(Qt::AlignCenter);
        ui.aplicarTituloNeon(titulo);

        comboClientesVenta = new QComboBox(this);
        comboMedicamentosVenta = new QComboBox(this);

        spinCantidadVenta = new QSpinBox(this);
        spinCantidadVenta->setRange(0, 1000);

        checkRecetaVenta = new QCheckBox("Receta medica");

        lblSubtotalVenta = new QLabel("Subtotal: CRC 0.00");
        lblTotalVenta = new QLabel("Total: CRC 0.00");
        lblTotalFiltro = new QLabel("Total filtro: CRC 0.00");

        comboFiltroClientes = new QComboBox(this);

        tablaVentas = new QTableWidget(this);

        tablaVentas->setColumnCount(5);
        tablaVentas->setHorizontalHeaderLabels({ "ID", "Fecha", "Cliente", "Cantidad", "Total" });
        tablaVentas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tablaVentas->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tablaVentas->setSelectionBehavior(QAbstractItemView::SelectRows);

        ui.aplicarEstiloTabla(tablaVentas);

        QPushButton* btnVender = new QPushButton("Procesar Venta");
        QPushButton* btnMenu = new QPushButton("Volver");

        ui.aplicarEstiloBoton(btnVender);
        ui.aplicarEstiloBoton(btnMenu);

        layout->addWidget(titulo, 0, 0, 1, 2);

        layout->addWidget(new QLabel("Cliente:"), 1, 0);
        layout->addWidget(comboClientesVenta, 1, 1);

        layout->addWidget(new QLabel("Medicamento:"), 2, 0);
        layout->addWidget(comboMedicamentosVenta, 2, 1);

        layout->addWidget(new QLabel("Cantidad:"), 3, 0);
        layout->addWidget(spinCantidadVenta, 3, 1);

        layout->addWidget(checkRecetaVenta, 4, 1);

        layout->addWidget(lblSubtotalVenta, 5, 0);
        layout->addWidget(lblTotalVenta, 5, 1);

        layout->addWidget(btnVender, 6, 0, 1, 2);

        layout->addWidget(new QLabel("Filtro de ventas por cliente:"), 7, 0);
        layout->addWidget(comboFiltroClientes, 7, 1);

        layout->addWidget(lblTotalFiltro, 8, 0, 1, 2);

        layout->addWidget(tablaVentas, 9, 0, 1, 2);

        layout->addWidget(btnMenu, 10, 0, 1, 2);

        connect(btnVender, &QPushButton::clicked, this, &VentasView::procesarVentaUI);
        connect(btnMenu, &QPushButton::clicked, this, &VentasView::volverAlMenu);
        connect(comboClientesVenta, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VentasView::actualizarVistaPrecioVenta);
        connect(comboMedicamentosVenta, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VentasView::actualizarVistaPrecioVenta);
        connect(spinCantidadVenta, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &VentasView::actualizarVistaPrecioVenta);
        connect(comboFiltroClientes, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VentasView::actualizarFiltroVentas);
    }

    void VentasView::actualizarVista() {

        cargarCombosVentas();
        actualizarTablaVentas();
        actualizarFiltroVentas();
    }

    void VentasView::cargarCombosVentas() {

        comboClientesVenta->clear();
        comboMedicamentosVenta->clear();
        comboFiltroClientes->clear();

        comboFiltroClientes->addItem("Todos", -1);

        for (int i = 0; i < sistema->getCantClientes(); i++) {

            Cliente* c = sistema->getClientePorIndice(i);

            if (c != nullptr) {

                comboClientesVenta->addItem(
                    QString::fromStdString(c->getNombre()),
                    c->getID());

                comboFiltroClientes->addItem(
                    QString::fromStdString(c->getNombre()),
                    c->getID());
            }
        }

        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {

            Medicamento* m = sistema->getMedicamentoPorIndice(i);

            if (m != nullptr) {

                comboMedicamentosVenta->addItem(
                    QString::fromStdString(m->getNombre()),
                    m->getID());
            }
        }
    }

    void VentasView::actualizarTablaVentas() {

        tablaVentas->setRowCount(0);

        for (int i = 0; i < sistema->getCantVentas(); i++) {

            Venta* v = sistema->getVentaPorIndice(i);

            if (v != nullptr) {

                int fila = tablaVentas->rowCount();

                tablaVentas->insertRow(fila);

                tablaVentas->setItem(fila, 0,
                    new QTableWidgetItem(QString::number(v->getId())));

                tablaVentas->setItem(fila, 1,
                    new QTableWidgetItem(QString::fromStdString(v->getFecha())));

                tablaVentas->setItem(fila, 2,
                    new QTableWidgetItem(QString::number(v->getIdCliente())));

                tablaVentas->setItem(fila, 3,
                    new QTableWidgetItem(QString::number(v->getCantidad())));

                tablaVentas->setItem(fila, 4,
                    new QTableWidgetItem("CRC " + QString::number(v->getPrecioFinal(), 'f', 2)));
            }
        }
    }

    void VentasView::actualizarVistaPrecioVenta() {

        Cliente* c = sistema->buscarClientePorID(
            comboClientesVenta->currentData().toInt());

        Medicamento* m = sistema->buscarMedicamentoPorID(
            comboMedicamentosVenta->currentData().toInt());

        if (c != nullptr && m != nullptr) {

            int cantidad = spinCantidadVenta->value();

            double subtotal = m->calcularPrecioFinal(cantidad);

            double total = sistema->calcularTotalVenta(c, m, cantidad);

            lblSubtotalVenta->setText(
                "Subtotal: CRC " + QString::number(subtotal, 'f', 2));

            lblTotalVenta->setText(
                "Total: CRC " + QString::number(total, 'f', 2));
        }
    }

    void VentasView::procesarVentaUI() {

        Cliente* c = sistema->buscarClientePorID(
            comboClientesVenta->currentData().toInt());

        Medicamento* m = sistema->buscarMedicamentoPorID(
            comboMedicamentosVenta->currentData().toInt());

        if (c == nullptr || m == nullptr) {

            QMessageBox::warning(
                this,
                "FarmaSystem",
                "Seleccione cliente y medicamento.");

            return;
        }

        QString fecha = QDate::currentDate().toString("dd/MM/yyyy");

        int resultado = sistema->registrarVenta(
            c->getCedula(),
            m->getID(),
            spinCantidadVenta->value(),
            checkRecetaVenta->isChecked(),
            fecha.toStdString()
        );

        if (resultado == 0) {

            QMessageBox::information(
                this,
                "FarmaSystem",
                "Venta registrada correctamente.");

            actualizarVista();

            emit datosActualizados();
        }
        else {

            QString mensaje = "Error al registrar venta.";

            if (resultado == 1) {
                mensaje = "Cliente no existe.";
            }
            else if (resultado == 2) {
                mensaje = "Medicamento no existe.";
            }
            else if (resultado == 3) {
                mensaje = "Cantidad invalida.";
            }
            else if (resultado == 4) {
                mensaje = "Fecha invalida.";
            }
            else if (resultado == 5) {
                mensaje = "El medicamento requiere receta.";
            }
            else if (resultado == 6) {
                mensaje = "Stock insuficiente.";
            }
            else if (resultado == 7) {
                mensaje = "La cantidad excede la dosis permitida.";
            }

            QMessageBox::warning(this, "FarmaSystem", mensaje);
        }
    }

    void VentasView::actualizarFiltroVentas() {

        int idCliente = comboFiltroClientes->currentData().toInt();

        double total = 0;

        tablaVentas->setRowCount(0);

        for (int i = 0; i < sistema->getCantVentas(); i++) {

            Venta* v = sistema->getVentaPorIndice(i);

            if (v != nullptr) {

                bool mostrar = false;

                if (idCliente == -1) {
                    mostrar = true;
                }
                else if (v->getIdCliente() == idCliente) {
                    mostrar = true;
                }

                if (mostrar) {

                    total += v->getPrecioFinal();

                    int fila = tablaVentas->rowCount();

                    tablaVentas->insertRow(fila);

                    tablaVentas->setItem(fila, 0,
                        new QTableWidgetItem(QString::number(v->getId())));

                    tablaVentas->setItem(fila, 1,
                        new QTableWidgetItem(QString::fromStdString(v->getFecha())));

                    tablaVentas->setItem(fila, 2,
                        new QTableWidgetItem(QString::number(v->getIdCliente())));

                    tablaVentas->setItem(fila, 3,
                        new QTableWidgetItem(QString::number(v->getCantidad())));

                    tablaVentas->setItem(fila, 4,
                        new QTableWidgetItem("CRC " + QString::number(v->getPrecioFinal(), 'f', 2)));
                }
            }
        }

        lblTotalFiltro->setText(
            "Total filtro: CRC " + QString::number(total, 'f', 2));
    }

    void VentasView::manejarCambioTextoCantidad(const QString&) {

        actualizarVistaPrecioVenta();
    }

    void VentasView::resetEstadoReceta() {

        checkRecetaVenta->setChecked(false);
        checkRecetaVenta->setEnabled(false);
    }

}