/**
 * @file command.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 25.03.2024
 * @brief Header file for user command input handling
 *
 * Gets user's command, validates and processes it. Result is t_command which will be further processes in client.h
 */

#ifndef __COMMAND_H__

#define __COMMAND_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "err_out.h"
#include "types.h"

extern int errno;
extern int client_msg_count;
extern t_user user;

/**
 * @brief Main function. Gets command from user and parses it
 * 
 * @param command Command struct to be filled
 * @return int Result code
 */
int get_command(t_command * command);

/**
 * @brief Parses command received from user
 * 
 * @param command Command struct to beb filled
 * @param cmd User entered command
 * @return int Result code
 */
int parse_cmd(t_command * command, t_string cmd);

/**
 * @brief Parses auth command
 * 
 * @param command Command struct to beb filled
 * @param cmd User entered command
 * @return int Result code
 */
int cmd_parse_auth(t_command * command, t_command_argv cmd_argv);

/**
 * @brief Parses join command
 * 
 * @param command Command struct to beb filled
 * @param cmd User entered command
 * @return int Result code
 */
int cmd_parse_join(t_command * command, t_command_argv cmd_argv);

/**
 * @brief Parses rename command
 * 
 * @param command Command struct to beb filled
 * @param cmd User entered command
 * @return int Result code
 */
int cmd_parse_rename(t_command * command, t_command_argv cmd_argv);

/**
 * @brief Splits user entered command on whitespaces into string array
 * 
 * @param cmd_argv Result array of strings
 * @param cmd User entered command
 * @return int Result code
 */
int split_argv(t_command_argv * cmd_argv, t_string cmd);

/**
 * @brief Validates user name
 * 
 * @param username User name
 * @return int Result code
 */
int validate_username(t_string username);

/**
 * @brief Validates channel id
 * 
 * @param channelid Channel id
 * @return int Result code
 */
int validate_channelid(t_string channelid);

/**
 * @brief Validates secret
 * 
 * @param secret Secret
 * @return int Result code
 */
int validate_secret(t_string secret);

/**
 * @brief Validates display name
 * 
 * @param displayname Display name
 * @return int Result code
 */
int validate_displayname(t_string displayname);

/**
 * @brief Validates message
 * 
 * @param message Message
 * @return int Result code
 */
int validate_message(t_string message);

/**
 * @brief Executes msg command
 * 
 * @param cmd Auth command
 * @param msg Result message
 * @return int Result code
 */
int exec_msg(t_command cmd, t_msg * msg);

/**
 * @brief Executes help command
 * 
 * @return int Result code
 */
int exec_help();

/**
 * @brief Executes auth command
 * 
 * @param cmd Auth command
 * @param msg Result message
 * @return int Result code
 */
int exec_auth(t_command cmd, t_msg * msg);

/**
 * @brief Executes join command
 * 
 * @param cmd Join command
 * @param msg Result message
 * @return int Result code
 */
int exec_join(t_command cmd, t_msg * msg);

/**
 * @brief Executes rename command
 * 
 * @param cmd Rename command
 * @return int Result code
 */
int exec_rename(t_command cmd);

/**
 * @brief Executes close command (Custom: sends BYE message to server)
 * 
 * @param cmd Close command
 * @return int Result code
 */
int exec_close(t_msg * msg);

/**
 * @brief Checks if command is processed locally and response should not be sent to server
 * 
 * @param cmd Command
 * @return true Result of command execution should be sent to server
 * @return false Result of command execution must not be sent to server
 */
bool is_command_local(t_command cmd);

#endif