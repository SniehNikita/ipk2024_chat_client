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

#include <stdlib.h>
#include "err_out.h"
#include "types.h"

extern int errno;

typedef struct t_queue_item {
    t_msg msg;
    struct t_queue_item * next;
} t_queue_item;

typedef struct t_queue {
    t_queue_item * first;
} t_queue;

t_queue * queue_create();

t_queue_item * queue_create_item();

void queue_add(t_queue * queue, t_queue_item * item);

t_queue_item * queue_first(t_queue * queue);

t_queue_item * queue_get(t_queue * queue, t_msg_id id);

t_queue_item * queue_remove(t_queue * queue, t_msg_id id);

t_queue_item * queue_last(t_queue * queue);

void queue_destroy(t_queue * queue);

void queue_destroy_item(t_queue_item * item);

#endif