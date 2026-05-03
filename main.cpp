#include "Spreadsheet.h"
#include "GUI.h"
#include <iostream>

int main() {
    std::cout << "Iniciando Hoja de Calculo..." << std::endl;
    Spreadsheet sheet;
    GUI gui(sheet);
    gui.run();
    return 0;
}
