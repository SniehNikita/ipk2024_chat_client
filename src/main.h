/**
 * @file main.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header file for main entry point (main.c) 
 */

#ifndef __MAIN_H__

#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>

#include "err_out.h"
#include "types.h"
#include "argv_parser.h"

int errno = 0;

/**
 * @brief Pointer to parsed arguments structure 
 */
argv_t * argv_p;

/**
 * @brief Hadler for sigint signal
 * 
 * @param signal Signal received from the system
 */
void  sigintHandler(int signal);

/**
 * @brief Exits as standard exit() function but with freeing of memory
 * 
 * @param exit_code Exit code
 */
void stop(int exit_code);

#endif