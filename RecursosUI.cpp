#include "RecursosUI.h"

#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QVariantAnimation>
#include <QLabel>
#include <QObject.h>
#include <QEasingCurve>

namespace FarmaSystem {

    void RecursosUI::aplicarTituloNeon(QLabel* titulo)
    {
        titulo->setStyleSheet("font-weight: bold;" "background: transparent;"   "font-size: 22px;"
            "color: white;" "letter-spacing: 1px;");

        QColor verdeNeon("#39FF14");
        QColor rojoNeon("#FF3131");

        QGraphicsDropShadowEffect* neonEfecto = new QGraphicsDropShadowEffect(titulo);
        neonEfecto->setBlurRadius(35);
        neonEfecto->setColor(verdeNeon);
        neonEfecto->setOffset(0, 0);

        titulo->setGraphicsEffect(neonEfecto);

        QVariantAnimation* animacion = new QVariantAnimation(titulo);
        animacion->setDuration(1000);
        animacion->setStartValue(verdeNeon);
        animacion->setEndValue(rojoNeon);
        animacion->setLoopCount(-1);

        QObject::connect(animacion, &QVariantAnimation::valueChanged,
            [neonEfecto](const QVariant& value) { neonEfecto->setColor(value.value<QColor>()); });

        animacion->start();
    }

    void RecursosUI::aplicarEstiloBuscador(QLineEdit* buscador) {

        buscador->setStyleSheet(
              "QLineEdit {"
            "  background-color: #000000;"
            "  color: #e0e0e0;"
            "  border: 2px solid #ff4d4d;"
            "  border-radius: 8px;"
            "  padding: 5px 10px;"
            "}"
            "QLineEdit:focus {"
            " border: 2px solid #39FF14;"
            " background-color: #000000;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloTabla(QTableWidget* tabla) {

        tabla->setStyleSheet(
            "QTableWidget {"
            "  background-color: #000000;"
            "  color: #e0e0e0;"
            "  gridline-color: #ff4d4d;"
            "  outline: 0;"
            "  border: 2px solid #ff4d4d;"
            "  border-radius: 5px;"
            "}"
            "QTableWidget:focus {"
            "  border: 2px solid #39FF14;"
            "}"
            "QTableWidget::item {"
            "  border: none;"
            "  padding: 6px;"
            "}"
            "QTableWidget::item:selected {"
            "  background-color: #3399ff;"
            "  color: #2b2b2b;"
            "}"
            "QTableWidget::item:focus {"
            "  outline: none;"
            "  border: none;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloBoton(QPushButton* boton) {

        boton->setStyleSheet(
            "QPushButton {"
            "  background-color: #000000;"
            "  color: #ff4d4d;"
            "  border: 2px solid #ff4d4d;"
            "  border-radius: 10px;"
            "  padding: 8px 16px;"
            "  font-weight: bold;"
            "  font-size: 13px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #323232;"
            "  color: #39FF14;"
            "  border: 2px solid #39FF14;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #4d0000;"
            "  color: #ff4d4d;"
            "  border: 2px solid #ff4d4d;"
            "}"
            "QPushButton:disabled {"
            "  background-color: #1a1a1a;"
            "  color: #444444;"
            "  border: 2px solid #333333;"
            "}"
        );
    }

    void RecursosUI::cambiarColorGrid(QTableWidget* tabla, bool activo) {

        if (activo) {
            tabla->setStyleSheet(
                tabla->styleSheet() +
                "QTableWidget { gridline-color: #39FF14; }"
            );
        }
        else {
            tabla->setStyleSheet(
                tabla->styleSheet() +
                "QTableWidget { gridline-color: #ff4d4d; }"
            );
        }
    }

    void RecursosUI::aplicarEstiloDialogo(QDialog* dialogo) {
        dialogo->setStyleSheet(
            "background-color: #000000;"
            "color: #e0e0e0;"
        );
    }

    void RecursosUI::aplicarEstiloTexto(QTextEdit* texto) {
        texto->setStyleSheet(
            "background-color: #000000;"
            "color: #b2fab4;"
            "border: 1px solid #39FF14;"
            "font-family: 'Courier New';"
        );
    }

    void RecursosUI::animarBordeTexto(QTextEdit* texto, const QStringList& colores, int& indiceActual) {

        if (colores.size() > 0) {

            QString color = colores.at(indiceActual);

            texto->setStyleSheet(
                "background-color: #000000;"
                "color: #b2fab4;"
                "font-family: 'Courier New';"
                "border: 2px solid " + color + ";"
                "border-radius: 5px;"
            );

            indiceActual = (indiceActual + 1) % colores.size();
        }
    }

    void RecursosUI::aplicarEstiloVentana(QWidget* ventana) {
        ventana->setStyleSheet(
            "background-color: #000000;"
            "color: #e0e0e0;"
        );
    }

    void RecursosUI::aplicarEstiloLista(QListWidget* lista) {
        lista->setStyleSheet(
            "background-color: #000000;"
            "color: #e0e0e0;"
            "border: 2px solid #39FF14;"
            "border-radius: 5px;"
        );
    }

    void RecursosUI::aplicarEstiloListaVerde(QListWidget* lista) {
        lista->setStyleSheet(
            "background-color: #000000;"
            "color: #b2fab4;"
            "border: 2px solid #39FF14;"
            "border-radius: 5px;"
        );
    }

    void RecursosUI::aplicarLabelTransparente(QLabel* label, const QString& color)
    {
        label->setStyleSheet(
            "background: transparent;"
            "color: " + color + ";"
        );
    }

    void RecursosUI::aplicarLabelSubtotal(QLabel* label)
    {
        label->setStyleSheet(
            "background: transparent;"
            "color: #F9FF00;"
            "font-weight: bold;"
            "font-size: 16px;"
        );
    }

    void RecursosUI::aplicarLabelTotal(QLabel* label)
    {
        label->setStyleSheet(
            "background: transparent;"
            "color: #00FFFF;"
            "font-weight: bold;"
            "font-size: 16px;"
        );
    }

    void RecursosUI::aplicarLabelGanancias(QLabel* label) {
        label->setStyleSheet(
            "background: transparent;"
            "color: #FFB300;"        // Dorado / Cyberpunk
            "font-weight: bold;"
            "font-size: 16px;"      
            "letter-spacing: 0.5px;"
        );
    }

    void RecursosUI::aplicarLabelInfo(QLabel* label)
    {
        label->setStyleSheet(
            "background: transparent;"
            "color: #e0e0e0;"
            "font-size: 16px;"
        );
    }
    void RecursosUI::aplicarLabelNeon(QLabel* label, const QString& color, int tamano) {

        label->setStyleSheet(
            "background: transparent;"
            "color: " + color + ";"
            "font-weight: bold;"
            "font-size: " + QString::number(tamano) + "px;"
        );

        QGraphicsDropShadowEffect* efecto = new QGraphicsDropShadowEffect(label);

        efecto->setBlurRadius(25);   // Intensidad del brillo
        efecto->setOffset(0, 0);     // Sin desplazamiento
        efecto->setColor(QColor(color)); // Mismo color del texto

        label->setGraphicsEffect(efecto);
    }

    RecursosUI::RecursosUI() {}
    RecursosUI::~RecursosUI() {}

    void RecursosUI::aplicarBordePulsante(QWidget* widget)
    {
        QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect(widget);

        glow->setColor(QColor("#39FF14"));
        glow->setOffset(0, 0);
        glow->setBlurRadius(10); // estado inicial suave

        widget->setGraphicsEffect(glow);

        QVariantAnimation* anim = new QVariantAnimation(widget);
        anim->setDuration(2500);
        anim->setStartValue(6);
        anim->setEndValue(25);
        anim->setLoopCount(-1);
        anim->setEasingCurve(QEasingCurve::SineCurve);
        anim->setLoopCount(-1);

        QObject::connect(anim, &QVariantAnimation::valueChanged,
            [glow](const QVariant& value) {
                glow->setBlurRadius(value.toInt());
            });

        anim->start();
    }

    void RecursosUI::aplicarEstiloFormularioDialogo(QDialog* dialogo) {
        dialogo->setWindowTitle(dialogo->windowTitle());
        dialogo->setStyleSheet(
            "QDialog {"
            "background-color: #000000;" // Fondo oscuro general de la app
            "}"
        );
    }

    void RecursosUI::aplicarEstiloCamposFormulario(QLineEdit* campo) {
        campo->setStyleSheet(
            "QLineEdit {"
            "  background-color: #000000;"
            "  color: #e0e0e0;"
            "  border: 1px solid #ff4d4d;" // Borde rojo por defecto
            "  border-radius: 6px;"
            "  padding: 6px 12px;"
            "  font-size: 14px;"
            "}"
            "QLineEdit:focus {"
            "  border: 2px solid #39FF14;" // Cambia a Neón Verde al escribir
            "  background-color: #000000;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloCheckBox(QCheckBox* checkbox) {
        checkbox->setStyleSheet(
            "QCheckBox {"
            "  color: #e0e0e0;"
            "  font-size: 13px;"
            "  font-weight: bold;"
            "  spacing: 8px;"
            "}"
            "QCheckBox::indicator {"
            "  width: 18px;"
            "  height: 18px;"
            "  border: 2px solid #ff4d4d;" // Borde rojo por defecto
            "  border-radius: 4px;"
            "  background-color: #000000;" // Fondo oscuro permanente
            "}"
            "QCheckBox::indicator:hover {"
            "  border: 2px solid #39FF14;" // El borde brilla en verde al pasar el mouse
            "}"
            // EL PUNTITO NEÓN CENTRADO: Un círculo verde pequeño en el puro centro
            "QCheckBox::indicator:checked {"
            "  border: 2px solid #39FF14;" // El borde exterior cambia a verde
            "  background-color: qradialgradient(cx:0.5, cy:0.5, radius:0.25, fx:0.5, fy:0.5, stop:0 #39FF14, stop:0.9 #39FF14, stop:0.91 #2b2b2b, stop:1 #2b2b2b);"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloComboBox(QComboBox* combo) {
        combo->setStyleSheet(
            "QComboBox {"
            "  background-color: #2b2b2b;"
            "  color: #e0e0e0;"
            "  border: 1px solid #ff4d4d;"
            "  border-radius: 6px;"
            "  padding: 5px 10px;"
            "}"
            "QComboBox:focus {"
            "  border: 2px solid #39FF14;"
            "}"
            "QComboBox::drop-down {"
            "  border: none;"
            "  width: 25px;"
            "}"
            "QComboBox QAbstractItemView {"
            "  background-color: #1f1f1f;"
            "  color: #e0e0e0;"
            "  selection-background-color: #39FF14;"
            "  selection-color: #1f1f1f;"
            "  border: 1px solid #ff4d4d;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloSpinBox(QSpinBox* spin) {
        spin->setStyleSheet(
            "QSpinBox {"
            "  background-color: #2b2b2b;"
            "  color: #e0e0e0;"
            "  border: 1px solid #ff4d4d;"
            "  border-radius: 6px;"
            "  padding: 5px 10px;"
            "}"
            "QSpinBox:focus {"
            "  border: 2px solid #39FF14;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloDoubleSpinBox(QDoubleSpinBox* spin) {
        spin->setStyleSheet(
            "QDoubleSpinBox {"
            "  background-color: #2b2b2b;"
            "  color: #e0e0e0;"
            "  border: 1px solid #ff4d4d;"
            "  border-radius: 6px;"
            "  padding: 5px 10px;"
            "}"
            "QDoubleSpinBox:focus {"
            "  border: 2px solid #39FF14;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloBotonMenuPrincipal(QPushButton* boton) {
        boton->setStyleSheet(
            "QPushButton {"
            "  background-color: #000000;"   // Fondo ultra oscuro tipo Visual Studio
            "  color: #39FF14;"             //  Texto verde neon oficial
            "  border: 2px solid #39FF14;"   // Borde verde neon
            "  border-radius: 12px;"         // Bordes bien redondeados y modernos para el menu
            "  padding: 14px 28px;"          // Botones grandes, imponentes y comodos para hacer clic
            "  font-weight: bold;"
            "  font-size: 12px;"             // Letra un poco mas grande para mejor lectura
            "  letter-spacing: 1px;"         // Espaciado tecnologico entre letras
            "}"
            "QPushButton:hover {"
            "  background-color: #111111;"   // Un gris casi imperceptible para que se note que esta activo
            "  color: #00FFFF;"             //  Muta magicamente a Celeste Cian neon
            "  border: 2px solid #00FFFF;"   // El borde tambien cambia a celeste
            "  font-size: 13px;"             // Un levisimo crecimiento para dar efecto de relieve
            "}"
            "QPushButton:pressed {"
            "  background-color: #001a1a;"   // Un toque cian muy oscuro al hacer clic sobre el fondo negro
            "  color: #00FFFF;"
            "  border: 2px solid #00FFFF;"
            "}"
            "QPushButton:disabled {"
            "  background-color: #050505;"   // Negro opaco para deshabilitado
            "  color: #444444;"
            "  border: 2px solid #111111;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloPanelEstadisticas(QFrame* panel) {
        if (!panel) return;
        panel->setStyleSheet(
            "QFrame {"
            "background-color: #000000;"
            "border: 2px solid #39FF14;"
            "border-radius: 10px;"
            "padding: 10px;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloBarraGrafico(QLabel* barra, const QString& colorHex) {
        if (!barra) return;
        barra->setFixedWidth(70);
        barra->setStyleSheet(
            QString("background-color: %1; border-radius: 8px;").arg(colorHex)
        );
    }

    void RecursosUI::configurarColumnaGrafico(QVBoxLayout* columna, QLabel* barra, QLabel* valor, QLabel* texto) {
        if (!columna || !barra || !valor || !texto) return;
        columna->addStretch();
        columna->addWidget(barra);
        columna->addWidget(valor);
        columna->addWidget(texto);
    }

    QLabel* RecursosUI::crearSeccionNeon(const QString& texto, const QString& colorNeon) {
        QLabel* label = new QLabel(texto);
        RecursosUI ui;
        ui.aplicarLabelNeon(label, colorNeon, 14);
        return label;
    }

    QLineEdit* RecursosUI::crearCampoTexto(const QString& placeholder) {
        QLineEdit* edit = new QLineEdit();
        edit->setPlaceholderText(placeholder);
        RecursosUI ui;
        ui.aplicarEstiloCamposFormulario(edit);
        return edit;
    }

    QDoubleSpinBox* RecursosUI::crearDoubleSpin(double min, double max, const QString& prefijo) {
        QDoubleSpinBox* spin = new QDoubleSpinBox();
        spin->setRange(min, max);
        spin->setPrefix(prefijo);
        RecursosUI ui;
        ui.aplicarEstiloDoubleSpinBox(spin);
        return spin;
    }

    QSpinBox* RecursosUI::crearSpin(int min, int max, const QString& prefijo) {
        QSpinBox* spin = new QSpinBox();
        spin->setRange(min, max);
        if (!prefijo.isEmpty()) {
            spin->setPrefix(prefijo);
        }
        RecursosUI ui;
        ui.aplicarEstiloSpinBox(spin);
        return spin;
    }

    QCheckBox* RecursosUI::crearCheck(const QString& texto) {
        QCheckBox* check = new QCheckBox(texto);
        RecursosUI ui;
        ui.aplicarEstiloCheckBox(check);
        return check;
    }

    QComboBox* RecursosUI::crearCombo() {
        QComboBox* combo = new QComboBox();
        RecursosUI ui;
        ui.aplicarEstiloComboBox(combo);
        return combo;
    }
} // namespace FarmaSystem
