#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/dijasktra.h"
#include "../include/constantes.h"
#include "../include/estructuras.h"

// Constante para representar infinito
#define INFINITO INT_MAX

/**
 * Convierte coordenadas (x,y) a un valor absoluto
 */
int coords_to_absolute(int x, int y) {
    return y * SIZE_MAP + x;
}

/**
 * Convierte un valor absoluto a coordenadas (x,y)
 */
void absolute_to_coords(int absolute, int *x, int *y) {
    *y = absolute / SIZE_MAP;
    *x = absolute % SIZE_MAP;
}

/**
 * Busca el ID de nodo en el arreglo nodos_ids que corresponde a las coordenadas dadas
 */
int buscar_id_nodo(int nodos_ids[MAX_NODOS], int x, int y) {
    int pos_absoluta = coords_to_absolute(x, y);
    
    for (int i = 0; i < MAX_NODOS; i++) {
        if (nodos_ids[i] == pos_absoluta) {
            return i;
        }
    }
    
    // No se encontró el nodo
    return -1;
}

/**
 * Encuentra el nodo con la distancia mínima que aún no ha sido visitado
 */
int minima_distancia(int distancias[MAX_NODOS], int visitado[MAX_NODOS]) {
    int min = INFINITO;
    int min_index = -1;
    
    for (int i = 0; i < MAX_NODOS; i++) {
        if (visitado[i] == 0 && distancias[i] < min) {
            min = distancias[i];
            min_index = i;
        }
    }
    
    return min_index;
}

/**
 * Implementación del algoritmo de Dijkstra
 */
void dijkstra(int calles[MAX_NODOS][MAX_NODOS], int inicio, int distancias[MAX_NODOS], int predecesores[MAX_NODOS]) {
    int visitado[MAX_NODOS];
    
    // Inicializar distancias, predecesores y visitado
    for (int i = 0; i < MAX_NODOS; i++) {
        distancias[i] = INFINITO;
        predecesores[i] = -1;
        visitado[i] = 0;
    }
    
    // La distancia del nodo inicial a sí mismo es 0
    distancias[inicio] = 0;
    
    // Encontrar el camino más corto para todos los nodos
    for (int count = 0; count < MAX_NODOS - 1; count++) {
        // Elegir el nodo con la distancia mínima del conjunto de nodos no visitados
        int u = minima_distancia(distancias, visitado);
        
        // Si no hay más nodos alcanzables, salir del bucle
        if (u == -1) {
            break;
        }
        
        // Marcar el nodo elegido como visitado
        visitado[u] = 1;
        
        // Actualizar las distancias de los nodos adyacentes al nodo elegido
        for (int v = 0; v < MAX_NODOS; v++) {
            // Actualizar distancia[v] solo si:
            // 1. Hay una arista de u a v (calles[u][v] != 0)
            // 2. v no ha sido visitado
            // 3. La distancia total a través de u es menor que la distancia actual de v
            if (calles[u][v] != 0 && visitado[v] == 0 && 
                distancias[u] != INFINITO && 
                distancias[u] + calles[u][v] < distancias[v]) {
                distancias[v] = distancias[u] + calles[u][v];
                predecesores[v] = u;
            }
        }
    }
}

/**
 * Encuentra el taxi disponible más cercano a las coordenadas dadas
 */
int encontrar_taxi_cercano(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], int x_origen, int y_origen) {
    int id_nodo_origen = buscar_id_nodo(nodos_ids, x_origen, y_origen);
    
    // Si no existe el nodo de origen, no se puede buscar un taxi
    if (id_nodo_origen == -1) {
        return -1;
    }
    
    // Aplicar Dijkstra desde el nodo de origen
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    dijkstra(calles, id_nodo_origen, distancias, predecesores);
    
    // Buscar el taxi más cercano
    int min_distancia = INFINITO;
    int id_taxi_cercano = -1;
    
    for (int i = 0; i < MAX_TAXIS; i++) {
        // Solo considerar taxis disponibles
        if (taxis[i].id != -1 && taxis[i].estado == 0) {
            int id_nodo_taxi = buscar_id_nodo(nodos_ids, taxis[i].x, taxis[i].y);
            
            // Si el taxi está en un nodo válido y es alcanzable
            if (id_nodo_taxi != -1 && distancias[id_nodo_taxi] != INFINITO) {
                if (distancias[id_nodo_taxi] < min_distancia) {
                    min_distancia = distancias[id_nodo_taxi];
                    id_taxi_cercano = taxis[i].id;
                }
            }
        }
    }
    
    // Regresa el id del taxi más cercano del array de taxis
    return id_taxi_cercano;
}

/**
 * Obtiene la ruta desde un origen hasta un destino usando Dijkstra
 */
int obtener_ruta(int calles[MAX_NODOS][MAX_NODOS], int nodos_ids[MAX_NODOS], 
                int x_origen, int y_origen, int x_destino, int y_destino, 
                int ruta[MAX_NODOS], int *longitud) {
    int id_nodo_origen = buscar_id_nodo(nodos_ids, x_origen, y_origen);
    int id_nodo_destino = buscar_id_nodo(nodos_ids, x_destino, y_destino);
    
    // Si alguno de los nodos no existe, no hay ruta
    if (id_nodo_origen == -1 || id_nodo_destino == -1) {
        *longitud = 0;
        return -1;
    }
    
    // Aplicar Dijkstra desde el nodo de origen
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    dijkstra(calles, id_nodo_origen, distancias, predecesores);
    
    // Si no hay ruta al destino
    if (distancias[id_nodo_destino] == INFINITO) {
        *longitud = 0;
        return -1;
    }
    
    // Reconstruir la ruta desde el destino hacia el origen
    int temp_ruta[MAX_NODOS];
    int temp_longitud = 0;
    int actual = id_nodo_destino;
    
    while (actual != -1) {
        temp_ruta[temp_longitud++] = actual;
        actual = predecesores[actual];
    }
    
    // Invertir la ruta (de origen a destino)
    *longitud = temp_longitud;
    for (int i = 0; i < temp_longitud; i++) {
        ruta[i] = temp_ruta[temp_longitud - 1 - i];
    }
    
    return distancias[id_nodo_destino];
}
