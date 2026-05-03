#ifndef NODE_H
#define NODE_H

#include <string>

struct Node {
    int row;
    int col;
    std::string rawContent; // Contenido original (ej: "=A1+2" o "123" o "Texto")
    double numericValue;    // Valor numérico evaluado
    bool isNumeric;         // Indica si es un número válido (para agregaciones)

    Node* nextInRow;
    Node* nextInCol;

    // Constructor
    Node(int r, int c, const std::string& content);
};

#endif // NODE_H
