/**
 * @file types.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header file for types declaration
 */

#ifndef __TYPES_H__

#define __TYPES_H__

#include <stdint.h>

/**
 * @brief Maximum string length for message content
 */
#define STR_MAX_LEN 65536

/**
 * @brief Protocol type 
 */
typedef enum {
    e_udp = 0,
    e_tcp = 1
} transport_protocol;

/**
 * @brief Command line arguments type 
 */
typedef struct t_argv {
    transport_protocol protocol;
    char host[256];
    uint16_t port;
    uint16_t udp_timeout;
    uint8_t udp_retransmission;
} t_argv;

/**
 * @brief Message type constants 
 */
typedef enum {
    e_confirm = 0x00,
    e_reply = 0x01,
    e_auth = 0x02,
    e_join = 0x03,
    e_msg = 0x04,
    e_err = 0xFE,
    e_bye = 0xFF
} t_msg_type;

/**
 * @brief MessageID predefined type 
 */
typedef uint8_t t_msg_id;

/**
 * @brief Common string (for message content etc.) 
 */
typedef unsigned char t_string[STR_MAX_LEN];

/**
 * @brief Content of confirm message 
 */
typedef struct t_msg_content_confirm {
    t_msg_id ref_id;
} t_msg_content_confirm;

/**
 * @brief Content of reply message 
 */
typedef struct t_msg_content_reply {
    uint8_t result;
    t_msg_id ref_id;
    t_string content;
} t_msg_content_reply;

/**
 * @brief Content of auth message 
 */
typedef struct t_msg_content_auth {
    t_string user_name;
    t_string display_name;
    t_string secret;
} t_msg_content_auth;

/**
 * @brief Content of join message 
 */
typedef struct t_msg_content_join {
    t_string channel_id;
    t_string display_name;
} t_msg_content_join;

/**
 * @brief Content of msg message 
 */
typedef struct t_msg_content_msg {
    t_string display_name;
    t_string msg;
} t_msg_content_msg;

/**
 * @brief Content of err message 
 */
typedef struct t_msg_content_err {
    t_string display_name;
    t_string msg;
} t_msg_content_err;

/**
 * @brief Union of all possible message contents 
 */
typedef union t_msg_content {
    t_msg_content_confirm confirm;
    t_msg_content_reply reply;
    t_msg_content_auth auth;
    t_msg_content_join join;
    t_msg_content_msg msg;
    t_msg_content_err err;
} t_msg_content;

/**
 * @brief Unified communication message
 */
typedef struct t_msg {
    t_msg_type type;
    t_msg_id id;
    t_msg_content content;
} t_msg;

/**
 * @brief States of messages in network communication 
 */
typedef enum {
    e_sent = 0,
    e_confirmed = 1
} t_msg_net_state;

#endif