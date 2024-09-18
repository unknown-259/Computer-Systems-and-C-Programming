// Author: Nathan Tran (ntran48@ucsc.edu)

// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define FP_DELTA 0.0001

/* Prints the 3X3 matrix
 * Doesn't return anything
 */
void MatrixPrint(float mat[3][3]) {
    // Loops through the rows
    for (int row = 0; row < 3; row++) {
        printf(" ---------------------------\n |");
        // Loops through the columns
        for (int col = 0; col < 3; col++) {
            printf("  %.2f  |", (float) mat[row][col]);
        }
        printf("\n");
    }
}

/* Compares the two 3X3 matrices
 * Returns True if two matrices are the same within tolerance, else False
 */
int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (fabs(mat1[row][col] - mat2[row][col]) > FP_DELTA) {
                return 0;
            }
        }
    }
    return 1;
}

/* Adds two 3X3 matrices
 * Doesn't return, but modifies third argument
 */
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result[row][col] = mat1[row][col] + mat2[row][col];
        }
    }
}

/* Multiplies two 3X3 matrices
 * Doesn't return, but modifies third argument
 */
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    // Needs to check if the matrices are constants such as all ones.
    // It doesn't modify properly if ran without this check
    int bools = 1;
    float firstElement1 = mat1[0][0];
    float firstElement2 = mat2[0][0];
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (mat1[row][col] != firstElement1){
                bools = 0;
            }
            if (mat2[row][col] != firstElement2){
                bools = 0;
            }
        }
   }
    
    // Decides which to run based on the check above
    if (bools == 0) {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                result[row][col] = 0;
                for (int resultCounter = 0; resultCounter < 3; resultCounter++) {
                    result[row][col] += mat1[row][resultCounter] * mat2[resultCounter][col];
                }
            }
        }
    } else {
        int calculated[3][3] = {0};
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                result[row][col] = 0;
                for (int resultCounter = 0; resultCounter < 3; resultCounter++) {
                    if (!calculated[row][col]) {
                        result[row][col] += mat1[row][resultCounter] * mat2[resultCounter][col];
                        calculated[row][col] = 1;
                    }
                }
            }
        }
    }
}

/* Adds a scalar to a 3X3 matrix
 * Doesn't return, but modifies third argument
 */
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result[row][col] = mat[row][col] + x;
        }
    }
}

/* Multiplies a scalar to a 3X3 matrix
 * Doesn't return, but modifies third argument
 */
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result[row][col] = mat[row][col] * x;
        }
    }
}

/* Calculates the trace (sum of diagonal) of a 3X3 matrix
 * Returns a float, the sum of diagonal
 */
float MatrixTrace(float mat[3][3]) {
    float sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += mat[i][i];
    }
    return sum;
}

/* Calculates the transpose of a 3X3 matrix
 * Doesn't return, but modifies second argument
 */
void MatrixTranspose(float mat[3][3], float result[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result[col][row] = mat[row][col];
        }
    }
}

/* Finds the submatrix of a 3X3 by removing a row and column
 * Doesn't return, but modifies fourth argument
 */
void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int row_index = 0;
    int col_index = 0;
    for (int row = 0; row < 3; row++) {
        if (row+1 == i) {
            // Skips row i
            continue;
        }
        for (int col = 0; col < 3; col++) {
            if (col+1 == j) {
                // Skips j column
                continue;
            }
            result[row_index][col_index] = mat[row][col];
            col_index++;
        }
        // Resets to zero for next row
        col_index = 0;
        row_index++;
    }
}

/* Calculates the determinant of a 3X3 matrix
 * Returns a float, the determinant
 */
float MatrixDeterminant(float mat[3][3]) {
    float det = 0;
    det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) +
          mat[0][1] * (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) +
          mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return det;
}

/* Calculates the inverse of a 3X3 matrix
 * Doesn't return, but modifies second argument
 */
void MatrixInverse(float mat[3][3], float result[3][3]) {
    float det = MatrixDeterminant(mat);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result[col][row] = ((mat[(row + 1) % 3][(col + 1) % 3] * 
                               mat[(row + 2) % 3][(col + 2) % 3]) - 
                               (mat[(row + 1) % 3][(col + 2) % 3] * 
                               mat[(row + 2) % 3][(col + 1) % 3])) / det;
        }
    }
}
