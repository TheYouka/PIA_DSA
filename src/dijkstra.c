#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../include/dijasktra.h"
#include "../include/constantes.h"

// Valor que representa la distancia infinita (no hay camino)
#define INFINITO INT_MAX

/**
 * @brief Encuentra el nodo no visitado con la menor distancia
 * @param distancias Array de distancias desde el origen
 * @param visitado Array que indica qué nodos han sido visitados
 * @param num_nodos Número total de nodos
 * @return Índice del nodo más cercano no visitado, o -1 si todos fueron visitados
 */
static int minima_distancia(int distancias[MAX_NODOS], int visitado[MAX_NODOS], int num_nodos) {
    int min = INFINITO;
    int min_index = -1;
    
    for (int v = 0; v < num_nodos; v++) {
        if (visitado[v] == 0 && distancias[v] <= min) {
            min = distancias[v];
            min_index = v;
        }
    }
    
    return min_index;
}

/**
 * @brief Ejecuta el algoritmo de Dijkstra para encontrar la ruta más corta desde un nodo origen
 * @param grafo Matriz de adyacencia que representa el mapa
 * @param num_nodos Número total de nodos en el grafo
 * @param origen Nodo de origen donde se encuentra el pasajero
 * @param distancias Array donde se almacenarán las distancias más cortas desde el origen
 * @param predecesores Array donde se almacenarán los nodos predecesores para reconstruir la ruta
 */
void dijkstra(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int nodos_ids[MAX_NODOS], int x_origen, int y_origen, int distancias[MAX_NODOS], int predecesores[MAX_NODOS]) {
    
    int origen = coordenadas_a_nodo(x_origen, y_origen);

    int visitado[MAX_NODOS] = {0}; // Array para rastrear nodos visitados
    
    // Inicializar distancias a INFINITO y predecesores a -1
    for (int i = 0; i < num_nodos; i++) {
        distancias[i] = INFINITO;
        predecesores[i] = -1;
    }
    
    // La distancia desde el origen a sí mismo es 0
    // Buscar la posición del nodo origen en nodos_ids
    for (int i = 0; i < MAX_NODOS; i++) {
        if (nodos_ids[i] == origen) {
            distancias[i] = 0;
            break;
        }
    }
    
    // Encontrar el camino más corto para todos los nodos
    //
    for (int count = 0; count < num_nodos - 1; count++) {
        // Obtener el nodo con la distancia mínima del conjunto de nodos no procesados
        int u = minima_distancia(distancias, visitado, num_nodos);
        
        // Si no hay más nodos alcanzables, salir del bucle
        if (u == -1) break;
        
        // Marcar el nodo seleccionado como visitado
        visitado[u] = 1;
        
        // Actualizar las distancias de los nodos adyacentes al nodo seleccionado
        for (int v = 0; v < MAX_NODOS; v++) {
            // Actualizar distancia[v] solo si:
            // 1. No está visitado
            // 2. Hay una arista desde u a v
            // 3. El peso total del camino desde origen a v a través de u es menor que el valor actual de distancia[v]

            if(nodos_ids[v] == -1) break;

            if (!visitado[v] && 
                grafo[u][v] && 
                distancias[u] != INFINITO && 
                distancias[u] + grafo[u][v] < distancias[v]) {
                
                distancias[v] = distancias[u] + grafo[u][v];
                predecesores[v] = u;
            }
        }
    }
};



/**
 * @brief Encuentra el taxi más cercano al nodo de origen del pasajero
 * @param grafo Matriz de adyacencia que representa el mapa
 * @param num_nodos Número total de nodos en el grafo
 * @param nodo_origen Nodo de origen del pasajero
 * @param taxis Array de taxis disponibles
 * @param num_taxis Número de taxis disponibles
 * @param taxi_asignado Puntero al índice donde se almacenará el taxi asignado
 * @param distancia_taxi Puntero donde se almacenará la distancia del taxi al pasajero
 * @param ruta_taxi Array donde se almacenará la ruta del taxi al pasajero
 * @param longitud_ruta Puntero donde se almacenará la longitud de la ruta
 * @return 1 si se encontró un taxi, 0 en caso contrario
 */
int encontrar_taxi_cercano(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int nodo_origen, 
                          Taxi taxis[], int num_taxis, int *taxi_asignado, 
                          int *distancia_taxi, int ruta_taxi[MAX_NODOS], int *longitud_ruta) {
    int distancia_minima = INFINITO;
    int taxi_mas_cercano = -1;
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    
    // Ejecutar Dijkstra desde el nodo de origen del pasajero
    dijkstra(grafo, num_nodos, nodo_origen, distancias, predecesores);
    
    // Encontrar el taxi disponible más cercano
    for (int i = 0; i < num_taxis; i++) {
        // Verificar si el taxi está disponible (estado = 0)
        // Nota: Ajusta esta condición según cómo definas los estados de los taxis
        if (taxis[i].estado == 0) {
            // Obtener el nodo donde está el taxi (conversión de coordenadas a nodo)
            int nodo_taxi = coordenadas_a_nodo(taxis[i].x, taxis[i].y, 15); // Asumiendo un ancho de 15
            
            // Verificar si hay un camino al taxi y si es más cercano que los anteriores
            if (distancias[nodo_taxi] != INFINITO && distancias[nodo_taxi] < distancia_minima) {
                distancia_minima = distancias[nodo_taxi];
                taxi_mas_cercano = i;
            }
        }
    }
    
    // Si se encontró un taxi disponible
    if (taxi_mas_cercano != -1) {
        *taxi_asignado = taxi_mas_cercano;
        *distancia_taxi = distancia_minima;
        
        // Calcular la ruta desde el taxi hasta el pasajero (ruta inversa)
        int nodo_taxi = coordenadas_a_nodo(taxis[taxi_mas_cercano].x, taxis[taxi_mas_cercano].y, 15);
        
        // Reconstruir la ruta desde el origen hasta el taxi (inversa)
        int temp_ruta[MAX_NODOS];
        int nodo_actual = nodo_taxi;
        int longitud = 0;
        
        while (nodo_actual != -1) {
            temp_ruta[longitud++] = nodo_actual;
            nodo_actual = predecesores[nodo_actual];
        }
        
        // Invertir la ruta para que vaya desde el taxi hasta el pasajero
        for (int i = 0; i < longitud; i++) {
            ruta_taxi[i] = temp_ruta[longitud - 1 - i];
        }
        
        *longitud_ruta = longitud;
        return 1;
    }
    
    return 0;  // No se encontró un taxi disponible
}

/**
 * @brief Calcula la ruta entre dos nodos usando el algoritmo de Dijkstra
 * @param grafo Matriz de adyacencia que representa el mapa
 * @param num_nodos Número total de nodos en el grafo
 * @param origen Nodo de origen
 * @param destino Nodo de destino
 * @param ruta Array donde se almacenará la ruta calculada
 * @param longitud_ruta Puntero donde se almacenará la longitud de la ruta
 * @return Distancia total de la ruta, o -1 si no hay camino
 */
int calcular_ruta(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int origen, int destino, 
                 int ruta[MAX_NODOS], int *longitud_ruta) {
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    
    // Ejecutar Dijkstra desde el nodo de origen
    dijkstra(grafo, num_nodos, origen, distancias, predecesores);
    
    // Verificar si hay un camino hasta el destino
    if (distancias[destino] == INFINITO) {
        *longitud_ruta = 0;
        return -1;  // No hay camino
    }
    
    // Reconstruir la ruta desde el destino hasta el origen
    int temp_ruta[MAX_NODOS];
    int nodo_actual = destino;
    int longitud = 0;
    
    while (nodo_actual != -1) {
        temp_ruta[longitud++] = nodo_actual;
        nodo_actual = predecesores[nodo_actual];
    }
    
    // Invertir la ruta para que vaya desde el origen hasta el destino
    for (int i = 0; i < longitud; i++) {
        ruta[i] = temp_ruta[longitud - 1 - i];
    }
    
    *longitud_ruta = longitud;
    return distancias[destino];  // Distancia total de la ruta
}

/**
 * @brief Convierte coordenadas (x,y) a un índice de nodo
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param ancho Ancho del mapa (número de columnas)
 * @return Índice del nodo correspondiente
 */
int coordenadas_a_nodo(int x, int y) {
    return y * SIZE_MAP + x;
}

/**
 * @brief Convierte un índice de nodo a coordenadas (x,y)
 * @param nodo Índice del nodo
 * @param ancho Ancho del mapa (número de columnas)
 * @param x Puntero donde se almacenará la coordenada X
 * @param y Puntero donde se almacenará la coordenada Y
 */
void nodo_a_coordenadas(int nodo, int ancho, int *x, int *y) {
    *y = nodo / ancho;
    *x = nodo % ancho;
} 