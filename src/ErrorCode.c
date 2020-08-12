#include "ErrorCode.h"

bool error_isSuccess(ErrorCode code) {
    if (code == ERROR_SUCCESS) {
        return true;
    }
    return false;
}

const char* error_getErrorMessage(ErrorCode code) {
    if (code == ERROR_SUCCESS) {
        return "there is no error";
    }
    if (code == ERROR_NO_MATRIX) {
        return "error: did not get a matrix";
    }
    if (code == ERROR_NO_HEIGHT) {
        return "error: did not get an height";
    }
    if (code == ERROR_NO_WIDTH) {
        return "error: did not get a width";
    }
    if (code == ERROR_NO_VALUE) {
        return "error: did not get a value";
    }
    if (code == ERROR_POINTER_NULL) {
        return "error: get a pointer to NULL";
    }
    if (code == ERROR_FAILD_MEMORY_MALLOC) {
        return "error: could not allocate memory";
    }
    if (code == ERROR_DONT_MATCH_MATRIX) {
        return "error: the matrixes dosent match for this action";
    }
    return "";
}

