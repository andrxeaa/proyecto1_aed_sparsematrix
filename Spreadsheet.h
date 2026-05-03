#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Node.h"
#include <vector>
#include <string>

class Spreadsheet {
private:
    // Usamos vectores para las cabeceras (mas estable en Windows)
    std::vector<Node*> rows;
    std::vector<Node*> cols;
    int n_rows;
    int n_cols;

    void reevaluateFormulas();

public:
    // Constructor inicializa con un tamaño base que puede crecer
    Spreadsheet(int n = 100, int m = 26);
    ~Spreadsheet();

    // Operaciones Core
    void insert(int i, int j, const std::string& value);
    Node* get(int i, int j) const;
    void remove(int i, int j);

    // Operaciones de Fila/Columna
    void deleteRow(int i);
    void deleteCol(int j);
    void deleteRange(int i1, int j1, int i2, int j2);

    // Agregaciones
    double sum(int i1, int j1, int i2, int j2) const;
    double average(int i1, int j1, int i2, int j2) const;
    double max(int i1, int j1, int i2, int j2) const;
    double min(int i1, int j1, int i2, int j2) const;
};

#endif