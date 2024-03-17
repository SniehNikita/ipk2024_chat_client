/**
 * @file err_out.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Prints error/warning message to stdout 
 */

#include "err_out.h"

int printErrMsg(error_code errno, int lineno, char *file_name, char *msg) {
    fprintf(stderr, "Error [%d] on line [%d]", errno, lineno);
    if (file_name != NULL) {
        fprintf(stderr, " in file [%s]: ", file_name);
    } else {
        fprintf(stderr, ": ");
    }
    switch (errno) {
        case err_malformed_arguments: fprintf(stderr, "Malformed arguments");break;
        default: fprintf(stderr, "Unknown error");break;
    }
    if (msg != NULL) {
        fprintf(stderr, ". With message [%s]", msg);
    }
    fprintf(stderr, ". Program was stopped.\n");
    return errno;
}

void printWarnMsg(warning_code warno, int lineno, char *file_name, char *msg) {
    fprintf(stderr, "Warning [%d] on line [%d]", warno, lineno);    
    if (file_name != NULL) {
        fprintf(stderr, " in file [%s]: ", file_name);
    } else {
        fprintf(stderr, ": ");
    }
    switch (warno) {
        default: fprintf(stderr, "Unknown warning");break;
    }
    if (msg != NULL) {
        fprintf(stderr, ". With message [%s]", msg);
    }
    fprintf(stderr, ".\n");
}