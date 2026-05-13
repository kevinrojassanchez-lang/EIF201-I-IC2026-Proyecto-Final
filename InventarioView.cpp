#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextedit>

#include "RecursosUI.h"
#include "InventarioView.h"

namespace FarmaSystem {

    InventarioView::InventarioView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent), sistema(sSistema) {
        construirUI();
        llenarTablaUI();
    }

    void InventarioView::construirUI() {

        RecursosUI ui;

        // Crear espacios
        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* layoutBusqueda = new QHBoxLayout();
        QHBoxLayout* botonesLayout = new QHBoxLayout();

        // Diseno titulo
        tituloSeccion = new QLabel("Gestion de Medicamentos");

        ui.aplicarTituloNeon(tituloSeccion);

        // Diseno buscador
        textoBuscarMedicamento = new QLineEdit();
        textoBuscarMedicamento->setPlaceholderText("🔍 Buscar por nombre o ID...");
        textoBuscarMedicamento->setMaximumWidth(250);
		textoBuscarMedicamento->installEventFilter(this);

        ui.aplicarEstiloBuscador(textoBuscarMedicamento);

        layoutBusqueda->addWidget(tituloSeccion);
        layoutBusqueda->addStretch();
        layoutBusqueda->addWidget(new QLabel("Buscar:"));
        layoutBusqueda->addWidget(textoBuscarMedicamento);

        // Diseno tabla
        tabla = new QTableWidget;

		tabla->setColumnCount(5);
        tabla->setHorizontalHeaderLabels({ "ID", "Nombre", "Categoria", "Stock", "Precio" });
        tabla->verticalHeader()->setVisible(false);
        tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
        tabla->setSelectionMode(QAbstractItemView::SingleSelection);
        tabla->setFocusPolicy(Qt::StrongFocus);
        tabla->setTabKeyNavigation(false);
        tabla->setShowGrid(true);

        ui.aplicarEstiloTabla(tabla);

        // Diseno botones
        botonEliminar = new QPushButton("Eliminar");
        botonStock = new QPushButton("Stock");
        botonDetalles = new QPushButton("Detalles");
        QPushButton* botonRegistrar = new QPushButton("Registrar");
        QPushButton* botonVolver = new QPushButton("Volver");

        ui.aplicarEstiloBoton(botonEliminar);
        ui.aplicarEstiloBoton(botonStock);
        ui.aplicarEstiloBoton(botonDetalles);
        ui.aplicarEstiloBoton(botonRegistrar);
        ui.aplicarEstiloBoton(botonVolver);

        botonesLayout->addWidget(botonRegistrar);
        botonesLayout->addWidget(botonStock);
        botonesLayout->addWidget(botonEliminar);
        botonesLayout->addWidget(botonDetalles);
        botonesLayout->addWidget(botonVolver);

        layout->addLayout(layoutBusqueda);
        layout->addWidget(tabla);
        layout->addLayout(botonesLayout);

        // Conexiones
        connect(botonRegistrar, &QPushButton::clicked, this, &InventarioView::abrirDialogRegistrarMedicamento);
        connect(botonEliminar, &QPushButton::clicked, this, &InventarioView::eliminarMedicamentoSeleccionado);
        connect(tabla, &QTableWidget::itemSelectionChanged, this, &InventarioView::actualizarEstadoBotones);
        connect(textoBuscarMedicamento, &QLineEdit::textChanged, this, &InventarioView::filtrarMedicamentos);
        connect(botonStock, &QPushButton::clicked, this, &InventarioView::actualizarStockMedicamento);
        connect(botonDetalles, &QPushButton::clicked, this, &InventarioView::mostrarDetallesSeleccionado);
        connect(botonVolver, &QPushButton::clicked, this, &InventarioView::volverAlMenu);
       
        QTimer::singleShot(0, this, [=]() { this->setFocus(); }); //limpiar el foco al terminar de construir UI

    

    } // Fin UI

    void InventarioView::llenarTablaUI() {

        tabla->setRowCount(0);
        tabla->clearSelection();

        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {

            Medicamento* medicamento = sistema->getMedicamentoPorIndice(i);

            if (medicamento != nullptr) {

                agregarDatos(medicamento);
            }
        }
        actualizarEstadoBotones();
    }

    void InventarioView::actualizarEstadoBotones() {

        RecursosUI ui;

        bool tieneSeleccion = !tabla->selectedItems().isEmpty();

        botonEliminar->setEnabled(tieneSeleccion);
        botonStock->setEnabled(tieneSeleccion);
        botonDetalles->setEnabled(tieneSeleccion);

        // efecto visual del grid al seleccionar
        if (tieneSeleccion) {

            ui.cambiarColorGrid(tabla, true);
        }
        else {

            ui.cambiarColorGrid(tabla, false);
        }
    }

    void InventarioView::showEvent(QShowEvent* event) {

        RecursosUI ui;

        QWidget::showEvent(event); // Llama a la implementacion base
        tabla->clearSelection();
        tabla->setCurrentCell(-1, -1);
        textoBuscarMedicamento->clear();
        this->setFocus();

        // Color del grid
        ui.cambiarColorGrid(tabla, false);

        actualizarEstadoBotones(); // Asegura que los botones se bloqueen
    }

    void InventarioView::abrirDialogRegistrarMedicamento() {

        QDialog dialog(this);
        dialog.setWindowTitle("FarmaSystem");
        dialog.setMinimumWidth(400);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // campos comunes
        QLineEdit* espacioNombre = new QLineEdit();
        espacioNombre->setPlaceholderText("Nombre del medicamento");

        QDoubleSpinBox* espacioPrecio = new QDoubleSpinBox();
        espacioPrecio->setRange(0, 1000000);
        espacioPrecio->setPrefix("Precio: CRC ");

        QSpinBox* espacioStock = new QSpinBox();
        espacioStock->setRange(0, 1000);
        espacioStock->setPrefix("Stock: ");

        QComboBox* espacioTipo = new QComboBox();
        espacioTipo->addItems({ "Generico", "De Marca", "Controlado" });

        // Campos dinamicos
        QLabel* labelDinamico = new QLabel("Principio Activo:");
        QLineEdit* editDinamico = new QLineEdit();
        QCheckBox* checkPromo = new QCheckBox("Aplicar Promocion (15% desc)");
        QLabel* labelNivel = new QLabel("Nivel de Control [1-4]:");
        QSpinBox* spinNivel = new QSpinBox();
        spinNivel->setRange(1, 4);

        // estado inicial bloqueados por defecto
        checkPromo->hide();
        labelNivel->hide();
        spinNivel->hide();

        // Construcion
        layout->addWidget(new QLabel("<b>Datos Basicos</b>"));
        layout->addWidget(espacioNombre);
        layout->addWidget(espacioPrecio);
        layout->addWidget(espacioStock);
        layout->addWidget(new QLabel("<b>Categoria</b>"));
        layout->addWidget(espacioTipo);
        layout->addWidget(checkPromo);
        layout->addWidget(labelDinamico);
        layout->addWidget(editDinamico);
        layout->addWidget(labelNivel);
        layout->addWidget(spinNivel);

        QPushButton* botonGuardar = new QPushButton("Guardar");
        layout->addWidget(botonGuardar);

        // Campos Dinamicos
        connect(espacioTipo, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {

            // Control de visibilidad base
            checkPromo->setVisible(index == 1);
            labelNivel->setVisible(index == 2);
            spinNivel->setVisible(index == 2);

            switch (index) {

            case 0: // Generico
                labelDinamico->setText("Principio Activo:");
                editDinamico->setPlaceholderText("Ej: Ibuprofeno");
                break;
            case 1: // Marca
                labelDinamico->setText("Pais de Origen:");
                editDinamico->setPlaceholderText("Ej: Costa Rica");
                break;
            case 2: // Controlado
                labelDinamico->setText("Dosis Maxima (mg):");
                editDinamico->setPlaceholderText("Ej: 500");
                break;
            }
        
        });

        connect(botonGuardar, &QPushButton::clicked, [&]() {

            std::string nombre = espacioNombre->text().trimmed().toStdString();
            double pre = espacioPrecio->value();
            int stock = espacioStock->value();
            std::string infoExtra = editDinamico->text().trimmed().toStdString();
            int res = -1;

            if (nombre.empty()) {
                QMessageBox::warning(&dialog, "Error", "El nombre es obligatorio.");
                return;
            }

            // Switch para decidir metodo del sistema a llamar
            switch (espacioTipo->currentIndex()) {

            case 0: // Generico
                res = sistema->registrarGenerico(nombre, pre, stock, infoExtra);
                break;

            case 1: // Marca
                res = sistema->registrarMarca(nombre, pre, stock, infoExtra, checkPromo->isChecked());
                break;

            case 2: // Controlado
            {
                bool ok;
                double dosis = QString::fromStdString(infoExtra).toDouble(&ok);

                if (!ok || dosis <= 0) {
                    QMessageBox::warning(&dialog, "Error", "Dosis invalida.");
                    return;
                }
                res = sistema->registrarControlado(nombre, pre, stock, spinNivel->value(), dosis);
            }
            break;
            }

            // Switch para manejar la respuesta del sistema
            switch (res) {

            case 0:
                QMessageBox::information(&dialog, "FarmaSystem", "Registrado correctamente.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
                break;
            case 1: QMessageBox::warning(&dialog, "FarmaSystem", "Faltan campos obligatorios."); break;
            case 2: QMessageBox::warning(&dialog, "FarmaSysten", "Precio o Stock invalidos."); break;
            case 3: QMessageBox::warning(&dialog, "FarmaSystem", "Nivel de control incorrecto."); break;
            default: QMessageBox::critical(&dialog, "FarmaSystem", "Error interno del sistema."); break;
          
            }  
        });

        dialog.exec();
    }

    void InventarioView::eliminarMedicamentoSeleccionado() {

        if (tabla->selectedItems().isEmpty()) return;

        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();

        if (QMessageBox::question(this, "FarmaSystem", "Eliminar medicamento?") == QMessageBox::Yes) {

            if (sistema->eliminarMedicamento(id)) {
                llenarTablaUI();
                emit datosActualizados();  // signal a MainWindow
            }
        }
    }

    void InventarioView::actualizarStockMedicamento() {

        if (tabla->selectedItems().isEmpty()) { return; }

        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();
        QString nombre = tabla->item(filaActual, 1)->text();

        bool ok;
        int cantidad = QInputDialog::getInt(this, "Actualizar Stock",
            "Ingrese cantidad para: " + nombre + "\n(Use negativos para restar):", 0, -10, 10, 1, &ok);

        if (ok == true) {

            int resultado = sistema->actualizarStock(id, cantidad);

            if (resultado == 0) {

                QMessageBox::information(this, "FarmaSystem", "Stock actualizado.");
                llenarTablaUI();
            }
            else if (resultado == 2) {

                QMessageBox::warning(this, "FarmaSystem", "La operacion resultaria en stock negativo.");
            }
        }
    }

    void InventarioView::filtrarMedicamentos(const QString& texto) {

        if (texto.isEmpty()) {
            llenarTablaUI();
            return;
        }

        tabla->setRowCount(0);

        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {

            Medicamento* medicamento = sistema->getMedicamentoPorIndice(i);

            if (medicamento != nullptr) {

                bool coincideNombre = QString::fromStdString(medicamento->getNombre()).contains(texto, Qt::CaseInsensitive);
                bool coincideID = QString::number(medicamento->getID()).contains(texto);

                if (coincideNombre == true || coincideID == true) {

                    agregarDatos(medicamento);
                }
            }
        }
    }

    void InventarioView::mostrarDetallesSeleccionado() {

		RecursosUI ui;

        if (tabla->selectedItems().isEmpty()) { return; }

        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();
        Medicamento* medicamento = sistema->buscarMedicamentoPorID(id);

        if (medicamento == nullptr) { return; }

        QDialog dialog(this);
        dialog.setWindowTitle("FarmaSystem");
        dialog.setMinimumWidth(400);

        ui.aplicarEstiloDialogo(&dialog);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // Construccion del texto 
        QString info = "DATOS GENERALES\n" + QString(30, '-') + "\n";
        info += "Nombre: " + QString::fromStdString(medicamento->getNombre()) + "\n";
        info += "Categoria: " + QString::fromStdString(medicamento->getCategoria()) + "\n";
        info += "Stock: " + QString::number(medicamento->getStock()) + "\n";
        info += "Precio: ₡" + QString::number(medicamento->getPrecio(), 'f', 2) + "\n\n";
        info += "DETALLES ESPECIFICOS\n" + QString(30, '-') + "\n";
        info += QString::fromStdString(medicamento->getInfoExtra()) + "\n\n";
        info += "⚠️ ADVERTENCIA\n" + QString::fromStdString(medicamento->getAdvertencia());

        QTextEdit* escritura = new QTextEdit();
        escritura->setReadOnly(true);
        escritura->setText(info);

        ui.aplicarEstiloTexto(escritura);

        QPushButton* botonCerrar = new QPushButton("Cerrar");

		ui.aplicarEstiloBoton(botonCerrar);

        connect(botonCerrar, &QPushButton::clicked, &dialog, &QDialog::accept);

        // Orden en pantalla
        layout->addWidget(escritura);
        layout->addWidget(botonCerrar);

        dialog.exec();
    }

    void InventarioView::agregarDatos(Medicamento* medicamento) {

        // Convierte datos del medicamento a QString para mostrar en la tabla
        QString id = QString::number(medicamento->getID());
        QString nombre = QString::fromStdString(medicamento->getNombre());
        QString categoria = QString::fromStdString(medicamento->getCategoria());
        QString stock = QString::number(medicamento->getStock());
        QString precio = QString::number(medicamento->calcularPrecioFinal(1), 'f', 2);

        // Obtener fila
        int filaActual = tabla->rowCount();

        // Insertar Fila
        tabla->insertRow(filaActual);

        // Agregar dato a cada celda de la fila
        tabla->setItem(filaActual, 0, new QTableWidgetItem(id));
        tabla->setItem(filaActual, 1, new QTableWidgetItem(nombre));
        tabla->setItem(filaActual, 2, new QTableWidgetItem(categoria));
        tabla->setItem(filaActual, 3, new QTableWidgetItem(stock));
        tabla->setItem(filaActual, 4, new QTableWidgetItem(precio));
    }

    bool InventarioView::eventFilter(QObject* obj, QEvent* event) {

        // limpia seleccion de la tabla al hacer focus en el buscador para evitar confusiones visuales
        if (obj == textoBuscarMedicamento && event->type() == QEvent::FocusIn)
        {
            tabla->clearSelection();
        }

        // Retorna el evento normalmente para que funcione todo como se espera
        return QWidget::eventFilter(obj, event);
    }

}
