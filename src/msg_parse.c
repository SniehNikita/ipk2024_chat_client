/**
 * @file msg_parse.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Message parser
 */

#include "msg_parse.h"

int parse(t_string * recv, int len, t_msg * msg) {
    if (len <= 0) {
        return errno = printErrMsg(err_msg_null, __LINE__, __FILE__, NULL);
    }
    switch ((unsigned char)(*recv)[0]) {
        case e_confirm: return parse_confirm(recv, msg); break;
        case e_reply: return parse_reply(recv, msg); break;
        case e_auth: return parse_auth(recv, msg); break;
        case e_join: return parse_join(recv, msg); break;
        case e_msg: return parse_msg(recv, msg); break;
        case e_err: return parse_err(recv, msg); break;
        case e_bye: return parse_bye(recv, msg); break;
        default: return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_confirm(t_string * recv, t_msg * msg) {
    msg->type = e_confirm;
    memcpy(&(msg->content.confirm.ref_id), recv + sizeof(char) * 1, 2);
    return 0;
}

int parse_reply(t_string * recv, t_msg * msg) {
    int index;
    msg->type = e_reply;
    memcpy(&(msg->id), recv + sizeof(char) * 1, 2);
    memcpy(&(msg->content.reply.result), recv + sizeof(char) * 3, 1);
    memcpy(&(msg->content.reply.ref_id), recv + sizeof(char) * 4, 2);
    index = 6;
    while (recv[index] != 0x00 && index-6 < STR_MAX_LEN) {
        msg->content.reply.content[index-6] = (*recv)[index];
        index++;
    }
    if (index-6 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_auth(t_string * recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_auth;
    memcpy(&(msg->id), recv + sizeof(char) * 1, 2);
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.auth.user_name[index-3] = (*recv)[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.auth.display_name[index-offset] = (*recv)[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.auth.secret[index-offset] = (*recv)[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_join(t_string * recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_join;
    memcpy(&(msg->id), recv + sizeof(char) * 1, 2);
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.join.channel_id[index-3] = (*recv)[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.join.display_name[index-offset] = (*recv)[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_msg(t_string * recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_msg;
    memcpy(&(msg->id), recv + sizeof(char) * 1, 2);
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.msg.display_name[index-3] = (*recv)[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.msg.msg[index-offset] = (*recv)[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_err(t_string * recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_err;
    memcpy(&(msg->id), recv + sizeof(char) * 1, 2);
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.err.display_name[index-3] = (*recv)[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.err.msg[index-offset] = (*recv)[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_bye(t_string * recv, t_msg * msg) {
    msg->type = e_bye;
    memcpy(&(msg->id), recv + sizeof(char) * 1, 2);
    return 0;
}