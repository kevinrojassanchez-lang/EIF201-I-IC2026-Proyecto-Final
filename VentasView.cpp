#include "VentasView.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QLineEdit>
#include "cargadorDatosUI.h"
#include "RecursosUI.h"
#include <QEvent> 

namespace FarmaSystem {

    VentasView::VentasView(SistemaFarmacia* sSistema, QWidget* parent)
        : QWidget(parent), sistema(sSistema), totalAcumuladoCarrito(0.0) {
        construirUI();
        actualizarVista();
    }

    void VentasView::construirUI() {
        RecursosUI ui;
        ui.aplicarEstiloVentana(this);

        QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);

        QLabel* titulo = new QLabel("Modulo de Ventas");
        titulo->setAlignment(Qt::AlignCenter);
        ui.aplicarTituloNeon(titulo);
        layoutPrincipal->addWidget(titulo);

        QGridLayout* layoutFormulario = new QGridLayout();

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

        lblSubtotalVenta = new QLabel;
        lblTotalVenta = new QLabel;
        lblSubtotalVenta->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        lblTotalVenta->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        ui.aplicarLabelSubtotal(lblSubtotalVenta);
        ui.aplicarLabelTotal(lblTotalVenta);

        layoutPrecios->addWidget(lblSubtotalVenta);
        layoutPrecios->addWidget(lblTotalVenta);

        // El boton viejo ahora agrega al carrito temporal
        QPushButton* btnAgregar = new QPushButton("Agregar al Carrito");
        btnAgregar->setFixedWidth(185);
        ui.aplicarEstiloBoton(btnAgregar);

        // El botón nuevo que procesa la compra como un supermercado
        btnFinalizarFactura = new QPushButton("Finalizar Factura");
        btnFinalizarFactura->setFixedWidth(185);
        ui.aplicarEstiloBoton(btnFinalizarFactura);

        btnQuitarProducto = new QPushButton("Quitar Producto Seleccionado");
        btnQuitarProducto->setFixedWidth(185);
        ui.aplicarEstiloBoton(btnQuitarProducto); // Usa tu estilo estandar de la U
        btnQuitarProducto->setFocusPolicy(Qt::NoFocus);

        QHBoxLayout* layoutFiltro = new QHBoxLayout();
        comboFiltroClientes = ui.crearCombo();
        comboFiltroClientes->setFixedWidth(185);

        lblTotalFiltro = new QLabel("Total Ventas: CRC 0.00");
        lblTotalFiltro->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui.aplicarLabelGanancias(lblTotalFiltro);

        QLabel* lblTxtFiltro = new QLabel("Historial de Ventas:");
        ui.aplicarLabelInfo(lblTxtFiltro);

        layoutFiltro->addWidget(lblTxtFiltro);
        layoutFiltro->addWidget(comboFiltroClientes, 0, Qt::AlignLeft);
        layoutFiltro->addStretch();

        tablaVentas = new QTableWidget(this);
        tablaVentas->setColumnCount(6);
        tablaVentas->setHorizontalHeaderLabels({ "ID / Estado", "Fecha", "Cliente/ID", "Medicamento", "Cantidad", "Total" });
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
        botonVolver->setFixedWidth(185);
        ui.aplicarEstiloBoton(botonVolver);

        QHBoxLayout* layoutInferior = new QHBoxLayout();
        layoutInferior->addWidget(lblTotalFiltro);
        layoutInferior->addStretch();
        layoutInferior->addWidget(btnFinalizarFactura); // Boton de pagar abajo
        layoutInferior->addSpacing(10);
        layoutInferior->addWidget(botonVolver);

        layoutPrincipal->addLayout(layoutFormulario);
        layoutPrincipal->addLayout(layoutPrecios, Qt::AlignRight);
        layoutPrincipal->addWidget(btnAgregar, 0, Qt::AlignRight);
        layoutPrincipal->addWidget(btnQuitarProducto, 0, Qt::AlignRight);
        layoutPrincipal->addSpacing(10);
        layoutPrincipal->addLayout(layoutFiltro);
        layoutPrincipal->addWidget(tablaVentas);
        layoutPrincipal->addLayout(layoutInferior);

        // Conexiones de Qt
        connect(btnAgregar, &QPushButton::clicked, this, &VentasView::agregarAlCarritoUI);
        connect(btnFinalizarFactura, &QPushButton::clicked, this, &VentasView::finalizarFacturaUI);
        connect(botonVolver, &QPushButton::clicked, this, &VentasView::volverAlMenu);
        connect(comboClientesVenta, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(comboMedicamentosVenta, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(spinCantidadVenta, QOverload<int>::of(&QSpinBox::valueChanged), this, &VentasView::actualizarVistaPrecioVenta);
        connect(btnQuitarProducto, &QPushButton::clicked, this, &VentasView::quitarProductoUI);

        QLineEdit* spinEdit = spinCantidadVenta->findChild<QLineEdit*>();
        if (spinEdit) {
            connect(spinEdit, &QLineEdit::textChanged, this, &VentasView::actualizarVistaPrecioVenta);
            spinEdit->installEventFilter(this);
        }
        connect(comboFiltroClientes, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &VentasView::actualizarFiltroVentas);
        resetearLabelsPrecio();
    }

    void VentasView::actualizarVista() {
        cargarCombosVentas();
        limpiarFormularioVenta();
        actualizarFiltroVentas();
    }

    void VentasView::resetearLabelsPrecio() {
        lblSubtotalVenta->setText("Subtotal: CRC 0.00");
        lblTotalVenta->setText("Total: CRC 0.00");
    }

    void VentasView::resetearCarrito() {
        carritoTemporal.clear();
        totalAcumuladoCarrito = 0.0;
        resetearLabelsPrecio();
    }

    void VentasView::cargarCombosVentas() {
        QStringList nombresClientes;
        QList<int> idsClientes;
        for (int i = 0; i < sistema->getCantClientes(); i++) {
            Cliente* c = sistema->getClientePorIndice(i);
            if (c != nullptr) {
                nombresClientes.append(QString::fromStdString(c->getNombre()));
                idsClientes.append(c->getID());
            }
        }

        QStringList nombresMedicamentos;
        QList<int> idsMedicamentos;
        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {
            Medicamento* m = sistema->getMedicamentoPorIndice(i);
            if (m != nullptr) {
                nombresMedicamentos.append(QString::fromStdString(m->getNombre()));
                idsMedicamentos.append(m->getID());
            }
        }

        EIF201::CargadorDatosUI::plasmarDatosEnCombo(comboMedicamentosVenta, nombresMedicamentos, idsMedicamentos, "Ninguno...");
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(comboClientesVenta, nombresClientes, idsClientes, "Ninguno...");
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(comboFiltroClientes, nombresClientes, idsClientes, "");

        comboFiltroClientes->insertItem(0, "Todos", -1);
        comboFiltroClientes->setCurrentIndex(0);
    }

    void VentasView::actualizarVistaPrecioVenta() {
        int idCliente = comboClientesVenta->currentData().toInt();
        int idMedicamento = comboMedicamentosVenta->currentData().toInt();

        if (idCliente <= 0 || idMedicamento <= 0) {
            resetearLabelsPrecio();
            return;
        }

        QLineEdit* spinEdit = spinCantidadVenta->findChild<QLineEdit*>();
        QString textoEnPantalla = (spinEdit) ? spinEdit->text().trimmed() : "";

        if (textoEnPantalla.isEmpty() || spinCantidadVenta->value() <= 0) {
            resetearLabelsPrecio();
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

    void VentasView::agregarAlCarritoUI() {
        Cliente* cliente = sistema->buscarClientePorID(comboClientesVenta->currentData().toInt());
        Medicamento* medicamento = sistema->buscarMedicamentoPorID(comboMedicamentosVenta->currentData().toInt());
        int cantidad = spinCantidadVenta->value();

        if (cliente == nullptr || medicamento == nullptr || cantidad <= 0) {
            QMessageBox::warning(this, "FarmaSystem", "Seleccione un cliente, un medicamento y cantidad valida.");
            return;
        }

        // Validacion de Stock 
        int cantidadEnCarrito = 0;
        for (int i = 0; i < carritoTemporal.size(); i++) {
            ItemCarrito item = carritoTemporal[i];
            if (item.medicamento->getID() == medicamento->getID()) {
                cantidadEnCarrito += item.cantidad;
            }
        }

        if ((cantidad + cantidadEnCarrito) > medicamento->getStock()) {
            QMessageBox::warning(this, "FarmaSystem", "Stock insuficiente. Ya tienes " +
                QString::number(cantidadEnCarrito) + " en el carrito.");
            return;
        }

        double totalItem = sistema->calcularTotalVenta(cliente, medicamento, cantidad);

        ItemCarrito nuevoItem;
        nuevoItem.medicamento = medicamento;
        nuevoItem.cantidad = cantidad;
        nuevoItem.presentoReceta = checkRecetaVenta->isChecked();
        nuevoItem.totalItem = totalItem;

        carritoTemporal.append(nuevoItem);

        // Bloqueamos el cliente para que no lo cambien a mitad de la compra
        comboClientesVenta->setEnabled(false);

        actualizarTablaVisualCarrito();
        limpiarFormularioVenta();
    }

    void VentasView::actualizarTablaVisualCarrito() {
        tablaVentas->setRowCount(0);
        totalAcumuladoCarrito = 0.0;

        for (int i = 0; i < carritoTemporal.size(); i++) {
            ItemCarrito item = carritoTemporal[i];
            totalAcumuladoCarrito += item.totalItem;

            int fila = tablaVentas->rowCount();
            tablaVentas->insertRow(fila);

            tablaVentas->setItem(fila, 0, new QTableWidgetItem("Item " + QString::number(i + 1)));
            tablaVentas->setItem(fila, 1, new QTableWidgetItem("Prefactura"));
            tablaVentas->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(sistema->buscarClientePorID(comboClientesVenta->currentData().toInt())->getNombre())));
            tablaVentas->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(item.medicamento->getNombre())));
            tablaVentas->setItem(fila, 4, new QTableWidgetItem(QString::number(item.cantidad)));
            tablaVentas->setItem(fila, 5, new QTableWidgetItem("CRC " + QString::number(item.totalItem, 'f', 2)));
            tablaVentas->setShowGrid(false);
        }

        lblTotalVenta->setText("Total Carrito: CRC " + QString::number(totalAcumuladoCarrito, 'f', 2));
    }

    void VentasView::finalizarFacturaUI() {
        if (carritoTemporal.isEmpty()) {
            QMessageBox::warning(this, "FarmaSystem", "El carrito de compras esta vacio.");
            return;
        }

        if (QMessageBox::question(this, "FarmaSystem", "Desea finalizar la factura de supermercado con los " +
            QString::number(carritoTemporal.size()) + " productos seleccionados",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            return;
        }

        Cliente* cliente = sistema->buscarClientePorID(comboClientesVenta->currentData().toInt());
        QString fecha = QDate::currentDate().toString("dd/MM/yyyy");
        int errores = 0;

        for (int i = 0; i < carritoTemporal.size(); i++) {
            ItemCarrito item = carritoTemporal[i];

            int resultado = sistema->registrarVenta(cliente->getCedula(), item.medicamento->getID(), item.cantidad,
                item.presentoReceta, fecha.toStdString(), true
            );

            // Si da un codigo diferente de 0, diagnosticamos de inmediato que paso
            if (resultado != 0) {
                errores++;
                QString medicamentoNombre = QString::fromStdString(item.medicamento->getNombre());
                QString motivo = "Error desconocido.";

                if (resultado == 1) { motivo = "El cliente no existe."; }
                else if (resultado == 2) { motivo = "El medicamento no existe."; }
                else if (resultado == 3) { motivo = "La cantidad ingresada es invalida."; }
                else if (resultado == 4) { motivo = "La fecha del sistema es invalida."; }
                else if (resultado == 5) { motivo = "Este producto requiere receta medica obligatoria."; }
                else if (resultado == 6) { motivo = "No hay suficiente stock en el inventario."; }
                else if (resultado == 7) { motivo = "La cantidad excede la dosis maxima permitida."; }

                QMessageBox::critical(this, "Falla en Carrito",
                    "No se pudo procesar: " + medicamentoNombre + "\nMotivo: " + motivo);
            }
        }

        if (errores == 0) {
            QMessageBox::information(this, "FarmaSystem", "Factura de supermercado procesada con exito");
            resetearCarrito();
            comboClientesVenta->setEnabled(true);
            actualizarVista();
            emit datosActualizados();
        }
        else {
            QMessageBox::warning(this, "FarmaSystem", "Se detectaron " + QString::number(errores) +
                " errores. Modifique el carrito quitando los productos rechazados e intente finalizar de nuevo.");

            actualizarTablaVisualCarrito(); // Vuelve a dibujar el carrito con lo que quedo pendiente
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
    void VentasView::resetextoReceta() {}
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

    void VentasView::quitarProductoUI() {
      
        int filaSeleccionada = tablaVentas->currentRow();
        if (filaSeleccionada < 0) {
            QMessageBox::warning(this, "FarmaSystem", "Por favor, seleccione un producto de la tabla para quitarlo.");
            return;
        }

        if (QMessageBox::question(this, "FarmaSystem", "Esta seguro de que desea remover este producto del carrito",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
            return;
        }

        carritoTemporal.removeAt(filaSeleccionada);

        if (carritoTemporal.isEmpty()) {
            comboClientesVenta->setEnabled(true);
        }

        // Refrescamos la pantalla para recalcular los totales y redibujar la tabla
        actualizarTablaVisualCarrito();

        QMessageBox::information(this, "FarmaSystem", "Producto removido del carrito.");
    }

    void VentasView::limpiarFormularioVenta() {
        comboMedicamentosVenta->setCurrentIndex(0);
        checkRecetaVenta->setChecked(false);
        spinCantidadVenta->setValue(0);
        resetearLabelsPrecio();
    }
} // namespace FarmaSystem