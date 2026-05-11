#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QGraphicsDropShadowEffect>
#include <QVariantAnimation>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextedit>

#include "InventarioView.h"

namespace FarmaSystem {

    InventarioView::InventarioView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent), sistema(sSistema) {
        construirUI();
        llenarTablaUI();
    }

    void InventarioView::construirUI() {
        // Crear espacios
        QVBoxLayout* layout = new QVBoxLayout(this);
        QHBoxLayout* layoutBusqueda = new QHBoxLayout();
        QHBoxLayout* botonesLayout = new QHBoxLayout();

        // Diseno titulo
        tituloSeccion = new QLabel("Gestion de Inventario");
        tituloSeccion->setStyleSheet("font-weight: bold; font-size: 20px; color: white; letter-spacing: 1px;");
        QGraphicsDropShadowEffect* neonEfecto = new QGraphicsDropShadowEffect();
        neonEfecto->setBlurRadius(35);
        neonEfecto->setColor(QColor(57, 255, 20)); // verde inicial
        neonEfecto->setOffset(0, 0);

        tituloSeccion->setGraphicsEffect(neonEfecto);

        QVariantAnimation* animacion = new QVariantAnimation(this);

        animacion->setDuration(1000);
        animacion->setStartValue(QColor(57, 255, 20));
        animacion->setEndValue(QColor(255, 49, 49));
        animacion->setLoopCount(-1);

        // Diseno buscador
        textoBuscarMedicamento = new QLineEdit();
        textoBuscarMedicamento->setPlaceholderText("🔍 Buscar por nombre o ID...");
        textoBuscarMedicamento->setMaximumWidth(250);
		textoBuscarMedicamento->installEventFilter(this);
        textoBuscarMedicamento->setStyleSheet(
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
        tabla->setStyleSheet(
            "QTableWidget {"
            "  background-color: #2b2b2b;"
            "  color: #e0e0e0;"
            "  gridline-color: #ff4d4d;"
            "  outline: 0;"
            "  border: 2px solid #ff4d4d;" // borde exterior en rojo pastel
            "  border-radius: 5px;"
            "}"

            "QTableWidget:focus {"
			"  border: 2px solid #39FF14;"   // verde al enfocar
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

        // Diseno botones
        botonEliminar = new QPushButton("Eliminar");
        botonStock = new QPushButton("Stock");
        botonDetalles = new QPushButton("Detalles");
        QPushButton* botonRegistrar = new QPushButton("Registrar");
        QPushButton* botonVolver = new QPushButton("Volver");

        botonEliminar->setStyleSheet(getEstiloBotones());
        botonStock->setStyleSheet(getEstiloBotones());
        botonDetalles->setStyleSheet(getEstiloBotones());
        botonRegistrar->setStyleSheet(getEstiloBotones());
        botonVolver->setStyleSheet(getEstiloBotones());

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
        connect(animacion, &QVariantAnimation::valueChanged, [=](const QVariant& v) { neonEfecto->setColor(v.value<QColor>()); });

		QTimer::singleShot(0, this, [=]() { this->setFocus(); }); //limpiar el foco al terminar de construir UI

        animacion->start();

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

        bool tieneSeleccion = !tabla->selectedItems().isEmpty();

        botonEliminar->setEnabled(tieneSeleccion);
        botonStock->setEnabled(tieneSeleccion);
        botonDetalles->setEnabled(tieneSeleccion);

        // efecto visual del grid al seleccionar
        if (tieneSeleccion) {

            tabla->setStyleSheet(tabla->styleSheet() + "QTableWidget { gridline-color: #39FF14; }");
        }
        else {

            tabla->setStyleSheet(tabla->styleSheet() + "QTableWidget { gridline-color: #ff4d4d; }");
        }
    }

    void InventarioView::showEvent(QShowEvent* event) {

        QWidget::showEvent(event); // Llama a la implementacion base
        tabla->clearSelection();
        tabla->setCurrentCell(-1, -1);
        textoBuscarMedicamento->clear();
        this->setFocus();

        // Color del grid
        tabla->setStyleSheet(tabla->styleSheet() + "QTableWidget { gridline-color: #ff4d4d; }");

        actualizarEstadoBotones(); // Asegura que los botones se bloqueen
    }

    void InventarioView::abrirDialogRegistrarMedicamento() {
        QDialog dialog(this);
        dialog.setWindowTitle("FarmaSystem");
        dialog.setMinimumWidth(400);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // campos comunes
        QLineEdit* escribirNombre = new QLineEdit();
        escribirNombre->setPlaceholderText("Nombre del medicamento");

        QDoubleSpinBox* spinPrecio = new QDoubleSpinBox();
        spinPrecio->setRange(0, 1000000);
        spinPrecio->setPrefix("Precio: CRC ");

        QSpinBox* spinStock = new QSpinBox();
        spinStock->setRange(0, 1000);
        spinStock->setPrefix("Stock: ");

        QComboBox* comboTipo = new QComboBox();
        comboTipo->addItems({ "Generico", "De Marca", "Controlado" });

        // Campos dinamicos
        QLabel* labelDinamico = new QLabel("Principio Activo:");
        QLineEdit* editDinamico = new QLineEdit();
        QCheckBox* checkPromo = new QCheckBox("Aplicar Promocion (15% desc)");
        QLabel* labelNivel = new QLabel("Nivel de Control (1-4):");
        QSpinBox* spinNivel = new QSpinBox();
        spinNivel->setRange(1, 4);

        // estado inicial bloqueados por defecto
        checkPromo->hide();
        labelNivel->hide();
        spinNivel->hide();

        // Construcion
        layout->addWidget(new QLabel("<b>Datos Basicos</b>"));
        layout->addWidget(escribirNombre);
        layout->addWidget(spinPrecio);
        layout->addWidget(spinStock);
        layout->addWidget(new QLabel("<b>Categoria</b>"));
        layout->addWidget(comboTipo);
        layout->addWidget(checkPromo);
        layout->addWidget(labelDinamico);
        layout->addWidget(editDinamico);
        layout->addWidget(labelNivel);
        layout->addWidget(spinNivel);

        QPushButton* botonGuardar = new QPushButton("Guardar");
        layout->addWidget(botonGuardar);

        // Campos Dinamicos
        connect(comboTipo, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
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
            std::string nombre = escribirNombre->text().trimmed().toStdString();
            double pre = spinPrecio->value();
            int sto = spinStock->value();
            std::string infoExtra = editDinamico->text().trimmed().toStdString();
            int res = -1;

            if (nombre.empty()) {
                QMessageBox::warning(&dialog, "Error", "El nombre es obligatorio.");
                return;
            }

            // Switch para decidir metodo del sistema a llamar
            switch (comboTipo->currentIndex()) {
            case 0: // Generico
                res = sistema->registrarGenerico(nombre, pre, sto, infoExtra);
                break;

            case 1: // Marca
                res = sistema->registrarMarca(nombre, pre, sto, infoExtra, checkPromo->isChecked());
                break;

            case 2: // Controlado
            {
                bool ok;
                double dosis = QString::fromStdString(infoExtra).toDouble(&ok);
                if (!ok || dosis <= 0) {
                    QMessageBox::warning(&dialog, "Error", "Dosis invalida.");
                    return;
                }
                res = sistema->registrarControlado(nombre, pre, sto, spinNivel->value(), dosis);
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
        if (tabla->selectedItems().isEmpty()) return;

        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();
        Medicamento* medicamento = sistema->buscarMedicamentoPorID(id);
        if (!medicamento) return;

        QDialog dialog(this);
        dialog.setWindowTitle("FarmaSystem");
        dialog.setMinimumWidth(400);
        dialog.setStyleSheet("background-color: #2b2b2b; color: #e0e0e0;"); // Fondo oscuro

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
        escritura->setStyleSheet("background-color: #1a1a1a; color: #b2fab4; border: 1px solid #39FF14; font-family: 'Courier New';");

        QPushButton* botonCerrar = new QPushButton("Cerrar");
        botonCerrar->setStyleSheet(getEstiloBotones());
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

    QString FarmaSystem::InventarioView::getEstiloBotones() const {
		    // Al estar en normal
        return "QPushButton {"
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
            "  color: #39FF14;"                  // Verde 
            "  border: 2px solid #39FF14;"
            "}"

            // Al hacer un clic
            "QPushButton:pressed {"
            "  background-color: #4d0000;"      //  rojo oscuro
            "  color: #ff4d4d;"                 //  rojo
            "  border: 2px solid #ff4d4d;"
            "}"

            // Cuando el boton esta bloqueado 
            "QPushButton:disabled {"
            "  background-color: #1a1a1a;"
            "  color: #444444;"                 // gris 
            "  border: 2px solid #333333;"
            "}";
    }
}
