#include "Node.h"

Node::Node(int r, int c, const std::string& content)
    : pos_row(r), pos_col(c), rawContent(content),
      numericValue(0.0), isNumeric(false),
      next_row(nullptr), next_col(nullptr) {

    // Intento de parseo numérico directo
    try {
        if (!content.empty() && content[0] != '=') {
            size_t idx;
            double val = std::stod(content, &idx);

            // Verifica si es un número puro
            if (idx == content.length() || content.find_first_not_of(" \t", idx) == std::string::npos) {
                numericValue = val;
                isNumeric = true;
            }
        }
    } catch (...) {
        // Se mantiene isNumeric = false
    }
}