#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include "Spreadsheet.h"
#include <string>

class GUI {
private:
    Spreadsheet& sheet;
    sf::RenderWindow window;
    sf::Font font;
    
    // Vista y scroll
    int scrollRow;
    int scrollCol;
    
    // Configuración visual
    const float cellWidth = 100.0f;
    const float cellHeight = 30.0f;
    int visibleRows = 20;
    int visibleCols = 10;
    
    // Estado de la UI
    int selectedRow;
    int selectedCol;
    bool isEditing;
    std::string currentInput;
    
    // Mensaje de estado inferior
    std::string statusMessage;

    void handleEvents();
    void render();
    void renderGrid();
    void renderHeaders();
    void renderCells();
    void renderInput();
    void renderStatus();

    void executeCommand(const std::string& cmd); // Para comandos tipo "SUMA A1:B2"

public:
    GUI(Spreadsheet& s);
    void run();
};

#endif // GUI_H
