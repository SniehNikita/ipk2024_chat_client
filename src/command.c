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
    return parse_cmd(command, cmd);
}

int parse_cmd(t_command * command, t_string cmd) {
    t_command_argv cmd_argv;

    memset(cmd_argv, 0x00, sizeof(cmd_argv));
    if (cmd[0] != '/') {
        command->type = e_cmd_msg;
        memcpy(command->content.msg.msg, cmd, STR_MAX_LEN);
        return 0;
    }
    split_argv(&cmd_argv, cmd);
    if (!strcmp(cmd_argv[0], "/auth")) {
        cmd_parse_auth(command, cmd_argv);
        if (!strcmp(cmd_argv[1], "") || !strcmp(cmd_argv[2], "") || !strcmp(cmd_argv[3], "")) {
            return printWarnMsg(warn_not_enough_argv, __LINE__, __FILE__, NULL);
        }
    } else if (!strcmp(cmd_argv[0], "/join")) {
        cmd_parse_join(command, cmd_argv);
        if (!strcmp(cmd_argv[1], "")) {
            return printWarnMsg(warn_not_enough_argv, __LINE__, __FILE__, NULL);
        }
    } else if (!strcmp(cmd_argv[0], "/rename")) {
        cmd_parse_rename(command, cmd_argv);
        if (!strcmp(cmd_argv[1], "")) {
            return printWarnMsg(warn_not_enough_argv, __LINE__, __FILE__, NULL);
        }
    } else if (!strcmp(cmd_argv[0], "/help")) {
        command->type = e_cmd_help;
    } else {
        return printWarnMsg(warn_command_not_found, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int cmd_parse_auth(t_command * command, t_command_argv cmd_argv) {
    command->type = e_cmd_auth;
    memcpy(command->content.auth.user_name, cmd_argv[1], STR_MAX_LEN); 
    memcpy(command->content.auth.secret, cmd_argv[2], STR_MAX_LEN);
    memcpy(command->content.auth.display_name, cmd_argv[3], STR_MAX_LEN);

    return 0;
}

int cmd_parse_join(t_command * command, t_command_argv cmd_argv) {
    command->type = e_cmd_join;
    memcpy(command->content.join.channel_id, cmd_argv[1], STR_MAX_LEN);

    return 0;
}

int cmd_parse_rename(t_command * command, t_command_argv cmd_argv) {
    command->type = e_cmd_rename;
    memcpy(command->content.rename.display_name, cmd_argv[1], STR_MAX_LEN); 

    return 0;
}

int split_argv(t_command_argv * cmd_argv, t_string cmd) {
    uint8_t index = 0;
    int offset = 0;
    int argc = 0;
    while (cmd[index] != 0x00 && argc < CMD_MAX_ARGV) {
        if (cmd[index] == ' ' || cmd[index] == '\n' || cmd[index] == '\t') {
            offset = index + 1;
            argc++;
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

int exec_msg(t_command cmd, t_msg * msg) {
    memcpy(msg->content.msg.msg, cmd.content.msg.msg, STR_MAX_LEN);
    memcpy(msg->content.msg.display_name, user.display_name, STR_MAX_LEN);

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