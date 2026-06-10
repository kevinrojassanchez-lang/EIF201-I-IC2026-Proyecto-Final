#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>

#include "ClientesView.h"
#include "RecursosUI.h"


namespace FarmaSystem { // Implementacion de ClientesView
    
    ClientesView::ClientesView(SistemaFarmacia* s, QWidget* parent) : QWidget(parent), sistema(s) {

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
        botonEditar = new QPushButton("Editar");

        QPushButton* botonRegistrar = new QPushButton("Registrar");
        QPushButton* botonVolver = new QPushButton("Volver");

        botonEliminar->setEnabled(false);
        botonEditar->setEnabled(false);

        botonRegistrar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		botonEditar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        botonEliminar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        botonVolver->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        layoutBotones->addWidget(botonRegistrar, 1);
        layoutBotones->addWidget(botonEditar, 1);
        layoutBotones->addWidget(botonEliminar, 1);
        layoutBotones->addWidget(botonVolver, 1);
        layoutBotones->addStretch();

        ui.aplicarEstiloBoton(botonRegistrar);
        ui.aplicarEstiloBoton(botonEditar);
        ui.aplicarEstiloBoton(botonEliminar);
        ui.aplicarEstiloBoton(botonVolver);

        // Orden en pantalla
        layout->addLayout(layoutBusqueda);
        layout->addWidget(tabla);
        layout->addLayout(layoutBotones);

        connect(botonRegistrar, &QPushButton::clicked, this, &ClientesView::abrirDialogRegistrarCliente);
        connect(botonEditar, &QPushButton::clicked, this, &ClientesView::abrirDialogEditarCliente);
        connect(botonEliminar, &QPushButton::clicked, this, &ClientesView::eliminarClienteSeleccionado);
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

            llenarTablaUI();

            return;
        }

        tabla->setRowCount(0);

        for (int i = 0; i < sistema->getCantClientes(); i++) {

            Cliente* cliente = sistema->getClientePorIndice(i);

            if (cliente != nullptr) {

                QString cedula = QString::fromStdString(cliente->getCedula());

                if (cedula.contains(texto, Qt::CaseInsensitive)) {
                    agregarDatos(cliente);
                }
            }
        }
    }

    void ClientesView::abrirDialogRegistrarCliente() {

        QDialog dialog(this);
        dialog.setWindowTitle("Registrar Cliente");

        // Instanciar e inicializar los estilos del dialogo
        RecursosUI ui;
        ui.aplicarEstiloFormularioDialogo(&dialog);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // Crear las etiquetas con estilo limpio de informacion
        QLabel* lblNombre = new QLabel("Nombre:");
        QLabel* lblCedula = new QLabel("Cedula:");
        ui.aplicarLabelInfo(lblNombre);
        ui.aplicarLabelInfo(lblCedula);

        // Campos de Texto optimizados usando los metodos estaticos de RecursosUI
        QLineEdit* editNombre = RecursosUI::crearCampoTexto("Nombre Completo");
        QLineEdit* editCedula = RecursosUI::crearCampoTexto("Cedula de Identidad");

        QPushButton* botonGuardar = new QPushButton("Guardar");
        ui.aplicarEstiloBoton(botonGuardar);

        // Organizar el layout de forma estructurada
        layout->addWidget(lblNombre);
        layout->addWidget(editNombre);
        layout->addWidget(lblCedula);
        layout->addWidget(editCedula);
        layout->addSpacing(10);                       // Un pequeno espacio de respiro antes del boton
        layout->addWidget(botonGuardar);

        connect(botonGuardar, &QPushButton::clicked, [&]() {

            std::string nombre = editNombre->text().trimmed().toStdString();
            std::string cedula = editCedula->text().trimmed().toStdString();

            // Logica de la controladora
            int resultado = sistema->registrarCliente(nombre, cedula);

            switch (resultado) {

            case 0:
                QMessageBox::information(&dialog, "FarmaSystem", "Cliente registrado.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
                break;

             case 1: QMessageBox::warning(&dialog, "FarmaSystem", "Campos vacios."); break;
             case 2: QMessageBox::warning(&dialog, "FarmaSystem", "Cedula ya registrada."); break;
            default: QMessageBox::critical(&dialog, "FarmaSystem", "Error interno."); break;
            }
        });
       dialog.exec();
    }

    void ClientesView::eliminarClienteSeleccionado() {

        if (tabla->selectedItems().isEmpty()) { return; }

        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();

        if (QMessageBox::question(this, "FarmaSystem", "Eliminar cliente") == QMessageBox::Yes) {

            if (sistema->eliminarCliente(id)) {

                llenarTablaUI();

                emit datosActualizados();
            }
            else {
                QMessageBox::critical(this,
                    "FarmaSystem", "Error al eliminar cliente:\n Verifique cedula o historial de ventas");
			}
        }
    }

    bool ClientesView::eventFilter(QObject* obj, QEvent* event) {

        if (obj == textoBuscarCliente && event->type() == QEvent::FocusIn) { tabla->clearSelection(); }

        return QWidget::eventFilter(obj, event);
    }

    void ClientesView::abrirDialogEditarCliente() {

        if (tabla->selectedItems().isEmpty()) { return; }

        int fila = tabla->currentRow();
        int id = tabla->item(fila, 0)->text().toInt();

        Cliente* cliente = sistema->buscarClientePorID(id);
        if (cliente == nullptr) { return; }

        QDialog dialog(this);
        dialog.setWindowTitle("Editar Cliente");

        // Instanciar e inicializar los estilos de la ventana emergente
        RecursosUI ui;
        ui.aplicarEstiloFormularioDialogo(&dialog);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // Crear componentes estilizados usando los metodos estaticos de RecursosUI 
        QLineEdit* nombre = RecursosUI::crearCampoTexto("");
        QLineEdit* cedula = RecursosUI::crearCampoTexto("");
        QCheckBox* fidelidad = RecursosUI::crearCheck("Tarjeta de fidelidad activa");

        QPushButton* guardar = new QPushButton("Guardar");
        ui.aplicarEstiloBoton(guardar);

        // Cargar los datos actuales del modelo en los componentes
        nombre->setText(QString::fromStdString(cliente->getNombre()));
        cedula->setText(QString::fromStdString(cliente->getCedula()));
        fidelidad->setChecked(cliente->getTarjeta());

        // Crear etiquetas con estilo limpio de información
        QLabel* lblNombre = new QLabel("Nombre:");
        QLabel* lblCedula = new QLabel("Cedula:");
        ui.aplicarLabelInfo(lblNombre);
        ui.aplicarLabelInfo(lblCedula);

        // Organizar el layout de manera ordenada
        layout->addWidget(lblNombre);
        layout->addWidget(nombre);
        layout->addWidget(lblCedula);
        layout->addWidget(cedula);
        layout->addWidget(fidelidad);
        layout->addSpacing(10); // Un espacio de separacion antes del boton
        layout->addWidget(guardar);

        connect(guardar, &QPushButton::clicked, [&]() {

            int resultado = sistema->editarCliente(id, nombre->text().trimmed().toStdString(),
                cedula->text().trimmed().toStdString(), fidelidad->isChecked());

            switch (resultado) {

            case 0:
                sistema->guardarDatos(); // la persistencia se confirma tras la validacion de la controladora
                QMessageBox::information(this, "FarmaSystem", "Cliente Actualizado.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
                break;

            case 1:  QMessageBox::warning(this, "FarmaSystem", "Cliente no encontrado.");break;
            case 2:  QMessageBox::warning(this, "FarmaSystem", "Nombre invalido."); break;
            case 3:  QMessageBox::warning(this, "FarmaSystem", "Cedula invalida."); break;
            default: QMessageBox::critical(this, "FarmaSystem", "Error interno."); break;
            }
        });

        dialog.exec();
    }

    void ClientesView::agregarDatos(Cliente* cliente) {

		// Convierte datos del cliente a QString para mostrar en la tabla
        QString id = QString::number(cliente->getID());
        QString nombre = QString::fromStdString(cliente->getNombre());
        QString cedula = QString::fromStdString(cliente->getCedula());
        QString estado = cliente->getTarjeta() ? "ACTIVA" : "INACTIVA";

		// Crear  fila 
        int fila = tabla->rowCount();

        // Insertar fila
        tabla->insertRow(fila);

		// Cargar dato a cada columna de la fila
        tabla->setItem(fila, 0, new QTableWidgetItem(id));
        tabla->setItem(fila, 1, new QTableWidgetItem(nombre));
        tabla->setItem(fila, 2, new QTableWidgetItem(cedula));
        tabla->setItem(fila, 3, new QTableWidgetItem(estado));
    }

	// Limpia seleccion, resetea buscador y actualiza estado de botones cada vez que se muestra la vista
    void ClientesView::showEvent(QShowEvent* event) {

        QWidget::showEvent(event);

        tabla->clearSelection();
        tabla->setCurrentCell(-1, -1);

        textoBuscarCliente->clear();
        textoBuscarCliente->clearFocus();

        this->setFocus();

        RecursosUI ui;
        ui.cambiarColorGrid(tabla, false);

        actualizarEstadoBotones();
    }

    void ClientesView::actualizarEstadoBotones() {

        bool tieneSeleccion = !tabla->selectedItems().isEmpty();

        botonEliminar->setEnabled(tieneSeleccion);
        botonEditar->setEnabled(tieneSeleccion);

        RecursosUI ui;
        ui.cambiarColorGrid(tabla, tieneSeleccion);
    }
} // namespace FarmaSystem