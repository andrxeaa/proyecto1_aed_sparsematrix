#include "Spreadsheet.h"
#include "FormulaEvaluator.h"
#include <iostream>
#include <limits>
#include <algorithm>

Spreadsheet::Spreadsheet() : maxRows(10), maxCols(10) {
    rowHeaders = new Node*[maxRows];
    for (int i = 0; i < maxRows; ++i) rowHeaders[i] = nullptr;
    
    colHeaders = new Node*[maxCols];
    for (int i = 0; i < maxCols; ++i) colHeaders[i] = nullptr;
}

Spreadsheet::~Spreadsheet() {
    for (int i = 0; i < maxRows; ++i) {
        Node* curr = rowHeaders[i];
        while (curr != nullptr) {
            Node* temp = curr;
            curr = curr->nextInRow;
            delete temp;
        }
    }
    delete[] rowHeaders;
    delete[] colHeaders;
}

void Spreadsheet::ensureRowCapacity(int r) {
    if (r >= maxRows) {
        int newMax = std::max(maxRows * 2, r + 1);
        Node** newHeaders = new Node*[newMax];
        for (int i = 0; i < newMax; ++i) {
            newHeaders[i] = (i < maxRows) ? rowHeaders[i] : nullptr;
        }
        delete[] rowHeaders;
        rowHeaders = newHeaders;
        maxRows = newMax;
    }
}

void Spreadsheet::ensureColCapacity(int c) {
    if (c >= maxCols) {
        int newMax = std::max(maxCols * 2, c + 1);
        Node** newHeaders = new Node*[newMax];
        for (int i = 0; i < newMax; ++i) {
            newHeaders[i] = (i < maxCols) ? colHeaders[i] : nullptr;
        }
        delete[] colHeaders;
        colHeaders = newHeaders;
        maxCols = newMax;
    }
}

void Spreadsheet::reevaluateFormulas() {
    // Recorre toda la matriz y reevalúa celdas que empiecen con '='
    for (int i = 0; i < maxRows; ++i) {
        Node* curr = rowHeaders[i];
        while (curr != nullptr) {
            if (!curr->rawContent.empty() && curr->rawContent[0] == '=') {
                double res = 0;
                if (FormulaEvaluator::evaluate(curr->rawContent, *this, res)) {
                    curr->numericValue = res;
                    curr->isNumeric = true;
                } else {
                    curr->isNumeric = false;
                }
            }
            curr = curr->nextInRow;
        }
    }
}

Node* Spreadsheet::get(int r, int c) const {
    if (r >= maxRows || c >= maxCols || r < 0 || c < 0) return nullptr;
    
    Node* curr = rowHeaders[r];
    while (curr != nullptr && curr->col <= c) {
        if (curr->col == c) return curr;
        curr = curr->nextInRow;
    }
    return nullptr;
}

void Spreadsheet::insert(int r, int c, const std::string& content) {
    if (r < 0 || c < 0) return;
    
    ensureRowCapacity(r);
    ensureColCapacity(c);

    Node* existing = get(r, c);
    if (existing != nullptr) {
        modify(r, c, content);
        return;
    }

    Node* newNode = new Node(r, c, content);

    // Enlazar en la fila
    if (rowHeaders[r] == nullptr || rowHeaders[r]->col > c) {
        newNode->nextInRow = rowHeaders[r];
        rowHeaders[r] = newNode;
    } else {
        Node* curr = rowHeaders[r];
        while (curr->nextInRow != nullptr && curr->nextInRow->col < c) {
            curr = curr->nextInRow;
        }
        newNode->nextInRow = curr->nextInRow;
        curr->nextInRow = newNode;
    }

    // Enlazar en la columna
    if (colHeaders[c] == nullptr || colHeaders[c]->row > r) {
        newNode->nextInCol = colHeaders[c];
        colHeaders[c] = newNode;
    } else {
        Node* curr = colHeaders[c];
        while (curr->nextInCol != nullptr && curr->nextInCol->row < r) {
            curr = curr->nextInCol;
        }
        newNode->nextInCol = curr->nextInCol;
        curr->nextInCol = newNode;
    }

    reevaluateFormulas();
}

void Spreadsheet::modify(int r, int c, const std::string& newContent) {
    Node* node = get(r, c);
    if (node != nullptr) {
        // Re-creamos el objeto en la misma direccion de memoria con placement new para simplificar
        // O simplemente reasignamos:
        node->rawContent = newContent;
        // Re-evaluar valor numerico basico
        node->isNumeric = false;
        node->numericValue = 0.0;
        try {
            if (!newContent.empty() && newContent[0] != '=') {
                size_t idx;
                double val = std::stod(newContent, &idx);
                if (idx == newContent.length() || newContent.find_first_not_of(" \t", idx) == std::string::npos) {
                    node->numericValue = val;
                    node->isNumeric = true;
                }
            }
        } catch (...) {}
        
        reevaluateFormulas();
    } else {
        insert(r, c, newContent);
    }
}

void Spreadsheet::remove(int r, int c) {
    if (r >= maxRows || c >= maxCols || r < 0 || c < 0) return;

    Node* target = nullptr;

    // Desenlazar de la fila
    if (rowHeaders[r] != nullptr) {
        if (rowHeaders[r]->col == c) {
            target = rowHeaders[r];
            rowHeaders[r] = target->nextInRow;
        } else {
            Node* curr = rowHeaders[r];
            while (curr->nextInRow != nullptr && curr->nextInRow->col != c) {
                curr = curr->nextInRow;
            }
            if (curr->nextInRow != nullptr) {
                target = curr->nextInRow;
                curr->nextInRow = target->nextInRow;
            }
        }
    }

    if (target == nullptr) return; // No existe

    // Desenlazar de la columna
    if (colHeaders[c] != nullptr) {
        if (colHeaders[c]->row == r) {
            colHeaders[c] = colHeaders[c]->nextInCol;
        } else {
            Node* curr = colHeaders[c];
            while (curr->nextInCol != nullptr && curr->nextInCol->row != r) {
                curr = curr->nextInCol;
            }
            if (curr->nextInCol != nullptr) {
                curr->nextInCol = curr->nextInCol->nextInCol;
            }
        }
    }

    delete target;
    reevaluateFormulas();
}

void Spreadsheet::deleteRow(int r) {
    if (r >= maxRows || r < 0) return;
    
    // Eliminamos cada celda de la fila desde la cabeza (usando nuestra logica robusta)
    while (rowHeaders[r] != nullptr) {
        remove(r, rowHeaders[r]->col);
    }
}

void Spreadsheet::deleteCol(int c) {
    if (c >= maxCols || c < 0) return;
    
    while (colHeaders[c] != nullptr) {
        remove(colHeaders[c]->row, c);
    }
}

void Spreadsheet::deleteRange(int r1, int c1, int r2, int c2) {
    int startRow = std::min(r1, r2);
    int endRow = std::max(r1, r2);
    int startCol = std::min(c1, c2);
    int endCol = std::max(c1, c2);

    for (int i = startRow; i <= endRow; ++i) {
        if (i >= maxRows) break;
        Node* curr = rowHeaders[i];
        while (curr != nullptr) {
            Node* next = curr->nextInRow;
            if (curr->col >= startCol && curr->col <= endCol) {
                remove(curr->row, curr->col);
            }
            curr = next;
        }
    }
}

double Spreadsheet::sum(int r1, int c1, int r2, int c2) const {
    double total = 0;
    int startRow = std::min(r1, r2);
    int endRow = std::max(r1, r2);
    int startCol = std::min(c1, c2);
    int endCol = std::max(c1, c2);

    for (int i = startRow; i <= endRow; ++i) {
        if (i >= maxRows) break;
        Node* curr = rowHeaders[i];
        while (curr != nullptr && curr->col <= endCol) {
            if (curr->col >= startCol && curr->isNumeric) {
                total += curr->numericValue;
            }
            curr = curr->nextInRow;
        }
    }
    return total;
}

double Spreadsheet::average(int r1, int c1, int r2, int c2) const {
    double total = 0;
    int count = 0;
    int startRow = std::min(r1, r2);
    int endRow = std::max(r1, r2);
    int startCol = std::min(c1, c2);
    int endCol = std::max(c1, c2);

    for (int i = startRow; i <= endRow; ++i) {
        if (i >= maxRows) break;
        Node* curr = rowHeaders[i];
        while (curr != nullptr && curr->col <= endCol) {
            if (curr->col >= startCol && curr->isNumeric) {
                total += curr->numericValue;
                count++;
            }
            curr = curr->nextInRow;
        }
    }
    return count > 0 ? total / count : 0.0;
}

double Spreadsheet::max(int r1, int c1, int r2, int c2) const {
    double mx = -std::numeric_limits<double>::infinity();
    bool found = false;
    int startRow = std::min(r1, r2);
    int endRow = std::max(r1, r2);
    int startCol = std::min(c1, c2);
    int endCol = std::max(c1, c2);

    for (int i = startRow; i <= endRow; ++i) {
        if (i >= maxRows) break;
        Node* curr = rowHeaders[i];
        while (curr != nullptr && curr->col <= endCol) {
            if (curr->col >= startCol && curr->isNumeric) {
                if (curr->numericValue > mx) {
                    mx = curr->numericValue;
                }
                found = true;
            }
            curr = curr->nextInRow;
        }
    }
    return found ? mx : 0.0;
}

double Spreadsheet::min(int r1, int c1, int r2, int c2) const {
    double mn = std::numeric_limits<double>::infinity();
    bool found = false;
    int startRow = std::min(r1, r2);
    int endRow = std::max(r1, r2);
    int startCol = std::min(c1, c2);
    int endCol = std::max(c1, c2);

    for (int i = startRow; i <= endRow; ++i) {
        if (i >= maxRows) break;
        Node* curr = rowHeaders[i];
        while (curr != nullptr && curr->col <= endCol) {
            if (curr->col >= startCol && curr->isNumeric) {
                if (curr->numericValue < mn) {
                    mn = curr->numericValue;
                }
                found = true;
            }
            curr = curr->nextInRow;
        }
    }
    return found ? mn : 0.0;
}
