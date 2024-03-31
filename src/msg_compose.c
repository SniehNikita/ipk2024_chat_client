/**
 * @file msg_compose.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Message composer
 */

#include "msg_compose.h"

int compose_udp(t_msg msg, t_string * send, int * len) { 
    switch (msg.type) {
        case e_confirm: return compose_udp_confirm(msg, send, len); break;
        case e_reply: return compose_udp_reply(msg, send, len); break;
        case e_auth: return compose_udp_auth(msg, send, len); break;
        case e_join: return compose_udp_join(msg, send, len); break;
        case e_msg: return compose_udp_msg(msg, send, len); break;
        case e_err: return compose_udp_err(msg, send, len); break;
        case e_bye: return compose_udp_bye(msg, send, len); break;
        default: return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    
    return 0;
}

int compose_udp_confirm(t_msg msg, t_string * send, int * len) {
    *len = 3;
    (*send)[0] = e_confirm;
    (*send)[1] = (msg.content.confirm.ref_id / 0xFF) % 0xFF;
    (*send)[2] = msg.content.confirm.ref_id % 0xFF;

    return 0;
}

int compose_udp_reply(t_msg msg, t_string * send, int * len) {
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

int compose_udp_auth(t_msg msg, t_string * send, int * len) {
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

int compose_udp_join(t_msg msg, t_string * send, int * len) {
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

int compose_udp_msg(t_msg msg, t_string * send, int * len) {
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
    compose_string(&(*send),msg.content.msg.msg, index, &added_len);
    index += added_len;
    (*send)[index++] = 0x00;
    *len = index;

    return 0;
}

int compose_udp_err(t_msg msg, t_string * send, int * len) {
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

int compose_udp_bye(t_msg msg, t_string * send, int * len) {
    *len = 3;
    (*send)[0] = e_bye;
    (*send)[1] = (msg.id / 0xFF) % 0xFF;
    (*send)[2] = msg.id % 0xFF;

    return 0;
}

int compose_tcp(t_msg msg, t_string * send, int * len) { 
    switch (msg.type) {
        case e_reply: return compose_tcp_reply(msg, send, len); break;
        case e_auth: return compose_tcp_auth(msg, send, len); break;
        case e_join: return compose_tcp_join(msg, send, len); break;
        case e_msg: return compose_tcp_msg(msg, send, len); break;
        case e_err: return compose_tcp_err(msg, send, len); break;
        case e_bye: return compose_tcp_bye(msg, send, len); break;
        default: return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    
    return 0;
}

int compose_tcp_reply(t_msg msg, t_string * send, int * len) {
    int i = 0;
    int added_len = 0;
    memcpy(*send+i, "REPLY ", strlen("REPLY "));
    i += (int)strlen("REPLY ");
    if (msg.content.reply.result) {
        memcpy(*send+i, "OK ", strlen("OK "));
        i += (int)strlen("OK ");
    } else {
        memcpy(*send+i, "NOK ", strlen("NOK "));
        i += (int)strlen("NOK ");
    }
    memcpy(*send+i, "IS ", strlen("IS "));
    i += (int)strlen("IS ");
    compose_string(send,msg.content.reply.content, i, &added_len);
    i += added_len;
    memcpy(*send+i, "\r\n", strlen("\r\n"));
    i += (int)strlen("\r\n");
    *len = i;

    return 0;
}

int compose_tcp_auth(t_msg msg, t_string * send, int * len) {
    int i = 0;
    memcpy(*send+i, "AUTH ", strlen("AUTH "));
    i += (int)strlen("AUTH ");
    memcpy(*send+i, msg.content.auth.user_name, strlen(msg.content.auth.user_name));
    i += (int)strlen(msg.content.auth.user_name);
    memcpy(*send+i, " AS ", strlen(" AS "));
    i += (int)strlen(" AS ");
    memcpy(*send+i, msg.content.auth.display_name, strlen(msg.content.auth.display_name));
    i += (int)strlen(msg.content.auth.display_name);
    memcpy(*send+i, " USING ", strlen(" USING "));
    i += (int)strlen(" USING ");
    memcpy(*send+i, msg.content.auth.secret, strlen(msg.content.auth.secret));
    i += (int)strlen(msg.content.auth.secret);
    memcpy(*send+i, "\r\n", strlen("\r\n"));
    i += (int)strlen("\r\n");
    *len = i;

    return 0;
}

int compose_tcp_join(t_msg msg, t_string * send, int * len) {
    int i = 0;
    memcpy(*send+i, "JOIN ", strlen("JOIN "));
    i += (int)strlen("JOIN ");
    memcpy(*send+i, msg.content.join.channel_id, strlen(msg.content.join.channel_id));
    i += (int)strlen(msg.content.join.channel_id);
    memcpy(*send+i, " AS ", strlen(" AS "));
    i += (int)strlen(" AS ");
    memcpy(*send+i, msg.content.join.display_name, strlen(msg.content.join.display_name));
    i += (int)strlen(msg.content.join.display_name);
    memcpy(*send+i, "\r\n", strlen("\r\n"));
    i += (int)strlen("\r\n");
    *len = i;

    return 0;
}

int compose_tcp_msg(t_msg msg, t_string * send, int * len) {
    int i = 0;
    int added_len = 0;
    memcpy(*send+i, "MSG FROM ", strlen("MSG FROM "));
    i += (int)strlen("MSG FROM ");
    memcpy(*send+i, msg.content.msg.display_name, strlen(msg.content.msg.display_name));
    i += (int)strlen(msg.content.msg.display_name);
    memcpy(*send+i, " IS ", strlen(" IS "));
    i += (int)strlen(" IS ");
    compose_string(send,msg.content.msg.msg, i, &added_len);
    i += added_len;
    memcpy(*send+i, "\r\n", strlen("\r\n"));
    i += (int)strlen("\r\n");
    *len = i;

    return 0;
}

int compose_tcp_err(t_msg msg, t_string * send, int * len) {
    int i = 0;
    int added_len = 0;
    memcpy(*send+i, "ERR FROM ", strlen("ERR FROM "));
    i += (int)strlen("ERR FROM ");
    memcpy(*send+i, msg.content.err.display_name, strlen(msg.content.err.display_name));
    i += (int)strlen(msg.content.err.display_name);
    memcpy(*send+i, " IS ", strlen(" IS "));
    i += (int)strlen(" IS ");
    compose_string(send,msg.content.err.msg, i, &added_len);
    i += added_len;
    memcpy(*send+i, "\r\n", strlen("\r\n"));
    i += (int)strlen("\r\n");
    *len = i;

    return 0;
}

int compose_tcp_bye(t_msg msg, t_string * send, int * len) {
    int i = 0;
    msg.type = msg.type; // -warn
    memcpy(*send+i, "BYE", strlen("BYE"));
    i += (int)strlen("BYE");
    memcpy(*send+i, "\r\n", strlen("\r\n"));
    i += (int)strlen("\r\n");
    *len = i;

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