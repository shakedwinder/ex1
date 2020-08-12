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

    for (int i = 0; i < height; i++) {
        (*matrix)->numbers[i] = (double*)malloc(width * sizeof(double));
        if ((*matrix)->numbers[i] == NULL) {
            return ERROR_FAILD_MEMORY_MALLOC;
        }
    }

    return ERROR_SUCCESS;
}

ErrorCode matrix_copy(PMatrix* result, CPMatrix source) {
    if (source->height == NULL) {
        return ERROR_NO_HEIGHT;
    }
    (*result)->height = source->height;

    if (source->width == NULL) {
        return ERROR_NO_WIDTH;
    }
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
    if (matrix->height == NULL) {
        return ERROR_NO_HEIGHT;
    }
    *result = matrix->height;
    
    return ERROR_SUCCESS;
}

ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t* result) {
    if (matrix->width == NULL) {
        return ERROR_NO_WIDTH;
    }
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
            matrix_getValue(lhs, i, j, lvalue);
            double* rvalue;
            matrix_getValue(lhs, i, j, rvalue);

            matrix_setValue(*result, i, j, *lvalue + *rvalue);
        }
    }
    return ERROR_SUCCESS;
}