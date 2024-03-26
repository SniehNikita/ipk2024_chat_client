/**
 * @file command.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 26.03.2024
 * @brief Header file for state handling
 *
 * Resolves states operation of automata
 */

#ifndef __STATE_H__

#define __STATE_H__

#include <stdbool.h>

#include "types.h"

/**
 * @brief Gets the next state
 * 
 * @param cur_state Current state
 * @param msg Received/pending message
 * @return int New state
 */
int get_next_state(int cur_state, t_msg msg);

/**
 * @brief Gets next state after start
 * 
 * @param msg_type Type of received/pending message
 * @return int New state
 */
int next_state_start(int msg_type);

/**
 * @brief Gets next state after auth
 * 
 * @param msg_type Type of received/pending message
 * @param is_reply Is reply positive (ignored if no need)
 * @return int New state
 */
int next_state_auth(int msg_type, bool is_reply);

/**
 * @brief Gets next state after open
 * 
 * @param msg_type Type of received/pending message
 * @return int New state
 */
int next_state_open(int msg_type);

/**
 * @brief Gets next state after err
 * 
 * @param msg_type Type of received/pending message
 * @return int New state
 */
int next_state_err();

/**
 * @brief Gets next state after end
 * 
 * @param msg_type Type of received/pending message
 * @return int New state
 */
int next_state_end();

#endif