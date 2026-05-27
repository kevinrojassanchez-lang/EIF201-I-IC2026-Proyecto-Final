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

    ProveedoresView::ProveedoresView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent), sistema(sSistema)
    {
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

        tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
        tabla->setSelectionMode(QAbstractItemView::SingleSelection);
        tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

        connect(tabla, &QTableWidget::itemSelectionChanged,
            this, &ProveedoresView::actualizarEstadoBotones);
    }

    void ProveedoresView::llenarTablaUI() {

        tabla->setRowCount(0);
        tabla->clearSelection();

        for (int i = 0; i < sistema->getListaProveedores().cantidad(); i++) {

            Proveedor* p = sistema->getListaProveedores().obtener(i);

            if (p != nullptr) {
                agregarDatos(p);
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

        QVBoxLayout layout(&dialog);

        QLineEdit nombre, telefono, email, pais;

        nombre.setPlaceholderText("Nombre");
        telefono.setPlaceholderText("Telefono");
        email.setPlaceholderText("Email");
        pais.setPlaceholderText("Pais");

        QPushButton guardar("Guardar");

        layout.addWidget(&nombre);
        layout.addWidget(&telefono);
        layout.addWidget(&email);
        layout.addWidget(&pais);
        layout.addWidget(&guardar);

        connect(&guardar, &QPushButton::clicked, [&]() {

            std::string n = nombre.text().toStdString();
            std::string t = telefono.text().toStdString();
            std::string e = email.text().toStdString();
            std::string pa = pais.text().toStdString();

            if (n.empty() || t.empty() || e.empty() || pa.empty()) {
                QMessageBox::warning(this, "FarmaSystem", "Campos incompletos");
                return;
            }

            int r = sistema->registrarProveedor(n, t, e, pa);

            if (r == 0) {
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

        Proveedor* p = sistema->getListaProveedores().buscarPorId(id);
        if (!p) return;

        QDialog dialog(this);
        dialog.setWindowTitle("Editar Proveedor");

        QVBoxLayout layout(&dialog);

        QLineEdit nombre, telefono, email, pais;

        nombre.setText(QString::fromStdString(p->getNombre()));
        telefono.setText(QString::fromStdString(p->getTelefono()));
        email.setText(QString::fromStdString(p->getEmail()));
        pais.setText(QString::fromStdString(p->getPais()));

        QPushButton guardar("Guardar");

        layout.addWidget(&nombre);
        layout.addWidget(&telefono);
        layout.addWidget(&email);
        layout.addWidget(&pais);
        layout.addWidget(&guardar);

        connect(&guardar, &QPushButton::clicked, [&]() {

            if (nombre.text().isEmpty() || telefono.text().isEmpty()
                || email.text().isEmpty() || pais.text().isEmpty()) {
                QMessageBox::warning(this, "FarmaSystem", "Campos incompletos");
                return;
            }

            p->setNombre(nombre.text().toStdString());
            p->setTelefono(telefono.text().toStdString());
            p->setEmail(email.text().toStdString());
            p->setPais(pais.text().toStdString());

            llenarTablaUI();
            dialog.accept();
            });

        dialog.exec();
    }

    void ProveedoresView::eliminarProveedor() {

        if (tabla->selectedItems().isEmpty()) return;

        int fila = tabla->currentRow();
        int id = tabla->item(fila, 0)->text().toInt();

        if (QMessageBox::question(this, "FarmaSystem",
            "¿Eliminar proveedor?") == QMessageBox::Yes) {

            if (sistema->eliminarProveedor(id)) {
                llenarTablaUI();
            }
            else {
                QMessageBox::warning(this, "FarmaSystem",
                    "No se puede eliminar, tiene medicamentos asociados.");
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
