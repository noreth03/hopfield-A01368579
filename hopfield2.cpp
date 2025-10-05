#include <iostream>
#include <fstream>
#include <sstream>

#define M 7
#define N 7
#define TOTAL (M*N)
#define PATTERNS 4

// leer patron desde archivo a matriz 
void pattern_file(const char* filename, int matrix[M][N]) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << filename << "\n";
        return;
    }
    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < M) {
        std::istringstream iss(line);
        for (int col = 0; col < N; col++) {
            iss >> matrix[row][col];
            if (matrix[row][col] == 0) matrix[row][col] = -1; 
        }
        row++;
    }
    file.close();
}

// aplanar matriz 
void flatten_matrix(int rows, int cols, int matrix[][N], int row[1][TOTAL]) {
    int k = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            row[0][k++] = matrix[i][j];
}

// multiplicar row (1xTOTAL) * col (TOTALx1) para crear matriz TOTALxTOTAL
void transpose_matrix(int rows, int cols, int col[TOTAL][1], int row[1][TOTAL], int result[TOTAL][TOTAL]) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i][j] = col[i][0] * row[0][j];
}

// suma de matrices
void add_matrix(int rows, int cols, int matrix1[TOTAL][TOTAL], int matrix2[TOTAL][TOTAL], int sum_matrix[TOTAL][TOTAL]) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            sum_matrix[i][j] = matrix1[i][j] + matrix2[i][j];
}

// diagonal en 0
void diagonal(int size, int matrix[TOTAL][TOTAL]) {
    for (int i = 0; i < size; i++)
        matrix[i][i] = 0;
}

// buscar patron 
void pattern(int cols, int patt_arr[1][TOTAL], int row[1][TOTAL], int matrix[TOTAL][TOTAL]) {
    for (int k = 0; k < cols; k++) {
        row[0][k] = 0;
        for (int i = 0; i < cols; i++)
            row[0][k] += patt_arr[0][i] * matrix[i][k];
        row[0][k] = (row[0][k] > 0) ? 1 : -1;
    }
}

// compara u(1) y u(2)
bool compare_rows(int cols, int row1[1][TOTAL], int row2[1][TOTAL]) {
    for (int j = 0; j < cols; j++)
        if (row1[0][j] != row2[0][j]) return false;
    return true;
}

int main() {
    int patron1[M][N], patron2[M][N], patron3[M][N], patron4[M][N], nopatron[M][N];
    int W[TOTAL][TOTAL] = {0};

    pattern_file("patron1.txt", patron1);
    pattern_file("patron2.txt", patron2);
    pattern_file("patron3.txt", patron3);
    pattern_file("patron4.txt", patron4);
    pattern_file("nopatron.txt", nopatron);


    // se entrena la red
    for (int p = 0; p < PATTERNS; p++) {
        int row[1][TOTAL], col[TOTAL][1], temp[TOTAL][TOTAL];
        int (*pat)[N];
        if (p == 0) pat = patron1;
        else if (p == 1) pat = patron2;
        else if (p == 2) pat = patron3;
        else pat = patron4;

        flatten_matrix(M, N, pat, row);
        for (int i = 0; i < TOTAL; i++) col[i][0] = row[0][i];
        transpose_matrix(TOTAL, TOTAL, col, row, temp);
        add_matrix(TOTAL, TOTAL, W, temp, W);
    }
    diagonal(TOTAL, W);

    // !! cambiar el tercer parametro de flatten_matrix para probar otra matriz. opciones - patron1, patron2, patron3, patron4, nopatron
    int row_test[1][TOTAL];
    flatten_matrix(M, N, nopatron, row_test);
    pattern(TOTAL, row_test, row_test, W);

    // comparaciones con la W matrix
    bool reconocido = false;
    for (int p = 0; p < PATTERNS; p++) {
        int temp_row[1][TOTAL];
        int (*pat)[N];
        if (p == 0) pat = patron1;
        else if (p == 1) pat = patron2;
        else if (p == 2) pat = patron3;
        else pat = patron4;

        flatten_matrix(M, N, pat, temp_row);
        pattern(TOTAL, temp_row, temp_row, W);
        if (compare_rows(TOTAL, row_test, temp_row)) {
            reconocido = true;
            break;
        }
    }

    // fue reconocido? 
    if (reconocido) std::cout << "patron reconocido\n";
    else std::cout << "wrong! no se encontro\n";

    return 0;
}
