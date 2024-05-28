#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

// Definicion de estructuras y funciones auxiliares para que los estados no tengan un ciclo SIIIIN FIIIIIIN :D

// Estructura del set para almacenar estados visitados, y que no siga en un cliclo SIIIIIIN FIIIIIIN.              No me resulto D;
// Definición de la estructura de un nodo en el conjunto
typedef struct SetNode {
    char key[10];           // Clave del elemento
    struct SetNode* next;   // Puntero al siguiente nodo en caso de colisión
} SetNode;

// Definición de la estructura del conjunto
typedef struct {
    SetNode** table;    // Tabla de dispersión para almacenar los elementos del conjunto
    int size;           // Tamaño actual de la tabla
    int count;          // Número de elementos en el conjunto
} Set;

// Función de dispersión para calcular el índice en la tabla hash
unsigned int hash_function(const char* str, int size) {
    unsigned int hash = 0;
    for (int i = 0; i < strlen(str); i++) {
        hash = 31 * hash + str[i]; // Aplicar la función de dispersión simple
    }
    return hash % size; // Obtener el índice dentro del tamaño de la tabla
}

// Crear un conjunto nuevo con la capacidad inicial especificada
Set* set_create(int size) {
    Set* set = (Set*)malloc(sizeof(Set)); // Asignar memoria para la estructura del conjunto
    set->table = (SetNode**)calloc(size, sizeof(SetNode*)); // Inicializar la tabla con punteros nulos
    set->size = size; // Establecer el tamaño inicial
    set->count = 0; // Inicializar el conteo de elementos
    return set;
}

// Redimensionar la tabla hash del conjunto
void set_resize(Set* set, int new_size);

// Insertar un elemento en el conjunto
bool set_insert(Set* set, const char* key) {
    if ((float)set->count / set->size > 0.75) { // Verificar si la carga del conjunto es demasiado alta
        set_resize(set, set->size * 2); // Redimensionar la tabla si es necesario
    }

    unsigned int hash = hash_function(key, set->size); // Calcular el índice utilizando la función de dispersión
    SetNode* new_node = (SetNode*)malloc(sizeof(SetNode)); // Crear un nuevo nodo para el elemento
    strcpy(new_node->key, key); // Copiar la clave en el nodo
    new_node->next = set->table[hash]; // Insertar el nodo al principio de la lista enlazada
    set->table[hash] = new_node; // Actualizar la tabla con el nuevo nodo
    set->count++; // Incrementar el conteo de elementos
    return true; // La inserción se realizó con éxito
}

// Verificar si un elemento está presente en el conjunto
bool set_contains(Set* set, const char* key) {
    unsigned int hash = hash_function(key, set->size); // Calcular el índice utilizando la función de dispersión
    SetNode* node = set->table[hash]; // Obtener el primer nodo en la lista enlazada correspondiente
    while (node != NULL) { // Recorrer la lista enlazada
        if (strcmp(node->key, key) == 0) { // Comparar las claves
            return true; // Se encontró el elemento
        }
        node = node->next; // Avanzar al siguiente nodo en caso de colisión
    }
    return false; // El elemento no se encontró
}

// Redimensionar la tabla hash del conjunto
void set_resize(Set* set, int new_size) {
    SetNode** old_table = set->table; // Mantener una referencia a la tabla original
    int old_size = set->size; // Mantener el tamaño original

    set->table = (SetNode**)calloc(new_size, sizeof(SetNode*)); // Crear una nueva tabla con el nuevo tamaño
    set->size = new_size; // Actualizar el tamaño del conjunto
    set->count = 0; // Reiniciar el conteo de elementos

    // Reinsertar todos los elementos en la nueva tabla
    for (int i = 0; i < old_size; i++) {
        SetNode* node = old_table[i]; // Obtener el primer nodo en la lista enlazada
        while (node != NULL) { // Recorrer la lista enlazada
            set_insert(set, node->key); // Insertar el elemento en la nueva tabla
            SetNode* temp = node;
            node = node->next;
            free(temp); // Liberar la memoria del nodo antiguo
        }
    }
    free(old_table); // Liberar la memoria de la tabla antigua
}

// Liberar la memoria asignada al conjunto y sus elementos
void set_destroy(Set* set) {
    for (int i = 0; i < set->size; i++) { // Recorrer cada elemento de la tabla
        SetNode* node = set->table[i]; // Obtener el primer nodo en la lista enlazada
        while (node != NULL) { // Recorrer la lista enlazada
            SetNode* temp = node;
            node = node->next;
            free(temp); // Liberar la memoria del nodo
        }
    }
    free(set->table); // Liberar la memoria de la tabla
    free(set); // Liberar la memoria del conjunto
}

// Función para convertir el estado a una cadena única
void state_to_string(State* state, char* str) 
{
    int pos = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            pos += sprintf(&str[pos], "%d", state->square[i][j]);
        }
    }
}
//============================************************=====================================

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
void imprimirEstado(const State *estado) 
{
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
int is_final_state(State* state) 
{
    int final_state[3][3] = {  //como deberia verse el puzzle al final
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}   
    };
    
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (state->square[i][j] != final_state[i][j])  //si no es el mismo valor en la misma posicion
                return 0;
    return 1;
}


// Funcion para copiar el estado, y no interferir con el otro etado.
State* copy(State* state) 
{
    State* new_state = (State*)malloc(sizeof(State));
    
    for (int i = 0; i < 3; i++)   //copiar el estado
    {
        for (int j = 0; j < 3; j++) 
        {
            new_state->square[i][j] = state->square[i][j];
        }
    }
    new_state-> x = state->x;  //copiar las coordenadas
    new_state-> y = state->y;
    new_state-> actions = list_create();

    // Copiar la lista de acciones
    void* action;
    for (action = list_first(state->actions); action != NULL; action = list_next(state->actions)) 
    {
        int* new_action = (int*) malloc(sizeof(int));
        *new_action = *(int*)action;
        list_pushBack(new_state->actions, new_action); //agrega a la accion, a las lista del nuevo estado
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
        list_pushBack(new_state->actions, &action);   //agregar a la lista

        printf("\nTransición (%d):\n", action);
        imprimirEstado(new_state);
        return new_state;
    }
    return NULL;             //el estado no es valido
}


// Funcion para obtener los nodos adyacente al estado actual
List* get_adyacent_node(State* state, Set* visited_states) 
{
    List* adyacent_node = list_create();    //crear lista

    for (int i = 1; i <= 4; i++)            //las cuatro opciones de movimiento
    {
        State* new_state = transition(state, i);         //hacemos la trancision

        if (new_state != NULL) 
        {
            char new_state_key[10];
            state_to_string(new_state, new_state_key);

            if (!set_contains(visited_states, new_state_key)) //mientras no este el nodo
            {                                 
                //mientras el estado sea valido y no esté en el conjunto de estados visitados
                Node* new_node = (Node*) malloc(sizeof(Node));         //creamos el nuevo nodo
                new_node->state = *new_state;                        //asignamos los datos
                new_node->coste = distancia_L1(new_state);
                list_pushBack(adyacent_node, new_node);                //agregamos el nuevo nodo(estado) a la lista
                set_insert(visited_states, new_state_key);           //añadimos el estado al conjunto de estados visitados
            }
            free(new_state);
        }
    }
    return adyacent_node;            //retornamos la lista de nodos adyacentes
}


// Implementación de DFS, búsqueda en profundidad
State* DFS(State* initial, int* cont, Set* visited_states) 
{
    Stack *S = stack_create();       //crear pila
    stack_push(S, initial);               //apilar estado inicial
    
    while (stack_top(S) != NULL) 
    {
        State *aux = stack_top(S);                         //obtener el primero
        stack_pop(S);                                      //desapilar

        //printf("\nDFS Iteración %d - Estado actual:\n", *cont);
        //imprimirEstado(aux);
        
        if (list_size(aux -> actions) >= 15 )           //un maximo de nodos, para que no se vuelva infinito
        {
            continue;
        }
        if (is_final_state(aux))                    //verificamos si es el estado final
        {
            stack_clean(S);
            free(S);
            return aux;
        }
        
        List *adj_nodes = get_adyacent_node(aux, visited_states);  //obtener los nodos adyacentes
        Node *list_index = list_first(adj_nodes);   //primer nodo
        while (list_index != NULL) 
        {
            stack_push(S, &list_index -> state);    //agregaos a la pila
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
State* BFS(State* initial, int* cont, Set* visited_states) 
{
    Queue *queue = queue_create();  //crear la cola
    queue_insert(queue, initial);  //insertar

    while (queue_front(queue) != NULL)  //validar
    {
        State *aux_queue = queue_front(queue);  //obtener el primer nodo(estado)
        queue_remove(queue);                          //lo sacamos de la cola   

        //printf("\nBFS Iteración %d - Estado actual:\n", *cont);
        //imprimirEstado(aux_queue);
        
        if (is_final_state(aux_queue))              //vemos si es el estado final
        {
            queue_clean(queue);
            free(queue);
            return aux_queue;
        }

        List *adj_nodes = get_adyacent_node(aux_queue, visited_states);   //los nodos adyacentes
        Node *list_index = list_first(adj_nodes);      //el primer nodo
        while (list_index != NULL) 
        {
            queue_insert(queue, &list_index -> state);   //lo insertamos en la cola
            list_index = list_next(adj_nodes);           //avanzamos
        }
        list_clean(adj_nodes);
        free(adj_nodes);
        *cont = *cont + 1;
    }
    queue_clean(queue);
    free(queue);
    return NULL;
}

// Función para imprimir todas las transiciones desde el estado inicial al final
void imprimir_todas_transiciones(State *inicial, State *result) 
{
    List *list = result -> actions;  //obtener lista de resultados de acciones
    int *aux = list_first(list);     //primera accion
    
    printf("\nEstado Inicial :\n");     //Mostrar
    imprimirEstado(inicial);  
    
    State *state = copy(inicial);      
    int cont = 2;                      //contador del numero del estado
    while (aux != NULL) 
    {
        printf("Estado n° %d del puzzle:\n", cont);
        State *new_node = transition(state, *aux);    //obtener el nuevo estado
        imprimirEstado(new_node);
        state = new_node;                          //actualizar el nuevo estado
        aux = list_next(list);                     //pasar al siguiente
        cont++;
    }
    free(list);
    free(state);
}


int main() 
{
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}}, // Tercera fila
        0, 1 // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();;;;;;;;;

    // Conjunto para almacenar estados visitados
     Set* visited_DFS = set_create(1000);
     Set* visited_BFS = set_create(1000);

    // Insertar el estado inicial en el conjunto de estados visitados
    char initial_state_key[10];
    state_to_string(&estado_inicial, initial_state_key);
    set_insert(visited_DFS, initial_state_key);
    set_insert(visited_BFS, initial_state_key);

    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("\nDistancia L1:%d\n", distancia_L1(&estado_inicial));

    int cont = 0;    //cant.estados
    int cont2 = 0;
    State *result;
    State *result2;
    int opcion;

    do {
        printf("\n********* MENU *********\n");
        printf("1) Búsqueda en Profundidad\n");
        printf("2) Búsqueda en Anchura\n");
        printf("3) Salir\n");
        printf("\nIngrese su opción: ");
        scanf ("%d", &opcion);

        switch (opcion) 
        {
            case 1:
            {
                printf("\n***.~~~¨¨--°°°°°°--¨¨~~~.***\n");  //mostrar
                printf("    Búsqueda en Profundidad \n");
                printf("***.~~~¨¨--°°°°°°--¨¨~~~.***\n");

                result = DFS(&estado_inicial, &cont, visited_DFS);
                imprimir_todas_transiciones(&estado_inicial, result);
                printf("Número de iteraciones para llegar a la solución: %d \n\n", cont);

                set_destroy(visited_DFS);
                visited_DFS = set_create(1000);
                cont = 0;   //de esta manera se suponia no iba a ir al infinito
                break;
            }
            case 2:
            {
                printf("\n***.~~~¨¨--°°°°°°--¨¨~~~.***\n");   //mostrar
                printf("    Búsqueda en Anchura     \n");
                printf("***.~~~¨¨--°°°°°°--¨¨~~~.***\n");

                result2 = BFS(&estado_inicial, &cont2, visited_BFS);
                imprimir_todas_transiciones(&estado_inicial, result2);
                printf("Número de iteraciones para llegar a la solución: %d \n\n", cont2); 
                set_destroy(visited_BFS);
                visited_BFS = set_create(1000);
                cont2 = 0;                           //lo mismo
                break;
            }
            case 3:
            {    
                printf("\n**El programa a finalizado.**\n");
                break;
            }
            default:
            {
                printf("~~Opción no válida, por favor intente de nuevo.~~\n");
                break;
            }
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

    } while (opcion != 3);

    // Liberar memoria
    set_destroy(visited_DFS);
    set_destroy(visited_BFS);

    return 0;
}