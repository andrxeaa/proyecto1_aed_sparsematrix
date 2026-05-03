#include "FormulaEvaluator.h"
#include "Spreadsheet.h"
#include <cctype>
#include <stdexcept>
#include <algorithm>

class Parser {
    std::string expr;
    size_t pos;
    const Spreadsheet& sheet;

    char peek() {
        while (pos < expr.length() && std::isspace(expr[pos])) pos++;
        if (pos >= expr.length()) return '\0';
        return expr[pos];
    }

    char get() {
        char c = peek();
        if (c != '\0') pos++;
        return c;
    }

    double parseFactor() {
        char c = peek();
        if (c == '+') { get(); return parseFactor(); }
        if (c == '-') { get(); return -parseFactor(); }
        if (c == '(') {
            get();
            double result = parseExpression();
            if (peek() == ')') get();
            return result;
        }

        // Parse number or CellRef
        std::string token = "";
        while (pos < expr.length() && (std::isalnum(expr[pos]) || expr[pos] == '.')) {
            token += expr[pos++];
        }

        if (token.empty()) return 0.0;

        // Is it a number?
        if (std::isdigit(token[0]) || token[0] == '.') {
            try {
                return std::stod(token);
            } catch (...) {
                return 0.0;
            }
        }

        // Must be a CellRef
        int r, col;
        if (FormulaEvaluator::parseCellReference(token, r, col)) {
            Node* n = sheet.get(r, col);
            if (n && n->isNumeric) {
                return n->numericValue;
            }
        }

        return 0.0; // Fallback para referencias inválidas o vacías
    }

    double parseTerm() {
        double result = parseFactor();
        while (true) {
            char c = peek();
            if (c == '*') { get(); result *= parseFactor(); }
            else if (c == '/') { 
                get(); 
                double divisor = parseFactor();
                if (divisor != 0.0) result /= divisor;
                else result = 0.0; // Evitar división por cero
            }
            else break;
        }
        return result;
    }

public:
    Parser(const std::string& e, const Spreadsheet& s) : expr(e), pos(0), sheet(s) {}

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            char c = peek();
            if (c == '+') { get(); result += parseTerm(); }
            else if (c == '-') { get(); result -= parseTerm(); }
            else break;
        }
        return result;
    }
};


bool FormulaEvaluator::evaluate(const std::string& formula, const Spreadsheet& sheet, double& outResult) {
    if (formula.empty() || formula[0] != '=') return false;

    try {
        Parser p(formula.substr(1), sheet); // skip '='
        outResult = p.parseExpression();
        return true;
    } catch (...) {
        return false;
    }
}

bool FormulaEvaluator::parseCellReference(const std::string& ref, int& outRow, int& outCol) {
    if (ref.empty()) return false;
    size_t i = 0;
    outCol = 0;
    while (i < ref.length() && std::isalpha(ref[i])) {
        outCol = outCol * 26 + (std::toupper(ref[i]) - 'A' + 1);
        i++;
    }
    if (i == 0 || i == ref.length()) return false; // Falta columna o fila
    
    outCol -= 1; // 0-indexed

    try {
        outRow = std::stoi(ref.substr(i)) - 1; // 0-indexed
        return outRow >= 0;
    } catch (...) {
        return false;
    }
}

std::string FormulaEvaluator::indexToColString(int col) {
    std::string res = "";
    col++;
    while (col > 0) {
        int rem = (col - 1) % 26;
        res += (char)('A' + rem);
        col = (col - 1) / 26;
    }
    std::reverse(res.begin(), res.end());
    return res;
}
