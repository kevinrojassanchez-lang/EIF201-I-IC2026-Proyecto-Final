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
        ui.aplicarEstiloVentana(this);

        QGridLayout* layoutPrincipal = new QGridLayout(this);
        layoutPrincipal->setContentsMargins(30, 5, 30, 30);
        layoutPrincipal->setSpacing(15);

        QLabel* tituloSeccion = new QLabel("Categorias");
        tituloSeccion->setAlignment(Qt::AlignLeft);
        ui.aplicarTituloNeon(tituloSeccion);

        // Instanciacion y estilizado de listas
        listaCategorias = new QListWidget();
        listaCategorias->addItems({ "Generico", "Marca", "Controlado" });
        ui.aplicarEstiloLista(listaCategorias);

        listaMedicamentosCategoria = new QListWidget();
        ui.aplicarEstiloListaVerde(listaMedicamentosCategoria);

        // Cuadro de texto para los reportes detallados
        infoCategoria = new QTextEdit();
        infoCategoria->setReadOnly(true);
        ui.aplicarEstiloTexto(infoCategoria); // Asegura que combine con la paleta visual

        QPushButton* botonVolver = new QPushButton("Volver");
        botonVolver->setCursor(Qt::PointingHandCursor);
        ui.aplicarEstiloBoton(botonVolver);

        // Inicializacion de la animacion del borde
        timerBorde = new QTimer(this);
        listaColores << "#39FF14" << "#00FFFF" << "#FF00FF" << "#FFFF00";
        ui.animarBordeTexto(infoCategoria, listaColores, colorActualIdx);

        // Organizacion limpia de filas y columnas en el Layout
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

        // Disparador inicial diferido para seleccionar la primera fila por defecto
        QTimer::singleShot(0, this, [=]() {
            listaCategorias->setCurrentRow(0);
            actualizarVistaCategorias();
        });

        timerBorde->start(500);

        // Conexiones de eventos de Qt
        connect(listaCategorias, &QListWidget::itemClicked, this, &CategoriasView::mostrarCategoriaSeleccionada);
        connect(listaMedicamentosCategoria, &QListWidget::itemClicked, this, &CategoriasView::mostrarMedicamentoCategoria);
        connect(botonVolver, &QPushButton::clicked, this, &CategoriasView::volverAlMenu);
        connect(timerBorde, &QTimer::timeout, this, &CategoriasView::animarBorde);
    }

    void CategoriasView::mostrarCategoriaSeleccionada() {
        QListWidgetItem* item = listaCategorias->currentItem();
        if (item == nullptr) return;

        QString categoria = item->text();
        listaMedicamentosCategoria->clear();
        infoCategoria->clear();

        // Recorrido optimizado de la jerarquia de herencia de medicamentos
        for (int i = 0; i < sistema->getCantMedicamentos(); i++) {
            Medicamento* medicamento = sistema->getMedicamentoPorIndice(i);

            if (medicamento != nullptr) {
                // Comparacion directa sin distinguir mayusculas/minusculas
                if (QString::fromStdString(medicamento->getCategoria()).compare(categoria, Qt::CaseInsensitive) == 0) {

                    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(medicamento->getNombre()));
                    newItem->setData(Qt::UserRole, medicamento->getID()); // Ocultamos el ID en el item de la lista

                    listaMedicamentosCategoria->addItem(newItem);
                }
            }
        }
    }

    void CategoriasView::mostrarMedicamentoCategoria() {
        if (listaMedicamentosCategoria->currentItem() == nullptr) return;

        int id = listaMedicamentosCategoria->currentItem()->data(Qt::UserRole).toInt();
        std::string reporte = sistema->obtenerReporteMedicamento(id);

        infoCategoria->setText(QString::fromStdString(reporte));
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

} // namespace FarmaSystem 