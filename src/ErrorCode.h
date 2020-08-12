#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ERROR_NO_MATRIX = 1,
    ERROR_NO_HEIGHT = 2,
    ERROR_NO_WIDTH = 3,
    ERROR_NO_VALUE = 4,
    ERROR_POINTER_NULL = 5,
    ERROR_FAILD_MEMORY_MALLOC = 6,
    // DON'T CHANGE THIS VALUE!
    ERROR_SUCCESS = 0,
} ErrorCode;

/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code);

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const char* error_getErrorMessage(ErrorCode code);