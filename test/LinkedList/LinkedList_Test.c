#include "unity.h"
#include "LinkedList.h"

LinkedList_t list;
int callbackCount;
int argSaver[2];

void TheCallback(void *context, void *args)
{
    argSaver[callbackCount] = *(int *)args;
    callbackCount++;
}

void setUp(void)
{
    callbackCount = 0;
    argSaver[0] = 0;
    argSaver[1] = 0;
}

void tearDown(void)
{

}

void ShouldAddANode(void)
{
    LinkedListNode_t node;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &node);

    TEST_ASSERT_EQUAL_PTR(&node, list.head);
}

void ShouldRemoveANode(void)
{
    LinkedListNode_t node;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &node);
    TEST_ASSERT_EQUAL_PTR(&node, list.head);
    LinkedList_Remove(&list, &node);

    TEST_ASSERT_EQUAL_PTR(NULL, list.head);
}

void ShouldHandleMultipleAdds(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);
    LinkedList_Push(&list, &nodeB);
    TEST_ASSERT_EQUAL_PTR(&nodeB, list.head);
    TEST_ASSERT_EQUAL_PTR(&nodeA, nodeB.next);
}

void ShouldHandleMultipleRemoves(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);
    LinkedList_Push(&list, &nodeB);
    TEST_ASSERT_EQUAL_PTR(&nodeB, list.head);
    TEST_ASSERT_EQUAL_PTR(&nodeA, nodeB.next);
    LinkedList_Remove(&list, &nodeA);
    TEST_ASSERT_EQUAL_PTR(NULL, nodeB.next);
}

void ShouldBeAbleToRemoveHead(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);
    LinkedList_Push(&list, &nodeB);
    TEST_ASSERT_EQUAL_PTR(&nodeB, list.head);
    TEST_ASSERT_EQUAL_PTR(&nodeA, nodeB.next);
    LinkedList_Remove(&list, &nodeB);

    TEST_ASSERT_EQUAL_PTR(&nodeA, list.head);
}

void ShouldFindNode(void)
{
    LinkedListNode_t node;
    bool result;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &node);

    TEST_ASSERT_TRUE(LinkedList_Contains(&list, &node));
}

void ShouldFindNodeWithMultiple(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);
    LinkedList_Push(&list, &nodeB);

    TEST_ASSERT_TRUE(LinkedList_Contains(&list, &nodeA));
    TEST_ASSERT_TRUE(LinkedList_Contains(&list, &nodeB));
}

void ShouldNotFindNonExistentNode(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);

    TEST_ASSERT_TRUE(LinkedList_Contains(&list, &nodeA));
    TEST_ASSERT_FALSE(LinkedList_Contains(&list, &nodeB));
}

void ShouldHandleNodeNotInListDuringRemove(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);
    LinkedList_Remove(&list, &nodeB);
    TEST_ASSERT_EQUAL_PTR(&nodeA, list.head);
}

void ShouldIssueCallbackForEachNode(void)
{
    LinkedListNode_t nodeA;
    LinkedListNode_t nodeB;
    int args = 23;

    LinkedList_Init(&list);
    LinkedList_Push(&list, &nodeA);
    LinkedList_Push(&list, &nodeB);
    LinkedList_ForEach(&list, TheCallback, &args);

    TEST_ASSERT_EQUAL_INT(args, argSaver[0]);
    TEST_ASSERT_EQUAL_INT(args, argSaver[1]);
    TEST_ASSERT_EQUAL_INT(2, callbackCount);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(ShouldAddANode);
    RUN_TEST(ShouldRemoveANode);
    RUN_TEST(ShouldHandleMultipleAdds);
    RUN_TEST(ShouldHandleMultipleRemoves);
    RUN_TEST(ShouldBeAbleToRemoveHead);
    RUN_TEST(ShouldFindNode);
    RUN_TEST(ShouldFindNodeWithMultiple);
    RUN_TEST(ShouldNotFindNonExistentNode);
    RUN_TEST(ShouldHandleNodeNotInListDuringRemove);
    RUN_TEST(ShouldIssueCallbackForEachNode);

    return UNITY_END();
}
