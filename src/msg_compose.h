/**
 * @file msg_compose.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Header for message compose (msg_compose.c)
 *
 * Takes internal message type and transforms it into byte string 
 */

#ifndef __MSG_COMPOSE_H__

#define __MSG_COMPOSE_H__

#include <string.h>

#include "err_out.h"
#include "types.h"

extern int errno;

/**
 * @brief Main udp composing function 
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp confirm message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_confirm(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp reply message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_reply(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp auth message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_auth(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp join message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_join(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp msg message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_msg(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp err message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_err(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose udp bye message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_udp_bye(t_msg msg, t_string * send, int * len);

/**
 * @brief Main tcp composing function 
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp confirm message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_confirm(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp reply message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_reply(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp auth message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_auth(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp join message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_join(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp msg message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_msg(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp err message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_err(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose tcp bye message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_tcp_bye(t_msg msg, t_string * send, int * len);

/**
 * @brief String copy
 * 
 * @param dst Destination
 * @param src Source
 * @param index Start index in source
 * @param len Length of copied string
 * @return int Result code
 */
int compose_string(t_string * dst, t_string src, int index, int * len);

#endif