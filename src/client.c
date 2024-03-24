/**
 * @file client.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Implementation of client UDP part
 */

#include "client.h"

int socket_fd;
struct sockaddr_in addr;
socklen_t addr_size;
/**
 * @brief Is any response from server is expected 
 */
bool is_await = false;

int client(t_argv * argv) {
    struct hostent * server_ip;

    server_ip = gethostbyname(argv->host);
    if (server_ip == NULL) {
        return errno = printErrMsg(err_host_not_found, __LINE__, __FILE__, NULL);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv->host);
    addr.sin_port = htons(argv->port);
    memcpy(&addr.sin_addr.s_addr, server_ip->h_addr_list[0], server_ip->h_length);
    addr_size = sizeof(addr);

    if (argv->protocol == e_udp) {
        udp_client(argv);
    } else {
        tcp_client(argv);
    }

    return 0;
}

int udp_client(t_argv * argv) {
    t_string recv_bytes;
    t_string send_bytes;
    t_msg msg_recv;
    t_msg msg_send;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd <= 0) {
        return errno = printErrMsg(err_socket_creation_failed, __LINE__, __FILE__, NULL);
    }
    
    while (!is_sigpipe) {
        int len = 0;
        memset(&msg_recv, 0, sizeof(msg_recv));
        memset(&msg_send, 0, sizeof(msg_send));

        if (is_await) {
            addr_size = sizeof(addr);
            len = recvfrom(socket_fd, &recv_bytes, STR_MAX_LEN, 0, (struct sockaddr * ) &addr, &addr_size);
            if (len < 0) {
                printWarnMsg(warn_net_receive_failed, __LINE__, __FILE__, NULL);
                continue;
            }
        }
        
        if (parse(&recv_bytes, len, &msg_recv)) {
            return errno;
        }
        printf("Received: %s\n", recv_bytes);
        // TODO check if already exists
        
        if (process(msg_recv, &msg_send)) {
            return errno;
        }
        
        len = 0;
        if (compose(msg_send, &send_bytes, &len)) {
            return errno;
        }
        memcpy(&send_bytes, "Hello", 6);
        if (sendto(socket_fd, send_bytes, len, 0, (struct sockaddr * ) &addr, sizeof(addr)) < 0) {
            printWarnMsg(warn_net_send_failed, __LINE__, __FILE__, NULL);
            continue;
        }

        is_sigpipe = true;
    }

    return 0;
}

int process(t_msg msg_recv, t_msg * msg_send) {

    return 0;
}

int tcp_client(t_argv * argv) {

    return 0;
}
