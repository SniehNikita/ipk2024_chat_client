/**
 * @file msg_parse.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Header for message parser (msg_parse.c)
 *
 * Takes byte string and transforms it into internal message type 
 */

#ifndef __MSG_PARSE_H__

#define __MSG_PARSE_H__

#include "err_out.h"
#include "types.h"

/**
 * @brief Main parsing function 
 * 
 * @param recv Received byte array
 * @param len Length of byte array
 * @param msg Result message
 * @return int Result code
 */
int parse(t_string * recv, int len, t_msg * msg);

#endif