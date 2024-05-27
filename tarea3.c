#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/extra.h"
#include <string.h>

// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

// Definicion de estructura del nodo
typedef struct {
    State state;  // Estado del puzzle
    int coste;    // Coste del estado    
} Node;

// Función para calcular la distancia L1 (Manhattan) del estado
int distancia_L1(State* state) {
    int ev = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int val = state->square[i][j];
            if (val == 0) continue;
            int ii = (val - 1) / 3;
            int jj = (val - 1) % 3;
            ev += abs(ii - i) + abs(jj - j);
        }
    return ev;
}

// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}

// Verificar si es el estado final (ordenado de menor a mayor 1-8)
int is_final_state(State* state) {
    int final_state[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}   
    };
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (state->square[i][j] != final_state[i][j])
                return 0;
    return 1;
}

// Copiar el estado
State* copy(State* state) 
{
    State *new_state = (State*) malloc(sizeof(State));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            new_state->square[i][j] = state->square[i][j];
        }
    }

    new_state -> x = state -> x;
    new_state -> y = state -> y;
    new_state -> actions = list_create();
    List* aux = list_first(state -> actions);
    while (aux != NULL) 
    {
        list_pushBack(new_state -> actions, aux);
        aux = list_next(state -> actions);
    }
    return new_state;
}

// Función para obtener los nuevos estados posibles a partir del estado actual.
State* transition(State* state, int action) 
{
    int dx[] = {0, 0, -1, 1};  // Desplazamientos posibles en filas, no se mueve hacia arriba ni abajo (0,0)
    int dy[] = {-1, 1, 0, 0};  // Desplazamientos posibles en columnas, no se mueve hacia los lados (0,0)
    int new_x = state -> x + dx[action - 1];          //ver hacia donde se mueve
    int new_y = state -> y + dy[action - 1];

    if ((new_x >= 0 && new_x < 3) && (new_y >= 0 && new_y < 3))  //y si es valido moverse hacia ese lugar
    {
        State* new_state = copy(state);              //copiamos el nodo
        new_state -> square[state -> x][state -> y] = new_state -> square[new_x][new_y];
        new_state -> square[new_x][new_y] = 0;               //reorganizamos el tablero con el nuevo estado
        new_state -> x = new_x;
        new_state -> y = new_y;
        return new_state;
    }
    return NULL;             //el estado no es valido
}

// Obtener los nodos adyacentes del estado actual
List* get_adyacent_node(State* state) 
{
    List* adyacent_node = list_create();    //crear lista

    for (int i = 1; i <= 4; i++)            //las cuatro opciones de movimiento
    {
        State* new_state = transition(state, i);         //hacemos la trancision
        if (new_state != NULL) 
        {                                 //mientras el estado sea valido
            Node* new_node = (Node*) malloc(sizeof(Node));         //creamos el nuevo nodo
            new_node -> state = *new_state;                        //asignamos los datos
            new_node -> coste = distancia_L1(new_state);
            list_pushBack(adyacent_node, new_node);                //agregamos el nuevo nodo(estado) a la lista
            free(new_state);
        }
    }
    return adyacent_node;            //retornamos la lista de nodos adyacentes
}

// Implementación de DFS, búsqueda en profundidad
State* DFS(State* initial, int* cont) 
{
    Stack *S = stack_create();       //crear pila

    stack_push(S, initial);               //apilar estado inicial
    while (stack_top(S) != NULL) 
    {
        State *aux = stack_top(S);                         //obtener el primero
        stack_pop(S);                                      //desapilar
        if (list_size(aux -> actions) >= 10)           //un maximo de nodos, para que no se vuelva infinito
        {
            continue;
        }
        if (is_final_state(aux))                    //verificamos si es el estado final
        {
            free(S);
            return aux;
        }
        List *adj_nodes = get_adyacent_node(aux);   //si no agregamos el nuevo nodo a la lista de estados
        Node *list_index = list_first(adj_nodes);
        while (list_index != NULL) 
        {
            stack_push(S, &list_index -> state);
            list_index = list_next(adj_nodes);
        }
        list_clean(adj_nodes);
        free(adj_nodes);
        *cont = *cont + 1;
    }
    free(S);
    return NULL;
}

// Implementación de BFS, búsqueda en anchura
State* BFS(State* initial, int* cont) 
{
    Queue *queue = queue_create();  //crear la cola
    queue_insert(queue, initial);  //insertar

    while (queue_front(queue) != NULL)  //validar
    {
        State *aux_queue = queue_front(queue);
        queue_remove(queue);
        if (is_final_state(aux_queue))              //vemos si es el estado final
        {
            free(queue);
            return aux_queue;
        }

        List *adj_nodes = get_adyacent_node(aux_queue); //si no, agregamos el nodo a la lista de estados
        Node *list_index = list_first(adj_nodes);
        while (list_index != NULL) 
        {
            queue_insert(queue, &list_index -> state);
            list_index = list_next(adj_nodes);
        }
        list_clean(adj_nodes);
        free(adj_nodes);
        *cont = *cont + 1;
    }
    free(queue);
    return NULL;
}

// Función para imprimir todas las transiciones desde el estado inicial al final
void imprimir_todas_transiciones(State *inicial, State *result) 
{
    List *list = result -> actions; 
    int *aux = list_first(list);
    printf("\nEstado Inicial :\n");
    imprimirEstado(inicial);
    State *state = copy(inicial);
    int cont = 2;
    while (aux != NULL) 
    {
        printf("Estado n° %d del puzzle:\n", cont);
        State *new_node = transition(state, *aux);
        imprimirEstado(new_node);
        state = new_node;
        aux = list_next(list);
        cont++;
    }
    free(list);
    free(state);
}

int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}}, // Tercera fila
        0, 1 // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);
    printf("holaaaaaas");

    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));

    int cont = 0;
    int cont2 = 0;
    State *result;
    State *result2;
    int opcion;

    do {
        printf("\n***** EJEMPLO MENU ******\n");
        printf("1) Búsqueda en Profundidad\n");
        printf("2) Búsqueda en Anchura\n");
        printf("3) Salir\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("\n==========================\n");
                printf(" Búsqueda en Profundidad:\n");
                printf("==========================\n");
                result = DFS(&estado_inicial, &cont);
                imprimir_todas_transiciones(&estado_inicial, result);
                printf("Número de iteraciones para llegar a la solución: %d \n\n", cont);
                cont = 0;
                break;
            case 2:
                printf("\n==========================\n");
                printf(" Búsqueda en Anchura:\n");
                printf("==========================\n");
                result2 = BFS(&estado_inicial, &cont2);
                imprimir_todas_transiciones(&estado_inicial, result2);
                printf("Número de iteraciones para llegar a la solución: %d \n\n", cont2);
                cont2 = 0;
                break;
            case 3:
                printf("\nEl programa a finalizado.\n");
                break;
            default:
                printf("Opción no válida, por favor intente de nuevo.\n");
                break;
        }
    } while (opcion != 3);

    return 0;
}
