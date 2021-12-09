#include <stdlib.h>

struct List {
    struct ListNode *head;
};

struct ListNode {
    struct ListNode *next;
    void *data;
};

List *list_new();

void list_insert(struct List *list, void *data);

ListNode *list_get(struct List *list, int index);

void *list_search(struct List *list, void *params, bool (*check)(ListNode *node, void *params));
