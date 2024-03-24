/**
 * @file queue.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Message queue implementation 
 */

#include "queue.h"

t_queue * queue_init(t_queue * queue) {
    queue->first = NULL;
    return queue;
}

t_queue_item * queue_item_init(t_queue_item * item) {
    item->next = NULL;
    return item;
}

t_queue * queue_add(t_queue * queue, t_queue_item * item) {
    queue_last(queue)->next = item;
    return queue;
}

t_queue_item * queue_first(t_queue * queue) {
    return queue->first;
}

t_queue_item * queue_get(t_queue * queue, t_msg_id id) {
    t_queue_item * item = queue->first;
    while (item != NULL && item->msg.id != id) {
        item = item->next;
    }
    return item;
}

t_queue_item * queue_remove(t_queue * queue, t_msg_id id) {
    t_queue_item * last_item = queue->first;
    t_queue_item * item;

    if (last_item == NULL) {
        return last_item;
    }
    if (last_item->msg.id == id) {
        queue->first = queue->first->next;
        return last_item;
    }
    item = last_item->next;
    while (item != NULL) {
        if (item->msg.id == id) {
            last_item->next = item->next;
            break;
        }
        last_item = item;
        item = item->next;
    }
    return item;
}

t_queue_item * queue_last(t_queue * queue) {
    t_queue_item * item = queue->first;
    while (item != NULL) {
        item = item->next;
    }
    return item;
}

