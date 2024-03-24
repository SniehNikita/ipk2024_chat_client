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
        case err_string_overflow: fprintf(stderr, "Max string length overflowed");break;
        case err_malformed_arguments: fprintf(stderr, "Malformed arguments");break;
        case err_socket_creation_failed: fprintf(stderr, "Socket creation failed");break;
        case err_msg_null: fprintf(stderr, "Message is null");break;
        case err_msg_type_unknown: fprintf(stderr, "Message type is unknown");break;
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
        case warn_net_receive_failed: fprintf(stderr, "recvfrom funtion failed");break;
        case warn_net_send_failed: fprintf(stderr, "sendto function failed");break;
        default: fprintf(stderr, "Unknown warning");break;
    }
    if (msg != NULL) {
        fprintf(stderr, ". With message [%s]", msg);
    }
    fprintf(stderr, ".\n");
}