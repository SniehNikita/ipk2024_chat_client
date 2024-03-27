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
#include "command.h"
#include "msg_parse.h"
#include "msg_compose.h"


extern int errno;
extern int lcl_msg_count;

/**
 * @brief SIGPIPE signal (might be set directly in the program). If appeared - program is stopped
 */
extern bool is_sigpipe;

/**
 * @brief Routes fd creation
 * 
 * @param argv Parsed program arguments
 * @param fd Result fd
 * @return Result code
 */
int client(t_argv argv, int * fd);

/**
 * @brief Routes message send
 * 
 * @param buf Result message
 * @param buf_size Result message length
 * @return int Result code
 */
int client_send(t_msg msg);

/**
 * @brief Routes message read
 * 
 * @param buf Result message
 * @param buf_size Result message length
 * @return int Result code
 */
int client_read(t_string * buf, int * buf_size);

/**
 * @brief Closes client. Clears all client-allocated resources
 * 
 * @return int Result code
 */
int client_close();

/**
 * @brief Creates UDP fd
 * 
 * @return Result code
 */
int udp_client();

/**
 * @brief Sends UDP message
 * 
 * @param buf Message to be send
 * @param buf_size Length of message to be send
 * @return int Result code
 */
int udp_send(t_string buf, int buf_size);

/**
 * @brief Reads UDP message
 * 
 * @param buf Result message
 * @param buf_size Result message length
 * @return int Result code
 */
int udp_read(t_string * buf, int * buf_size);

/**
 * @brief Creates TCP fd
 * 
 * @param argv Parsed program arguments
 * @return Result code
 */
int tcp_client(t_argv argv);

/**
 * @brief Sends TCP message
 * 
 * @param buf Message to be send
 * @param buf_size Length of message to be send
 * @return int Result code
 */
int tcp_send(t_string buf, int buf_size);

/**
 * @brief Reads TCP message
 * 
 * @param buf Result message
 * @param buf_size Result message length
 * @return int Result code
 */
int tcp_read(t_string * buf, int * buf_size);

#endif