#include "ErrorCode.h"

bool error_isSuccess(const ErrorCode code) {
    return code == ERROR_SUCCESS;
}

const char* error_getErrorMessage(const ErrorCode code) {
    switch (code) {
        case ERROR_SUCCESS: return "there is no error";
        case ERROR_NO_VALUE: return "error: did not get a value";
        case ERROR_POINTER_NULL: return "error: get a pointer to NULL";
        case ERROR_FAILD_MEMORY_MALLOC: return "error: could not allocate memory";
        case ERROR_DONT_MATCH_MATRIX: return "error: dont match matrices";
        default : return "";
    }
}

