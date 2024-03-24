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

/**
 * @brief Main composing function 
 * 
 * @param msg Message to compose
 * @param send Byte array which will be sent back
 * @param len Length of byte array
 * @return int Result code
 */
int compose(t_msg msg, t_string * send, int * len);

#endif