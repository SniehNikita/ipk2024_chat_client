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

#include "err_out.h"
#include "types.h"

extern int errno;

/**
 * @brief Main composing function 
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose confirm message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_confirm(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose reply message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_reply(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose auth message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_auth(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose join message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_join(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose msg message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_msg(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose err message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_err(t_msg msg, t_string * send, int * len);

/**
 * @brief Compose bye message
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose_bye(t_msg msg, t_string * send, int * len);

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