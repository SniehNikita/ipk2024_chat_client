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
        }
        // Check for confirm messages timeouts
        if (argv_parsed.protocol == e_udp) {
            process_timeouts(POLL_INTERVAL);
        }
        if (argv_parsed.protocol == e_tcp && (errno || user.state == e_state_end)) {
            send_bye();
            stop(0); // If tcp -> no need for confirmation
        } else if (argv_parsed.protocol == e_udp && (errno || user.state == e_state_end) && !queue_length(client_msg_queue)) { // UDP might be already waiting for confirm
            send_bye();
        }
    }

    stop(0);
}

int process_command() {
    t_queue_item * new_item;
    t_command cmd;
    int new_state;
    
    memset(&cmd, '\0', sizeof(cmd));
    
    if (get_command(&cmd)) { return errno; } // Gets command from stdin
    if (queue_length(client_msg_queue)) { // Limits ability to wait for multiple msg in a moment
        printWarnMsg(warn_msg_still_pending, __LINE__, __FILE__, NULL);
        return 0;
    }
    new_item = queue_create_item();
    if (errno) { return errno; } // Mem alloc
    if (exec(cmd, &(new_item->msg))) {  // Processing of command
        queue_destroy_item(new_item);
        new_item = NULL;
        return errno;
    }
    if (is_command_local(cmd)) { // help/rename no need to send to server
        queue_destroy_item(new_item);
        new_item = NULL;
        return 0;
    }
    new_state = get_next_state(user.state, new_item->msg); // Calculation of next state
    if (new_state == e_state_null) { // Forbidden command from current state
        printWarnMsg(warn_invalid_state_transition, __LINE__, __FILE__, NULL);
        queue_destroy_item(new_item);
        new_item = NULL;
        return 0;
    }
    if (new_item->msg.type != e_null && new_item->msg.type != e_bye) { // Sending message, BYE should be sent only by routine
        client_send(new_item->msg);
        if (argv_parsed.protocol == e_udp) {
            queue_add(client_msg_queue, new_item);
        }
        new_item->udp.is_confirmed = false;
        new_item->udp.retry_after = argv_parsed.udp_timeout;
    } else {
        queue_destroy_item(new_item);
        new_item = NULL;
    }
    user.state = new_state;

    if (argv_parsed.protocol == e_tcp && new_item != NULL) {
        queue_destroy_item(new_item);
        new_item = NULL;
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
        case e_cmd_close: return exec_close(msg); break;
        default: return errno = printErrMsg(err_command_not_found, __LINE__, __FILE__, NULL); break;
    }

    return 0;
}

int process_packet() {
    t_string buf;
    t_msg msg;
    int new_state;
    int buf_size;

    memset(&buf, '\0', sizeof(buf));
    memset(&msg, '\0', sizeof(msg));

    if (client_read(&buf, &buf_size)) { return errno; }
    if (argv_parsed.protocol == e_tcp) {
        if (parse_tcp(buf, buf_size, &msg)) {
            send_error();
            return errno;
        }
    } else {
        if (parse_udp(buf, buf_size, &msg)) {
            send_error();
            return errno;
        }
    }

    if (msg.type != e_confirm || argv_parsed.protocol == e_tcp) { // Process all messages except confirm
        if (argv_parsed.protocol == e_udp) { // Confirmation is only for udp
            confirm_msg(msg);
            if (is_msg_confirmed(msg)) { // If message is in confirmed array -> leave
                return 0;
            }
            add_msg_confirmed(msg);
        }
        new_state = get_next_state(user.state, msg); // Calculation of next state
        if (new_state == e_state_null) { // Forbidden packet from current state
            printWarnMsg(warn_invalid_state_transition_serv, __LINE__, __FILE__, NULL);
            return warn_invalid_state_transition_serv;
        }
        user.state = new_state;
    }

    switch(msg.type) {
        case e_confirm:
            received_confirm(msg);
            // Close client if confirmation of BYE received
            if (user.state == e_state_end && queue_length(client_msg_queue) == 0) {
                stop(0); // Program stopped by client
            }
            break;
        case e_reply: received_reply(msg); break;
        case e_msg: received_msg(msg); break;
        case e_err: received_err(msg); break;
        case e_bye:
            send_bye(); // Send bye before closing
            if (argv_parsed.protocol == e_tcp) {
                stop(0);
            }
            break;
        default: break;
    }
    return 0;
}

int confirm_msg(t_msg msg) {
    t_msg confirm;
    memset(&confirm, '\0', sizeof(confirm));
    confirm.type = e_confirm;
    confirm.content.confirm.ref_id = msg.id;
    client_send(confirm);
    return 0;
}

bool is_msg_confirmed(t_msg msg) {
    int i = confirmed_msg.length - 1;
    while (confirmed_msg.length - 1 - i < CONFIRM_MAX_REC && i >= 0) {
        if (msg.id == confirmed_msg.confirmed[i % CONFIRM_MAX_REC]) {
            return true;
        }
        i--;
    }
    return false;
}

int add_msg_confirmed(t_msg msg) {
    confirmed_msg.confirmed[confirmed_msg.length % CONFIRM_MAX_REC] = msg.id;
    confirmed_msg.length++;
    return 0;
}

int received_confirm(t_msg msg) {
    queue_destroy_item(queue_remove(client_msg_queue, msg.content.confirm.ref_id));
    return 0;
}

int received_reply(t_msg msg) {
    fprintf(stderr, "%s: ", msg.content.reply.result ? "Success" : "Failure");
    fprintf(stderr, "%s\n", msg.content.reply.content);
    return 0;
}

int received_msg(t_msg msg) {
    printf("%s: %s\n", msg.content.msg.display_name, msg.content.msg.msg);
    return 0;
}

int received_err(t_msg msg) {
    printf("ERR FROM %s: %s\n", msg.content.msg.display_name, msg.content.msg.msg);
    return 0;
}

int process_timeouts(int time_delta) {
    t_queue_item * item;
    item = queue_first(client_msg_queue);
    while (item != NULL) {
        item->udp.retry_after -= time_delta;
        if (item->udp.retry_after <= 0) {
            if (item->udp.retry_count >= argv_parsed.udp_retransmission) { // No reply -> close connection
                stop(errno = printErrMsg(err_retransmission_number_exceeded, __LINE__, __FILE__, NULL));
            }
            client_send(item->msg);
            item->udp.retry_count++;
        }
        item = queue_next(item);
    }
    return 0;
}

int send_error() {
    t_queue_item * new_item;
    new_item = queue_create_item();
    new_item->msg.id = client_msg_count++;
    new_item->msg.type = e_err;
    memcpy(new_item->msg.content.err.display_name, user.display_name, sizeof(user.display_name));    
    memcpy(new_item->msg.content.err.msg, "Message is corrupted", sizeof("Message is corrupted"));
    new_item->udp.is_confirmed = false;
    new_item->udp.retry_after = argv_parsed.udp_timeout;
    user.state = get_next_state(user.state, new_item->msg); // Should always be e_state_end or e_state_err
    if (user.state == e_state_null) { // No communication was started if bye leads to nowhere
        queue_destroy_item(new_item);
        new_item = NULL;
        stop(errno);
    }
    client_send(new_item->msg);    
    if (argv_parsed.protocol == e_udp) {
        queue_add(client_msg_queue, new_item);
    }
    if (argv_parsed.protocol == e_tcp && new_item != NULL) {
        queue_destroy_item(new_item);
        new_item = NULL;
    }
    return 0;    
}

int send_bye() {
    t_queue_item * new_item;
    new_item = queue_create_item();
    new_item->msg.id = client_msg_count++;
    new_item->msg.type = e_bye;
    new_item->udp.is_confirmed = false;
    new_item->udp.retry_after = argv_parsed.udp_timeout;
    user.state = get_next_state(user.state, new_item->msg); // Should always be e_state_end
    if (user.state == e_state_null) { // No communication was started if bye leads to nowhere
        queue_destroy_item(new_item);
        new_item = NULL;
        stop(errno);
    }
    client_send(new_item->msg);
    if (argv_parsed.protocol == e_udp) {
        queue_add(client_msg_queue, new_item);
    }
    if (argv_parsed.protocol == e_tcp && new_item != NULL) {
        queue_destroy_item(new_item);
        new_item = NULL;
    } 
    return 0;
}

void sigintHandler(int signal) {
    send_bye(); // Will send bye or close program if no communication has been started yet
    if (argv_parsed.protocol == e_tcp) {
        stop(0); // If tcp -> no need to wait for confirmation
    }
}

void stop(int exit_code) {
    queue_destroy(client_msg_queue);
    client_close();
    exit(exit_code);
}