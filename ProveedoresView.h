#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class ProveedoresView : public QWidget {

        Q_OBJECT

    private:

        SistemaFarmacia* sistema;

        QTableWidget* tabla;

        QPushButton* botonRegistrar;
        QPushButton* botonEditar;
        QPushButton* botonEliminar;
        QPushButton* botonVolver;

        QLabel* titulo;

	protected:
		void showEvent(QShowEvent* event) override;

    public:

        ProveedoresView(SistemaFarmacia* sSistema, QWidget* parent = nullptr);

        void construirUI();
        void llenarTablaUI();
        void agregarDatos(Proveedor* proveedor);

        void actualizarEstadoBotones();

        void abrirDialogRegistrar();
        void abrirDialogEditar();
        void eliminarProveedor();


    signals:

        void volverAlMenu();

    };

}

