/**
 * @file main.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @(date 16.03.2024
 * @brief Entry point of a program
 */

#include "main.h"

t_argv argv_parsed;

int main(int argc, char **argv) {
    struct pollfd poll_fds[2];

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
        int ret = poll(poll_fds, 2, POLL_INTERVAL); 
        if (ret > 0) {
            if (poll_fds[0].revents & POLLIN) {
                process_packet();
            }
            if (poll_fds[1].revents & POLLIN) {
                process_command();
            }
            // Check for confirm messages timeouts
            if (argv_parsed.protocol == e_udp) {
                if (process_timeouts(POLL_INTERVAL)) { stop(errno); }
            }
        }
    }

    stop(0);
}

int process_command() {
    t_queue_item * new_item;
    t_command cmd;
    int new_state;
    
    if (get_command(&cmd)) { return errno; }
    new_item = queue_create_item();
    if (errno != 0) { stop(errno); }
    if (exec(cmd, &(new_item->msg))) { stop(errno); }
    if (is_command_local(cmd)) {
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
        client_send(new_item->msg);
        new_item->udp.is_confirmed = false;
        new_item->udp.retry_after = argv_parsed.udp_timeout;
    } else {
        queue_destroy_item(new_item);
    }
    user.state = new_state;

    return 0;
}

int process_packet() {
    t_string buf;
    t_msg msg;
    int buf_size;

    if (client_read(&buf, &buf_size)) { return errno; }
    parse(&buf, buf_size, &msg);

    return 0;
}

int process_timeouts(int time_delta) {
    t_queue_item * item;
    item = queue_first(client_msg_queue);
    while (item != NULL) {
        if (item->udp.is_confirmed) { // TODO move to process_packet()
            item = queue_next(item);
            queue_destroy_item(queue_remove(client_msg_queue, item->msg.id));
            continue;
        } 
        item->udp.retry_after -= time_delta;
        if (item->udp.retry_after <= 0) {
            if (item->udp.retry_count >= argv_parsed.udp_retransmission) {
                return errno = printErrMsg(err_retransmission_number_exceeded, __LINE__, __FILE__, NULL);
            }
            client_send(item->msg);
            item->udp.retry_count++;
        }
        item = queue_next(item);
    }
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
    stop(0);
}

void stop(int exit_code) {
    queue_destroy(client_msg_queue);
    queue_destroy(server_msg_queue);
    client_close();
    exit(exit_code);
}