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
    PMatrix* end;
    ErrorCode ec = ERROR_SUCCESS;
    ec = matrix_create(end, source->height, source->width);
    if (ec != ERROR_SUCCESS) {
        return ec;
    }
    for (int i = 0; i < source->height; i++) {
        for (int j =0; j < source->width; j++) {
            (*end)->numbers[i][j] = source->numbers[i][j];
        }
    }
    result = end;

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
    if (result == NULL) {
        return ERROR_NO_VALUE;
    }
    if (matrix == NULL) {
        return ERROR_POINTER_NULL;
    }
    *result = matrix->height;
    
    return ERROR_SUCCESS;
}

ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t* result) {
    if (result == NULL) {
        return ERROR_POINTER_NULL;
    }
    if (matrix == NULL) {
        return ERROR_POINTER_NULL;
    }
    *result = matrix->width;

    return ERROR_SUCCESS;
}

ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double value)
{
    if (matrix == NULL) {
        return ERROR_POINTER_NULL;
    }
    matrix->numbers[rowIndex][colIndex] = value;
    
    return ERROR_SUCCESS;
}

ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double* value) 
{
    if (value == NULL) {
        return ERROR_POINTER_NULL;
    }
    if (matrix == NULL) {
        return ERROR_POINTER_NULL;
    }
    *value = matrix->numbers[rowIndex][colIndex];

    return ERROR_SUCCESS;
}

ErrorCode matrix_add(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs == NULL) {
        return ERROR_POINTER_NULL;
    }
    if (rhs == NULL) {
        return ERROR_POINTER_NULL;
    }
    if (lhs->height != rhs->height || lhs->width != rhs->width) {
        return ERROR_DONT_MATCH_MATRIX;
    }
    ErrorCode ec = ERROR_SUCCESS;
    ec = matrix_create(result, rhs->height, rhs->width);
    if (ec != ERROR_SUCCESS) {
        return ec;
    }
    for (int i = 0; i < rhs->height; i++) {
        for (int j = 0; j < rhs->width; i++) {

            double* lvalue;
            *lvalue = 0.0;
            ec = matrix_getValue(lhs, i, j, lvalue);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }
            double* rvalue;
            *rvalue = 0.0;
            ec = matrix_getValue(lhs, i, j, rvalue);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }

            ec = matrix_setValue(*result, i, j, *lvalue + *rvalue);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }
        }
    }
    return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyMatrices(PMatrix* result, CPMatrix lhs, CPMatrix rhs) {
    if (lhs == NULL) {
        return ERROR_POINTER_NULL;
    }
    if (rhs == NULL) {
        return ERROR_POINTER_NULL;
    }
    if (lhs->width != rhs->height) {
        return ERROR_DONT_MATCH_MATRIX;
    }
    ErrorCode ec = ERROR_SUCCESS;
    ec = matrix_create(result, lhs->height, rhs->width);
    if (ec != ERROR_SUCCESS) {
        return ec;
    }
    for (int i = 0; i < (*result)->height; i++) {
        for (int j = 0; j < (*result)->width; j++) {
            double sum = 0.0;
            for (int n = 0; n < lhs->width; n++) {
                double* lvalue;
                *lvalue = 0.0;
                double* rvalue;
                *rvalue = 0.0;
                ec = matrix_getValue(lhs, i, n, lvalue);
                if (ec != ERROR_SUCCESS) {
                    return ec;
                }
                ec = matrix_getValue(rhs, n, j, rvalue);
                if (ec != ERROR_SUCCESS) {
                    return ec;
                }
                sum += (*lvalue)*(*rvalue);
            }
            ec = matrix_setValue(*result, i, j, sum);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }
        }
    }
    return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
    if (matrix == NULL) {
        return ERROR_POINTER_NULL;
    }
    ErrorCode ec = ERROR_SUCCESS;
    for (int i = 0; i < matrix->height; i++) {
        for (int j = 0; j < matrix->width; j++) {

            double* value;
            *value = 0.0;
            ec = matrix_getValue(matrix, i, j, value);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }
            
            double newval = (*value)*scalar;
            ec = matrix_setValue(matrix, i, j, newval);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }
        }
    }
    return ERROR_SUCCESS;
}