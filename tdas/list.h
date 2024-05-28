#include <stdlib.h>
#include <stdio.h>

#ifndef LIST_H
#define LIST_H

typedef struct List List;
typedef List Stack;
typedef List Queue;

List *list_create();
void *list_first(List *L);
void *list_next(List *L);
void list_pushFront(List *L, void *dato);
void list_pushBack(List *L, void *dato);
void list_pushCurrent(List *L, void *dato);
void *list_popFront(List *L);
void *list_popBack(List *L);
void *list_popCurrent(List *L);
void list_clean(List *L);
void list_sortedInsert(List *L, void *data,
                       int (*lower_than)(void *data1, void *data2));
int list_size(List* L);
void list_destroy(List* list, void (*freeData)(void*));

//Funciones Stack
Stack *stack_create();
void stack_push(Stack *stack, void *data);
void *stack_top(Stack *stack);
void *stack_pop(Stack *stack); 
void stack_clean(Stack *stack);
int stack_size(Stack *stack);

//funciones Cola
Queue *queue_create();
void queue_insert(Queue *queue, void *data);
void *queue_remove(Queue *queue); 
void *queue_front(Queue *queue);
void queue_clean(Queue *queue); 
int queue_size(Queue *queue);

#endif