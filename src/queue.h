/**
 * @file queue.h
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Header file for queue implementation
 *
 * Queue for messages managing
 */


#ifndef __QUEUE_H__

#define __QUEUE_H__

#include "err_out.h"
#include "types.h"

typedef struct t_queue_item {
    t_msg msg;
    t_queue_item * next;
} t_queue_item;

typedef struct t_queue {
    t_queue_item * first;
} t_queue;

#endif