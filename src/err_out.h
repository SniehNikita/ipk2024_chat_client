/**
 * @file err_out.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header for error output (err_out.c)
 *
 * Defines methods to output warning or error message to stdout. 
 */

#ifndef __ERR_OUT_H__

#define __ERR_OUT_H__

#include <stdio.h>

typedef enum {
    // Common
    err_string_overflow = 1,
    // Arguments
    err_malformed_arguments = 100,
    // UDP client
    err_socket_creation_failed = 200,
    err_host_not_found = 201,
    // Parser
    err_msg_null = 400,
    err_msg_type_unknown = 401,
    // Command
    err_cmd_corrupted = 500,
    err_command_not_found = 501
} error_code;
typedef enum {
    warn_net_receive_failed = 200,
    warn_net_send_failed = 201,
    warn_too_many_argv = 500
} warning_code;

/**
 * @brief Prints error message to stdout. Program should exit if error occured
 * 
 * @param errno Code of error
 * @param lineno Number of line where error occured
 * @param file_name Name of file where error occured
 * @param msg Optional message to be printed
 * @return Received errno parameter
 */
int printErrMsg(error_code errno, int lineno, char *file_name, char *msg);

/**
 * @brief Prints error message to stdout. Program can continue if warning occured
 * 
 * @param warno Code of warning
 * @param lineno Number of line where warning occured
 * @param file_name Name of file where warning occured
 * @param msg Optional message to be printed
 */
void printWarnMsg(warning_code warno, int lineno, char *file_name, char *msg);

#endif