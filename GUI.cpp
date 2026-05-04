#include "GUI.h"
#include "FormulaEvaluator.h"
#include <iostream>
#include <sstream>

GUI::GUI(Spreadsheet& s) 
    : sheet(s), scrollRow(0), scrollCol(0), selectedRow(0), selectedCol(0), isEditing(false) {
    window.create(sf::VideoMode(1200, 800), "Hoja de Calculo Dispersa - Proyecto 1");
    
    // Cargar fuente local
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Advertencia: No se pudo cargar arial.ttf. Asegurese de que el archivo este junto al ejecutable." << std::endl;
    }
    statusMessage = "Listo. Usa las flechas para moverte, Enter para editar. Comandos en consola inferior.";
}

void GUI::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void GUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (isEditing) {
                if (event.key.code == sf::Keyboard::Enter) {
                    isEditing = false;
                    if (currentInput.length() > 0 && currentInput[0] == '/') {
                        executeCommand(currentInput);
                    } else if (currentInput.empty()) {
                        sheet.remove(selectedRow, selectedCol);
                    } else {
                        sheet.insert(selectedRow, selectedCol, currentInput);
                    }
                } else if (event.key.code == sf::Keyboard::Escape) {
                    isEditing = false;
                } else if (event.key.code == sf::Keyboard::Backspace) {
                    if (!currentInput.empty()) currentInput.pop_back();
                }
            } else {
                // Navegación
                if (event.key.code == sf::Keyboard::Right) selectedCol++;
                if (event.key.code == sf::Keyboard::Left && selectedCol > 0) selectedCol--;
                if (event.key.code == sf::Keyboard::Down) selectedRow++;
                if (event.key.code == sf::Keyboard::Up && selectedRow > 0) selectedRow--;
                
                // Ajustar scroll
                if (selectedRow < scrollRow) scrollRow = selectedRow;
                if (selectedRow >= scrollRow + visibleRows) scrollRow = selectedRow - visibleRows + 1;
                if (selectedCol < scrollCol) scrollCol = selectedCol;
                if (selectedCol >= scrollCol + visibleCols) scrollCol = selectedCol - visibleCols + 1;
                
                if (event.key.code == sf::Keyboard::Enter) {
                    isEditing = true;
                    Node* n = sheet.get(selectedRow, selectedCol);
                    currentInput = n ? n->rawContent : "";
                }
                
                if (event.key.code == sf::Keyboard::Delete) {
                    sheet.remove(selectedRow, selectedCol);
                }
            }
        }
        
        if (event.type == sf::Event::TextEntered && isEditing) {
            // Ignorar caracteres de control (Enter, Backspace, etc)
            if (event.text.unicode >= 32 && event.text.unicode < 128) {
                currentInput += static_cast<char>(event.text.unicode);
            }
        }
    }
}

void GUI::executeCommand(const std::string& cmd) {
    // Parser simple para comandos tipo "/SUMA A1 B2"
    std::stringstream ss(cmd);
    std::string action, arg1, arg2;
    ss >> action >> arg1 >> arg2;
    
    int r1, c1, r2, c2;
    if (FormulaEvaluator::parseCellReference(arg1, r1, c1) && FormulaEvaluator::parseCellReference(arg2, r2, c2)) {
        if (action == "/SUMA") {
            double res = sheet.sum(r1, c1, r2, c2);
            statusMessage = "Suma: " + std::to_string(res);
            sheet.insert(selectedRow, selectedCol, std::to_string(res));
        } else if (action == "/PROM") {
            double res = sheet.average(r1, c1, r2, c2);
            statusMessage = "Promedio: " + std::to_string(res);
            sheet.insert(selectedRow, selectedCol, std::to_string(res));
        } else if (action == "/MAX") {
            double res = sheet.max(r1, c1, r2, c2);
            statusMessage = "Maximo: " + std::to_string(res);
            sheet.insert(selectedRow, selectedCol, std::to_string(res));
        } else if (action == "/MIN") {
            double res = sheet.min(r1, c1, r2, c2);
            statusMessage = "Minimo: " + std::to_string(res);
            sheet.insert(selectedRow, selectedCol, std::to_string(res));
        } else if (action == "/DELRANGE") {
            sheet.deleteRange(r1, c1, r2, c2);
            statusMessage = "Rango eliminado.";
        }
    } else {
        if (action == "/DELROW") {
            try { sheet.deleteRow(std::stoi(arg1) - 1); statusMessage = "Fila eliminada."; } catch(...) {}
        } else if (action == "/DELCOL") {
            int r, c;
            if (FormulaEvaluator::parseCellReference(arg1 + "1", r, c)) { // Hack para parsear col
                sheet.deleteCol(c);
                statusMessage = "Columna eliminada.";
            }
        } else if (action == "/SUMROW") {
            try { 
                int row = std::stoi(arg1) - 1; 
                double res = sheet.sum(row, 0, row, sheet.getMaxColIndex());
                statusMessage = "Suma Fila: " + std::to_string(res);
                sheet.insert(selectedRow, selectedCol, std::to_string(res));
            } catch(...) {}
        } else if (action == "/SUMCOL") {
            int r, c;
            if (FormulaEvaluator::parseCellReference(arg1 + "1", r, c)) { 
                double res = sheet.sum(0, c, sheet.getMaxRowIndex(), c);
                statusMessage = "Suma Columna: " + std::to_string(res);
                sheet.insert(selectedRow, selectedCol, std::to_string(res));
            }
        } else {
            statusMessage = "Comando invalido. Use /SUMA A1 B2, /DELROW 1, etc.";
        }
    }
}

void GUI::render() {
    window.clear(sf::Color(240, 240, 240)); // Fondo claro
    renderGrid();
    renderHeaders();
    renderCells();
    renderInput();
    renderStatus();
    window.display();
}

void GUI::renderGrid() {
    sf::RectangleShape line(sf::Vector2f(window.getSize().x, 1));
    line.setFillColor(sf::Color(200, 200, 200));
    
    for (int r = 0; r <= visibleRows; ++r) {
        line.setPosition(0, (r + 1) * cellHeight); // +1 para headers
        window.draw(line);
    }
    
    line.setSize(sf::Vector2f(1, window.getSize().y));
    for (int c = 0; c <= visibleCols; ++c) {
        line.setPosition((c + 1) * cellWidth, 0); // +1 para headers
        window.draw(line);
    }
}

void GUI::renderHeaders() {
    sf::Text text("", font, 14);
    text.setFillColor(sf::Color::Black);
    
    sf::RectangleShape bg(sf::Vector2f(cellWidth, cellHeight));
    bg.setFillColor(sf::Color(220, 220, 220));
    
    // Headers de Columnas
    for (int c = 0; c < visibleCols; ++c) {
        bg.setPosition((c + 1) * cellWidth, 0);
        window.draw(bg);
        text.setString(FormulaEvaluator::indexToColString(c + scrollCol));
        text.setPosition((c + 1) * cellWidth + 10, 5);
        window.draw(text);
    }
    
    // Headers de Filas
    for (int r = 0; r < visibleRows; ++r) {
        bg.setPosition(0, (r + 1) * cellHeight);
        window.draw(bg);
        text.setString(std::to_string(r + scrollRow + 1));
        text.setPosition(10, (r + 1) * cellHeight + 5);
        window.draw(text);
    }
}

void GUI::renderCells() {
    sf::Text text("", font, 14);
    text.setFillColor(sf::Color::Black);
    
    // Dibujar celdas ocupadas
    for (int r = scrollRow; r < scrollRow + visibleRows; ++r) {
        for (int c = scrollCol; c < scrollCol + visibleCols; ++c) {
            float px = (c - scrollCol + 1) * cellWidth;
            float py = (r - scrollRow + 1) * cellHeight;
            
            if (r == selectedRow && c == selectedCol) {
                sf::RectangleShape sel(sf::Vector2f(cellWidth - 2, cellHeight - 2));
                sel.setPosition(px + 1, py + 1);
                sel.setFillColor(sf::Color::Transparent);
                sel.setOutlineThickness(2);
                sel.setOutlineColor(isEditing ? sf::Color::Red : sf::Color::Blue);
                window.draw(sel);
            }
            
            Node* n = sheet.get(r, c);
            if (n != nullptr) {
                // Resaltar celdas ocupadas sutilmente
                if (!(r == selectedRow && c == selectedCol)) {
                    sf::RectangleShape occ(sf::Vector2f(cellWidth - 2, cellHeight - 2));
                    occ.setPosition(px + 1, py + 1);
                    occ.setFillColor(sf::Color(245, 250, 255));
                    window.draw(occ);
                }
                
                std::string displayStr = n->rawContent;
                if (!isEditing || !(r == selectedRow && c == selectedCol)) {
                    // Mostrar valor evaluado si es formula
                    if (!n->rawContent.empty() && n->rawContent[0] == '=') {
                        if (n->isNumeric) {
                            displayStr = std::to_string(n->numericValue);
                            // Quitar ceros extra
                            displayStr.erase(displayStr.find_last_not_of('0') + 1, std::string::npos);
                            if (displayStr.back() == '.') displayStr.pop_back();
                        } else {
                            displayStr = "#ERR";
                        }
                    }
                }
                
                text.setString(displayStr);
                text.setPosition(px + 5, py + 5);
                window.draw(text);
            }
        }
    }
}

void GUI::renderInput() {
    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, 30));
    bg.setPosition(0, window.getSize().y - 60);
    bg.setFillColor(sf::Color(255, 255, 220));
    window.draw(bg);
    
    sf::Text text("", font, 16);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, window.getSize().y - 55);
    
    if (isEditing) {
        text.setString("Editando " + FormulaEvaluator::indexToColString(selectedCol) + std::to_string(selectedRow + 1) + ": " + currentInput + "_");
    } else {
        Node* n = sheet.get(selectedRow, selectedCol);
        std::string val = n ? n->rawContent : "(vacia)";
        text.setString("Seleccion: " + FormulaEvaluator::indexToColString(selectedCol) + std::to_string(selectedRow + 1) + " | Valor: " + val);
    }
    window.draw(text);
}

void GUI::renderStatus() {
    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, 30));
    bg.setPosition(0, window.getSize().y - 30);
    bg.setFillColor(sf::Color(200, 200, 200));
    window.draw(bg);
    
    sf::Text text(statusMessage, font, 14);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, window.getSize().y - 25);
    window.draw(text);
}
