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

#include "err_out.h"
#include "types.h"

extern int errno;

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

#endif