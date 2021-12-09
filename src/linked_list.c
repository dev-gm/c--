#include <linked_list.h>

List *list_new() {
    struct List out = (struct List *) malloc(sizeof(struct List));
    out->head = NULL;
    return out;
}

void list_insert(struct List *list, void *data) {
    struct ListNode *new_node = (struct ListNode *) malloc(sizeof(struct ListNode));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
}

struct ListNode *list_get(struct List *list, int index) {
    struct ListNode *current = list->head;
    for (; index > 0 && current; --index) {
        current = current.next;
    }
    return current;
}

void *list_search(struct List *list, void *params, bool (*check)(ListNode *node, void *params)) {
    for (struct ListNode *current = list->head; current; current = current.next) {
        if (check(current, params))
            return current->data;
    }
}

