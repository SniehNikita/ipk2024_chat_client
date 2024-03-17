/**
 * @file err_out.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header file for argument parser (argv_parser.c)
 */

#ifndef __ARGV_PARSER_H__

#define __ARGV_PARSER_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "err_out.h"
#include "types.h"

/**
 * @brief Parses parameters
 * 
 * @param argc Parameter count
 * @param argv Array with parameters
 * @param argv_p Pointer to argument structure
 * @return int Result code
 */
int optRead(int argc, char **argv, argv_t *argv_p);

/**
 * @brief Prints help to stdin
 */
void printHelp();

#endif