#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>

#include "ClientesView.h"
#include "RecursosUI.h"


namespace FarmaSystem {
    
    ClientesView::ClientesView(SistemaFarmacia* s, QWidget* parent) : QWidget(parent), sistema(s) 
    {
        construirUI();
        llenarTablaUI();
    }

    // UI
    void ClientesView::construirUI() {

        RecursosUI ui;

        // Crear layouts
        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* layoutBotones = new QHBoxLayout();
        QHBoxLayout* layoutBusqueda = new QHBoxLayout();

        // Titulo
        QLabel* tituloSeccion = new QLabel("Gestion de Clientes");
        ui.aplicarTituloNeon(tituloSeccion);

		// Buscador
        textoBuscarCliente = new QLineEdit();
        textoBuscarCliente->setPlaceholderText("🔍 Buscar cedula...");
        textoBuscarCliente->setMaximumWidth(250);
        textoBuscarCliente->installEventFilter(this);

        ui.aplicarEstiloBuscador(textoBuscarCliente);

		// comparten espacio en el mismo layout
        layoutBusqueda->addWidget(tituloSeccion);
        layoutBusqueda->addStretch();
        layoutBusqueda->addWidget(new QLabel("Buscar:"));
        layoutBusqueda->addWidget(textoBuscarCliente);

		// Tabla 
        tabla = new QTableWidget;

        tabla->setColumnCount(4);
        tabla->setHorizontalHeaderLabels({ "ID", "Nombre", "Cedula", "Fidelidad (5%)" });
        tabla->verticalHeader()->setVisible(false);
        tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
        tabla->setSelectionMode(QAbstractItemView::SingleSelection);
        tabla->setFocusPolicy(Qt::StrongFocus);
        tabla->setTabKeyNavigation(false);
        tabla->setShowGrid(true);

        ui.aplicarEstiloTabla(tabla);

        botonEliminar = new QPushButton("Eliminar");
        botonFidelidad = new QPushButton("Fidelidad");
        QPushButton* botonRegistrar = new QPushButton("Registrar");
        QPushButton* botonVolver = new QPushButton("Volver");

        botonEliminar->setEnabled(false);
        botonFidelidad->setEnabled(false);

        botonRegistrar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        botonEliminar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        botonFidelidad->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        botonVolver->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        layoutBotones->addWidget(botonRegistrar, 1);
        layoutBotones->addWidget(botonEliminar, 1);
        layoutBotones->addWidget(botonFidelidad, 1);
        layoutBotones->addWidget(botonVolver, 1);
        layoutBotones->addStretch();

        ui.aplicarEstiloBoton(botonRegistrar);
        ui.aplicarEstiloBoton(botonEliminar);
        ui.aplicarEstiloBoton(botonFidelidad);
        ui.aplicarEstiloBoton(botonVolver);

        // Orden en pantalla
        layout->addLayout(layoutBusqueda);
        layout->addWidget(tabla);
        layout->addLayout(layoutBotones);

        connect(botonRegistrar, &QPushButton::clicked, this, &ClientesView::abrirDialogRegistrarCliente);
        connect(botonEliminar, &QPushButton::clicked, this, &ClientesView::eliminarClienteSeleccionado);
        connect(botonFidelidad, &QPushButton::clicked, this, &ClientesView::toggleFidelidadCliente);
        connect(botonVolver, &QPushButton::clicked, this, &ClientesView::volverAlMenu);
        connect(textoBuscarCliente, &QLineEdit::textChanged, this, &ClientesView::filtrarClientes);
        connect(tabla, &QTableWidget::itemSelectionChanged, this, [=]() { textoBuscarCliente->deselect(); });
        connect(tabla, &QTableWidget::itemSelectionChanged,this, &ClientesView::actualizarEstadoBotones);

        QTimer::singleShot(0, this, [=]() { this->setFocus(); });
    }

    void ClientesView::llenarTablaUI() {

		// Inicializar filas
        tabla->setRowCount(0);
        tabla->clearSelection();

        for (int i = 0; i < sistema->getCantClientes(); i++) {

            Cliente* cliente = sistema->getClientePorIndice(i);

            if (cliente != nullptr) {

                agregarDatos(cliente);
            }
        }
	} 

    void ClientesView::filtrarClientes(const QString& texto) {
        tabla->clearSelection();

        if (texto.isEmpty()) {
			llenarTablaUI(); // Llenar tabla normalmente si el texto de busqueda est vacio
            return;
        }

        tabla->setRowCount(0);

        for (int i = 0; i < sistema->getCantClientes(); i++) {

            Cliente* cliente = sistema->getClientePorIndice(i);

            if (cliente != nullptr) {

                QString cedula = QString::fromStdString(cliente->getCedula());

                if (cedula.contains(texto)) {

                    agregarDatos(cliente);
                }
            }
        }
    }

    void ClientesView::abrirDialogRegistrarCliente() {

		// Crear dialogo
        QDialog dialog(this);
        dialog.setWindowTitle("Registrar");
        QVBoxLayout* layout = new QVBoxLayout(&dialog);

		// Campos de texto
        QLineEdit* editNombre = new QLineEdit();
        editNombre->setPlaceholderText("Nombre Completo");
        QLineEdit* editCedula = new QLineEdit();
        editCedula->setPlaceholderText("Cedula de Identidad");
        
		// Agregar campos al layout
        layout->addWidget(new QLabel("Nombre:"));
        layout->addWidget(editNombre);
        layout->addWidget(new QLabel("Cedula:"));
        layout->addWidget(editCedula);

		// Boton
        QPushButton* botonGuardar = new QPushButton("Guardar");
        layout->addWidget(botonGuardar);

		// Conexion
        connect(botonGuardar, &QPushButton::clicked, [&]() {

			std::string nombre = editNombre->text().toStdString();
			std::string cedula = editCedula->text().toStdString();

            int resultado = sistema->registrarCliente(nombre, cedula);

            if (resultado == 0) {

                QMessageBox::information(&dialog, "FarmaSystem", "Cliente registrado.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
            }

            else {

                QMessageBox::warning(&dialog, "FarmaSystem", resultado == 2 ? "Cedula Existente." : "Campos vacios.");
            }
        });
        
        dialog.exec();
    }

    void ClientesView::eliminarClienteSeleccionado() {

        if (tabla->selectedItems().isEmpty()) { return; }
        
        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();

        if (QMessageBox::question(this, "FarmaSystem", "Eliminar cliente?") == QMessageBox::Yes) {

            if (sistema->eliminarCliente(id)) {
                llenarTablaUI();
                emit datosActualizados();  // signal a MainWindow
            }
        }
    }

    void ClientesView::toggleFidelidadCliente() {

        if (tabla->selectedItems().isEmpty()) { return; }

		int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();

        sistema->toggleFidelidad(id);
        llenarTablaUI();
		emit datosActualizados();
    }

    bool ClientesView::eventFilter(QObject* obj, QEvent* event) {

		// limpia seleccion de la tabla al hacer focus en el buscador para evitar confusiones visuales
        if (obj == textoBuscarCliente && event->type() == QEvent::FocusIn)
        { tabla->clearSelection(); }

        // Retorna el evento normalmente para que funcione todo como se espera
        return QWidget::eventFilter(obj, event);
    }

    void ClientesView::agregarDatos(Cliente* cliente) {

		// Convierte datos del cliente a QString para mostrar en la tabla
        QString id = QString::number(cliente->getID());
        QString nombre = QString::fromStdString(cliente->getNombre());
        QString cedula = QString::fromStdString(cliente->getCedula());
        QString estado = cliente->getTarjeta() ? "ACTIVA" : "INACTIVA";

		// Crear  fila 
        int fila = tabla->rowCount();

        // Insertar Fila
        tabla->insertRow(fila);

		// Agregar celda de la fila
        tabla->setItem(fila, 0, new QTableWidgetItem(id));
        tabla->setItem(fila, 1, new QTableWidgetItem(nombre));
        tabla->setItem(fila, 2, new QTableWidgetItem(cedula));
        tabla->setItem(fila, 3, new QTableWidgetItem(estado));
    }

    void FarmaSystem::ClientesView::showEvent(QShowEvent* event) {
       
        QWidget::showEvent(event); // Llama a la implementacion base
        tabla->clearSelection();
        tabla->setCurrentCell(-1, -1);
        textoBuscarCliente->clear();
        this->setFocus();

        RecursosUI ui;

        ui.cambiarColorGrid(tabla, false);

        actualizarEstadoBotones(); // Asegura que los botones se bloqueen
    }

    void ClientesView::actualizarEstadoBotones() {

        // Si la lista de items seleccionados no esta vacia, devuelve true
        bool tieneSeleccion = !tabla->selectedItems().isEmpty();

        botonEliminar->setEnabled(tieneSeleccion);
        botonFidelidad->setEnabled(tieneSeleccion); 
        RecursosUI ui;

        if (tieneSeleccion) {
            ui.cambiarColorGrid(tabla, true);
        }
        else {
            ui.cambiarColorGrid(tabla, false);
        }
    }
}