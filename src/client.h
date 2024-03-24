/**
 * @file client.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header file for client implementation 
 */

#ifndef __CLIENT_H__

#define __CLIENT_H__

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h> 
#include <netdb.h>

#include "err_out.h"
#include "types.h"
#include "msg_parse.h"
#include "msg_compose.h"


extern int errno;
/**
 * @brief SIGPIPE signal (might be set directly in the program). If appeared - program is stopped
 */
extern bool is_sigpipe;

/**
 * @brief Main method for client creation
 * 
 * @param argv Parsed program arguments
 * @return Result code
 */
int client(t_argv * argv);

/**
 * @brief UDP client variant
 * 
 * @param argv Parsed program arguments
 * @return Result code
 */
int udp_client(t_argv * argv);

/**
 * @brief TCP client variant
 * 
 * @param argv Parsed program arguments
 * @return Result code
 */
int tcp_client(t_argv * argv);

/**
 * @brief Message process for both tcp and udp
 * 
 * @param msg_recv Message received from server
 * @param msg_send Result message (will be sent as response)
 * @return int 
 */
int process(t_msg msg_recv, t_msg * msg_send);

#endif