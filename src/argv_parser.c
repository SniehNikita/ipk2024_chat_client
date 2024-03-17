/**
 * @file argv_parser.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Parses input arguments from command line 
 */

#include "argv_parser.h"

int optRead(int argc, char **argv, argv_t *argv_p) {
    return 0;
}

void printHelp() {
    printf("Usage: ipk24chat-client [-h] -t <tcp/udp> -s <IP/hostname> -p <port> -d <udp timeout> -r <maximum number of udp transmission>\n");
}