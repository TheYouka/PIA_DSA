#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "constantes.h"
#include "estructuras.h"

/**
 * @brief Convierte coordenadas (x,y) a un valor absoluto
 * @param x Coordenada x
 * @param y Coordenada y
 * @return Valor absoluto calculado como y * SIZE_MAP + x
 */
int coords_to_absolute(int x, int y);

/**
 * @brief Convierte un valor absoluto a coordenadas (x,y)
 * @param absolute Valor absoluto
 * @param x Puntero donde se almacenará la coordenada x
 * @param y Puntero donde se almacenará la coordenada y
 */
void absolute_to_coords(int absolute, int *x, int *y);

/**
 * @brief Busca el ID de nodo en el arreglo nodos_ids que corresponde a las coordenadas dadas
 * @param nodos_ids Arreglo de nodos con valores absolutos
 * @param x Coordenada x
 * @param y Coordenada y
 * @return ID del nodo si existe, -1 si no existe
 */
int buscar_id_nodo(int nodos_ids[MAX_NODOS], int x, int y);

/**
 * @brief Implementación del algoritmo de Dijkstra
 * @param calles Matriz de adyacencia del grafo que representa las calles
 * @param inicio ID del nodo de inicio
 * @param distancias Arreglo donde se almacenarán las distancias mínimas
 * @param predecesores Arreglo donde se almacenarán los predecesores
 */
void dijkstra(int calles[MAX_NODOS][MAX_NODOS], int inicio, int distancias[MAX_NODOS], int predecesores[MAX_NODOS]);

/**
 * @brief Encuentra el taxi disponible más cercano a las coordenadas dadas
 * @param taxis Arreglo de taxis
 * @param nodos_ids Arreglo de nodos con valores absolutos
 * @param calles Matriz de adyacencia del grafo
 * @param x_origen Coordenada x de origen
 * @param y_origen Coordenada y de origen
 * @return ID del taxi más cercano, -1 si no hay taxis disponibles
 */
int encontrar_taxi_cercano(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], int x_origen, int y_origen);

/**
 * @brief Obtiene la ruta desde un origen hasta un destino usando Dijkstra
 * @param calles Matriz de adyacencia del grafo
 * @param nodos_ids Arreglo de nodos con valores absolutos
 * @param x_origen Coordenada x de origen
 * @param y_origen Coordenada y de origen
 * @param x_destino Coordenada x de destino
 * @param y_destino Coordenada y de destino
 * @param ruta Arreglo donde se almacenará la ruta (IDs de nodos)
 * @param longitud Puntero donde se almacenará la longitud de la ruta
 * @return Distancia total de la ruta, -1 si no existe ruta
 */
int obtener_ruta(int calles[MAX_NODOS][MAX_NODOS], int nodos_ids[MAX_NODOS], 
                int x_origen, int y_origen, int x_destino, int y_destino, 
                int ruta[MAX_NODOS], int *longitud);

#endif /* DIJKSTRA_H */    