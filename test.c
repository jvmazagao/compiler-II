#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    LinkedList *list = create();
    insertFirst(list, "Test");
    insertFirst(list, "Test 2");
    printList(list);
    reset(list);
    printList(list);
}