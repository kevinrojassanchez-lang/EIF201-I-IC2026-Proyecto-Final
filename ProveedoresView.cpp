#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QShowEvent>

#include "ProveedoresView.h"
#include "RecursosUI.h"

namespace FarmaSystem {

    ProveedoresView::ProveedoresView(SistemaFarmacia* sSistema, QWidget* parent) : 
        QWidget(parent), sistema(sSistema) {

        construirUI();
        llenarTablaUI();
    }

    void ProveedoresView::construirUI() {

        RecursosUI ui;

        QVBoxLayout* layout = new QVBoxLayout(this);

        QHBoxLayout* botonesLayout = new QHBoxLayout();

        titulo = new QLabel("Gestion de Proveedores");

        ui.aplicarTituloNeon(titulo);

        tabla = new QTableWidget;

        tabla->setColumnCount(5);
        tabla->setHorizontalHeaderLabels({ "ID", "Nombre", "Telefono", "Email", "Pais" });
        tabla->verticalHeader()->setVisible(false);
        tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
        tabla->setSelectionMode(QAbstractItemView::SingleSelection);
        tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tabla->setFocusPolicy(Qt::StrongFocus);
        tabla->setTabKeyNavigation(false);
        tabla->setShowGrid(true);

        ui.aplicarEstiloTabla(tabla);

        botonRegistrar = new QPushButton("Registrar");
        botonEditar = new QPushButton("Editar");
        botonEliminar = new QPushButton("Eliminar");
        botonVolver = new QPushButton("Volver");

        botonEditar->setEnabled(false);
        botonEliminar->setEnabled(false);

        ui.aplicarEstiloBoton(botonRegistrar);
        ui.aplicarEstiloBoton(botonEditar);
        ui.aplicarEstiloBoton(botonEliminar);
        ui.aplicarEstiloBoton(botonVolver);

        botonesLayout->addWidget(botonRegistrar);
        botonesLayout->addWidget(botonEditar);
        botonesLayout->addWidget(botonEliminar);
        botonesLayout->addWidget(botonVolver);

        layout->addWidget(titulo);
        layout->addWidget(tabla);
        layout->addLayout(botonesLayout);

        connect(botonRegistrar, &QPushButton::clicked, this, &ProveedoresView::abrirDialogRegistrar);
        connect(botonEditar, &QPushButton::clicked, this, &ProveedoresView::abrirDialogEditar);
        connect(botonEliminar, &QPushButton::clicked, this, &ProveedoresView::eliminarProveedor);
        connect(botonVolver, &QPushButton::clicked, this, &ProveedoresView::volverAlMenu);
        connect(tabla, &QTableWidget::itemSelectionChanged, this, &ProveedoresView::actualizarEstadoBotones);
    }

    void ProveedoresView::llenarTablaUI() {

        tabla->setRowCount(0);
        tabla->clearSelection();

        for (int i = 0; i < sistema->getCantProveedores(); i++) {

            Proveedor* proveedor = sistema->getProveedorPorIndice(i);

            if (proveedor != nullptr) {
                agregarDatos(proveedor);
            }
        }
        actualizarEstadoBotones();
    }

    void ProveedoresView::agregarDatos(Proveedor* p) {

        int fila = tabla->rowCount();

        tabla->insertRow(fila);

        tabla->setItem(fila, 0, new QTableWidgetItem(QString::number(p->getID())));
        tabla->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p->getNombre())));
        tabla->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p->getTelefono())));
        tabla->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(p->getEmail())));
        tabla->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(p->getPais())));
    }

    void ProveedoresView::actualizarEstadoBotones() {

        bool sel = !tabla->selectedItems().isEmpty();

        botonEditar->setEnabled(sel);
        botonEliminar->setEnabled(sel);
    }

    void ProveedoresView::abrirDialogRegistrar() {

        QDialog dialog(this);
        dialog.setWindowTitle("Registrar Proveedor");

        RecursosUI ui;
        ui.aplicarEstiloFormularioDialogo(&dialog);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        // Crear y estilizar etiquetas informativas
        QLabel* lblNombre = new QLabel("Nombre:");
        QLabel* lblTelefono = new QLabel("Telefono:");
        QLabel* lblEmail = new QLabel("Email:");
        QLabel* lblPais = new QLabel("Pais:");

        ui.aplicarLabelInfo(lblNombre);
        ui.aplicarLabelInfo(lblTelefono);
        ui.aplicarLabelInfo(lblEmail);
        ui.aplicarLabelInfo(lblPais);

        // Crear y estilizar cajas de texto usando los metodos esteticos de RecursosUI
        QLineEdit* nombre =   RecursosUI::crearCampoTexto("Ej: Distribuidora Medica");
        QLineEdit* telefono = RecursosUI::crearCampoTexto("Ej: 8888-8888");
        QLineEdit* email =    RecursosUI::crearCampoTexto("Ej: contacto@prov.com");
        QLineEdit* pais =     RecursosUI::crearCampoTexto("Ej: Costa Rica");

        QPushButton* guardar = new QPushButton("Guardar");
        ui.aplicarEstiloBoton(guardar);

        // Construccion ordenada del layout con espaciados discretos
        layout->addWidget(lblNombre);
        layout->addWidget(nombre);
        layout->addWidget(lblTelefono);
        layout->addWidget(telefono);
        layout->addWidget(lblEmail);
        layout->addWidget(email);
        layout->addWidget(lblPais);
        layout->addWidget(pais);
        layout->addSpacing(15); // Espacio 
        layout->addWidget(guardar);

        connect(guardar, &QPushButton::clicked, [&]() {

            std::string nNombre = nombre->text().trimmed().toStdString();
            std::string tTelefono = telefono->text().trimmed().toStdString();
            std::string eEmail = email->text().trimmed().toStdString();
            std::string pPais = pais->text().trimmed().toStdString();

            if (nNombre.empty() || tTelefono.empty() || eEmail.empty() || pPais.empty()) {
                QMessageBox::warning(this, "FarmaSystem", "Campos incompletos");
                return;
            }

            int resultado = sistema->registrarProveedor(nNombre, tTelefono, eEmail, pPais);

            if (resultado == 0) {
                QMessageBox::information(this, "FarmaSystem", "Proveedor registrado");
                llenarTablaUI();
                dialog.accept();
            }
            else {
                QMessageBox::warning(this, "FarmaSystem", "Error al registrar proveedor");
            }
        });

        dialog.exec();
    }

    void ProveedoresView::abrirDialogEditar() {

        if (tabla->selectedItems().isEmpty()) return;

        int fila = tabla->currentRow();
        int id = tabla->item(fila, 0)->text().toInt();

        Proveedor* proveedor = sistema->buscarProveedorPorID(id);
        if (!proveedor) { return; }

        QDialog dialog(this);
        dialog.setWindowTitle("Editar Proveedor");

        RecursosUI ui;
        ui.aplicarEstiloFormularioDialogo(&dialog);

        QVBoxLayout layout(&dialog);

        // Crear y estilizar etiquetas informativas
        QLabel* lblNombre = new QLabel("Nombre:");
        QLabel* lblTelefono = new QLabel("Telefono:");
        QLabel* lblEmail = new QLabel("Email:");
        QLabel* lblPais = new QLabel("Pais:");

        ui.aplicarLabelInfo(lblNombre);
        ui.aplicarLabelInfo(lblTelefono);
        ui.aplicarLabelInfo(lblEmail);
        ui.aplicarLabelInfo(lblPais);

        // Crear y estilizar cajas de texto
        QLineEdit nombre, telefono, email, pais;
        ui.aplicarEstiloCamposFormulario(&nombre);
        ui.aplicarEstiloCamposFormulario(&telefono);
        ui.aplicarEstiloCamposFormulario(&email);
        ui.aplicarEstiloCamposFormulario(&pais);

        // Cargar datos actuales del modelo
        nombre.setText(QString::fromStdString(proveedor->getNombre()));
        telefono.setText(QString::fromStdString(proveedor->getTelefono()));
        email.setText(QString::fromStdString(proveedor->getEmail()));
        pais.setText(QString::fromStdString(proveedor->getPais()));

        QPushButton guardar("Guardar Cambios");
        ui.aplicarEstiloBoton(&guardar);

        // Construccion ordenada del layout con espaciados
        layout.addWidget(lblNombre);
        layout.addWidget(&nombre);
        layout.addWidget(lblTelefono);
        layout.addWidget(&telefono);
        layout.addWidget(lblEmail);
        layout.addWidget(&email);
        layout.addWidget(lblPais);
        layout.addWidget(&pais);
        layout.addSpacing(15);
        layout.addWidget(&guardar);

        connect(&guardar, &QPushButton::clicked, [&]() {

            if (nombre.text().isEmpty() || telefono.text().isEmpty() ||
                email.text().isEmpty() || pais.text().isEmpty()) {
                QMessageBox::warning(this, "FarmaSystem", "Campos incompletos");
                return;
            }

            std::string nNombre = nombre.text().trimmed().toStdString();
            std::string tTelefono = telefono.text().trimmed().toStdString();
            std::string eEmail = email.text().trimmed().toStdString();
            std::string pPais = pais.text().trimmed().toStdString();

            if (sistema->editarProveedor(id, nNombre, tTelefono, eEmail, pPais)) {
                QMessageBox::information(this, "FarmaSystem", "Proveedor editado con exito");
                llenarTablaUI();
                dialog.accept();
            }
            else {
                QMessageBox::warning(this, "FarmaSystem", "Datos invalidos");
            }
        });

        dialog.exec();
    }

    void ProveedoresView::eliminarProveedor() {

        if (tabla->selectedItems().isEmpty()) return;

        int fila = tabla->currentRow();
        int id = tabla->item(fila, 0)->text().toInt();

        if (QMessageBox::question(this, "FarmaSystem", "Eliminar proveedor") == QMessageBox::Yes) {

            if (sistema->eliminarProveedor(id)) {
                llenarTablaUI();
            }

            else {
                QMessageBox::warning(this,
                    "FarmaSystem", "No se puede eliminar, tiene medicamentos asociados.");
            }
        }
    }

    void ProveedoresView::showEvent(QShowEvent* event) {

        QWidget::showEvent(event);

        tabla->clearSelection();

        tabla->setCurrentCell(-1, -1);

        actualizarEstadoBotones();
    }

} // namespace FarmaSystem
