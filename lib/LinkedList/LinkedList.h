#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdbool.h>

typedef struct LinkedListNode_t
{
    struct LinkedListNode_t *next;
} LinkedListNode_t;

typedef struct
{
    LinkedListNode_t *head;
} LinkedList_t;

void LinkedList_Init(LinkedList_t *instance);
bool LinkedList_Contains(LinkedList_t *instance, LinkedListNode_t *node);
void LinkedList_Push(LinkedList_t *instance, LinkedListNode_t *node);
void LinkedList_Remove(LinkedList_t *instance, LinkedListNode_t *node);
void LinkedList_ForEach(LinkedList_t *instance, void *func(void*,void*), void *context);

#endif
