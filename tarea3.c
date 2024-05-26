#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
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
typedef struct
{
    State state;  //estado del puzzle
    int coste;    //coste del estado    
} Node;


int distancia_L1(State* state) {  //la ocuparemos para calcular el coste
    int ev=0;
    int k=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            int val=state->square[i][j];
            if (val==0) continue;
            int ii=(val)/3;
            int jj=(val)%3;
            ev+= abs(ii-i) + abs(jj-j);
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


// Ver si es el estado final, ordenado de menor a mayor 1-8 
int is_final_state(State* state)
{
    int final_state[3][3] = {  //asi deberia verse la solucion final
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}   
    };

    for (int i = 0; i < 3; i++)   //recorrer el puzzle (COMO MATRIZ)
    {
        for (int j = 0; j < 3; j++)
        {
            if (state -> square[i][j] == final_state[i][j]) return 1;  //comparar el cuadrado con la final
        }
    }
    return 0;
}


// Copiar el nodo
State* copy(State* state)
{
    if (state == NULL) return NULL;

    State* new_node = (State*) malloc(sizeof(State));
    if (new_node == NULL) return NULL;

    *new_node = *state;
    return new_node;
}


// Funcion para obtener los nuevos estados posibles a partir del estado actual.
State* transition(State* state, int action)
{
    int dx[] = {0, 0, -1, 1};     //filas -> se puede mover a la derecha(+1) o izquierda(-1)
    int dy[] = {-1, 1, 0, 0};     //columnas -> se puede mover hacia arriba(-1) o hacia abajo(1)
    int new_x = state -> x + dx[action -1];   //obtener para donde se tiene que mover.
    int new_y = state -> y + dy[action -1];

    if ((new_x >= 0 && new_x < 3) && (new_y >= 0 && new_y < 3)) //si la posición es valida.
    {
        State* new_state = copy(state);
                                                               //intercambio de posiciones
        new_state -> square[state -> x][state -> y] = new_state -> square[new_x][new_y]; //intercambio de los datos espacio vacio con el espacio seleccionado
        new_state -> square[new_x][new_y] = 0;                         //al espacio seleccionado, se le asigna 0
        new_state -> x = new_x;                                        //actualizar coordenadas.
        new_state -> y = new_y;

        return new_state;
    }
    
    return NULL;
}


// Obtener los nodos adyacentes del estado actual
List* get_adyacent_node(State* state)
{
    List* adyacent_node = list_create();  //crear lista de nodos adyacentes
    for (int i = 1; i <= 4; i++)
    {
        State* new_state = transition(state, i); //llamada a funcion para obtener el nuevo estado
        if (new_state != NULL)                      //ver que el estado sea valido
        {
            Node* newNode = (Node*) malloc(sizeof(Node));  //crear un nodo
            newNode -> state = *new_state;                 //asignar el nuevo estado a un nodo.
            newNode -> coste = distancia_L1(new_state);    //asignar el coste del nodo.
            list_pushBack(adyacent_node, newNode);

            free(new_state);
        }
    }

    return adyacent_node;
}


int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}, // Tercera fila
         },  
        0, 1   // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));

    //Ejemplo de heap (cola con prioridad)
    printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
    Heap* heap = heap_create();
    char* data = strdup("Cinco");
    printf("Insertamos el elemento %s con prioridad -5\n", data);
    heap_push(heap, data, -5 /*prioridad*/);
    data = strdup("Seis");
    printf("Insertamos el elemento %s con prioridad -6\n", data);
    heap_push(heap, data, -6 /*prioridad*/);
    data = strdup("Siete");
    printf("Insertamos el elemento %s con prioridad -7\n", data);
    heap_push(heap, data, -7 /*prioridad*/);

    printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
    while (heap_top(heap) != NULL){
        printf("Top: %s\n", (char*) heap_top(heap));      
        heap_pop(heap);
    }
    printf("No hay más elementos en el Heap\n");

    int opcion;
    do {
        printf("\n***** EJEMPLO MENU ******\n");
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");
        
        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");
    
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
    
        switch (opcion) {
        case '1':
          //dfs(estado_inicial);
          break;
        case '2':
          //bfs(estado_inicial);
          break;
        case '3':
          //best_first(estado_inicial);
          break;
        }
        presioneTeclaParaContinuar();
        limpiarPantalla();

  } while (opcion != '4');

  return 0;
}
