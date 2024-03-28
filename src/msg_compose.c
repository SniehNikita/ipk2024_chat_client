/**
 * @file msg_compose.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Message composer
 */

#include "msg_compose.h"

int compose(t_msg msg, t_string * send, int * len) { 
    switch (msg.type) {
        case e_confirm: return compose_confirm(msg, send, len); break;
        case e_reply: return compose_reply(msg, send, len); break;
        case e_auth: return compose_auth(msg, send, len); break;
        case e_join: return compose_join(msg, send, len); break;
        case e_msg: return compose_msg(msg, send, len); break;
        case e_err: return compose_err(msg, send, len); break;
        case e_bye: return compose_bye(msg, send, len); break;
        default: return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    
    return 0;
}

int compose_confirm(t_msg msg, t_string * send, int * len) {
    *len = 3;
    (*send)[0] = e_confirm;
    (*send)[1] = (msg.content.confirm.ref_id / 0xFF) % 0xFF;
    (*send)[2] = msg.content.confirm.ref_id % 0xFF;

    return 0;
}

int compose_reply(t_msg msg, t_string * send, int * len) {
    int index;
    int added_len = 0;
    (*send)[0] = e_reply;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;
    (*send)[3] = msg.content.reply.result;
    (*send)[4] = (msg.content.reply.ref_id / 0xFF) % 0xFF;
    (*send)[5] = msg.content.reply.ref_id % 0xFF;
    index = 6;
    compose_string(&(*send),msg.content.reply.content, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    *len = index;

    return 0;
}

int compose_auth(t_msg msg, t_string * send, int * len) {
    int index;
    int added_len = 0;
    (*send)[0] = e_auth;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;
    index = 3;
    compose_string(send,msg.content.auth.user_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    added_len = 0;
    compose_string(send, msg.content.auth.display_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    added_len = 0;
    compose_string(send,msg.content.auth.secret, index, &added_len);
    index += added_len;
    *len = index;
    (*len)++;

    return 0;
}

int compose_join(t_msg msg, t_string * send, int * len) {
    int index;
    int added_len = 0;
    (*send)[0] = e_join;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;
    index = 3;
    compose_string(&(*send),msg.content.join.channel_id, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    added_len = 0;
    compose_string(&(*send),msg.content.join.display_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    *len = index;

    return 0;
}

int compose_msg(t_msg msg, t_string * send, int * len) {
    int index;
    int added_len = 0;
    (*send)[0] = e_msg;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;
    index = 3;
    compose_string(&(*send),msg.content.msg.display_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    added_len = 0;
    compose_string(&(*send),msg.content.msg.display_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    *len = index;

    return 0;
}

int compose_err(t_msg msg, t_string * send, int * len) {
    int index;
    int added_len = 0;
    (*send)[0] = e_err;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;
    index = 3;
    compose_string(&(*send),msg.content.err.display_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    added_len = 0;
    compose_string(&(*send),msg.content.err.display_name, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    *len = index;

    return 0;
}

int compose_bye(t_msg msg, t_string * send, int * len) {
    *len = 3;
    (*send)[0] = e_bye;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;

    return 0;
}

int compose_string(t_string * dst, t_string src, int index, int * len) {
    int i = 0;
    while (src[i] != 0x00) {
        (*dst)[index++] = src[i++];
    }
    *len = i;

    return 0;
}