/**
 * @file argv_parser.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Parses input arguments from command line 
 */

#include "argv_parser.h"

int argvParse(int argc, char **argv, t_argv *argv_p) {
    int curr_opt = 1;
    bool is_protocol = false;
    bool is_host = false;
    
    argv_p->port = 4567;
    argv_p->udp_timeout = 250;
    argv_p->udp_retransmission = 3;

    while(curr_opt < argc) {
        if (!strcmp(argv[curr_opt], "-h") || !strcmp(argv[curr_opt], "--help")) {
            printHelp();
            return 1;
        } else if (!strcmp(argv[curr_opt], "-t")) {
            if (!strcmp(argv[curr_opt+1], "tcp")) {
                argv_p->protocol = e_tcp;
            } else {
                argv_p->protocol = e_udp;
            }
            is_protocol = true;
            curr_opt++;
        } else if (!strcmp(argv[curr_opt], "-s")) {
            strcpy(argv_p->host, argv[curr_opt+1]);
            is_host = true;
            curr_opt++;
        } else if (!strcmp(argv[curr_opt], "-p")) {
            argv_p->port = atoi(argv[curr_opt+1]);
            curr_opt++;
        } else if (!strcmp(argv[curr_opt], "-d")) {
            argv_p->udp_timeout = atoll(argv[curr_opt+1]);
            curr_opt++;
        } else if (!strcmp(argv[curr_opt], "-r")) {
            argv_p->udp_retransmission = atoi(argv[curr_opt+1]);
            curr_opt++;
        } else {
            return errno = printErrMsg(err_malformed_arguments, __LINE__, __FILE__, NULL);
            return errno = printErrMsg(err_malformed_arguments, __LINE__, __FILE__, NULL);
        }
        curr_opt++;
    }

    if (!is_host || !is_protocol) {
        return errno = printErrMsg(err_malformed_arguments, __LINE__, __FILE__, NULL);
    }

    return 0;
}

void printHelp() {
    printf("Usage: ipk24chat-client [-h] -t <tcp/udp> -s <IP/hostname> -p <port> -d <udp timeout> -r <maximum number of udp transmission>\n");
}