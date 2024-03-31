/**
 * @file msg_parse.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Message parser
 */

#include "msg_parse.h"

int parse_udp(t_string recv, int len, t_msg * msg) {
    if (len <= 0) {
        return errno = printErrMsg(err_msg_null, __LINE__, __FILE__, NULL);
    }
    switch ((unsigned char)recv[0]) {
        case e_confirm: return parse_udp_confirm(recv, msg); break;
        case e_reply: return parse_udp_reply(recv, msg); break;
        case e_auth: return parse_udp_auth(recv, msg); break;
        case e_join: return parse_udp_join(recv, msg); break;
        case e_msg: return parse_udp_msg(recv, msg); break;
        case e_err: return parse_udp_err(recv, msg); break;
        case e_bye: return parse_udp_bye(recv, msg); break;
        default: return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_udp_confirm(t_string recv, t_msg * msg) {
    msg->type = e_confirm;
    msg->content.confirm.ref_id = recv[1] * 0xFF + recv[2];
    return 0;
}

int parse_udp_reply(t_string recv, t_msg * msg) {
    int index;
    msg->type = e_reply;
    msg->id = recv[1] * 0xFF + recv[2];
    msg->content.reply.result = recv[3];
    msg->content.reply.ref_id = recv[4] * 0xFF + recv[5];
    index = 6;
    while (recv[index] != 0x00 && index-6 < STR_MAX_LEN) {
        msg->content.reply.content[index-6] = recv[index];
        index++;
    }
    if (index-6 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_udp_auth(t_string recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_auth;
    msg->id = recv[1] * 0xFF + recv[2];
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.auth.user_name[index-3] = recv[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.auth.display_name[index-offset] = recv[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.auth.secret[index-offset] = recv[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_udp_join(t_string recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_join;
    msg->id = recv[1] * 0xFF + recv[2];
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.join.channel_id[index-3] = recv[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.join.display_name[index-offset] = recv[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_udp_msg(t_string recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_msg;
    msg->id = recv[1] * 0xFF + recv[2];
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.msg.display_name[index-3] = recv[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.msg.msg[index-offset] = recv[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_udp_err(t_string recv, t_msg * msg) {
    int index;
    int offset;
    msg->type = e_err;
    msg->id = recv[1] * 0xFF + recv[2];
    index = 3;
    while (recv[index] != 0x00 && index-3 < STR_MAX_LEN) {
        msg->content.err.display_name[index-3] = recv[index];
        index++;
    }
    if (index-3 == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }
    index++;
    offset = index;
    while (recv[index] != 0x00 && index-offset < STR_MAX_LEN) {
        msg->content.err.msg[index-offset] = recv[index];
        index++;
    }
    if (index-offset == STR_MAX_LEN) {
        return errno = printErrMsg(err_string_overflow, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_udp_bye(t_string recv, t_msg * msg) {
    msg->type = e_bye;
    msg->id = recv[1] * 0xFF + recv[2];
    return 0;
}

int parse_tcp(t_string recv, int len, t_msg * msg) {
    if (len <= 0) {
        return errno = printErrMsg(err_msg_null, __LINE__, __FILE__, NULL);
    }
    if (cmp_part(recv, "ERR", 0)) {
        return parse_tcp_err(recv, msg);
    } else if (cmp_part(recv, "REPLY", 0)) {
        return parse_tcp_reply(recv, msg);
    } else if (cmp_part(recv, "AUTH", 0)) {
        return parse_tcp_auth(recv, msg);
    } else if (cmp_part(recv, "JOIN", 0)) {
        return parse_tcp_join(recv, msg);
    } else if (cmp_part(recv, "MSG", 0)) {
        return parse_tcp_msg(recv, msg);
    } else if (cmp_part(recv, "BYE", 0)) {
        return parse_tcp_bye(recv, msg);
    } else {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    return 0;
}

int parse_tcp_reply(t_string recv, t_msg * msg) {
    int i = 0;
    if (!cmp_part(recv, "REPLY ", 0)) {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    msg->type = e_reply;

    i += strlen("REPLY ");
    if (!cmp_part(recv, "OK ", i) && !cmp_part(recv, "NOK ", i) ) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    if (cmp_part(recv, "OK ", i)) {
        msg->content.reply.result = true;
        i += strlen("OK ");
    } else {
        msg->content.reply.result = false;
        i += strlen("NOK ");
    }
    if (!cmp_part(recv, "IS ", i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen("IS ");
    if (!get_content(&(msg->content.reply.content), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.reply.content);
    if (recv[i] != '\r' || recv[i+1] != '\n') {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    
    return 0;
}

int parse_tcp_auth(t_string recv, t_msg * msg) {
    int i = 0;
    if (!cmp_part(recv, "AUTH ", 0)) {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    msg->type = e_auth;

    i += strlen("AUTH ");
    if (!get_id(&(msg->content.auth.user_name), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.auth.user_name) + 1;
    if (!cmp_part(recv, "AS ", i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen("AS ");
    if (!get_dname(&(msg->content.auth.display_name), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.auth.display_name) + 1;
    if (!cmp_part(recv, "USING ", i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen("USING ");
    if (!get_secret(&(msg->content.auth.secret), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.auth.secret);
    if (recv[i] != '\r' || recv[i+1] != '\n') {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_tcp_join(t_string recv, t_msg * msg) {
    int i = 0;
    if (!cmp_part(recv, "JOIN ", 0)) {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    msg->type = e_join;

    i += strlen("JOIN ");
    if (!get_id(&(msg->content.join.channel_id), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.join.channel_id) + 1;
    if (!cmp_part(recv, "AS ", i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen("AS ");
    if (!get_dname(&(msg->content.join.display_name), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.join.display_name);
    if (recv[i] != '\r' || recv[i+1] != '\n') {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_tcp_msg(t_string recv, t_msg * msg) {
    int i = 0;
    if (!cmp_part(recv, "MSG FROM ", 0)) {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    msg->type = e_msg;

    i += strlen("MSG FROM ");
    if (!get_dname(&(msg->content.msg.display_name), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.msg.display_name) + 1;
    if (!cmp_part(recv, "IS ", i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen("IS ");
    if (!get_content(&(msg->content.msg.msg), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.msg.msg);
    if (recv[i] != '\r' || recv[i+1] != '\n') {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_tcp_err(t_string recv, t_msg * msg) {
    int i = 0;
    if (!cmp_part(recv, "ERR FROM ", 0)) {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    msg->type = e_err;

    i += strlen("ERR FROM ");
    if (!get_dname(&(msg->content.err.display_name), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.err.display_name) + 1;
    if (!cmp_part(recv, "IS ", i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen("IS ");
    if (!get_content(&(msg->content.err.msg), recv, i)) {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }
    i += strlen(msg->content.err.msg);
    if (recv[i] != '\r' || recv[i+1] != '\n') {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int parse_tcp_bye(t_string recv, t_msg * msg) {
    int i = 0;
    if (!cmp_part(recv, "BYE ", 0)) {
        return errno = printErrMsg(err_msg_type_unknown, __LINE__, __FILE__, NULL);
    }
    msg->type = e_msg;
    i += strlen("BYE ");
    if (recv[i] != '\r' || recv[i+1] != '\n') {
        return errno = printErrMsg(err_msg_parse_failed, __LINE__, __FILE__, NULL);
    }

    return 0;
}

bool cmp_part(t_string str, char * ref, int start) {
    t_string word;
    t_string ref_cpy;
    if ((int)strlen(ref) > (int)strlen(str) - start) {
        return false;
    }
    memset(word, '\0', sizeof(t_string));
    memset(ref_cpy, '\0', sizeof(t_string));
    memcpy(&word, str+start, strlen(ref));
    memcpy(ref_cpy, ref, strlen(ref));
    uppercase(&word);
    uppercase(&ref_cpy);
    if (strcmp(ref_cpy, word)) {
        return false;
    }
    return true;
}

void uppercase(t_string * str) {
    char * c = *str;
    while (*c != '\0') {
        *c = toupper(*c);
        c++;
    }
}

void get_word(t_string * word, t_string str, int start) {
    int i = start;
    while (str[i] != '\0' && !is_whitespace(str[i])) {
        (*word)[i-start] = str[i];
    }
}

bool get_dname(t_string * dname, t_string str, int start) {
    int i = start;
    (*dname)[0] = '\0';
    while (true) {
        if (str[i] == '\0') {
            return false;
        }
        if (str[i] == ' ' || str[i] == '\r') {
            break;
        }
        if (str[i] <= 0x20) {
            return false;
        }
        (*dname)[i-start] = str[i];
        i++;
    }
    if (i == start) {
        return false;
    }
    if (i - start > DISPLAYNAME_MAX_LEN) {
        return false;
    }
    return true;
}

bool get_id(t_string * id, t_string str, int start) {
    int i = start;
    (*id)[0] = '\0';
    while (true) {
        if (str[i] == '\0') {
            return false;
        }
        if (str[i] == ' ' || str[i] == '\r') {
            break;
        }
        if (str[i] != '-' && (str[i] < 'A' || str[i] > 'Z') && (str[i] < 'a' || str[i] > 'z') && (str[i] < '0' || str[i] > '9')) {
            return false;
        }
        (*id)[i-start] = str[i];
        i++;
    }
    if (i == start) {
        return false;
    }
    if (i - start > USERNAME_MAX_LEN) {
        return false;
    }
    return true;
}

bool get_secret(t_string * secret, t_string str, int start) {
    int i = start;
    (*secret)[0] = '\0';
    while (true) {
        if (str[i] == '\0') {
            return false;
        }
        if (str[i] == ' ' || str[i] == '\r') {
            break;
        }
        if (str[i] != '-' && (str[i] < 'A' || str[i] > 'Z') && (str[i] < 'a' || str[i] > 'z') && (str[i] < '0' || str[i] > '9')) {
            return false;
        }
        (*secret)[i-start] = str[i];
        i++;
    }
    if (i == start) {
        return false;
    }
    if (i - start > SECRET_MAX_LEN) {
        return false;
    }
    return true;
}

bool get_content(t_string * content, t_string str, int start) {
    int i = start;
    (*content)[0] = '\0';
    if (strlen(str) - start < 2) {
        return false;
    }
    while (i < MESSAGE_MAX_LEN) {
        if (str[i] == '\r' && str[i+1] == '\n') {
            break;
        }
        if (str[i] < 0x20) {
            return false;
        }
        (*content)[i-start] = str[i];
        i++;
    }
    return true;
}

bool is_whitespace(char c) {
    if (c == '\n' || c == '\r' || c == '\t' || c == ' ') {
        return true;
    }
    return false;
}