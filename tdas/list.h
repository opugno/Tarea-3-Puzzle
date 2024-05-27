#ifndef LIST_H
#define LIST_H

typedef struct List List;
typedef List Stack;
typedef List Queue;

// Esta función crea una lista vacía y devuelve un puntero a la lista.
List *list_create();

// Esta función devuelve un puntero al primer elemento de la lista.
void *list_first(List *L);

// Esta función mueve el puntero de la lista al siguiente elemento y devuelve un
// puntero a dicho elemento.
void *list_next(List *L);

// Esta función inserta un nuevo elemento al inicio de la lista.
void list_pushFront(List *L, void *dato);

// Esta función inserta un nuevo elemento al final de la lista.
void list_pushBack(List *L, void *dato);

// Esta función inserta un nuevo elemento a continuación del actual de la lista.
void list_pushCurrent(List *L, void *dato);

// Esta función elimina el primer elemento de la lista.
void *list_popFront(List *L);

// Esta función elimina el último elemento de la lista.
void *list_popBack(List *L);

// Esta función elimina el elemento actual de la lista.
void *list_popCurrent(List *L);

// Esta función elimina todos los elementos de la lista.
void list_clean(List *L);

// Función para insertar ordenado de acuerdo a la función lower_than
void list_sortedInsert(List *L, void *data,
                       int (*lower_than)(void *data1, void *data2));

int list_size(List* L);

//Funciones Stack
Stack *stack_create();

void stack_push(Stack *stack, void *data) { list_pushFront(stack, data); }

void *stack_top(Stack *stack) { return list_first(stack); }

void *stack_pop(Stack *stack) { return list_popFront(stack); }

void stack_clean(Stack *stack) { list_clean(stack); }

//funciones Cola
Queue *queue_create(Queue *queue) { return list_create(); }

void queue_insert(Queue *queue, void *data) { list_pushBack(queue, data); }

void *queue_remove(Queue *queue) { return list_popFront(queue); }

void *queue_front(Queue *queue) { return list_first(queue); }

void queue_clean(Queue *queue) { list_clean(queue); }

#endif