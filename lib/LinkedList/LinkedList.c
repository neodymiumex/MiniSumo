
#include "LinkedList.h"
#include <stddef.h>

void LinkedList_Init(LinkedList_t *instance)
{
    instance->head = NULL;
}

bool LinkedList_Contains(LinkedList_t *instance, LinkedListNode_t *node)
{
    bool found = false;

    if(instance->head == node)
    {
        found = true;
    }
    else
    {
        LinkedListNode_t *current = instance->head;

        while(current)
        {
            if(current->next == node)
            {
                found = true;
                break;
            }

            current = current->next;
        }
    }

    return found;
}

void LinkedList_Push(LinkedList_t *instance, LinkedListNode_t *node)
{
    node->next = instance->head;
    instance->head = node;
}

void LinkedList_Remove(LinkedList_t *instance, LinkedListNode_t *node)
{
    if(instance->head == node)
    {
        instance->head = node->next;
    }
    else
    {
        LinkedListNode_t *current = instance->head;

        while(current)
        {
            if(current->next == node)
            {
                current->next = node->next;
                break;
            }

            current = current->next;
        }
    }
}

void LinkedList_ForEach(LinkedList_t *instance, void *func(void*,void*), void *context)
{
    LinkedListNode_t *current = instance->head;

    while(current)
    {
        func(current, context);
        current = current->next;
    }
}
