/**
 * @file command.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 25.03.2024
 * @brief Implementation of client input
 */

#include "command.h"

int get_command(t_command * command) {
    t_string cmd;

    memset(cmd, 0x00, STR_MAX_LEN);
    fgets(cmd, STR_MAX_LEN, stdin);
    cmd[strlen(cmd)-1] = '\0'; // Remove \n
    return parse_cmd(command, cmd);
}

int parse_cmd(t_command * command, t_string cmd) {
    t_command_argv cmd_argv;

    memset(cmd_argv, 0x00, sizeof(cmd_argv));
    if (cmd[0] != '/') {
        command->type = e_cmd_msg;
        if (validate_message(cmd)) {
            return errno;
        }
        memcpy(command->content.msg.msg, cmd, STR_MAX_LEN);
        return 0;
    }
    split_argv(&cmd_argv, cmd);
    if (!strcmp(cmd_argv[0], "/auth")) {
        if (!strcmp(cmd_argv[1], "") || !strcmp(cmd_argv[2], "") || !strcmp(cmd_argv[3], "")) {
            return printWarnMsg(warn_not_enough_argv, __LINE__, __FILE__, NULL);
        }
        if (cmd_parse_auth(command, cmd_argv)) {
            return errno;
        }
    } else if (!strcmp(cmd_argv[0], "/join")) {
        if (!strcmp(cmd_argv[1], "")) {
            return printWarnMsg(warn_not_enough_argv, __LINE__, __FILE__, NULL);
        }
        if (cmd_parse_join(command, cmd_argv)) {
            return errno;
        }
    } else if (!strcmp(cmd_argv[0], "/rename")) {
        if (!strcmp(cmd_argv[1], "")) {
            return printWarnMsg(warn_not_enough_argv, __LINE__, __FILE__, NULL);
        }
        if (cmd_parse_rename(command, cmd_argv)) {
            return errno;
        }
    } else if (!strcmp(cmd_argv[0], "/help")) {
        command->type = e_cmd_help;
    } else if (!strcmp(cmd_argv[0], "/close")) {
        command->type = e_cmd_close;
    } else {
        return printWarnMsg(warn_command_not_found, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int cmd_parse_auth(t_command * command, t_command_argv cmd_argv) {
    command->type = e_cmd_auth;
    if (validate_username(cmd_argv[1]) || validate_secret(cmd_argv[2]) || validate_displayname(cmd_argv[3])) {
        return errno;
    }
    memcpy(command->content.auth.user_name, cmd_argv[1], STR_MAX_LEN);
    memcpy(command->content.auth.secret, cmd_argv[2], STR_MAX_LEN);
    memcpy(command->content.auth.display_name, cmd_argv[3], STR_MAX_LEN);

    return 0;
}

int cmd_parse_join(t_command * command, t_command_argv cmd_argv) {
    command->type = e_cmd_join;
    if (validate_channelid(cmd_argv[1])) {
        return errno;
    }
    memcpy(command->content.join.channel_id, cmd_argv[1], STR_MAX_LEN);

    return 0;
}

int cmd_parse_rename(t_command * command, t_command_argv cmd_argv) {
    command->type = e_cmd_rename;
    if (validate_displayname(cmd_argv[1])) {
        return errno;
    }
    memcpy(command->content.rename.display_name, cmd_argv[1], STR_MAX_LEN); 

    return 0;
}

int split_argv(t_command_argv * cmd_argv, t_string cmd) {
    uint8_t index = 0;
    int offset = 0;
    int argc = 0;
    while (cmd[index] != 0x00 && argc < CMD_MAX_ARGV) {
        if (cmd[index] == ' ' || cmd[index] == '\n' || cmd[index] == '\t') {
            if (index != 0 && cmd[index-1] != ' ' && cmd[index-1] != '\n' && cmd[index-1] != '\t') {
                offset = index + 1;
                argc++;
            }
        } else {
            (*cmd_argv)[argc][index-offset] = cmd[index];
        }
        index++;
    }
    
    if (strlen(cmd) > index) {
        printWarnMsg(warn_too_many_argv, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int validate_username(t_string username) {
    if (strlen(username) > USERNAME_MAX_LEN) {
        printWarnMsg(warn_username_len_exceeded, __LINE__, __FILE__, NULL);
        return warn_username_len_exceeded;
    }
    for (size_t i = 0; i < strlen(username); i++) {
        if (username[i] != '-' && (username[i] < 'A' || username[i] > 'Z') && (username[i] < 'a' || username[i] > 'z') && (username[i] < '0' || username[i] > '9')) {
            printWarnMsg(warn_username_char_forbidden, __LINE__, __FILE__, NULL);
            return warn_username_char_forbidden;
        }
    }
    return 0;
}

int validate_channelid(t_string channelid) {
    if (strlen(channelid) > CHANNELID_MAX_LEN) {
        printWarnMsg(warn_channelid_len_exceeded, __LINE__, __FILE__, NULL);
        return warn_channelid_len_exceeded;
    }
    for (size_t i = 0; i < strlen(channelid); i++) {
        if (channelid[i] != '.' && channelid[i] != '-' && (channelid[i] < 'A' || channelid[i] > 'Z') && (channelid[i] < 'a' || channelid[i] > 'z') && (channelid[i] < '0' || channelid[i] > '9')) {
            printWarnMsg(warn_channelid_char_forbidden, __LINE__, __FILE__, NULL);
            return warn_channelid_char_forbidden;
        }
    }
    return 0;
}

int validate_secret(t_string secret) {
    if (strlen(secret) > SECRET_MAX_LEN) {
        printWarnMsg(warn_secret_len_exceeded, __LINE__, __FILE__, NULL);
        return warn_secret_len_exceeded;
    }
    for (size_t i = 0; i < strlen(secret); i++) {
        if (secret[i] != '-' && (secret[i] < 'A' || secret[i] > 'Z') && (secret[i] < 'a' || secret[i] > 'z') && (secret[i] < '0' || secret[i] > '9')) {
            printWarnMsg(warn_secret_char_forbidden, __LINE__, __FILE__, NULL);
            return warn_secret_char_forbidden;
        }
    }
    return 0;
}

int validate_displayname(t_string displayname) {
    if (strlen(displayname) > DISPLAYNAME_MAX_LEN) {
        printWarnMsg(warn_displayname_len_exceeded, __LINE__, __FILE__, NULL);
        return warn_displayname_len_exceeded;
    }
    for (size_t i = 0; i < strlen(displayname); i++) {
        if (displayname[i] < 0x21) {
            printWarnMsg(warn_displayname_char_forbidden, __LINE__, __FILE__, NULL);
            return warn_displayname_char_forbidden;
        }
    }
    return 0;
}

int validate_message(t_string message) {
    if (strlen(message) > MESSAGE_MAX_LEN) {
        printWarnMsg(warn_message_len_exceeded, __LINE__, __FILE__, NULL);
        return warn_message_len_exceeded;
    }
    for (size_t i = 0; i < strlen(message); i++) {
        if (message[i] < 0x20) {
            printWarnMsg(warn_message_char_forbidden, __LINE__, __FILE__, NULL);
            return warn_message_char_forbidden;
        }
    }
    return 0;
}

int exec_msg(t_command cmd, t_msg * msg) {
    msg->type = e_msg;
    msg->id = client_msg_count++;
    memcpy(msg->content.msg.display_name, user.display_name, STR_MAX_LEN);
    memcpy(msg->content.msg.msg, cmd.content.msg.msg, STR_MAX_LEN);

    return 0;
}

int exec_help() {
    printf("Type any message and press enter to send message to selected channel.\nUse only a-zA-Z0-9_ symbols.\nCommand starts with \'/\'.\nList of available commands:\n - /help : Shows this message\n - /auth {Username} {Secret} {DisplayName} : authorize on server\n - /join {ChannelID} : join channel\n - /rename {DisplayName} : change your display name\n");

    return 0;
}

int exec_auth(t_command cmd, t_msg * msg) {
    msg->type = e_auth;
    msg->id = client_msg_count++;
    memcpy(msg->content.auth.user_name, cmd.content.auth.user_name, STR_MAX_LEN);
    memcpy(msg->content.auth.secret, cmd.content.auth.secret, STR_MAX_LEN);
    memcpy(msg->content.auth.display_name, cmd.content.auth.display_name, STR_MAX_LEN);

    memcpy(user.user_name, cmd.content.auth.user_name, STR_MAX_LEN);
    memcpy(user.display_name, cmd.content.auth.display_name, STR_MAX_LEN);

    return 0;
}

int exec_join(t_command cmd, t_msg * msg) {
    msg->type = e_join;
    msg->id = client_msg_count++;
    memcpy(msg->content.join.channel_id, cmd.content.join.channel_id, STR_MAX_LEN);
    memcpy(msg->content.auth.display_name, user.display_name, STR_MAX_LEN);

    memcpy(user.channel_id, cmd.content.join.channel_id, STR_MAX_LEN);

    return 0;
}

int exec_rename(t_command cmd) {
    memcpy(user.display_name, cmd.content.rename.display_name, STR_MAX_LEN);

    return 0;
}

int exec_close(t_msg * msg) {
    msg->type = e_bye;
    msg->id = client_msg_count++;

    return 0;
}

bool is_command_local(t_command cmd) {
    switch(cmd.type) {
        case e_cmd_help: return true;
        case e_cmd_rename: return true;
        default: return false;
    }
}