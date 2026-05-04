#include "Spreadsheet.h"
#include "FormulaEvaluator.h"
#include <algorithm>
#include <limits>

Spreadsheet::Spreadsheet(int n, int m) : n_rows(n), n_cols(m) {
    rows.resize(n_rows, nullptr);
    cols.resize(n_cols, nullptr);
}

Spreadsheet::~Spreadsheet() {
    for (int i = 0; i < n_rows; ++i) {
        Node* curr = rows[i];
        while (curr) {
            Node* temp = curr;
            curr = curr->next_row; // Avanzar por la fila
            delete temp;
        }
    }
}

Node* Spreadsheet::get(int i, int j) const {
    if (i < 0 || j < 0 || i >= n_rows || j >= n_cols) return nullptr;
    Node* curr = rows[i];
    while (curr != nullptr && curr->pos_col < j) curr = curr->next_row;
    return (curr != nullptr && curr->pos_col == j) ? curr : nullptr;
}

void Spreadsheet::insert(int i, int j, const std::string& value) {
    if (i < 0 || j < 0) return;

    // Expansión dinámica de la matriz dispersa (Crece automáticamente)
    if (i >= n_rows) {
        rows.resize(i + 1, nullptr);
        n_rows = i + 1;
    }
    if (j >= n_cols) {
        cols.resize(j + 1, nullptr);
        n_cols = j + 1;
    }

    // Si la celda existe, la eliminamos primero para re-insertar
    remove(i, j);
    if (value.empty()) return;

    Node* new_node = new Node(i, j, value);

    // Enlazar en la FILA (recorriendo punteros next_row)
    Node* prev_r = nullptr;
    Node* curr_r = rows[i];
    while (curr_r != nullptr && curr_r->pos_col < j) {
        prev_r = curr_r;
        curr_r = curr_r->next_row;
    }
    if (prev_r == nullptr) {
        new_node->next_row = rows[i];
        rows[i] = new_node;
    } else {
        new_node->next_row = prev_r->next_row;
        prev_r->next_row = new_node;
    }

    // Enlazar en la COLUMNA (recorriendo punteros next_col)
    Node* prev_c = nullptr;
    Node* curr_c = cols[j];
    while (curr_c != nullptr && curr_c->pos_row < i) {
        prev_c = curr_c;
        curr_c = curr_c->next_col;
    }
    if (prev_c == nullptr) {
        new_node->next_col = cols[j];
        cols[j] = new_node;
    } else {
        new_node->next_col = prev_c->next_col;
        prev_c->next_col = new_node;
    }

    reevaluateFormulas();
}

void Spreadsheet::remove(int i, int j) {
    if (i < 0 || j < 0 || i >= n_rows || j >= n_cols) return;

    // Localizar y desenlazar de la FILA
    Node* prev_r = nullptr;
    Node* target = rows[i];
    while (target != nullptr && target->pos_col < j) {
        prev_r = target;
        target = target->next_row;
    }

    if (target == nullptr || target->pos_col != j) return; // No existe

    if (prev_r == nullptr) rows[i] = target->next_row;
    else prev_r->next_row = target->next_row;

    // Desenlazar de la COLUMNA
    Node* prev_c = nullptr;
    Node* curr_c = cols[j];
    while (curr_c != nullptr && curr_c != target) {
        prev_c = curr_c;
        curr_c = curr_c->next_col;
    }
    if (prev_c == nullptr) cols[j] = target->next_col;
    else prev_c->next_col = target->next_col;

    delete target;
    reevaluateFormulas();
}

// Implementación de SUMA de Rango
double Spreadsheet::sum(int i1, int j1, int i2, int j2) const {
    double total = 0;
    int r_start = std::min(i1, i2), r_end = std::max(i1, i2);
    int c_start = std::min(j1, j2), c_end = std::max(j1, j2);

    for (int i = r_start; i <= r_end; ++i) {
        if (i >= n_rows) break;
        Node* curr = rows[i];
        while (curr != nullptr && curr->pos_col <= c_end) {
            if (curr->pos_col >= c_start && curr->isNumeric) {
                total += curr->numericValue;
            }
            curr = curr->next_row;
        }
    }
    return total;
}

void Spreadsheet::reevaluateFormulas() {
    for (int i = 0; i < n_rows; ++i) {
        Node* curr = rows[i];
        while (curr) {
            if (!curr->rawContent.empty() && curr->rawContent[0] == '=') {
                double res = 0;
                if (FormulaEvaluator::evaluate(curr->rawContent, *this, res)) {
                    curr->numericValue = res;
                    curr->isNumeric = true;
                }
            }
            curr = curr->next_row;
        }
    }
}


// Operaciones de Eliminación
void Spreadsheet::deleteRow(int i) {
    if (i < 0 || i >= n_rows) return;
    // Mientras la fila tenga nodos, eliminamos el primero repetidamente
    while (rows[i] != nullptr) {
        remove(i, rows[i]->pos_col);
    }
}

void Spreadsheet::deleteCol(int j) {
    if (j < 0 || j >= n_cols) return;
    // Mientras la columna tenga nodos, eliminamos el primero repetidamente
    while (cols[j] != nullptr) {
        remove(cols[j]->pos_row, j);
    }
}

void Spreadsheet::deleteRange(int i1, int j1, int i2, int j2) {
    int r_start = std::min(i1, i2), r_end = std::max(i1, i2);
    int c_start = std::min(j1, j2), c_end = std::max(j1, j2);

    for (int i = r_start; i <= r_end; ++i) {
        if (i >= n_rows) break;
        Node* curr = rows[i];
        while (curr != nullptr) {
            Node* next = curr->next_row; // Guardar el siguiente antes de borrar el actual
            if (curr->pos_col >= c_start && curr->pos_col <= c_end) {
                remove(curr->pos_row, curr->pos_col);
            }
            curr = next;
        }
    }
}

// Operaciones de Agregación
double Spreadsheet::average(int i1, int j1, int i2, int j2) const {
    double total = 0;
    int count = 0;
    int r_start = std::min(i1, i2), r_end = std::max(i1, i2);
    int c_start = std::min(j1, j2), c_end = std::max(j1, j2);

    for (int i = r_start; i <= r_end; ++i) {
        if (i >= n_rows) break;
        Node* curr = rows[i];
        while (curr != nullptr && curr->pos_col <= c_end) {
            if (curr->pos_col >= c_start && curr->isNumeric) {
                total += curr->numericValue;
                count++;
            }
            curr = curr->next_row;
        }
    }
    return (count > 0) ? (total / count) : 0.0;
}

double Spreadsheet::max(int i1, int j1, int i2, int j2) const {
    double mx = -std::numeric_limits<double>::infinity();
    bool found = false;
    int r_start = std::min(i1, i2), r_end = std::max(i1, i2);
    int c_start = std::min(j1, j2), c_end = std::max(j1, j2);

    for (int i = r_start; i <= r_end; ++i) {
        if (i >= n_rows) break;
        Node* curr = rows[i];
        while (curr != nullptr && curr->pos_col <= c_end) {
            if (curr->pos_col >= c_start && curr->isNumeric) {
                if (curr->numericValue > mx) mx = curr->numericValue;
                found = true;
            }
            curr = curr->next_row;
        }
    }
    return found ? mx : 0.0;
}

double Spreadsheet::min(int i1, int j1, int i2, int j2) const {
    double mn = std::numeric_limits<double>::infinity();
    bool found = false;
    int r_start = std::min(i1, i2), r_end = std::max(i1, i2);
    int c_start = std::min(j1, j2), c_end = std::max(j1, j2);

    for (int i = r_start; i <= r_end; ++i) {
        if (i >= n_rows) break;
        Node* curr = rows[i];
        while (curr != nullptr && curr->pos_col <= c_end) {
            if (curr->pos_col >= c_start && curr->isNumeric) {
                if (curr->numericValue < mn) mn = curr->numericValue;
                found = true;
            }
            curr = curr->next_row;
        }
    }
    return found ? mn : 0.0;
}