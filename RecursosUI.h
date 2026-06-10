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
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QVBoxLayout>

namespace FarmaSystem {

    class RecursosUI {
    private:

    public:
        // Constructor y destructor
        RecursosUI();
        ~RecursosUI();

        void aplicarEstiloBotonMenuPrincipal(QPushButton* boton);
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
        void aplicarEstiloCheckBox(QCheckBox* checkbox);
        void aplicarEstiloFormularioDialogo(QDialog* dialogo);
        void aplicarEstiloCamposFormulario(QLineEdit* campo);
        void aplicarEstiloComboBox(QComboBox* combo);
        void aplicarEstiloSpinBox(QSpinBox* spin);
        void aplicarEstiloDoubleSpinBox(QDoubleSpinBox* spin);
        void aplicarLabelGanancias(QLabel* label);
        void aplicarEstiloPanelEstadisticas(QFrame* panel);
        void aplicarEstiloBarraGrafico(QLabel* barra, const QString& colorHex);
        void configurarColumnaGrafico(QVBoxLayout* columna, QLabel* barra, QLabel* valor, QLabel* texto);

        static QLabel* crearSeccionNeon(const QString& texto, const QString& colorNeon = "#ff4d4d");
        static QLineEdit* crearCampoTexto(const QString& placeholder);
        static QDoubleSpinBox* crearDoubleSpin(double min, double max, const QString& prefijo);
        static QSpinBox* crearSpin(int min, int max, const QString& prefijo);
        static QCheckBox* crearCheck(const QString& texto);
        static QComboBox* crearCombo();
        
    };

} // namespace 

#endif // RECURSOSUI_H

