/**
 * @file queue.c
 * @author Sniehovskyi Nikita (xsnieh00)
 * @date 24.03.2024
 * @brief Message queue implementation 
 */

#include "queue.h"

t_queue * queue_create() {
    t_queue * queue = malloc(sizeof(t_queue));
    if (queue == NULL) {
        errno = printErrMsg(err_command_not_found, __LINE__, __FILE__, NULL);
    }
    memset(&(*queue), '\0', sizeof(t_queue));
    queue->first = NULL;
    return queue;
}

t_queue_item * queue_create_item() {
    t_queue_item * item = malloc(sizeof(t_queue_item));
    if (item == NULL) {
        errno = printErrMsg(err_command_not_found, __LINE__, __FILE__, NULL);
    }
    memset(&(*item), '\0', sizeof(t_queue_item));
    item->next = NULL;
    return item;
}

int queue_length(t_queue * queue) {
    int cnt = 0;
    t_queue_item * item = queue_first(queue);
    while (item != NULL) {
        cnt++;
        item = item->next;
    }
    return cnt;
}

void queue_add(t_queue * queue, t_queue_item * item) {
    if (queue_last(queue) == NULL) {
        queue->first = item;
    } else {
        queue_last(queue)->next = item;
    }    
}

t_queue_item * queue_first(t_queue * queue) {
    return queue->first;
}

t_queue_item * queue_next(t_queue_item * item) {
    return item->next;
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
    
    if (item == NULL) {
        return item;
    }
    while (item->next != NULL) {
        item = item->next;
    }
    return item;
}

void queue_destroy(t_queue * queue) {
    if (queue != NULL) { 
        while (queue->first != NULL) {
            queue_destroy_item(queue_remove(queue, queue->first->msg.id));
        }
        free(queue);
        queue = NULL;
    }
}

void queue_destroy_item(t_queue_item * item) {
    if (item != NULL) {
        free(item);
        item = NULL;
    }
}