#include "EstadisticasView.h"
#include "RecursosUI.h"

#include <QString>

namespace FarmaSystem {

    EstadisticasView::EstadisticasView(SistemaFarmacia* sSistema, QWidget* parent) : QWidget(parent),
        sistema(sSistema) {

        construirUI();
        actualizarDatos();
    }

    void EstadisticasView::construirUI() {

        RecursosUI ui;

        QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);

        ui.aplicarEstiloVentana(this);

        QLabel* titulo = new QLabel("Panel de Estadisticas");
        titulo->setAlignment(Qt::AlignCenter);

        ui.aplicarTituloNeon(titulo);

        layoutPrincipal->addWidget(titulo);

        // PANEL 

        QFrame* panelInfo = new QFrame;

        panelInfo->setStyleSheet(
            "QFrame {"
            "background-color: #1f1f1f;"
            "border: 2px solid #39FF14;"
            "border-radius: 10px;"
            "padding: 10px;"
            "}"
        );

        QVBoxLayout* layoutInfo = new QVBoxLayout(panelInfo);

        lblMenorStock = new QLabel;
        lblMasVendido = new QLabel;
        lblClienteVIP = new QLabel;
        lblIngresos = new QLabel;

        ui.aplicarLabelInfo(lblMenorStock);
        ui.aplicarLabelInfo(lblMasVendido);
        ui.aplicarLabelInfo(lblClienteVIP);
        ui.aplicarLabelInfo(lblIngresos);

        layoutInfo->addWidget(lblMenorStock);
        layoutInfo->addWidget(lblMasVendido);
        layoutInfo->addWidget(lblClienteVIP);
        layoutInfo->addWidget(lblIngresos);

        layoutPrincipal->addWidget(panelInfo);

        // TITULO GRAFICO

        QLabel* tituloGrafico = new QLabel("Medicamentos por Categoria");

        tituloGrafico->setAlignment(Qt::AlignCenter);

        ui.aplicarLabelNeon(tituloGrafico, "#39FF14", 18);

        layoutPrincipal->addWidget(tituloGrafico);

        QHBoxLayout* layoutGrafico = new QHBoxLayout;

        // GENERICOS

        QVBoxLayout* columnaGenericos = new QVBoxLayout;

        barraGenericos = new QLabel;
        barraGenericos->setFixedWidth(70);

        barraGenericos->setStyleSheet(
            "background-color: #39FF14;"
            "border-radius: 8px;"
        );

        valorGenericos = new QLabel("0");
        valorGenericos->setAlignment(Qt::AlignCenter);

        QLabel* textoGenericos = new QLabel("Genericos");
        textoGenericos->setAlignment(Qt::AlignCenter);

        ui.aplicarLabelInfo(valorGenericos);
        ui.aplicarLabelInfo(textoGenericos);

        columnaGenericos->addStretch();
        columnaGenericos->addWidget(barraGenericos);
        columnaGenericos->addWidget(valorGenericos);
        columnaGenericos->addWidget(textoGenericos);

        // MARCA

        QVBoxLayout* columnaMarca = new QVBoxLayout;

        barraMarca = new QLabel;
        barraMarca->setFixedWidth(70);

        barraMarca->setStyleSheet(
            "background-color: #FF3131;"
            "border-radius: 8px;"
        );

        valorMarca = new QLabel("0");
        valorMarca->setAlignment(Qt::AlignCenter);

        QLabel* textoMarca = new QLabel("Marca");
        textoMarca->setAlignment(Qt::AlignCenter);

        ui.aplicarLabelInfo(valorMarca);
        ui.aplicarLabelInfo(textoMarca);

        columnaMarca->addStretch();
        columnaMarca->addWidget(barraMarca);
        columnaMarca->addWidget(valorMarca);
        columnaMarca->addWidget(textoMarca);

        // CONTROLADOS

        QVBoxLayout* columnaControlados = new QVBoxLayout;

        barraControlados = new QLabel;
        barraControlados->setFixedWidth(70);

        barraControlados->setStyleSheet(
            "background-color: #00BFFF;"
            "border-radius: 8px;"
        );

        valorControlados = new QLabel("0");
        valorControlados->setAlignment(Qt::AlignCenter);

        QLabel* textoControlados = new QLabel("Controlados");
        textoControlados->setAlignment(Qt::AlignCenter);

        ui.aplicarLabelInfo(valorControlados);
        ui.aplicarLabelInfo(textoControlados);

        columnaControlados->addStretch();
        columnaControlados->addWidget(barraControlados);
        columnaControlados->addWidget(valorControlados);
        columnaControlados->addWidget(textoControlados);

        layoutGrafico->addStretch();
        layoutGrafico->addLayout(columnaGenericos);
        layoutGrafico->addSpacing(30);
        layoutGrafico->addLayout(columnaMarca);
        layoutGrafico->addSpacing(30);
        layoutGrafico->addLayout(columnaControlados);
        layoutGrafico->addStretch();

        layoutPrincipal->addLayout(layoutGrafico);

        // BOTONES

        QHBoxLayout* layoutBotones = new QHBoxLayout;

        botonActualizar = new QPushButton("Actualizar");
        botonVolver = new QPushButton("Volver");

        ui.aplicarEstiloBoton(botonActualizar);
        ui.aplicarEstiloBoton(botonVolver);

        layoutBotones->addWidget(botonActualizar);
        layoutBotones->addWidget(botonVolver);

        layoutPrincipal->addSpacing(15);
        layoutPrincipal->addLayout(layoutBotones);

        // CONEXIONES

        connect(botonActualizar,
            &QPushButton::clicked,
            this,
            &EstadisticasView::actualizarDatos);

        connect(botonVolver,
            &QPushButton::clicked,
            this,
            &EstadisticasView::volverAlMenu);
    }

    void EstadisticasView::actualizarDatos() {

        Medicamento* menorStock = sistema->obtenerMenorStock();

        Medicamento* masVendido = sistema->obtenerMasVendido();

        Cliente* vip =
            sistema->obtenerClienteVIP();

        double ingresos =
            sistema->obtenerIngresosTotales();

        if (menorStock != nullptr) {

            lblMenorStock->setText(
                "Menor stock: " +
                QString::fromStdString(menorStock->getNombre()) +
                " (" +
                QString::number(menorStock->getStock()) +
                " unidades)"
            );
        }
        else {

            lblMenorStock->setText(
                "Menor stock: Sin datos"
            );
        }

        if (masVendido != nullptr) {

            lblMasVendido->setText(
                "Mas vendido: " +
                QString::fromStdString(masVendido->getNombre())
            );
        }
        else {

            lblMasVendido->setText(
                "Mas vendido: Sin datos"
            );
        }

        if (vip != nullptr) {

            lblClienteVIP->setText(
                "Cliente VIP: " +
                QString::fromStdString(vip->getNombre())
            );
        }
        else {

            lblClienteVIP->setText(
                "Cliente VIP: Sin datos"
            );
        }

        lblIngresos->setText("Ingresos totales: CRC " + QString::number(ingresos, 'f', 2));

        actualizarGraficoCategorias();
    }

    void EstadisticasView::actualizarGraficoCategorias() {

        int genericos   = sistema->contarPorCategoria("Generico");
        int marca       = sistema->contarPorCategoria("Marca");
        int controlados = sistema->contarPorCategoria("Controlado");

        valorGenericos->setText(QString::number(genericos));
        valorMarca->setText(QString::number(marca));
        valorControlados->setText(QString::number(controlados));

        int alturaGenericos = genericos * 25;
        int alturaMarca = marca * 25;
        int alturaControlados = controlados * 25;

        if (alturaGenericos < 20) {
            alturaGenericos = 20;
        }

        if (alturaMarca < 20) {
            alturaMarca = 20;
        }

        if (alturaControlados < 20) {
            alturaControlados = 20;
        }

        barraGenericos->setFixedHeight(alturaGenericos);
        barraMarca->setFixedHeight(alturaMarca);
        barraControlados->setFixedHeight(alturaControlados);
    }

} // namespace FarmaSystem
