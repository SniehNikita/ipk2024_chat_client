/**
 * @file err_out.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Prints error/warning message to stdout 
 */

#include "err_out.h"

int printErrMsg(error_code errno, int lineno, char *file_name, char *msg) {
    fprintf(stderr, "ERR: ");
    fprintf(stderr, "Error [%d] on line [%d]", errno, lineno);
    if (file_name != NULL) {
        fprintf(stderr, " in file [%s]: ", file_name);
    } else {
        fprintf(stderr, ": ");
    }
    switch (errno) {
        case err_string_overflow: fprintf(stderr, "Max string length overflowed");break;
        case err_memory_alloc: fprintf(stderr, "Memory allocation error");break;
        case err_malformed_arguments: fprintf(stderr, "Malformed arguments");break;
        case err_socket_creation_failed: fprintf(stderr, "Socket creation failed");break;
        case err_msg_null: fprintf(stderr, "Message is null");break;
        case err_msg_type_unknown: fprintf(stderr, "Message type is unknown");break;
        case err_cmd_corrupted: fprintf(stderr, "Command is corrupted. Use /help");break;
        case err_command_not_found: fprintf(stderr, "Command is unknown");break;
        case err_retransmission_number_exceeded: fprintf(stderr, "Server did not confirm receipt of the message repeatedly");break;
        default: fprintf(stderr, "Unknown error");break;
    }
    if (msg != NULL) {
        fprintf(stderr, ". With message [%s]", msg);
    }
    fprintf(stderr, ". Program was stopped.\n");
    return errno;
}

int printWarnMsg(warning_code warno, int lineno, char *file_name, char *msg) {
    fprintf(stderr, "ERR: ");
    fprintf(stderr, "Warning [%d] on line [%d]", warno, lineno);    
    if (file_name != NULL) {
        fprintf(stderr, " in file [%s]: ", file_name);
    } else {
        fprintf(stderr, ": ");
    }
    switch (warno) {
        case warn_net_receive_failed: fprintf(stderr, "recvfrom funtion failed");break;
        case warn_net_send_failed: fprintf(stderr, "sendto function failed");break;
        case warn_too_many_argv: fprintf(stderr, "Too many arguments");break;
        case warn_command_not_found: fprintf(stderr, "Command not found. Use /help");break;
        case warn_not_enough_argv: fprintf(stderr, "Not enough arguments. Use /help");break;
        case warn_msg_still_pending: fprintf(stderr, "Previous command/message is still being processed. Input was ignored");break;
        case warn_username_len_exceeded: fprintf(stderr, "User name is too long");break;
        case warn_channelid_len_exceeded: fprintf(stderr, "Channel id is too long");break;
        case warn_secret_len_exceeded: fprintf(stderr, "Secret is too long");break;
        case warn_displayname_len_exceeded: fprintf(stderr, "Display name is too long");break;
        case warn_message_len_exceeded: fprintf(stderr, "Message is too long");break;
        case warn_username_char_forbidden: fprintf(stderr, "User name contains forbidden char");break;
        case warn_channelid_char_forbidden: fprintf(stderr, "Channel id contains forbidden char");break;
        case warn_secret_char_forbidden: fprintf(stderr, "Secret contains forbidden char");break;
        case warn_displayname_char_forbidden: fprintf(stderr, "Diplay name contains forbidden char");break;
        case warn_message_char_forbidden: fprintf(stderr, "Message contains forbidden char");break;
        case warn_invalid_state_transition: fprintf(stderr, "Invalid state transition was ignored");break;
        default: fprintf(stderr, "Unknown warning");break;
    }
    if (msg != NULL) {
        fprintf(stderr, ". With message [%s]", msg);
    }
    fprintf(stderr, ".\n");
    return warno;
}