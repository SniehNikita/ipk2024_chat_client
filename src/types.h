/**
 * @file types.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 16.03.2024
 * @brief Header file for types declaration
 */

#ifndef __TYPES_H__

#define __TYPES_H__

#include <stdint.h>

/**
 * @brief Protocol type 
 */
typedef enum {
    udp = 0,
    tcp = 1
} transport_protocol;

/**
 * @brief Command line arguments type 
 */
typedef struct argv_t {
    transport_protocol protocol;
    char host[256];
    uint16_t port;
    uint16_t udp_timeout;
    uint8_t udp_retransmission;
} argv_t;

#endif