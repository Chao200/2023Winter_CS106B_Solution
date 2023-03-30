#include "GUI/MemoryDiagnostics.h"
#include <string>
using namespace std;

/* Type representing a doubly-linked list of strings. */
struct Cell {
    string value;
    Cell* next;
    Cell* prev;

    TRACK_ALLOCATIONS_OF(Cell); // Helps SimpleTest find memory leaks
};

/* Computes the length of a doubly-linked list of cells. This function must
 * be implemented iteratively, rather than recursively.
 */
int lengthOf(Cell* list) {
    /* TODO: Delete this comment and the next two lines, then implement this
     * function.
     */
//    (void) list;
//    return -1;
    int count = 0;
    while (list) {
        list = list->next;
        ++count;
    }
    return count;
}

/* Frees all memory used by the given doubly-linked list. This function must
 * be implemented iteratively, rather than recursively.
 */
void freeDoublyLinkedList(Cell* list) {
    /* TODO: Delete this comment and the next line, then implement this
     * function.
     */
//    (void) list;
    Cell *del;
    while (list) {
        del = list;
        list = list->next;
        delete del;
    }
}

/* Adds the given value to the front of the specified doubly-linked list. */
void prependTo(Cell*& list, const string& value) {
    /* TODO: Delete this comment and the next two lines, then implement this
     * function.
     */
//    (void) list;
//    (void) value;
    Cell *temp = new Cell;
    temp->value = value;
    if (list) {
        list->prev = temp;
        temp->next = list;
        temp->prev = nullptr;
        list = temp;
    } else {
        list = temp;
    }
}

/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"

/* Space for your own test cases, if you want to add them. */









/* * * * * Provided Tests Below This Point * * * * */

/* Tests for lengthOf */

PROVIDED_TEST("lengthOf: Empty list.") {
    EXPECT_EQUAL(lengthOf(nullptr), 0);
}

PROVIDED_TEST("lengthOf: List of length 1.") {
    /* Create a one-element list. */
    Cell* list = new Cell;
    list->next = list->prev = nullptr; // Cell has nothing before or after it.

    EXPECT_EQUAL(lengthOf(list), 1);

    /* Free memory for the list. */
    delete list;
}

PROVIDED_TEST("lengthOf: List of length 2.") {
    /* Create two cells and wire them together. */
    Cell* one = new Cell;
    Cell* two = new Cell;
    one->next = two;
    two->prev = one;
    one->prev = two->next = nullptr;

    EXPECT_EQUAL(lengthOf(one), 2);

    delete one;
    delete two;
}

PROVIDED_TEST("lengthOf: List of length 3.") {
    /* Create three cells and wire them together. */
    Cell* a = new Cell;
    Cell* b = new Cell;
    Cell* c = new Cell;
    a->next = b;
    b->next = c;
    c->next = nullptr;
    a->prev = nullptr;
    b->prev = a;
    c->prev = b;

    EXPECT_EQUAL(lengthOf(a), 3);

    delete a;
    delete b;
    delete c;
}

/* Tests for freeDoublyLinkedList */

PROVIDED_TEST("freeDoublyLinkedList: Empty list.") {
    freeDoublyLinkedList(nullptr); // Should do nothing
}

PROVIDED_TEST("freeDoublyLinkedList: List of length 1.") {
    /* Create a one-element list. */
    Cell* list = new Cell;
    list->next = list->prev = nullptr; // Cell has nothing before or after it.
    freeDoublyLinkedList(list);
}

PROVIDED_TEST("freeDoublyLinkedList: List of length 2.") {
    /* Create two cells and wire them together. */
    Cell* one = new Cell;
    Cell* two = new Cell;
    one->next = two;
    two->prev = one;
    one->prev = two->next = nullptr;

    freeDoublyLinkedList(one);
}

PROVIDED_TEST("freeDoublyLinkedList: List of length 3.") {
    /* Create three cells and wire them together. */
    Cell* a = new Cell;
    Cell* b = new Cell;
    Cell* c = new Cell;
    a->next = b;
    b->next = c;
    c->next = nullptr;
    a->prev = nullptr;
    b->prev = a;
    c->prev = b;

    freeDoublyLinkedList(a);
}

/* Tests for prependTo */

PROVIDED_TEST("prependTo: Prepend to empty list.") {
    Cell* list = nullptr;
    prependTo(list, "A");

    EXPECT_NOT_EQUAL(list, nullptr);
    EXPECT_EQUAL(list->next, nullptr);
    EXPECT_EQUAL(list->prev, nullptr);
    EXPECT_EQUAL(list->value, "A");

    freeDoublyLinkedList(list);
}

PROVIDED_TEST("prependTo: Prepend to length-1 list.") {
    /* Create a one-element list. */
    Cell* list = new Cell;
    list->next = list->prev = nullptr; // Cell has nothing before or after it.
    list->value = "B";

    prependTo(list, "A");

    EXPECT_NOT_EQUAL(list, nullptr);
    EXPECT_NOT_EQUAL(list->next, nullptr);
    EXPECT_EQUAL(list->prev, nullptr);
    EXPECT_EQUAL(list->value, "A");
    EXPECT_EQUAL(list->next->prev, list);
    EXPECT_EQUAL(list->next->next, nullptr);
    EXPECT_EQUAL(list->next->value, "B");

    freeDoublyLinkedList(list);
}

PROVIDED_TEST("prependTo: Prepend to length-2 list.") {
    /* Create two cells and wire them together. */
    Cell* one = new Cell;
    Cell* two = new Cell;
    one->next = two;
    two->prev = one;
    one->prev = two->next = nullptr;
    one->value = "B";
    two->value = "C";

    prependTo(one, "A");

    EXPECT_NOT_EQUAL(one, nullptr);
    EXPECT_NOT_EQUAL(one->next, nullptr);
    EXPECT_EQUAL(one->prev, nullptr);
    EXPECT_EQUAL(one->value, "A");
    EXPECT_EQUAL(one->next->prev, one);
    EXPECT_NOT_EQUAL(one->next->next, nullptr);
    EXPECT_EQUAL(one->next->value, "B");
    EXPECT_EQUAL(one->next->next->prev, one->next);
    EXPECT_EQUAL(one->next->next->next, nullptr);
    EXPECT_EQUAL(one->next->next->value, "C");

    freeDoublyLinkedList(one);
}

PROVIDED_TEST("prependTo: Prepend to length-3 list.") {
    /* Create three cells and wire them together. */
    Cell* a = new Cell;
    Cell* b = new Cell;
    Cell* c = new Cell;
    a->next = b;
    b->next = c;
    c->next = nullptr;
    a->prev = nullptr;
    b->prev = a;
    c->prev = b;
    a->value = "B";
    b->value = "C";
    c->value = "D";

    prependTo(a, "A");

    EXPECT_NOT_EQUAL(a, nullptr);
    EXPECT_NOT_EQUAL(a->next, nullptr);
    EXPECT_EQUAL(a->prev, nullptr);
    EXPECT_EQUAL(a->value, "A");
    EXPECT_EQUAL(a->next->prev, a);
    EXPECT_NOT_EQUAL(a->next->next, nullptr);
    EXPECT_EQUAL(a->next->value, "B");
    EXPECT_EQUAL(a->next->next->prev, a->next);
    EXPECT_NOT_EQUAL(a->next->next->next, nullptr);
    EXPECT_EQUAL(a->next->next->value, "C");
    EXPECT_EQUAL(a->next->next->next->prev, a->next->next);
    EXPECT_EQUAL(a->next->next->next->next, nullptr);
    EXPECT_EQUAL(a->next->next->next->value, "D");

    freeDoublyLinkedList(a);
}
