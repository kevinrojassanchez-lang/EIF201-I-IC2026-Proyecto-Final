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
#include "cargadorDatosUI.h"

namespace FarmaSystem {

    InventarioView::InventarioView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent),
        sistema(sSistema) {

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

        tabla->setColumnCount(6);
        tabla->setHorizontalHeaderLabels({ "ID", "Nombre", "Categoria", "Stock", "Precio", "Proveedor" });
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
        botonEditar = new QPushButton("Editar");
        QPushButton* botonRegistrar = new QPushButton("Registrar");
        QPushButton* botonVolver = new QPushButton("Volver");

        ui.aplicarEstiloBoton(botonEliminar);
		ui.aplicarEstiloBoton(botonEditar);
        ui.aplicarEstiloBoton(botonRegistrar);
        ui.aplicarEstiloBoton(botonVolver);

        botonesLayout->addWidget(botonRegistrar);
        botonesLayout->addWidget(botonEditar);
        botonesLayout->addWidget(botonEliminar);
        botonesLayout->addWidget(botonVolver);

        layout->addLayout(layoutBusqueda);
        layout->addWidget(tabla);
        layout->addLayout(botonesLayout);

        // Conexiones
        connect(botonRegistrar, &QPushButton::clicked, this, &InventarioView::abrirDialogRegistrarMedicamento);
        connect(botonEditar, &QPushButton::clicked, this, &InventarioView::abrirDialogEditarMedicamento);
        connect(botonEliminar, &QPushButton::clicked, this, &InventarioView::eliminarMedicamentoSeleccionado);
        connect(tabla, &QTableWidget::itemSelectionChanged, this, &InventarioView::actualizarEstadoBotones);
        connect(textoBuscarMedicamento, &QLineEdit::textChanged, this, &InventarioView::filtrarMedicamentos);
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
        botonEditar->setEnabled(tieneSeleccion);

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

        if (sistema->getCantProveedores() == 0) {
            QMessageBox::warning(this, "FarmaSystem", "Debe registrar al menos un proveedor");
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("FarmaSystem - Registrar Medicamento");
        dialog.setMinimumWidth(400);

        // Instanciar RecursosUI solo para estilos generales del dialogo y elementos especificos
        RecursosUI ui;
        ui.aplicarEstiloFormularioDialogo(&dialog);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        QLabel* lblDatosBasicos = ui.crearSeccionNeon("Datos Basicos", "#ff4d4d");
        QLabel* lblCategoria = ui.crearSeccionNeon("Categoria", "#ff4d4d");
        QLabel* lblProveedor = ui.crearSeccionNeon("Proveedor", "#ff4d4d");
        QLineEdit* espacioNombre = ui.crearCampoTexto("Nombre del medicamento");
        QDoubleSpinBox* espacioPrecio = ui.crearDoubleSpin(0, 1000000, "Precio: CRC ");
        QSpinBox* espacioStock = ui.crearSpin(0, 1000, "Stock: ");
        QCheckBox* checkReceta = ui.crearCheck("Requiere receta medica");
        QCheckBox* checkPromo = ui.crearCheck("Promocion (15% desc)");
        QComboBox* espacioTipo = ui.crearCombo();

        espacioTipo->addItems({ "Generico", "Marca", "Controlado" });

        QComboBox* espacioProveedor = ui.crearCombo();

		// Recopilar datos de proveedores para el combo
        QStringList nombresProveedores;
        QList<int> idsProveedores;

        for (int i = 0; i < sistema->getCantProveedores(); i++) {
            Proveedor* proveedor = sistema->getProveedorPorIndice(i);
            if (proveedor != nullptr) {
                nombresProveedores.append(QString::fromStdString(proveedor->getNombre()));
                idsProveedores.append(proveedor->getID());
            }
        }

        // Plasmar los datos en el combo usando el Cargador utilitario bajo la etiqueta EIF201
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(espacioProveedor, nombresProveedores, idsProveedores,
            "Seleccione un proveedor...");

        // Campos dinamicos optimizados
        QLabel* labelDinamico = new QLabel("Principio Activo:");
        ui.aplicarLabelInfo(labelDinamico);

        QLineEdit* editDinamico = ui.crearCampoTexto("");

        QLabel* labelNivel = new QLabel("Nivel de Control [1-4]:");
        ui.aplicarLabelInfo(labelNivel);

        QSpinBox* spinNivel = ui.crearSpin(1, 4, "");

        QPushButton* botonGuardar = new QPushButton("Guardar");
        ui.aplicarEstiloBoton(botonGuardar);

        // Estado inicial de dinamicos
        checkPromo->hide();
        labelNivel->hide();
        spinNivel->hide();

        // Construccion ordenada del Layout
        layout->addWidget(lblDatosBasicos);
        layout->addWidget(espacioNombre);
        layout->addWidget(espacioPrecio);
        layout->addWidget(espacioStock);
        layout->addSpacing(10);

        layout->addWidget(lblCategoria);
        layout->addWidget(espacioTipo);
        layout->addSpacing(10);

        layout->addWidget(lblProveedor);
        layout->addWidget(espacioProveedor);
        layout->addWidget(checkReceta);
        layout->addSpacing(5);

        layout->addWidget(checkPromo);
        layout->addWidget(labelDinamico);
        layout->addWidget(editDinamico);
        layout->addWidget(labelNivel);
        layout->addWidget(spinNivel);
        layout->addSpacing(15);
        layout->addWidget(botonGuardar);

        // Evento de cambio de tipo de medicamento 
        connect(espacioTipo, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index) {
            checkPromo->setVisible(index == 1);
            labelNivel->setVisible(index == 2);
            spinNivel->setVisible(index == 2);

            switch (index) {
            case 0: // Generico
                labelDinamico->setText("Principio Activo:");
                editDinamico->setPlaceholderText("Ej: Ibuprofeno");
                checkReceta->setEnabled(true);
                break;
            case 1: // Marca
                labelDinamico->setText("Pais de Origen:");
                editDinamico->setPlaceholderText("Ej: Costa Rica");
                checkReceta->setEnabled(true);
                break;
            case 2: // Controlado
                labelDinamico->setText("Dosis Maxima (mg):");
                editDinamico->setPlaceholderText("Ej: 500");
                checkReceta->setChecked(true);
                checkReceta->setEnabled(false);
                break;
            }
            });

        espacioTipo->setCurrentIndex(0);

        // Guardado
        connect(botonGuardar, &QPushButton::clicked, [&]() {
            std::string nombre = espacioNombre->text().trimmed().toStdString();
            double precio = espacioPrecio->value();
            int stock = espacioStock->value();
            std::string infoExtra = editDinamico->text().trimmed().toStdString();

            if (nombre.empty()) {
                QMessageBox::warning(&dialog, "Error", "El nombre es obligatorio.");
                return;
            }

            int indexProv = espacioProveedor->currentIndex();
            int idProv = espacioProveedor->currentData().toInt();

            if (indexProv <= 0 || idProv <= 0) {
                QMessageBox::warning(&dialog, "Error", "Debe seleccionar un proveedor valido.");
                return;
            }

            Proveedor* prov = sistema->buscarProveedorPorID(idProv);
            if (prov == nullptr) {
                QMessageBox::critical(&dialog, "Error", "El proveedor seleccionado no existe.");
                return;
            }

            int resultado = -1;

            switch (espacioTipo->currentIndex()) {
            case 0:
                resultado = sistema->registrarGenerico(nombre, precio, stock, infoExtra, idProv, checkReceta->isChecked());
                break;
            case 1:
                resultado = sistema->registrarMarca(nombre, precio, stock, infoExtra, checkPromo->isChecked(), idProv, checkReceta->isChecked());
                break;
            case 2: {
                bool ok = false;
                double dosis = QString::fromStdString(infoExtra).toDouble(&ok);
                if (!ok || dosis <= 0) {
                    QMessageBox::warning(&dialog, "Error", "Dosis invalida.");
                    return;
                }
                resultado = sistema->registrarControlado(nombre, precio, stock, spinNivel->value(), dosis, idProv, checkReceta->isChecked());
                break;
            }
            }

            switch (resultado) {
            case 0:
                QMessageBox::information(&dialog, "FarmaSystem", "Registrado correctamente.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
                break;
            case 1: QMessageBox::warning(&dialog, "FarmaSystem", "Faltan campos obligatorios."); break;
            case 2: QMessageBox::warning(&dialog, "FarmaSystem", "Precio o stock invalidos."); break;
            case 3: QMessageBox::warning(&dialog, "FarmaSystem", "Nivel de control incorrecto."); break;
            default: QMessageBox::critical(&dialog, "FarmaSystem", "Error interno del sistema."); break;
            }
            });

        dialog.exec();
    }

    void InventarioView::abrirDialogEditarMedicamento() {

        if (tabla->selectedItems().isEmpty()) {
            return;
        }

        int fila = tabla->currentRow();
        int id = tabla->item(fila, 0)->text().toInt();

        Medicamento* medicamento = sistema->buscarMedicamentoPorID(id);
        if (medicamento == nullptr) {
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("Editar Medicamento");
        dialog.setMinimumWidth(400);

        // Inicializar estilos de la ventana emergente
        RecursosUI ui;
        ui.aplicarEstiloFormularioDialogo(&dialog);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // Etiquetas fijas estilizadas
        QLabel* lblNombre = new QLabel("Nombre del Medicamento:");
        QLabel* lblPrecio = new QLabel("Precio Unitario:");
        QLabel* lblStock = new QLabel("Ajustar Stock:");
        QLabel* lblTipo = new QLabel("Tipo de Medicamento (No editable):");
        QLabel* lblProveedor = new QLabel("Proveedor Asignado:");

        ui.aplicarLabelInfo(lblNombre);
        ui.aplicarLabelInfo(lblPrecio);
        ui.aplicarLabelInfo(lblStock);
        ui.aplicarLabelInfo(lblTipo);
        ui.aplicarLabelInfo(lblProveedor);

        // Inicializar componentes graficos usando los metodos de RecursosUI
        QLineEdit* espacioNombre =      ui.crearCampoTexto("");
        QDoubleSpinBox* espacioPrecio = ui.crearDoubleSpin(0, 1000000, "Precio:CRC ");
        QSpinBox* espacioStock =        ui.crearSpin(0, 10000, "");
        QCheckBox* checkReceta =        ui.crearCheck("Requiere receta medica");
        QComboBox* espacioProveedor =   ui.crearCombo();

        QStringList nombresProveedores;
        QList<int> idsProveedores;

        for (int i = 0; i < sistema->getCantProveedores(); i++) {
            Proveedor* p = sistema->getProveedorPorIndice(i);
            if (p != nullptr) {
                nombresProveedores.append(QString::fromStdString(p->getNombre()));
                idsProveedores.append(p->getID());
            }
        }

        // Plasmar la ráfaga de datos en el combo usando el Cargador bajo la etiqueta de seguridad EIF201
        EIF201::CargadorDatosUI::plasmarDatosEnCombo(espacioProveedor, nombresProveedores, idsProveedores, "Seleccione proveedor...");

        QComboBox* tipo = ui.crearCombo();
        tipo->addItems({ "Generico", "Marca", "Controlado" });

        // Componentes dinamicos optimizados
        QLabel* labelExtra = new QLabel();
        ui.aplicarLabelInfo(labelExtra);

        QLineEdit* editExtra = ui.crearCampoTexto("");
        QCheckBox* checkPromo = ui.crearCheck("Promocion:");

        QLabel* labelNivel = new QLabel("Nivel control:");
        ui.aplicarLabelInfo(labelNivel);

        QSpinBox* spinNivel = ui.crearSpin(1, 4, "");

        QPushButton* guardar = new QPushButton("Guardar Cambios");
        ui.aplicarEstiloBoton(guardar);

        // Cargar datos actuales del modelo
        espacioNombre->setText(QString::fromStdString(medicamento->getNombre()));
        espacioPrecio->setValue(medicamento->getPrecio());
        espacioStock->setValue(medicamento->getStock());
        checkReceta->setChecked(medicamento->getReceta());
        checkReceta->setEnabled(true);

        int idProvActual = medicamento->getIdProveedor();
        for (int i = 0; i < espacioProveedor->count(); i++) {
            if (espacioProveedor->itemData(i).toInt() == idProvActual) {
                espacioProveedor->setCurrentIndex(i);
                break;
            }
        }

        // Distribucion visual ordenada del Layout con espaciados
        layout->addWidget(lblNombre);
        layout->addWidget(espacioNombre);
        layout->addWidget(lblPrecio);
        layout->addWidget(espacioPrecio);
        layout->addWidget(lblStock);
        layout->addWidget(espacioStock);
        layout->addSpacing(5);
        layout->addWidget(checkReceta);
        layout->addSpacing(5);

        layout->addWidget(lblTipo);
        layout->addWidget(tipo);
        layout->addWidget(lblProveedor);
        layout->addWidget(espacioProveedor);
        layout->addSpacing(5);

        layout->addWidget(checkPromo);
        layout->addWidget(labelExtra);
        layout->addWidget(editExtra);
        layout->addWidget(labelNivel);
        layout->addWidget(spinNivel);

        layout->addSpacing(15);
        layout->addWidget(guardar);

        // Lambda de actualizacian dinamica de la UI
        auto actualizarUI = [&](int index) {
            checkPromo->setVisible(index == 1);
            labelNivel->setVisible(index == 2);
            spinNivel->setVisible(index == 2);

            switch (index) {
            case 0:
                labelExtra->setText("Principio activo:");
                break;
            case 1:
                labelExtra->setText("Pais origen:");
                break;
            case 2:
                labelExtra->setText("Dosis maxima:");
                checkReceta->setChecked(true);
                checkReceta->setEnabled(false);
                break;
            }
        };

        connect(tipo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int i) { actualizarUI(i); });

        // Configuracion inicial basada en la herencia actual
        std::string catMedicamento = medicamento->getCategoria();

        if (catMedicamento == "Generico") {
            tipo->setCurrentIndex(0);
            actualizarUI(0);
            editExtra->setText(QString::fromStdString(medicamento->getPrincipioActivo()));
        }
        else if (catMedicamento == "Marca") {
            tipo->setCurrentIndex(1);
            actualizarUI(1);
            editExtra->setText(QString::fromStdString(medicamento->getPaisOrigen()));
            checkPromo->setChecked(medicamento->getPromocion());
        }
        else if (catMedicamento == "Controlado") {
            tipo->setCurrentIndex(2);
            actualizarUI(2);
            spinNivel->setValue(medicamento->getNivelControl());
            editExtra->setText(QString::number(medicamento->getDosisMaxima()));
        }

        // Congelar el tipo para mantener rigida la jerarquía de herencia al editar
        tipo->setEnabled(false);

        // Guardado y logica de negocio
        connect(guardar, &QPushButton::clicked, this, [=, &dialog]() {

            std::string nombre = espacioNombre->text().trimmed().toStdString();
            double precio = espacioPrecio->value();
            int stock = espacioStock->value();
            bool receta = checkReceta->isChecked();
            int idProveedor = espacioProveedor->currentData().toInt();
            std::string extra = editExtra->text().trimmed().toStdString();

            if (nombre.empty()) {
                QMessageBox::warning(&dialog, "FarmaSystem", "El nombre es obligatorio.");
                return;
            }

            int resultado = -1;

            switch (tipo->currentIndex()) {
            case 0:
                resultado = sistema->editarGenerico(id, nombre, precio, stock, receta, extra, idProveedor);
                break;

            case 1:
                resultado = sistema->editarMarca(id, nombre, precio, stock, receta, extra, checkPromo->isChecked(), idProveedor);
                break;

            case 2:
            {
                bool ok = false;
                double dosis = QString::fromStdString(extra).toDouble(&ok);

                if (!ok || dosis <= 0) {
                    QMessageBox::warning(&dialog, "FarmaSystem", "Dosis invalida");
                    return;
                }
                resultado = sistema->editarControlado(id, nombre, precio, stock, receta, spinNivel->value(), dosis, idProveedor);
            }
            break;
            }

            switch (resultado) {
            case 0:
                sistema->guardarDatos();
                QMessageBox::information(&dialog, "FarmaSystem", "Medicamento actualizado.");
                llenarTablaUI();
                emit datosActualizados();
                dialog.accept();
                break;

            case 1: QMessageBox::warning(&dialog, "FarmaSystem", "Medicamento no encontrado"); break;

            default: QMessageBox::critical(&dialog, "FarmaSystem", "Error interno del sistema."); break;
            }
        });

        dialog.exec();
    }

    void InventarioView::eliminarMedicamentoSeleccionado() {

        if (tabla->selectedItems().isEmpty()) { return; }

        int filaActual = tabla->currentRow();
        int id = tabla->item(filaActual, 0)->text().toInt();

        if (QMessageBox::question(this, "FarmaSystem", "Eliminar medicamento") == QMessageBox::Yes) {

            if (sistema->eliminarMedicamento(id)) {

				QMessageBox::information(this, "FarmaSystem", "Medicamento eliminado.");
                llenarTablaUI();
                emit datosActualizados();  // signal a MainWindow
            }
            else {
				QMessageBox::warning(this,
                    "FarmaSystem", "No se puede eliminar el medicamento.\n Verifique ID o historial de ventas");
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

    void InventarioView::agregarDatos(Medicamento* medicamento) {

        // Convierte datos del medicamento a QString para mostrar en la tabla
        QString id = QString::number(medicamento->getID());
        QString nombre = QString::fromStdString(medicamento->getNombre());
        QString categoria = QString::fromStdString(medicamento->getCategoria());
        QString stock = QString::number(medicamento->getStock());
        QString precio = QString::number(medicamento->calcularPrecioFinal(1), 'f', 2);

        // Obtener fila
        int filaActual = tabla->rowCount();

        // Insertar fila
        tabla->insertRow(filaActual);

        // Agregar dato a cada columna de la fila
        tabla->setItem(filaActual, 0, new QTableWidgetItem(id));
        tabla->setItem(filaActual, 1, new QTableWidgetItem(nombre));
        tabla->setItem(filaActual, 2, new QTableWidgetItem(categoria));
        tabla->setItem(filaActual, 3, new QTableWidgetItem(stock));
        tabla->setItem(filaActual, 4, new QTableWidgetItem(precio));

		// nuevo
        Proveedor* proveedor = sistema->buscarProveedorPorID(medicamento->getIdProveedor());

        QString nombreProv = (proveedor != nullptr) ? QString::fromStdString(proveedor->getNombre()) : "Null";

        tabla->setItem(filaActual, 5, new QTableWidgetItem(nombreProv));
    }

    bool InventarioView::eventFilter(QObject* obj, QEvent* event) {

        if (obj == textoBuscarMedicamento && event->type() == QEvent::FocusIn) { tabla->clearSelection(); }

        return QWidget::eventFilter(obj, event);
    }
} // namespace FarmaSystem