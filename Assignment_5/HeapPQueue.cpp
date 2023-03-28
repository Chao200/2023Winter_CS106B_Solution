#include "HeapPQueue.h"
using namespace std;

HeapPQueue::HeapPQueue() {
    elems = new DataPoint[INITIAL_SIZE];
    logicalSize = 0;
    allocatedSize = INITIAL_SIZE;
}

HeapPQueue::~HeapPQueue() {
    delete[] elems;
}

int HeapPQueue::size() const {
//    /* TODO: Delete the next line and implement this. */
//    return 0;
    return logicalSize;
}

bool HeapPQueue::isEmpty() const {
//    /* TODO: Delete the next line and implement this. */
//    return 0;
    return !logicalSize;
}

void HeapPQueue::enqueue(const DataPoint& data) {
    /* TODO: Delete the next line and implement this. */
//    (void) data;
    if (logicalSize == allocatedSize - 1) {   // 第 0 个位置不用，所以 -1
        allocatedSize *= 2;  // 两倍存储空间
        DataPoint *new_elems = new DataPoint[allocatedSize];
        for (int i = 1; i <= logicalSize; i++) {  // 拷贝到新的数组
            new_elems[i] = elems[i];
        }
        delete [] elems;    // 释放旧的内存
        elems = new_elems;
    }
    ++logicalSize;
    int index = logicalSize;
    elems[index] = data;
    while (index / 2 > 0 && elems[index].weight < elems[index/2].weight) {
        swap(elems[index], elems[index/2]);
        index /= 2;
    }
}

DataPoint HeapPQueue::peek() const {
    /* TODO: Delete the next line and implement this. */
//    return {};
    if (isEmpty()) { // isempty()
        error("empty!");
        return {};
    }
    return elems[1];
}

DataPoint HeapPQueue::dequeue() {
    /* TODO: Delete the next line and implement this. */
//    return {};
    if (isEmpty()) {
        error("empty!");
        return {};
    }
    swap(elems[1], elems[logicalSize]);  // 交换首尾元素
    logicalSize--;    // 个数减一，表示移除了最小元素
    int parent = 1;   // 被交换的元素位置
    int child;        // 存储子节点位置
    // 表示有子节点，子节点为 2 * parent <=logicalSize ==> parent <= logicalSize / 2
    while (parent <= logicalSize / 2) {
        child = 2 * parent;    // 左子节点
        // 如果右子节点存在，则找到两个子节点中 weight 最小的那个
        if (child + 1 <= logicalSize && elems[child].weight > elems[child+1].weight) {
            child++;
        }
        // 如果父节点不大于子节点，则 break
        if (elems[parent].weight <= elems[child].weight) {
            break;
        }
        // 父节点小于子节点，swap
        swap(elems[parent], elems[child]);
        // 将交换的新的位置作为新的父节点的位置
        parent = child;
    }
    return elems[logicalSize + 1];
}



/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */














/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Newly-created heap is empty.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

PROVIDED_TEST("Newly-created heap allocates an array of the right size.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    EXPECT_EQUAL(pq.logicalSize, 0);
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Can enqueue a single element.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    pq.enqueue({ "First", 137.0 });

    /* Check other fields for validity. */
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.logicalSize, 1);
    EXPECT(!pq.isEmpty());

    /* Slot zero in the priority queue is not used, so we won't check
     * its value here.
     *
     * Slot one in the priority queue should contain the newly-added
     * element.
     */
    EXPECT_EQUAL(pq.elems[1], { "First", 137.0 });

    /* There is still space left, so no array growth
     * should have been necessary.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueues two elements in sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one = { "First",  1.6108 };
    DataPoint two = { "Second", 2.7182 };

    pq.enqueue(one);

    /* Make sure it's there. */
    EXPECT_EQUAL(pq.elems[1], one);

    pq.enqueue(two);

    /* We should have
     *
     * [ unused ] [ one ] [ two ]
     *
     * because one.weight < two.weight.
     */
    EXPECT_EQUAL(pq.size(), 2);
    EXPECT_EQUAL(pq.elems[1], one);
    EXPECT_EQUAL(pq.elems[2], two);

    /* There is still space left, so no array growth
     * should have been necessary.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueues two elements in reverse sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one = { "First",  3.1415 };
    DataPoint two = { "Second", 2.7182 };

    pq.enqueue(one);

    /* Make sure it's there. */
    EXPECT_EQUAL(pq.elems[1], one);

    pq.enqueue(two);

    /* We should have
     *
     * [ unused ] [ two ] [ one ]
     *
     * because one.weight > two.weight.
     */
    EXPECT_EQUAL(pq.size(), 2);
    EXPECT_EQUAL(pq.elems[1], two);
    EXPECT_EQUAL(pq.elems[2], one);

    /* There is still space left, so no array growth
     * should have been necessary.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueues three elements in sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one   = { "First",  1.6108 };
    DataPoint two   = { "Second", 2.7182 };
    DataPoint three = { "Third",  3.1415 };

    pq.enqueue(one);
    pq.enqueue(two);
    pq.enqueue(three);

    /* We should have these contents:
     *
     * [ unused ] [ one ] [ two ] [ three ]
     */
    EXPECT_EQUAL(pq.size(), 3);
    EXPECT_EQUAL(pq.elems[1], one);
    EXPECT_EQUAL(pq.elems[2], two);
    EXPECT_EQUAL(pq.elems[3], three);

    /* There is still space left, so no array growth
     * should have been necessary.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueues three elements in reverse-sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one   = { "First",  161 };
    DataPoint two   = { "Second", 106 };
    DataPoint three = { "Third",  103 };

    pq.enqueue(one);
    pq.enqueue(two);   // Bubbles up
    pq.enqueue(three); // Bubbles up

    /* We should have these contents:
     *
     * [ unused ] [ three ] [ one ] [ two ]
     *
     * The reason is that inserting two should swap with
     * one, and inserting three should swap with two.
     */
    EXPECT_EQUAL(pq.size(), 3);
    EXPECT_EQUAL(pq.elems[1], three);
    EXPECT_EQUAL(pq.elems[2], one);
    EXPECT_EQUAL(pq.elems[3], two);

    /* There is still space left, so no array growth
     * should have been necessary.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueues five elements in sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one   = { "First",  1 };
    DataPoint two   = { "Second", 2 };
    DataPoint three = { "Third",  3 };
    DataPoint four  = { "Four",   4 };
    DataPoint five  = { "Five",   5 };

    pq.enqueue(one);
    pq.enqueue(two);
    pq.enqueue(three);
    pq.enqueue(four);
    pq.enqueue(five);

    /* We should have these contents:
     *
     * [ unused ] [ one ] [ two ] [ three ] [ four ] [ five ]
     *
     * No insertion should trigger a bubble up.
     */
    EXPECT_EQUAL(pq.size(), 5);
    EXPECT_EQUAL(pq.elems[1], one);
    EXPECT_EQUAL(pq.elems[2], two);
    EXPECT_EQUAL(pq.elems[3], three);
    EXPECT_EQUAL(pq.elems[4], four);
    EXPECT_EQUAL(pq.elems[5], five);

    /* We are exactly at capacity. No growth should have
     * occurred here.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueues five elements in reverse-sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one   = { "First",  10 };
    DataPoint two   = { "Second", 9 };
    DataPoint three = { "Third",  8 };
    DataPoint four  = { "Four",   7 };
    DataPoint five  = { "Five",   6 };

    pq.enqueue(one);
    pq.enqueue(two);
    pq.enqueue(three);
    pq.enqueue(four);
    pq.enqueue(five);

    /* We should have these contents:
     *
     * [ unused ] [ five ] [ four ] [ two ] [ one ] [ three ]
     *
     * To see why this is, get out a pencil and paper and trace
     * through the exact series of insertions performed.
     */
    EXPECT_EQUAL(pq.size(), 5);
    EXPECT_EQUAL(pq.elems[1], five);
    EXPECT_EQUAL(pq.elems[2], four);
    EXPECT_EQUAL(pq.elems[3], two);
    EXPECT_EQUAL(pq.elems[4], one);
    EXPECT_EQUAL(pq.elems[5], three);

    /* We are exactly at capacity. No growth should have
     * occurred here.
     */
    EXPECT_EQUAL(pq.allocatedSize, HeapPQueue::INITIAL_SIZE);
}

PROVIDED_TEST("Enqueue grows the internal array.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add these items in this order. */
    DataPoint one   = { "First",  1 };
    DataPoint two   = { "Second", 2 };
    DataPoint three = { "Third",  3 };
    DataPoint four  = { "Four",   4 };
    DataPoint five  = { "Five",   5 };
    DataPoint six   = { "Six",    6 };

    pq.enqueue(one);
    pq.enqueue(two);
    pq.enqueue(three);
    pq.enqueue(four);
    pq.enqueue(five);

    /* This next one triggers the grow step. */
    auto* oldArray = pq.elems;
    pq.enqueue(six);

    /* The array should have been replaced with a new one. */
    EXPECT_NOT_EQUAL(pq.elems, oldArray);
    EXPECT_EQUAL(pq.allocatedSize, 2 * HeapPQueue::INITIAL_SIZE);

    /* We should have these contents:
     *
     * [ unused ] [ one ] [ two ] [ three ] [ four ] [ five ] [ six ]
     *
     * No insertion should trigger a bubble up.
     */
    EXPECT_EQUAL(pq.elems[1], one);
    EXPECT_EQUAL(pq.elems[2], two);
    EXPECT_EQUAL(pq.elems[3], three);
    EXPECT_EQUAL(pq.elems[4], four);
    EXPECT_EQUAL(pq.elems[5], five);
    EXPECT_EQUAL(pq.elems[6], six);
    EXPECT_EQUAL(pq.size(), 6);
}

PROVIDED_TEST("Enqueue repeatedly grows the internal array.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Insert lots of values in sorted order. No bubble-up should
     * ever occur in this process, but the internal array will
     * need to grow many times.
     */
    for (int i = 1; i < 100; i++) {
        pq.enqueue({ to_string(i), double(i) });
        EXPECT_EQUAL(pq.size(), i);

        /* Confirm the array contents exactly match what they
         * should be - namely, the elements in sorted order.
         */
        for (int j = 1; j <= i; j++) {
            EXPECT_EQUAL(pq.elems[j], { to_string(j), double(j) });
        }
    }
}

PROVIDED_TEST("Enqueue can bubble up long chains.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* First, create this heap:
     *
     *                2
     *             /     \
     *          3           4
     *         /  \       /   \
     *        5    6     7     8
     *       / \  / \   / \   /
     *      9 10 11 12 13 14 15
     *
     * This can be done by inserting all these elements in order
     * into the priority queue.
     */
    for (int i = 2; i <= 15; i++) {
        pq.enqueue({ to_string(i), double(i) });
    }

    /* Confirm the queue is set up correctly. */
    for (int i = 2; i <= 15; i++) {
        EXPECT_EQUAL(pq.elems[i - 1], { to_string(i), double(i) });
    }
    EXPECT_EQUAL(pq.size(), 14);

    /* Now, enqueue 1. This will cause a chain of swaps that
     * ends with the following:
     *
     *                1
     *             /     \
     *          3           2
     *         /  \       /   \
     *        5    6     7     4
     *       / \  / \   / \   / \
     *      9 10 11 12 13 14 15  8
     */
    pq.enqueue({ "1", 1 });
    EXPECT_EQUAL(pq.size(), 15);

    EXPECT_EQUAL(pq.elems[1],   {  "1",  1 });
    EXPECT_EQUAL(pq.elems[2],   {  "3",  3 });
    EXPECT_EQUAL(pq.elems[3],   {  "2",  2 });
    EXPECT_EQUAL(pq.elems[4],   {  "5",  5 });
    EXPECT_EQUAL(pq.elems[5],   {  "6",  6 });
    EXPECT_EQUAL(pq.elems[6],   {  "7",  7 });
    EXPECT_EQUAL(pq.elems[7],   {  "4",  4 });
    EXPECT_EQUAL(pq.elems[8],   {  "9",  9 });
    EXPECT_EQUAL(pq.elems[9],   { "10", 10 });
    EXPECT_EQUAL(pq.elems[10],  { "11", 11 });
    EXPECT_EQUAL(pq.elems[11],  { "12", 12 });
    EXPECT_EQUAL(pq.elems[12],  { "13", 13 });
    EXPECT_EQUAL(pq.elems[13],  { "14", 14 });
    EXPECT_EQUAL(pq.elems[14],  { "15", 15 });
    EXPECT_EQUAL(pq.elems[15],  {  "8",  8 });
}

PROVIDED_TEST("Can enqueue and dequeue a single element.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    DataPoint first = { "First", 137 };
    pq.enqueue(first);

    EXPECT_EQUAL(pq.dequeue(), first);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());
}

PROVIDED_TEST("Reports an error when dequeuing from an empty queue.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    EXPECT_ERROR(pq.dequeue());
}

PROVIDED_TEST("Can bubble down to the left.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Form this heap:
     *
     *         1
     *        / \
     *       2   3
     *      /
     *     4
     */
    DataPoint v1 = { "First",  1 };
    DataPoint v2 = { "Second", 2 };
    DataPoint v3 = { "Third",  3 };
    DataPoint v4 = { "Fourth", 4 };

    pq.enqueue(v1);
    pq.enqueue(v2);
    pq.enqueue(v3);
    pq.enqueue(v4);

    EXPECT_EQUAL(pq.size(), 4);
    EXPECT_EQUAL(pq.elems[1], v1);
    EXPECT_EQUAL(pq.elems[2], v2);
    EXPECT_EQUAL(pq.elems[3], v3);
    EXPECT_EQUAL(pq.elems[4], v4);

    /* Do a dequeue, which leaves this as the first step:
     *
     *         4
     *        / \
     *       2   3
     *
     * This should then swap the 4 down to the left to give
     *
     *         2
     *        / \
     *       4   3
     */
     EXPECT_EQUAL(pq.dequeue(), v1);
     EXPECT_EQUAL(pq.size(), 3);
     EXPECT_EQUAL(pq.elems[1], v2);
     EXPECT_EQUAL(pq.elems[2], v4);
     EXPECT_EQUAL(pq.elems[3], v3);
}

PROVIDED_TEST("Can bubble down to the right.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Form this heap:
     *
     *         1
     *        / \
     *       3   2
     *      /
     *     4
     */
    DataPoint v1 = { "First",  1 };
    DataPoint v2 = { "Second", 2 };
    DataPoint v3 = { "Third",  3 };
    DataPoint v4 = { "Fourth", 4 };

    pq.enqueue(v1);
    pq.enqueue(v3);
    pq.enqueue(v2);
    pq.enqueue(v4);

    EXPECT_EQUAL(pq.size(), 4);
    EXPECT_EQUAL(pq.elems[1], v1);
    EXPECT_EQUAL(pq.elems[2], v3);
    EXPECT_EQUAL(pq.elems[3], v2);
    EXPECT_EQUAL(pq.elems[4], v4);

    /* Do a dequeue, which leaves this as the first step:
     *
     *         4
     *        / \
     *       3   2
     *
     * This should then swap the 4 down to the right to give
     *
     *         2
     *        / \
     *       3   4
     */
     EXPECT_EQUAL(pq.dequeue(), v1);
     EXPECT_EQUAL(pq.size(), 3);
     EXPECT_EQUAL(pq.elems[1], v2);
     EXPECT_EQUAL(pq.elems[2], v3);
     EXPECT_EQUAL(pq.elems[3], v4);
}

PROVIDED_TEST("Can bubble down two layers.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Form this heap:
     *
     *           1
     *        /     \
     *       2       3
     *      / \     /
     *     4   5   6
     */
    DataPoint v1 = { "First",  1 };
    DataPoint v2 = { "Second", 2 };
    DataPoint v3 = { "Third",  3 };
    DataPoint v4 = { "Fourth", 4 };
    DataPoint v5 = { "Fifth",  5 };
    DataPoint v6 = { "Sixth",  6 };

    pq.enqueue(v1);
    pq.enqueue(v2);
    pq.enqueue(v3);
    pq.enqueue(v4);
    pq.enqueue(v5);
    pq.enqueue(v6);

    EXPECT_EQUAL(pq.size(), 6);
    EXPECT_EQUAL(pq.elems[1], v1);
    EXPECT_EQUAL(pq.elems[2], v2);
    EXPECT_EQUAL(pq.elems[3], v3);
    EXPECT_EQUAL(pq.elems[4], v4);
    EXPECT_EQUAL(pq.elems[5], v5);
    EXPECT_EQUAL(pq.elems[6], v6);

    /* Do a dequeue, which leaves this as the first step:
     *
     *           6
     *        /     \
     *       2       3
     *      / \
     *     4   5
     *
     * This should then swap the 6 down to the left to give
     *
     *           2
     *        /     \
     *       6       3
     *      / \
     *     4   5
     *
     * This should swap the 6 down to the left again to give
     *
     *           2
     *        /     \
     *       4       3
     *      / \
     *     6   5
     */
     EXPECT_EQUAL(pq.dequeue(), v1);
     EXPECT_EQUAL(pq.size(), 5);
     EXPECT_EQUAL(pq.elems[1], v2);
     EXPECT_EQUAL(pq.elems[2], v4);
     EXPECT_EQUAL(pq.elems[3], v3);
     EXPECT_EQUAL(pq.elems[4], v6);
     EXPECT_EQUAL(pq.elems[5], v5);
}

PROVIDED_TEST("Can bubble down when only one child exists.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Form this heap:
     *
     *         1
     *        / \
     *       2   3
     *      / \
     *     4   5
     */
    DataPoint v1 = { "First",  1 };
    DataPoint v2 = { "Second", 2 };
    DataPoint v3 = { "Third",  3 };
    DataPoint v4 = { "Fourth", 4 };
    DataPoint v5 = { "Fifth",  5 };

    pq.enqueue(v1);
    pq.enqueue(v2);
    pq.enqueue(v3);
    pq.enqueue(v4);
    pq.enqueue(v5);

    EXPECT_EQUAL(pq.size(), 5);
    EXPECT_EQUAL(pq.elems[1], v1);
    EXPECT_EQUAL(pq.elems[2], v2);
    EXPECT_EQUAL(pq.elems[3], v3);
    EXPECT_EQUAL(pq.elems[4], v4);
    EXPECT_EQUAL(pq.elems[5], v5);

    /* Do a dequeue, which leaves this as the first step:
     *
     *         5
     *        / \
     *       2   3
     *      /
     *     4
     *
     * This should then swap the 5 down to the left to give
     *
     *         2
     *        / \
     *       5   3
     *      /
     *     4
     *
     * This should swap the 5 down to the left again to give
     *
     *         2
     *        / \
     *       4   3
     *      /
     *     5
     */
     EXPECT_EQUAL(pq.dequeue(), v1);
     EXPECT_EQUAL(pq.size(), 4);
     EXPECT_EQUAL(pq.elems[1], v2);
     EXPECT_EQUAL(pq.elems[2], v4);
     EXPECT_EQUAL(pq.elems[3], v3);
     EXPECT_EQUAL(pq.elems[4], v5);
}

PROVIDED_TEST("Enqueue / dequeue single element") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    DataPoint point = { "enqueue me!", 4 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Dequeue / peek on empty heap throws error") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Enqueue elements in sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "elem" + to_string(i), double(i) });
    }

    EXPECT_EQUAL(pq.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i), double(i) });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), double(i) });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Enqueue many elements in sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 10000; i++) {
        pq.enqueue({ "elem" + to_string(i), double(i) });
    }

    EXPECT_EQUAL(pq.size(), 10000);
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i), double(i) });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), double(i) });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Enqueue elements in reverse-sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 10; i >= 0; i--) {
        pq.enqueue({ "elem" + to_string(i), double(i) });
    }

    EXPECT_EQUAL(pq.size(), 11);
    for (int i = 0; i <= 10; i++) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i), double(i) });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), double(i) });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Enqueue many elements in reverse-sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 10000; i >= 0; i--) {
        pq.enqueue({ "elem" + to_string(i), double(i) });
    }

    EXPECT_EQUAL(pq.size(), 10001);
    for (int i = 0; i <= 10000; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            "elem" + to_string(i), double(i)
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert ascending and descending sequences.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + to_string(i), 2 * double(i) });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), 2 * double(i) + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        auto removed = pq.dequeue();
        EXPECT_EQUAL(removed.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert large ascending and descending sequences.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 20000; i++) {
        pq.enqueue({ "a" + to_string(i), 2 * double(i) });
    }
    for (int i = 19999; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), 2 * double(i) + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40000);
    for (int i = 0; i < 40000; i++) {
        auto removed = pq.dequeue();
        EXPECT_EQUAL(removed.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert random permutation.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        auto removed = pq.dequeue();
        DataPoint expected = {
            string(1, 'A' + i), double(i)
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert duplicate elements.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + to_string(i), double(i) });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), double(i) });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        auto one = pq.dequeue();
        auto two = pq.dequeue();

        EXPECT_EQUAL(one.weight, i);
        EXPECT_EQUAL(two.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Insert many duplicate elements.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 20000; i++) {
        pq.enqueue({ "a" + to_string(i), double(i) });
    }
    for (int i = 19999; i >= 0; i--) {
        pq.enqueue({ "b" + to_string(i), double(i) });
    }

    EXPECT_EQUAL(pq.size(), 40000);
    for (int i = 0; i < 20000; i++) {
        auto one = pq.dequeue();
        auto two = pq.dequeue();

        EXPECT_EQUAL(one.weight, i);
        EXPECT_EQUAL(two.weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Handles data points with empty string name.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , double(i) });
    }
    EXPECT_EQUAL(pq.size(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQUAL(pq.dequeue(), { "", double(i) });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());
}

PROVIDED_TEST("Handles many data points with empty string name.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = 0; i < 10000; i++) {
        pq.enqueue({ "" , double(i) });
    }
    EXPECT_EQUAL(pq.size(), 10000);
    for (int i = 0; i < 10000; i++) {
        EXPECT_EQUAL(pq.dequeue(), { "", double(i) });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());
}

PROVIDED_TEST("Handles data points with negative weights.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , double(i) });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
}

PROVIDED_TEST("Handles many data points with negative weights.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    for (int i = -10000; i < 10000; i++) {
        pq.enqueue({ "" , double(i) });
    }
    EXPECT_EQUAL(pq.size(), 20000);
    for (int i = -10000; i < 10000; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
}

PROVIDED_TEST("Interleave enqueues and dequeues.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", double(i)});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", double(i)});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Interleave many enqueues and dequeues.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    int n = 10000;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", double(i)});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", double(i)});
    }
    EXPECT_EQUAL(pq.size(), n / 2);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().weight, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}


PROVIDED_TEST("Enqueue fractional values in sorted order.") {
    HeapPQueue pq;
    EXPECT_NOT_EQUAL(pq.elems, nullptr);

    /* Add 1/10, 1/9, 1/8, ..., and 1/1 to the queue. This is in sorted
     * order.
     */
    for (int i = 10; i >= 1; i--) {
        pq.enqueue({ "elem" + to_string(i), 1.0 / i });
    }

    /* We should get these values back in the same order that we inserted
     * them. If we don't, it could mean that the enqueue/dequeue logic contains
     * a small bug, or it could mean that the logic is correct, but one of the
     * weights (doubles) was stored as an int at some point in a way that caused
     * the weight to be rounded down.
     */
    EXPECT_EQUAL(pq.size(), 10);
    for (int i = 10; i >= 1; i--) {
        EXPECT_EQUAL(pq.peek(), { "elem" + to_string(i),    1.0 / i });
        EXPECT_EQUAL(pq.dequeue(), { "elem" + to_string(i), 1.0 / i });
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Stress test: cycle 250,000 elems") {
    /* You must complete this test in 5.5 seconds. Why that number? We took
     * our (unoptimized) reference solution, ran this test on a middle-of-the-line
     * computer, and multiplied the runtime by 10 so you have a comfortable
     * margin of safety.
     */
    EXPECT_COMPLETES_IN(5.5,
        HeapPQueue pq;
        EXPECT_NOT_EQUAL(pq.elems, nullptr);

        const int n = 250000;
        for (int i = 0; i < n; i++) {
            pq.enqueue({ "", randomReal(0, 100000) });
        }
        EXPECT_EQUAL(pq.size(), n);

        for (int i = 0; i < n; i++) {
            pq.dequeue();
        }
        EXPECT_EQUAL(pq.size(), 0);
        EXPECT_EQUAL(pq.isEmpty(), true);

        for (int i = 0; i < n; i++) {
            pq.enqueue({ "", randomReal(0, 100000) });
        }
        EXPECT_EQUAL(pq.size(), n);
    );
}
