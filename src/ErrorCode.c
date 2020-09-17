#include "ErrorCode.h"

bool error_isSuccess(ErrorCode code) {
    return code == ERROR_SUCCESS;
}

const char* error_getErrorMessage(ErrorCode code) {
    switch (code) {
        case ERROR_SUCCESS: return "there is no error";
        case ERROR_NO_MATRIX: return "error: did not get a matrix";
        case ERROR_NO_HEIGHT: return "error: did not get an height";
        case ERROR_NO_WIDTH: return "error: did not get a width";
        case ERROR_NO_VALUE: return "error: did not get a value";
        case ERROR_POINTER_NULL: return "error: get a pointer to NULL";
        case ERROR_FAILD_MEMORY_MALLOC: return "error: could not allocate memory";
        case ERROR_DONT_MATCH_MATRIX: return "error: dont match matrices";
        default : return "";
    }
}

