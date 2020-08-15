#include <malloc.h>
#include <stdio.h>
#include "Matrix.h"

typedef struct Matrix
{
    double** numbers;
    int height;
    int width;
}Matrix;

ErrorCode matrix_create(PMatrix* matrix, uint32_t height, uint32_t width) {
    *matrix = (PMatrix) malloc(sizeof(PMatrix));
    if (*matrix == NULL) {
        return ERROR_FAILD_MEMORY_MALLOC;
    }

    (*matrix)->height = height;
    (*matrix)->width = width;
    (*matrix)->numbers = (double**)malloc(height * sizeof(double*));
    if ((*matrix)->numbers == NULL) {
        return ERROR_FAILD_MEMORY_MALLOC;
    }

    for (int i = 0; i < (*matrix)->height; i++) {
        (*matrix)->numbers[i] = (double*)malloc(width * sizeof(double));
        if ((*matrix)->numbers[i] == NULL) {
            return ERROR_FAILD_MEMORY_MALLOC;
        }
    }

    for (int i = 0; i < (*matrix)->height; i++) {
        for (int j = 0; j < (*matrix)->width; j++) {
            (*matrix)->numbers[i][j] = 0;
        }
    }

    return ERROR_SUCCESS;
}

ErrorCode matrix_copy(PMatrix* result, CPMatrix source) {
    (*result)->height = source->height;

    (*result)->width = source->width;

    if (source->numbers == NULL) {
        return ERROR_NO_MATRIX;
    }
    (*result)->numbers = source->numbers;

    return ERROR_SUCCESS;
}

void matrix_destroy(PMatrix matrix) {
    for (int i = 0; i < matrix->height; i++) {
        free(matrix->numbers[i]);
    }
    free(matrix->numbers);
    free(matrix);
}

ErrorCode matrix_getHeight(CPMatrix matrix, uint32_t* result) {
    *result = matrix->height;
    
    return ERROR_SUCCESS;
}

ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t* result) {
    *result = matrix->width;

    return ERROR_SUCCESS;
}

ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double value)
{
 matrix->numbers[rowIndex][colIndex] = value;

 return ERROR_SUCCESS;
}

ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double* value) 
{
    *value = matrix->numbers[rowIndex][colIndex];

    return ERROR_SUCCESS;
}

ErrorCode matrix_add(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs->height != rhs->height || lhs->width != rhs->width) {
        return ERROR_DONT_MATCH_MATRIX;
    }
    matrix_create(result, rhs->height, rhs->width);
    for (int i = 0; i < rhs->height; i++) {
        for (int j = 0; j < rhs->width; i++) {

            double* lvalue;
            *lvalue = 0.0;
            matrix_getValue(lhs, i, j, lvalue);
            double* rvalue;
            *rvalue = 0.0;
            matrix_getValue(lhs, i, j, rvalue);

            matrix_setValue(*result, i, j, *lvalue + *rvalue);
        }
    }
    return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyMatrices(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs->width != rhs->height) {
        return ERROR_DONT_MATCH_MATRIX;
    }
    matrix_create(result, lhs->height, rhs->width);
    for (int i = 0; i < (*result)->height; i++) {
        for (int j = 0; j < (*result)->width; j++) {
            double sum = 0.0;
            for (int n = 0; n < lhs->width; n++) {
                double* lvalue;
                *lvalue = 0.0;
                double* rvalue;
                *rvalue = 0.0;
                matrix_getValue(lhs, i, n, lvalue);
                matrix_getValue(rhs, n, j, rvalue);

                sum += (*lvalue)*(*rvalue);
            }
            matrix_setValue(*result, i, j, sum);
        }
    }
    return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
    for (int i = 0; i < matrix->height; i++) {
        for (int j = 0; j < matrix->width; j++) {
            double* value;
            *value = 0.0;
            matrix_getValue(matrix, i, j, value);
            double newval = (*value)*scalar;
            matrix_setValue(matrix, i, j, newval);
        }
    }
    return ERROR_SUCCESS;
}