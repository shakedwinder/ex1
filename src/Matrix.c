#include <malloc.h>
#include <stdio.h>
#include "Matrix.h"

typedef struct Matrix
{
    double** numbers;
    unsigned int height;
    unsigned int width;
}Matrix;

ErrorCode matrix_create(PMatrix* matrix, uint32_t height, uint32_t width) {
    PMatrix* matrix1;
    *matrix1 = (PMatrix) malloc(sizeof(PMatrix));
    if (*matrix1 == NULL) {
        return ERROR_FAILD_MEMORY_MALLOC;
    }

    (*matrix1)->height = height;
    (*matrix1)->width = width;
    (*matrix1)->numbers = (double**)calloc(height, sizeof(double*));
    if ((*matrix1)->numbers == NULL) {
        free(*matrix1);
        return ERROR_FAILD_MEMORY_MALLOC;
    }

    for (int i = 0; i < (*matrix1)->height; i++) {
        (*matrix1)->numbers[i] = (double*)calloc(width, sizeof(double));
        if ((*matrix1)->numbers[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free((*matrix1)->numbers[j]);
            }
            free((*matrix1)->numbers);
            free(*matrix1);
            return ERROR_FAILD_MEMORY_MALLOC;
        }
    }

    for (int i = 0; i < (*matrix1)->height; i++) {
        for (int j = 0; j < (*matrix1)->width; j++) {
            (*matrix1)->numbers[i][j] = 0;
        }
    }
    matrix = matrix1;

    return ERROR_SUCCESS;
}

ErrorCode matrix_copy(PMatrix* result, CPMatrix source) {
    PMatrix* end;
    ErrorCode ec = ERROR_SUCCESS;
    //create new matrix with the source values
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
    if (rowIndex >= matrix->height || colIndex >= matrix->width) {
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
    if (rowIndex >= matrix->height || colIndex >= matrix->width) {
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
    //ec is for checking every call to function bring us an error 
    ErrorCode ec = ERROR_SUCCESS;
    //create new matrix to put the final matrix in
    ec = matrix_create(result, rhs->height, rhs->width);
    if (ec != ERROR_SUCCESS) {
        return ec;
    }
    //move on every place and bring the sum to the new matrix
    for (int i = 0; i < rhs->height; i++) {
        for (int j = 0; j < rhs->width; j++) {

            double lval = 0.0;
            double* lvalue;
            lvalue = &lval;
            ec = matrix_getValue(lhs, i, j, lvalue);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }

            double rval = 0.0;
            double* rvalue;
            rvalue = &rval;
            ec = matrix_getValue(rhs, i, j, rvalue);
            if (ec != ERROR_SUCCESS) {
                return ec;
            }

            //set the final after + in the place
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
    //ec is for checking every call to function bring us an error 
    ErrorCode ec = ERROR_SUCCESS;
    ec = matrix_create(result, lhs->height, rhs->width);
    if (ec != ERROR_SUCCESS) {
        return ec;
    }
    //multiply matrices by the rules
    for (int i = 0; i < (*result)->height; i++) {
        for (int j = 0; j < (*result)->width; j++) {
            double sum = 0.0;
            for (int n = 0; n < lhs->width; n++) {
                double lval = 0.0;
                double* lvalue;
                lvalue = &lval;

                double rval;
                double* rvalue;
                rvalue = &rval;
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
    //ec is for checking every call to function bring us an error 
    ErrorCode ec = ERROR_SUCCESS;
    //multiply every value in the matrix with the scalar
    for (int i = 0; i < matrix->height; i++) {
        for (int j = 0; j < matrix->width; j++) {
            double val = 0.0;
            double* value;
            value = &val;
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