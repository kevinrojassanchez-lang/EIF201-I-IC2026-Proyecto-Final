#ifndef INVENTARIOVIEW_H
#define INVENTARIOVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QEvent>

#include "SistemaFarmacia.h"

namespace FarmaSystem {

    class InventarioView : public QWidget {
        Q_OBJECT

    private:
        SistemaFarmacia* sistema;
        QTableWidget* tabla;
        QLineEdit* textoBuscarMedicamento;
        QPushButton* botonEliminar;
        QPushButton* botonStock;
        QPushButton* botonDetalles;
        QLabel* tituloSeccion;

        void construirUI();
        void llenarTablaUI();
        void agregarDatos(Medicamento* medicamento);
        QString getEstiloBotones() const;

    protected:
        bool eventFilter(QObject* obj, QEvent* event) override;
        void showEvent(QShowEvent* event) override;

    public:
        InventarioView(SistemaFarmacia* sistema, QWidget* parent = nullptr);

    private slots:
        void actualizarEstadoBotones();
        void abrirDialogRegistrarMedicamento();
        void eliminarMedicamentoSeleccionado();
        void actualizarStockMedicamento();
        void mostrarDetallesSeleccionado();
        void filtrarMedicamentos(const QString& texto);

    signals:
		void volverAlMenu();
        void datosActualizados();
    };
}

#endif
