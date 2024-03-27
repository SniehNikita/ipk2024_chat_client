/**
 * @file client.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Implementation of client
 */

#include "client.h"

int protocol;
int socket_fd;
struct sockaddr_in addr;

int client(t_argv argv, int * fd) {
    struct hostent * server_ip;

    protocol = argv.protocol;
    server_ip = gethostbyname(argv.host);
    if (server_ip == NULL) {
        return errno = printErrMsg(err_host_not_found, __LINE__, __FILE__, NULL);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv.host);
    addr.sin_port = htons(argv.port);
    memcpy(&addr.sin_addr.s_addr, server_ip->h_addr_list[0], server_ip->h_length);

    if (protocol == e_udp) {
        if (udp_client(argv)) { return errno; }
    } else {
        if (tcp_client(argv)) { return errno; }
    }
    *fd = socket_fd;

    return 0;
}

int client_send(t_msg msg) {
    t_string buf;
    int buf_size;

    compose(msg, &buf, &buf_size);    
    if (protocol == e_udp) {
        if (udp_send(buf, buf_size)) { return errno; }
    } else {
        if (tcp_send(buf, buf_size)) { return errno; }
    }
    return 0;
}

int client_read(t_string * buf, int * buf_size) {
    if (protocol == e_udp) {
        if (udp_read(buf, buf_size)) { return errno; }
    } else {
        if (tcp_read(buf, buf_size)) { return errno; }
    }
    return 0;
}

int client_close() {
    close(socket_fd);
    return 0;
}

int udp_client() {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd <= 0) {
        return errno = printErrMsg(err_socket_creation_failed, __LINE__, __FILE__, NULL);
    }

    return 0;
}

int udp_send(t_string buf, int buf_size) {
    sendto(socket_fd, buf, buf_size, 0, (struct sockaddr * ) &addr, sizeof(addr));
    return 0;
}

int udp_read(t_string * buf, int * buf_size) {
    socklen_t addr_size = sizeof(addr);
    *buf_size = recvfrom(socket_fd, buf, STR_MAX_LEN, 0, (struct sockaddr * ) &addr, &addr_size);
    return 0;
}

int tcp_send(t_string buf, int buf_size) {

    return 0;
}

int tcp_client(t_argv argv) {

    return 0;
}

int tcp_read(t_string * buf, int * buf_size) {

    return 0;
}

void lprintf(t_string str, int len) {
    printf("lprintf>%d:", len);
    for (int i=0;i<len;i++) {
        if (str[i] == 0x00) {
            printf(".");
        } else {
            printf("%c", str[i]);                
        }
    }
    printf("\n");
}