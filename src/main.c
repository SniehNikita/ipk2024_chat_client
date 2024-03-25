/**
 * @file main.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Entry point of a program
 */

#include "main.h"

int main(int argc, char **argv) {
    t_argv argv_parsed;
    struct pollfd poll_fds[2];
    t_command cmd;
    t_msg msg;
    t_string buf;
    int buf_size;

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
                get_command(&cmd);
                if (exec(cmd, &msg)) { stop(errno); }
                if (msg.type != e_null) {
                    compose(msg, &buf, &buf_size);
                    client_send(buf, buf_size);
                }
            }
        }
    }

    return 0;
}

int exec(t_command cmd, t_msg * msg) {
    msg->type = e_null;
    switch(cmd.type) {
        case e_cmd_help: return exec_help(); break;
        case e_cmd_auth: return exec_auth(cmd, msg); break;
        case e_cmd_join: return exec_join(cmd, msg); break;
        case e_cmd_rename: return exec_rename(cmd); break;
        default: return errno = printErrMsg(err_command_not_found, __LINE__, __FILE__, NULL); break;
    }

    return 0;
}

void sigintHandler(int signal) {
    printf("\nProgram ended with %d signal\n", signal);
    stop(0);
}

void stop(int exit_code) {
    exit(exit_code);
}