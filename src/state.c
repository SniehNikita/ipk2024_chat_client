/**
 * @file state.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 26.03.2024
 * @brief Resolves states operation of automata
 */

#include "state.h"

int get_next_state(int cur_state, t_msg msg) {
    switch (cur_state) {
        case e_state_null: return e_state_null; break;
        case e_state_start: return next_state_start(msg.type); break;
        case e_state_auth: return next_state_auth(msg.type, msg.content.reply.result); break;
        case e_state_open: return next_state_open(msg.type); break;
        case e_state_err: return next_state_err(); break;
        case e_state_end: return next_state_end(); break; 
    }
    return e_state_null;
}

int next_state_start(int msg_type) {
    switch(msg_type) {
        case e_auth: return e_state_auth; break; 
    }
    return e_state_null;
}

int next_state_auth(int msg_type, bool is_reply) {
    switch(msg_type) {
        case e_reply: if (is_reply) { return e_state_open; } else { return e_state_auth; } break; 
        case e_auth: return e_state_auth; break; 
        case e_err: return e_state_end; break; 
        case e_bye: return e_state_end; break; 
    }
    return e_state_null;
}

int next_state_open(int msg_type) {
    switch(msg_type) {
        case e_reply: return e_state_open; break; 
        case e_join: return e_state_open; break; 
        case e_msg: return e_state_open; break; 
        case e_err: return e_state_end; break; 
        case e_bye: return e_state_end; break; 
    }
    return e_state_null;
}

int next_state_err() {
    return e_state_end;
}

int next_state_end() {
    return e_state_end;
}