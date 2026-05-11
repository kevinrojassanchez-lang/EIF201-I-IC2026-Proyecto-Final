#ifndef CLIENTESVIEW_H
#define CLIENTESVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QEvent> 
#include <QTimer>
#include <QPushButton>
#include <QVariantAnimation>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class ClientesView : public QWidget {
        Q_OBJECT

    private:
        SistemaFarmacia* sistema;
        QTableWidget* tabla;
        QLineEdit* textoBuscarCliente;
        QPushButton* botonEliminar;
        QPushButton* botonFidelidad;

        void construirUI();
        void llenarTablaUI();
        void agregarDatos(Cliente* cliente);

    protected:
        bool eventFilter(QObject* obj, QEvent* event) override;
        void showEvent(QShowEvent* event) override;

    public:
        ClientesView(SistemaFarmacia* sistema, QWidget* parent = nullptr);

    private slots:
        void eliminarClienteSeleccionado();
        void abrirDialogRegistrarCliente();
        void toggleFidelidadCliente();
        void filtrarClientes(const QString& texto);
        void actualizarEstadoBotones();

    signals:   
        void volverAlMenu();
		void datosActualizados();

    };

}

#endif
