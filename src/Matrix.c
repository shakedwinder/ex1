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
}

void matrix_destroy(PMatrix matrix) {
    for (int i = 0; i < matrix->height; i++) {
        free(matrix->numbers[i]);
    }
    free(matrix->numbers);
    free(matrix);
}
