
# Puzzle
Este codigo, permite ordenar una matriz de 9x9 con ocho numeros (desde el 1 al 8). Ingresa con un puzzle desordenado y debiera complir la funcion de mostrar los estados hasta llegar a un estado ordenado. 
Tiene dos opciones busqueda en profundidad(DFS) o busqueda en anchura(BFS).

## Funcionalidades

1. **Búsqueda en Profundidad**: Realiza una búsqueda en profundidad para encontrar la solución del puzzle. Sin embargo, actualmente, esta opción actualmente;
 ```bash
  "**fallo de segmentación (core dump)**"
 ```
3. **Búsqueda en Anchura**: Realiza una búsqueda en anchura para encontrar la solución del puzzle. Esta opción actualmente;
```bash
 "**ejecuta indefinidamente**"
```
## Mejoramientos

-Optimizar el manejo de memoria para evitar fallos de segmentación en la opción DFS.

-Implementar límites de profundidad y anchura para evitar bucles infinitos.

-Mejorar la interfaz del usuario para facilitar el uso y la comprensión de los errores.

## Ejecución del Programa

*Visita este enlace* [Repl.it](https://replit.com/@MAURAGONZALEZ2/Tarea-2-estructura-1#tarea2.c).
---
Dirigete a `Shell`

1. **Compilar el Programa**: Para compilar el programa, ejecuta el siguiente comando en tu terminal:

   ```bash
   gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
   ```

2. **Ejecutar el Programa**: Una vez compilado, puedes ejecutar el programa con el siguiente comando:

   ```bash
   ./tarea3
   ```


## Ejemplo de Uso

Una vez ejecutado el programa, se mostrará un menú principal 
```bash
Estado inicial del puzzle:
x 2 8 
1 3 4 
6 5 7 

Distancia L1:14

********* MENU *********
1) Búsqueda en Profundidad
2) Búsqueda en Anchura
3) Salir
```

1. Búsqueda en Profundidad
   ```bash
   Ingrese su opción: 1
   
   ***.~~~¨¨--°°°°°°--¨¨~~~.***
       Búsqueda en Profundidad 
   ***.~~~¨¨--°°°°°°--¨¨~~~.***
   "Los estados, desde el inicial hasta el final"
   DFS Iteración 0 - Estado actual:

   Transición (1):
   x x 8 
   1 3 4 
   6 5 7 

   Transición (2):
   x 8 x 
   1 3 4 
   6 5 7 

   Transición (4):
   x 3 8 
   1 x 4 
   6 5 7 

   Número de iteraciones para llegar a la solución:
   
3. Búsqueda en Anchura
   ```bash
   Ingrese su opción: 2
   
   ***.~~~¨¨--°°°°°°--¨¨~~~.***
       Búsqueda en Anchura 
   ***.~~~¨¨--°°°°°°--¨¨~~~.***
   "Los estados, desde el inicial hasta el final"
   BFS Iteración 1 - Estado actual:

   Transición (2):
   x x 8 
   1 3 4 
   6 5 7 

   Transición (4):
   1 x 8 
   x 3 4 
   6 5 7 

   Número de iteraciones para llegar a la solución: