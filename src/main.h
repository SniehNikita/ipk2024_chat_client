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
#include <poll.h>

#include "err_out.h"
#include "types.h"
#include "argv_parser.h"
#include "client.h"
#include "queue.h"
#include "state.h"

// Poll interval in miliseconds
#define POLL_INTERVAL 125

int errno = 0;

/**
 * @brief SIGPIPE signal (might be set directly in the program). If appeared - program is stopped
 */
bool is_sigpipe = false;

/**
 * @brief Counter for local messages 
 */
int client_msg_count = 0;

/**
 * @brief User data 
 */
t_user user;

/**
 * @brief Client messages records 
 */
t_queue * client_msg_queue;

/**
 * @brief Server messages records 
 */
t_queue * server_msg_queue;

/**
 * @brief Hadler for sigint signal
 * 
 * @param signal Signal received from the system
 */
void  sigintHandler(int signal);

/**
 * @brief Processes received packet
 * 
 * @return int Result code
 */
int process_command();

/**
 * @brief Processes received packet
 * 
 * @return int Result code
 */
int process_packet();

/**
 * @brief Checks if any message confirm timeout exceeded and resends them if needed
 * 
 * @param time_delta Time delta since last call
 * @return int Result code
 */
int process_timeouts(int time_delta);

/**
 * @brief Routes command execution
 * 
 * @param cmd Command
 * @param msg Result message (e_null if was not used)
 * @return int Result code
 */
int exec(t_command cmd, t_msg * msg);

/**
 * @brief Exits as standard exit() function but with freeing of memory
 * 
 * @param exit_code Exit code
 */
void stop(int exit_code);

#endif