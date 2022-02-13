//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Caesar Dai
//
#include <stdio.h>
#include <stdlib.h>


int checkMagic(int numRow, int numCol, int** matrix, int magic_constant);

int main() { //int num_args, char* argv[]

    int **matrix;
    int *rowSum_arr;

    int nRow, mCol, magic_constant; // row, column
    scanf(" %d", &nRow);
    scanf(" %d", &mCol);


    rowSum_arr = malloc(sizeof(int) * nRow);
    if (rowSum_arr == NULL) {
        printf("allocate new memory failed. Exit.\n");
        exit(1);
    }
   
    matrix = malloc(sizeof(int*) * nRow);    // allocate an array of N ponters to ints
    if (matrix == NULL) {                    // because each pointer will point to an array of N integers
        printf("allocate new memory failed. Exit.\n");
        exit(1);
    }
    
    for (int i = 0; i < nRow; i++) {                // for each row, malloc space for the column elements and 
        matrix[i] = malloc(sizeof(int) * mCol);     // add it to the array of arrays 
        if (matrix[i] == NULL) {
        printf("allocate new memory failed. Exit.\n");
        exit(1);
        }
    }

    for (int i = 0; i < nRow; i++) {        // fill matrix
        for (int j = 0; j <  mCol; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    magic_constant = 0;                 // calculate magic constant for comparison
    for (int i = 0; i < nRow; i++) {
        magic_constant = magic_constant + matrix[i][0]; 
    }

    // check whether it's magic or not
    int result = checkMagic(nRow, mCol, matrix, magic_constant);
    if (result == 1) {
        printf("M is a magic square (magic constant = %d)\n", magic_constant);
    }
    else if (result == 0) {
        printf("M is NOT a magic square (magic constant = %d)\n", magic_constant);
    }
    
    free(rowSum_arr);
    rowSum_arr = NULL;
    free(matrix);
    matrix = NULL;

    return 0;
}

int checkMagic(int numRow, int numCol, int** matrix, int magic_constant) {

    for (int i = 0; i < numRow; i++) {
        int prev_row_sum = 0, prev_col_sum = 0;

        // check row sum
        for (int j = 0; j < numRow; j++) {
            prev_row_sum += matrix[j][i];
        }
        if (prev_row_sum != magic_constant) {
            return 0; // false
        }

        // check col sum
        for (int j = 0; j < numCol; j++) {
            prev_col_sum += matrix[i][j];
        }
        if (prev_col_sum != magic_constant) {
            return 0; // false
        }
    }

    // check left to right diagnal
    int l_to_r_diag = 0;
    for (int i = 0; i < numRow; i++) {
        int col = 0;
        l_to_r_diag += matrix[i][col];
        col++;
    }
    if (l_to_r_diag != magic_constant) {
        return 0;
    }

    // check right to left diagnal
    int r_to_l_diag = 0;
    for (int i = 0; i < numRow; i++) {
        r_to_l_diag += matrix[i][numCol - 1 - i];
    }
    if (r_to_l_diag != magic_constant) {
        return 0;
    }

    return 1; // true
}

