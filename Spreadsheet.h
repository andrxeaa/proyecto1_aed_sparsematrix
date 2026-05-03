#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Node.h"
#include <string>

class Spreadsheet {
private:
    Node** rowHeaders;
    Node** colHeaders;
    int maxRows;
    int maxCols;

    void ensureRowCapacity(int r);
    void ensureColCapacity(int c);
    
    // Función auxiliar para re-evaluar todas las fórmulas después de un cambio
    void reevaluateFormulas();

public:
    Spreadsheet();
    ~Spreadsheet();

    // 1. Operaciones sobre celdas
    void insert(int r, int c, const std::string& content);
    Node* get(int r, int c) const;
    void modify(int r, int c, const std::string& newContent);
    void remove(int r, int c);

    // 2. Operaciones sobre filas, columnas y rangos
    void deleteRow(int r);
    void deleteCol(int c);
    void deleteRange(int r1, int c1, int r2, int c2);

    // 3. Agregaciones
    double sum(int r1, int c1, int r2, int c2) const;
    double average(int r1, int c1, int r2, int c2) const;
    double max(int r1, int c1, int r2, int c2) const;
    double min(int r1, int c1, int r2, int c2) const;
    
    // Auxiliares
    int getMaxRowIndex() const { return maxRows - 1; }
    int getMaxColIndex() const { return maxCols - 1; }
};

#endif // SPREADSHEET_H
