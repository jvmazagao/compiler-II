#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    char  *data;
    struct node *next;
} node;

typedef struct LinkedList {
    node *head;
    node *current;
} LinkedList;

LinkedList* create() {
    LinkedList* list = malloc(sizeof list);
    list->head = malloc(sizeof *list->head);
    list->current = malloc(sizeof *list->current);
    return list;
}

LinkedList* reset(LinkedList *list) {
    free(list);
    return create();
}

void printList(LinkedList* list) {
    struct node *ptr = list->head;
    printf("\n[ ");

    while(ptr != NULL) {
        printf("%s -> ",ptr->data);
        ptr = ptr->next;
    }

    printf(" ]\n");
}

void insertFirst(LinkedList *list, char *data) {
    node *link = (struct node*) malloc(sizeof(struct node));
    link->data = data;
    link->next = list->head;
    list->head = link;
}

void insert(LinkedList *list, char *data) {
    node* current = list->head;
    if(current == NULL) {
        current->data = data;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = (node *) malloc(sizeof(node *));
        current->next->data = data;
        current->next->next = NULL;
    }
}

struct node* deleteFirst(LinkedList* list) {

    struct node *tempLink = list->head;

    list->head = list->head->next;

    return tempLink;
}

bool isEmpty(LinkedList *list) {
    return list->head == NULL;
}