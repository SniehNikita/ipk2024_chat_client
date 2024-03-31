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
#include <ctype.h>

#include "err_out.h"
#include "types.h"

extern int errno;

/**
 * @brief Main udp parsing function 
 * 
 * @param recv Received byte array
 * @param len Length of byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp(t_string recv, int len, t_msg * msg);

/**
 * @brief Parses udp confirm message
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_confirm(t_string recv, t_msg * msg);

/**
 * @brief Parses udp reply message
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_reply(t_string recv, t_msg * msg);

/**
 * @brief Parses udp auth message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_auth(t_string recv, t_msg * msg);

/**
 * @brief Parses udp join message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_join(t_string recv, t_msg * msg);

/**
 * @brief Parses udp msg message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_msg(t_string recv, t_msg * msg);

/**
 * @brief Parses udp err message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_err(t_string recv, t_msg * msg);

/**
 * @brief Parses udp bye message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_udp_bye(t_string recv, t_msg * msg);

/**
 * @brief Main tcp parsing function 
 * 
 * @param recv Received byte array
 * @param len Length of byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp(t_string recv, int len, t_msg * msg);

/**
 * @brief Parses tcp reply message
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp_reply(t_string recv, t_msg * msg);

/**
 * @brief Parses tcp auth message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp_auth(t_string recv, t_msg * msg);

/**
 * @brief Parses tcp join message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp_join(t_string recv, t_msg * msg);

/**
 * @brief Parses tcp msg message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp_msg(t_string recv, t_msg * msg);

/**
 * @brief Parses tcp err message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp_err(t_string recv, t_msg * msg);

/**
 * @brief Parses tcp bye message 
 * 
 * @param recv Received byte array
 * @param msg Result message
 * @return int Result code
 */
int parse_tcp_bye(t_string recv, t_msg * msg);

bool cmp_part(t_string str, char * ref, int start);

void uppercase(t_string * str);

void get_word(t_string * word, t_string str, int start);

bool get_dname(t_string * dname, t_string str, int start);

bool get_id(t_string * id, t_string str, int start);

bool get_secret(t_string * secret, t_string str, int start);

bool get_content(t_string * content, t_string str, int start);

bool is_whitespace(char c);

#endif