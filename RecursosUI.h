#ifndef RECURSOSUI_H
#define RECURSOSUI_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTextedit>
#include <QDialog>
#include <QWidget>
#include <QListWidget>
#include <QCheckBox>


namespace FarmaSystem {

    class RecursosUI {

    public:
        // Constructor y destructor
        RecursosUI();
        ~RecursosUI();

        void animarBordeTexto(QTextEdit* texto, const QStringList& colores, int& indiceActual);
        void aplicarTituloNeon(QLabel* titulo);
        void aplicarEstiloVentana(QWidget* ventana);
        void aplicarEstiloLista(QListWidget* lista);
        void aplicarEstiloListaVerde(QListWidget* lista);
        void aplicarEstiloBuscador(QLineEdit* buscador);
        void aplicarEstiloTabla(QTableWidget* tabla);
        void aplicarEstiloBoton(QPushButton* boton);
        void aplicarEstiloDialogo(QDialog* dialogo);
        void aplicarEstiloTexto(QTextEdit* texto);
        void aplicarLabelTransparente(QLabel* label, const QString& color);
        void cambiarColorGrid(QTableWidget* tabla, bool activo);
        void aplicarLabelTotal(QLabel* label);
        void aplicarLabelSubtotal(QLabel* label);
        void aplicarLabelInfo(QLabel* label);
        void aplicarLabelNeon(QLabel* label, const QString& color, int tamano = 14);
        void aplicarBordePulsante(QWidget* entrada);
        void aplicarCheckboxReceta(QCheckBox* check);
        
    };

} // namespace 

#endif // RECURSOSUI_H

