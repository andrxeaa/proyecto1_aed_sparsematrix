#include "Node.h"

Node::Node(int r, int c, const std::string& content) 
    : row(r), col(c), rawContent(content), numericValue(0.0), isNumeric(false), nextInRow(nullptr), nextInCol(nullptr) {
    
    // Tratamos de parsear si es un numero directo
    // No manejamos formulas aqui, el evaluador de formulas se encargará de eso despues y actualizará numericValue/isNumeric
    try {
        if (!content.empty() && content[0] != '=') {
            size_t idx;
            double val = std::stod(content, &idx);
            // Si consumio toda la cadena, es puramente numerico (ignorando espacios finales)
            if (idx == content.length() || content.find_first_not_of(" \t", idx) == std::string::npos) {
                numericValue = val;
                isNumeric = true;
            }
        }
    } catch (...) {
        // Falló la conversión, se queda como texto (isNumeric = false)
    }
}
