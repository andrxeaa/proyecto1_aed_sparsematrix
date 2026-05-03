#ifndef FORMULA_EVALUATOR_H
#define FORMULA_EVALUATOR_H

#include <string>

class Spreadsheet; // Forward declaration

class FormulaEvaluator {
public:
    // Evalúa una expresión como "=A1+2" y devuelve el resultado numérico.
    // También puede parsear referencias directas e indicar si hubo error.
    static bool evaluate(const std::string& formula, const Spreadsheet& sheet, double& outResult);

    // Funciones utilitarias para convertir "A1" a fila/columna y viceversa
    static bool parseCellReference(const std::string& ref, int& outRow, int& outCol);
    static std::string indexToColString(int col);
};

#endif // FORMULA_EVALUATOR_H
