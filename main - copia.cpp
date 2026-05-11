#include <QApplication>
#include "MainWindow.h"
#include "SistemaFarmacia.h"


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    FarmaSystem::SistemaFarmacia* sistema = new FarmaSystem::SistemaFarmacia(3);
    FarmaSystem::MainWindow ventanaPrincipal(sistema);
    ventanaPrincipal.show();

    int resultado = a.exec(); // La app corre aqui

    delete sistema; 
	sistema = nullptr; // null para evitar dangling pointers

    return resultado;
}

