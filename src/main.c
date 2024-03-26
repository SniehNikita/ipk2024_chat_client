/**
 * @file main.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @(date 16.03.2024
 * @brief Entry point of a program
 */

#include "main.h"

int main(int argc, char **argv) {
    struct pollfd poll_fds[2];
    t_argv argv_parsed;

    client_msg_queue = queue_create();
    if (errno != 0) { stop(errno); }
    server_msg_queue = queue_create();
    if (errno != 0) { stop(errno); }
    user.state = e_state_start;
    signal(SIGINT, sigintHandler);
    if (argvParse(argc, argv, &argv_parsed)) { stop(errno); }
    if (client(argv_parsed, &(poll_fds[0].fd))) { stop(errno); }
    poll_fds[0].events = POLLIN;
    poll_fds[1].fd = fileno(stdin);
    poll_fds[1].events = POLLIN;

    while (true) {
        int ret = poll(poll_fds, 2, 250); 
        if (ret > 0) {
            if (poll_fds[0].revents & POLLIN) {
                printf("Got smth from socket!\n");
            }
            if (poll_fds[1].revents & POLLIN) {
                process_command();
            }
        }
    }

    stop(0);
}

int process_command() {
    t_queue_item * new_item;
    t_command cmd;
    t_string buf;
    int buf_size;
    int new_state;
    
    if (get_command(&cmd)) { return errno; }
    new_item = queue_create_item();
    if (errno != 0) { stop(errno); }
    if (exec(cmd, &(new_item->msg))) { stop(errno); }
    if (cmd.type == e_cmd_help || cmd.type == e_cmd_rename) {
        queue_destroy_item(new_item);
        return 0;
    }
    new_state = get_next_state(user.state, new_item->msg);
    if (new_state == e_state_null) {
        queue_destroy_item(new_item);
        printWarnMsg(warn_invalid_state_transition, __LINE__, __FILE__, NULL);
        return 0;
    }
    if (new_item->msg.type != e_null) {
        queue_add(client_msg_queue, new_item);
        compose(new_item->msg, &buf, &buf_size);
        // client_send(buf, buf_size);
    } else {
        queue_destroy_item(new_item);
    }
    user.state = new_state;

    return 0;
}

int exec(t_command cmd, t_msg * msg) {
    msg->type = e_null;
    switch(cmd.type) {
        case e_cmd_msg: return exec_msg(cmd, msg); break;
        case e_cmd_help: return exec_help(); break;
        case e_cmd_auth: return exec_auth(cmd, msg); break;
        case e_cmd_join: return exec_join(cmd, msg); break;
        case e_cmd_rename: return exec_rename(cmd); break;
        default: return errno = printErrMsg(err_command_not_found, __LINE__, __FILE__, NULL); break;
    }

    return 0;
}

void sigintHandler(int signal) {
    // printf("\nProgram ended with %d signal\n", signal);
    stop(0);
}

void stop(int exit_code) {
    queue_destroy(client_msg_queue);
    queue_destroy(server_msg_queue);
    exit(exit_code);
}