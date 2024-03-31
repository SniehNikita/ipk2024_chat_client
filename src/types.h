/**
 * @file types.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header file for types declaration
 */

#ifndef __TYPES_H__

#define __TYPES_H__

#include <stdint.h>
#include <stdbool.h>

#define USERNAME_MAX_LEN 20
#define CHANNELID_MAX_LEN 20
#define DISPLAYNAME_MAX_LEN 20
#define SECRET_MAX_LEN 128
#define MESSAGE_MAX_LEN 1400

/**
 * @brief Maximum string length for message content
 */
#define STR_MAX_LEN MESSAGE_MAX_LEN + USERNAME_MAX_LEN + DISPLAYNAME_MAX_LEN + SECRET_MAX_LEN

/**
 * @brief Maximum number of parameters in command 
 */
#define CMD_MAX_ARGV 4

/**
 * @brief Maximum number of confirmed messages ids in t_confirmed_list 
 */
#define CONFIRM_MAX_REC 256

/**
 * @brief Protocol type 
 */
typedef enum {
    e_udp = 0,
    e_tcp = 1
} transport_protocol;

/**
 * @brief Common string (for message content etc.) 
 */
typedef char t_string[STR_MAX_LEN];

/**
 * @brief Command line arguments type 
 */
typedef struct t_argv {
    transport_protocol protocol;
    t_string host;
    uint16_t port;
    int64_t udp_timeout;
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
    e_bye = 0xFF,
    e_null = 0xF0
} t_msg_type;

/**
 * @brief MessageID predefined type 
 */
typedef uint8_t t_msg_id;

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

/**
 * @brief Types of available commands 
 */
typedef enum {
    e_cmd_null = 0,
    e_cmd_msg = 1,
    e_cmd_auth = 2,
    e_cmd_join = 3,
    e_cmd_rename = 4,
    e_cmd_help = 5,
    e_cmd_close = 0xFF // Custom: sends BYE message to server
} t_command_type;

/**
 * @brief Message command type 
 */
typedef struct t_command_msg {
    t_string msg;
} t_command_msg;

/**
 * @brief Authorization command type 
 */
typedef struct t_command_auth {
    t_string user_name;
    t_string display_name;
    t_string secret;
} t_command_auth;

/**
 * @brief Join command type 
 */
typedef struct t_command_join {
    t_string channel_id;
} t_command_join;

/**
 * @brief Rename command type 
 */
typedef struct t_command_rename {
    t_string display_name;
} t_command_rename;

/**
 * @brief Unified command content 
 */
typedef union t_command_content {
    t_command_msg msg;
    t_command_auth auth;
    t_command_join join;
    t_command_rename rename;
} t_command_content;

/**
 * @brief Unified command type 
 */
typedef struct t_command {
    t_command_type type;
    t_command_content content;
} t_command;

/**
 * @brief Array of command arguments 
 */
typedef t_string t_command_argv[CMD_MAX_ARGV];

typedef enum {
    e_state_null = 0,
    e_state_start = 1,
    e_state_auth = 2,
    e_state_open = 3,
    e_state_err = 4,
    e_state_end = 5
} t_state;

/**
 * @brief User data 
 */
typedef struct t_user {
    t_string user_name;
    t_string display_name;
    t_string channel_id;
    t_state state;
} t_user;

/**
 * @brief Data about state of sent udp message 
 */
typedef struct t_udp_data {
    bool is_confirmed; // Is message confirmed by server
    int64_t retry_after; // Send msg again after [ms]
    uint8_t retry_count; // Count of total processed retries
} t_udp_data;

typedef struct t_confirmed_list {
    int length;
    int confirmed[CONFIRM_MAX_REC];
} t_confirmed_list;

#endif