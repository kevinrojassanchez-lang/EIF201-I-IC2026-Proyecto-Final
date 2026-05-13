#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "CategoriasView.h"
#include "RecursosUI.h"

namespace FarmaSystem {

    CategoriasView::CategoriasView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent), sistema(sSistema) {

        construirUI();
    }

    void CategoriasView::construirUI() {

        RecursosUI ui;

        QGridLayout* layoutPrincipal = new QGridLayout(this);

        ui.aplicarEstiloVentana(this);

        layoutPrincipal->setContentsMargins(30, 5, 30, 30);
        layoutPrincipal->setSpacing(15);

        QLabel* tituloSeccion = new QLabel("Categorias");
        tituloSeccion->setAlignment(Qt::AlignLeft );

        ui.aplicarTituloNeon(tituloSeccion);

        listaCategorias = new QListWidget;
        listaCategorias->addItems({ "Generico", "Marca", "Controlado" });

        ui.aplicarEstiloLista(listaCategorias);

        listaMedicamentosCategoria = new QListWidget;

        ui.aplicarEstiloListaVerde(listaMedicamentosCategoria);

        infoCategoria = new QTextEdit;
        infoCategoria->setReadOnly(true);

        QPushButton* botonVolver = new QPushButton("Volver");
        botonVolver->setCursor(Qt::PointingHandCursor);

        ui.aplicarEstiloBoton(botonVolver);

        timerBorde = new QTimer(this);
        listaColores << "#39FF14" << "#00FFFF" << "#FF00FF" << "#FFFF00";

        ui.animarBordeTexto(infoCategoria, listaColores, colorActualIdx);

        layoutPrincipal->addWidget(tituloSeccion, 0, 0, Qt::AlignTop | Qt::AlignLeft);
        layoutPrincipal->addWidget(new QLabel("<b>Categorias:</b>"), 1, 0);
        layoutPrincipal->addWidget(new QLabel("<b>Medicamentos:</b>"), 1, 1);
        layoutPrincipal->addWidget(listaCategorias, 2, 0, 3, 1);
        layoutPrincipal->addWidget(listaMedicamentosCategoria, 2, 1);
        layoutPrincipal->addWidget(new QLabel("<b>Informacion Detallada:</b>"), 3, 1);
        layoutPrincipal->addWidget(infoCategoria, 4, 1);
        layoutPrincipal->addWidget(botonVolver, 5, 0, 1, 2, Qt::AlignCenter);

        layoutPrincipal->setColumnStretch(0, 1);
        layoutPrincipal->setColumnStretch(1, 2);

        listaCategorias->setCurrentRow(0);
        actualizarVistaCategorias();

        timerBorde->start(500);

        // Conexiones
        connect(listaCategorias, &QListWidget::itemClicked, this, &CategoriasView::mostrarCategoriaSeleccionada);
        connect(listaMedicamentosCategoria, &QListWidget::itemClicked, this, &CategoriasView::mostrarMedicamentoCategoria);
        connect(botonVolver, &QPushButton::clicked, this, &CategoriasView::volverAlMenu);
        connect(timerBorde, &QTimer::timeout, this, &CategoriasView::animarBorde);
    }

    void CategoriasView::mostrarCategoriaSeleccionada() {

        if (listaCategorias->currentItem() != nullptr) {

            QString categoria = listaCategorias->currentItem()->text();
            listaMedicamentosCategoria->clear();
            infoCategoria->clear();

            for (int i = 0; i < sistema->getCantMedicamentos(); i++) {
                Medicamento* medicamento = sistema->getMedicamentoPorIndice(i);
                if (medicamento != nullptr) {

                    if (QString::fromStdString(medicamento->getCategoria()).compare(categoria, Qt::CaseInsensitive) == 0) {
                        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(medicamento->getNombre()));
                        item->setData(Qt::UserRole, medicamento->getID());
                        listaMedicamentosCategoria->addItem(item);
                    }
                }
            }
        }
    }

    void CategoriasView::mostrarMedicamentoCategoria() {

        if (listaMedicamentosCategoria->currentItem() != nullptr) {

            int id = listaMedicamentosCategoria->currentItem()->data(Qt::UserRole).toInt();
            Medicamento* medicamento = sistema->buscarMedicamentoPorID(id);

            if (medicamento != nullptr) {

                QString informacion;
                informacion += "⚠️ ADVERTENCIA:\n\n";
                informacion += QString::fromStdString(medicamento->getAdvertencia()) + "\n\n";

                double base = medicamento->getPrecio();
                double final = medicamento->calcularPrecioFinal(1);

                informacion += "🧠 LOGICA DE PRECIO:\n\n";
                informacion += "Precio Base (1 unidad): CRC " + QString::number(base, 'f', 2) + "\n";
                informacion += "Precio Fin  (1 unidad): CRC " + QString::number(final, 'f', 2) + "\n\n";
                informacion += QString::fromStdString(medicamento->getInfoPrecio());

                infoCategoria->setText(informacion);
            }
        }
    }

    void CategoriasView::animarBorde() {
           
       RecursosUI ui;
       ui.animarBordeTexto(infoCategoria, listaColores, colorActualIdx);
        
    }

    void CategoriasView::actualizarVistaCategorias() {
        mostrarCategoriaSeleccionada();
        listaMedicamentosCategoria->clearSelection();
        infoCategoria->clear();
    }

    void CategoriasView::actualizarVista() {
        actualizarVistaCategorias();
    }


} 