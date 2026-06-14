/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_LIST_H__
#define __QUA_LIST_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct qua_list_node_t {
    struct qua_list_node_t *next;
    struct qua_list_node_t *prev;
} qua_list_node_t;

#define qua_node_to_item(node, container, member) \
    (container *) (((char*) (node)) - offsetof(container, member))

#define qua_list_declare(name) \
    qua_list_node_t name = { \
        .next = &(name), \
        .prev = &(name), \
    }

#define qua_list_declare_static(name) \
    static qua_list_node_t name = { \
        .next = &(name), \
        .prev = &(name), \
    }

#define qua_list_for_each(node, list) \
    for ((node) = (list)->next; (node) != (list); (node) = (node)->next)

#define qua_list_for_each_reverse(node, list) \
    for ((node) = (list)->prev; (node) != (list); (node) = (node)->prev)

#define qua_list_for_each_safe(node, n, list) \
    for ((node) = (list)->next, (n) = (node)->next; \
         (node) != (list); \
         (node) = (n), (n) = (node)->next)

static inline void qua_list_init(qua_list_node_t *node) {
    node->next = node;
    node->prev = node;
}

static inline void qua_list_add_tail(qua_list_node_t *head, qua_list_node_t *item) {
    item->next = head;
    item->prev = head->prev;
    head->prev->next = item;
    head->prev = item;
}

static inline void qua_list_add_head(qua_list_node_t *head, qua_list_node_t *item) {
    item->next = head->next;
    item->prev = head;
    head->next->prev = item;
    head->next = item;
}

static inline void qua_list_insert(qua_list_node_t *pos, qua_list_node_t *item) {
    if (pos->prev == pos) {
        qua_list_add_tail(pos, item);
    } else {
        item->next = pos;
        item->prev = pos->prev;

        pos->prev->next = item;
        pos->prev = item;
    }
}

static inline void qua_list_remove(qua_list_node_t *item) {
    item->next->prev = item->prev;
    item->prev->next = item->next;
}

static inline int qua_list_length(qua_list_node_t *list) {
    qua_list_node_t *node;
    int length = 0;

    qua_list_for_each(node, list)
        length++;

    return length;
}

#define qua_list_empty(list) ((list) == (list)->next)
#define qua_list_head(list) ((list)->next)
#define qua_list_tail(list) ((list)->prev)

#ifdef __cplusplus
};
#endif /* __QUA_LIST_H__ */

#endif
