#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>
#include <QGraphicsDropShadowEffect>

#include "ClientesView.h"


namespace FarmaSystem {
    
    ClientesView::ClientesView(SistemaFarmacia* s, QWidget* parent) : QWidget(parent), sistema(s) 
    {
        construirUI();
        llenarTablaUI();
    }

    // UI
    void ClientesView::construirUI() {

		// Crear espacios 
        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* layoutBotones = new QHBoxLayout();
        QHBoxLayout* layoutBusqueda = new QHBoxLayout();

		// Diseno titulo
        QLabel* tituloSeccion = new QLabel("Gestion de Clientes");
        tituloSeccion->setStyleSheet("font-weight: bold; font-size: 20px; color: white; letter-spacing: 1px;");
        QColor verdeNeon("#39FF14");
        QColor rojoNeon("#FF3131");

        QGraphicsDropShadowEffect* neonEfecto = new QGraphicsDropShadowEffect();

        neonEfecto->setBlurRadius(35);       
        neonEfecto->setColor(QColor(57, 255, 20));
        neonEfecto->setOffset(0, 0);       

        tituloSeccion->setGraphicsEffect(neonEfecto);

        QVariantAnimation* animacion = new QVariantAnimation(this);

        animacion->setDuration(1000); // 1 segundos por ciclo
        animacion->setStartValue(verdeNeon);
        animacion->setEndValue(rojoNeon);
        animacion->setLoopCount(-1);

        // Diseno busqueda
        textoBuscarCliente = new QLineEdit();
        textoBuscarCliente->setPlaceholderText("🔍 Buscar cedula...");
        textoBuscarCliente->setMaximumWidth(250);
        textoBuscarCliente->installEventFilter(this);
        textoBuscarCliente->setStyleSheet(
            "QLineEdit {"
            "  background-color: #2b2b2b;" 
            "  color: #e0e0e0;"             
            "  border: 2px solid #ff4d4d;"  
            "  border-radius: 8px;"        
            "  padding: 5px 10px;"
            "}"

            "QLineEdit:focus {"
            "  border: 2px solid #39FF14;"  
            "  background-color: #333333;"  
            "}"
        );

		layoutBusqueda->addWidget(tituloSeccion);
        layoutBusqueda->addStretch();
        layoutBusqueda->addWidget(new QLabel("Buscar:"));
        layoutBusqueda->addWidget(textoBuscarCliente);
        
        // Diseno Tabla
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
        tabla->setStyleSheet(
            "QTableWidget {"
            "  background-color: #2b2b2b;" 
            "  color: #e0e0e0;"           
            "  gridline-color: #ff4d4d;" 
            "  outline: 0;"
            "  border: 2px solid #ff4d4d;" // borde exterior en rojo pastel
            "  border - radius: 5px;"
            "}"
            
            "QTableWidget:focus {"
			"  border: 2px solid #39FF14;" 
            "}"

            "QTableWidget::item {"
            "border: none;"
            "padding: 6px;"
            "}"

            "QTableWidget::item:selected {"
            "background-color: #3399ff;"
            "color: #2b2b2b;"
            "}"

            "QTableWidget::item:focus {"
            "outline: none;"
            "border: none;"
            "}"
        );

        // Diseno Botones
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

        layoutBotones->addWidget(botonRegistrar,1);
        layoutBotones->addWidget(botonEliminar,1);
        layoutBotones->addWidget(botonFidelidad,1);
        layoutBotones->addWidget(botonVolver,1);
        layoutBotones->addStretch();

        QString estiloBotones =
            "QPushButton {"
            "  background-color: #2b2b2b;"     
            "  color: #ff4d4d;"                 
            "  border: 2px solid #ff4d4d;"    //Rojo
            "  border-radius: 10px;"
            "  padding: 8px 16px;"
            "  font-weight: bold;"
            "  font-size: 13px;"
            "}"

            // Al pasar el mouse
            "QPushButton:hover {"
            "  background-color: #323232;"
            "  color: #39FF14;"                 // Verde 
            "  border: 2px solid #39FF14;"
            "}"

            // Al hacer clic
            "QPushButton:pressed {"
            "  background-color: #4d0000;"      // Fondo rojo oscuro
            "  color: #ff4d4d;"                 // Texto rojo
            "  border: 2px solid #ff4d4d;"
            "}"

            // Cuando el boton esta bloqueado 
            "QPushButton:disabled {"
			"  background-color: #1a1a1a;"      
			"  color: #444444;"                 // gris 
            "  border: 2px solid #333333;"
            "}";

        botonRegistrar->setStyleSheet(estiloBotones);
        botonEliminar->setStyleSheet(estiloBotones);
        botonFidelidad->setStyleSheet(estiloBotones);
        botonVolver->setStyleSheet(estiloBotones);

        // Orden final en pantalla
        layout->addLayout(layoutBusqueda);
        layout->addWidget(tabla);
        layout->addLayout(layoutBotones);

        // Conexiones
        connect(botonRegistrar, &QPushButton::clicked, this, &ClientesView::abrirDialogRegistrarCliente);
        connect(botonEliminar, &QPushButton::clicked, this, &ClientesView::eliminarClienteSeleccionado);
        connect(botonFidelidad, &QPushButton::clicked, this, &ClientesView::toggleFidelidadCliente);
        connect(botonVolver, &QPushButton::clicked, this, &ClientesView::volverAlMenu);
        connect(textoBuscarCliente, &QLineEdit::textChanged, this, &ClientesView::filtrarClientes);
        connect(tabla, &QTableWidget::itemSelectionChanged, this, [=]() { textoBuscarCliente->deselect(); });
        connect(tabla, &QTableWidget::itemSelectionChanged, this, &ClientesView::actualizarEstadoBotones);
        connect(animacion, &QVariantAnimation::valueChanged, [=](const QVariant& value) { neonEfecto->setColor(value.value<QColor>()); });

        QTimer::singleShot(0, this, [=]() { this->setFocus(); });

        animacion->start();

    } // Fin UI

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

                QMessageBox::information(&dialog, "Exito", "Cliente registrado.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
            }

            else {

                QMessageBox::warning(&dialog, "Error", resultado == 2 ? "Cedula Existente." : "Campos vacios.");
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

		// Crear celdas para cada fila 
        int celda = tabla->rowCount();

        // Insertar Fila
        tabla->insertRow(celda);

		// Agregar dato a cada celda de la fila
        tabla->setItem(celda, 0, new QTableWidgetItem(id));
        tabla->setItem(celda, 1, new QTableWidgetItem(nombre));
        tabla->setItem(celda, 2, new QTableWidgetItem(cedula));
        tabla->setItem(celda, 3, new QTableWidgetItem(estado));
    }

    void FarmaSystem::ClientesView::showEvent(QShowEvent* event) {
       
        QWidget::showEvent(event); // Llama a la implementacion base
        tabla->clearSelection();
        tabla->setCurrentCell(-1, -1);
        textoBuscarCliente->clear();
        this->setFocus();

        // Color del Grid
        tabla->setStyleSheet(tabla->styleSheet() + "QTableWidget { gridline-color: #ff4d4d; }");

        actualizarEstadoBotones(); // Asegura que los botones se bloqueen
    }

    void ClientesView::actualizarEstadoBotones() {

        // Si la lista de items seleccionados no esta vacia, devuelve true
        bool tieneSeleccion = !tabla->selectedItems().isEmpty();

        botonEliminar->setEnabled(tieneSeleccion);
        botonFidelidad->setEnabled(tieneSeleccion); 

        if (tieneSeleccion) {
            tabla->setStyleSheet(tabla->styleSheet() + "QTableWidget { gridline-color: #39FF14; }");
        }
        else {
            tabla->setStyleSheet(tabla->styleSheet() + "QTableWidget { gridline-color: #ff4d4d; }");
        }
    }
}