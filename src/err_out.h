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
    err_malformed_arguments = 100
} error_code;
typedef enum {
    warn_placeholder = 0
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