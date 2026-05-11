#include "CategoriasView.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QVariantAnimation>

namespace FarmaSystem {

    CategoriasView::CategoriasView(SistemaFarmacia* s, QWidget* parent) : QWidget(parent), sistema(s) {

        setupUI();
    }

    void CategoriasView::setupUI() {
        QGridLayout* layoutPrincipal = new QGridLayout(this);
        this->setStyleSheet("background-color: #2b2b2b; color: #e0e0e0;");
        layoutPrincipal->setContentsMargins(30, 5, 30, 30);
        layoutPrincipal->setSpacing(15);

        QLabel* tituloSeccion = new QLabel("Categorias");
        tituloSeccion->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        tituloSeccion->setStyleSheet(
            "background: transparent; color: white; font-weight: bold; "
            "font-size: 24px; padding: 0px; letter-spacing: 1px;"
        );

        listaCategorias = new QListWidget;
        listaCategorias->addItems({ "Generico", "Marca", "Controlado" });
        listaCategorias->setStyleSheet("background-color: #1a1a1a; color: #e0e0e0; border: 2px solid #39FF14; border-radius: 5px;");

        listaMedicamentosCategoria = new QListWidget;
        listaMedicamentosCategoria->setStyleSheet("background-color: #1a1a1a; color: #b2fab4; border: 2px solid #39FF14; border-radius: 5px;");

        infoCategoria = new QTextEdit;
        infoCategoria->setReadOnly(true);
        infoCategoria->setStyleSheet("background-color: #1a1a1a; color: #b2fab4; font-family: 'Courier New'; border: 2px solid #39FF14; border-radius: 5px;");

        QPushButton* botonVolver = new QPushButton("Volver");
        botonVolver->setCursor(Qt::PointingHandCursor);
        botonVolver->setStyleSheet(getEstiloBotones());

        QGraphicsDropShadowEffect* neonEfecto = new QGraphicsDropShadowEffect(this);

        neonEfecto->setBlurRadius(50);
        neonEfecto->setOffset(0, 0);
        neonEfecto->setColor(QColor("#39FF14"));
        tituloSeccion->setGraphicsEffect(neonEfecto);

        QVariantAnimation* animacionColor = new QVariantAnimation(this);

        animacionColor->setDuration(1000);
        animacionColor->setStartValue(QColor("#39FF14"));
        animacionColor->setEndValue(QColor("#FF3131"));
        animacionColor->setLoopCount(-1);

        timerBorde = new QTimer(this);
        listaColores << "#39FF14" << "#00FFFF" << "#FF00FF" << "#FFFF00";

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

        animacionColor->start();
        timerBorde->start(500);

        // Conexiones
        connect(listaCategorias, &QListWidget::itemClicked, this, &CategoriasView::mostrarCategoriaSeleccionada);
        connect(listaMedicamentosCategoria, &QListWidget::itemClicked, this, &CategoriasView::mostrarMedicamentoCategoria);
        connect(botonVolver, &QPushButton::clicked, this, &CategoriasView::volverAlMenu);
        connect(timerBorde, &QTimer::timeout, this, &CategoriasView::animarBorde);
        connect(animacionColor, &QVariantAnimation::valueChanged, [=](const QVariant& value) { neonEfecto->setColor(value.value<QColor>()); });
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

        if (listaColores.size() > 0) {

            QString color = listaColores.at(colorActualIdx);
            QString estiloInfo = "background-color: #1a1a1a; color: #b2fab4; font-family: 'Courier New'; "
                "border: 2px solid " + color + "; border-radius: 5px;";
            infoCategoria->setStyleSheet(estiloInfo);
            colorActualIdx = (colorActualIdx + 1) % listaColores.size();
        }
    }

    void CategoriasView::actualizarVistaCategorias() {
        mostrarCategoriaSeleccionada();
        listaMedicamentosCategoria->clearSelection();
        infoCategoria->clear();
    }

    void CategoriasView::actualizarVista() {
        actualizarVistaCategorias();
    }

    QString FarmaSystem::CategoriasView::getEstiloBotones() const {
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