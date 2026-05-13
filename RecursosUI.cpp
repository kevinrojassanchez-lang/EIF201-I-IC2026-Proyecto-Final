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
            "  background-color: #2b2b2b;"
            "  color: #e0e0e0;"
            "  border: 2px solid #ff4d4d;"
            "  border-radius: 8px;"
            "  padding: 5px 10px;"
            "}"
            "QLineEdit:focus {"
            "  border: 2px solid #39FF14;"
            "  background-color: #333333;"
            "}"
        );
    }

    void RecursosUI::aplicarEstiloTabla(QTableWidget* tabla) {

        tabla->setStyleSheet(
            "QTableWidget {"
            "  background-color: #2b2b2b;"
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
            "  background-color: #2b2b2b;"
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
            "background-color: #2b2b2b;"
            "color: #e0e0e0;"
        );
    }

    void RecursosUI::aplicarEstiloTexto(QTextEdit* texto) {
        texto->setStyleSheet(
            "background-color: #1a1a1a;"
            "color: #b2fab4;"
            "border: 1px solid #39FF14;"
            "font-family: 'Courier New';"
        );
    }

    void RecursosUI::animarBordeTexto(QTextEdit* texto, const QStringList& colores, int& indiceActual) {

        if (colores.size() > 0) {

            QString color = colores.at(indiceActual);

            texto->setStyleSheet(
                "background-color: #1a1a1a;"
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
            "background-color: #2b2b2b;"
            "color: #e0e0e0;"
        );
    }

    void RecursosUI::aplicarEstiloLista(QListWidget* lista) {
        lista->setStyleSheet(
            "background-color: #1a1a1a;"
            "color: #e0e0e0;"
            "border: 2px solid #39FF14;"
            "border-radius: 5px;"
        );
    }

    void RecursosUI::aplicarEstiloListaVerde(QListWidget* lista) {
        lista->setStyleSheet(
            "background-color: #1a1a1a;"
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
            "color: #39FF14;"
            "font-weight: bold;"
            "font-size: 20px;"
        );
    }

    void RecursosUI::aplicarLabelTotal(QLabel* label)
    {
        label->setStyleSheet(
            "background: transparent;"
            "color: #F9FF00;"
            "font-weight: bold;"
            "font-size: 20px;"
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

    void RecursosUI::aplicarCheckboxReceta(QCheckBox* check)
    {
        check->setStyleSheet(
            "QCheckBox {"
            "    background: transparent;"
            "    color: #E0E0E0;"   // gris claro
            "    font-size: 14px;"
            "}"

            "QCheckBox::indicator {"
            "    width: 16px;"
            "    height: 16px;"
            "    border-radius: 3px;"
            "    border: 2px solid #39FF14;"  // verde suave
            "    background: transparent;"
            "}"

            "QCheckBox::indicator:checked {"
            "    background-color: #39FF14;"  // check visible
            "    border: 2px solid #39FF14;"
            "}"

            "QCheckBox::indicator:disabled {"
            "    border: 2px solid #555555;"
            "    background-color: #2b2b2b;"
            "}"

            "QCheckBox:disabled {"
            "    color: #666666;"  // texto gris apagado
            "}"
        );
    }

} // namespace FarmaSystem
