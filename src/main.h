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
 * @brief List of messages from server that were already confirmed 
 */
t_confirmed_list confirmed_msg;

/**
 * @brief Processes received packet
 * 
 * @return int Result code
 */
int process_command();

/**
 * @brief Routes command execution
 * 
 * @param cmd Command
 * @param msg Result message (e_null if was not used)
 * @return int Result code
 */
int exec(t_command cmd, t_msg * msg);

/**
 * @brief Processes received packet
 * 
 * @return int Result code
 */
int process_packet();

/**
 * @brief Confirms recieved message
 * 
 * @param msg Recieved message
 * @return int 
 */
int confirm_msg(t_msg msg);

/**
 * @brief Checks if this message id was already confirmed (thus processed)
 * 
 * @param msg Message
 * @return true Was confirmed
 * @return false Was not confirmed yet
 */
bool is_msg_confirmed(t_msg msg);

/**
 * @brief Adds message id to confirmed ids list
 * 
 * @param msg Message
 * @return int Result code
 */
int add_msg_confirmed(t_msg msg);

/**
 * @brief Incoming confirm handler
 * 
 * @param msg Confirm message
 * @return int Result code
 */
int received_confirm(t_msg msg);

/**
 * @brief Incoming reply handler
 * 
 * @param msg Reply message
 * @return int Result code
 */
int received_reply(t_msg msg);

/**
 * @brief Incoming msg handler
 * 
 * @param msg Msg message
 * @return int Result code
 */
int received_msg(t_msg msg);

/**
 * @brief Incoming err handler
 * 
 * @param msg Err message
 * @return int Result code
 */
int received_err(t_msg msg);

/**
 * @brief Checks if any message confirm timeout exceeded and resends them if needed
 * 
 * @param time_delta Time delta since last call
 * @return int Result code
 */
int process_timeouts(int time_delta);

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