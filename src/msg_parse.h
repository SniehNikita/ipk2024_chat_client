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

#include <string.h>
#include "err_out.h"
#include "types.h"

extern int errno;

/**
 * @brief Main parsing function 
 * 
 * @param recv Received byte array
 * @param len Length of byte array
 * @param msg Result message
 * @return int Result code
 */
int parse(t_string recv, int len, t_msg * msg);

/**
 * @brief Parses confirm message
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_confirm(t_string recv, t_msg * msg);

/**
 * @brief Parses reply message
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_reply(t_string recv, t_msg * msg);

/**
 * @brief Parses auth message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_auth(t_string recv, t_msg * msg);

/**
 * @brief Parses join message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_join(t_string recv, t_msg * msg);

/**
 * @brief Parses msg message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_msg(t_string recv, t_msg * msg);

/**
 * @brief Parses err message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_err(t_string recv, t_msg * msg);

/**
 * @brief Parses bye message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_bye(t_string recv, t_msg * msg);

#endif