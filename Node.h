#ifndef NODE_H
#define NODE_H

#include <string>

struct Node {
    // Coordenadas
    int pos_row;
    int pos_col;

    // Contenido lógico
    std::string rawContent;
    double numericValue;
    bool isNumeric;

    // Punteros de la matriz dispersa
    Node* next_row;
    Node* next_col;

    // Constructor
    Node(int r, int c, const std::string& content);
};

#endif // NODE_H