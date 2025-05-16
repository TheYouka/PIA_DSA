#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "estructuras.h"

/**
 * @brief Ejecuta el algoritmo de Dijkstra para encontrar la ruta más corta desde un nodo origen
 * @param grafo Matriz de adyacencia que representa el mapa
 * @param num_nodos Número total de nodos en el grafo
 * @param origen Nodo de origen
 * @param distancias Array donde se almacenarán las distancias más cortas desde el origen
 * @param predecesores Array donde se almacenarán los nodos predecesores para reconstruir la ruta
 */
void dijkstra(int grafo[MAX_NODOS][MAX_NODOS], int num_nodos, int origen, int distancias[MAX_NODOS], int predecesores[MAX_NODOS]);

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
                          int *distancia_taxi, int ruta_taxi[MAX_NODOS], int *longitud_ruta);

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
                 int ruta[MAX_NODOS], int *longitud_ruta);

/**
 * @brief Convierte coordenadas (x,y) a un índice de nodo
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param ancho Ancho del mapa (número de columnas)
 * @return Índice del nodo correspondiente
 */
int coordenadas_a_nodo(int x, int y, int ancho);

/**
 * @brief Convierte un índice de nodo a coordenadas (x,y)
 * @param nodo Índice del nodo
 * @param ancho Ancho del mapa (número de columnas)
 * @param x Puntero donde se almacenará la coordenada X
 * @param y Puntero donde se almacenará la coordenada Y
 */
void nodo_a_coordenadas(int nodo, int ancho, int *x, int *y);

#endif /* DIJKSTRA_H */
