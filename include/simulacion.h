#ifndef SIMULACION_H
#define SIMULACION_H

#include "constantes.h"
#include "estructuras.h"
#include "cola.h"

/**
 * @brief Procesa una solicitud de viaje, encontrando el taxi más cercano y calculando la ruta
 * @param taxis Arreglo de taxis
 * @param nodos_ids Arreglo de nodos con valores absolutos
 * @param calles Matriz de adyacencia del grafo
 * @param mapa Representación visual del mapa
 * @param solicitud Solicitud de viaje a procesar
 * @return ID del taxi asignado, -1 si no hay taxis disponibles
 */
int procesar_solicitud(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], 
                       char mapa[SIZE_MAP][SIZE_MAP], Solicitud solicitud);

/**
 * @brief Calcula el costo del viaje basado en la distancia
 * @param distancia_taxi_pasajero Distancia que recorre el taxi para llegar al pasajero
 * @param distancia_viaje Distancia que recorre el taxi con el pasajero
 * @return Costo total del viaje
 */
float calcular_costo(int distancia_taxi_pasajero, int distancia_viaje);

/**
 * @brief Actualiza la posición del taxi después de completar un viaje
 * @param taxis Arreglo de taxis
 * @param id_taxi ID del taxi a actualizar
 * @param x_destino Coordenada x de destino
 * @param y_destino Coordenada y de destino
 * @param mapa_taxis Matriz que representa la posición de los taxis en el mapa
 */
void actualizar_posicion_taxi(Taxi taxis[MAX_TAXIS], int id_taxi, int x_destino, int y_destino, int mapa_taxis[SIZE_MAP][SIZE_MAP]);

/**
 * @brief Ejecuta un ciclo de simulación, procesando una solicitud de la cola
 * @param taxis Arreglo de taxis
 * @param nodos_ids Arreglo de nodos con valores absolutos
 * @param calles Matriz de adyacencia del grafo
 * @param mapa Representación visual del mapa
 * @param mapa_taxis Matriz que representa la posición de los taxis en el mapa
 * @param solicitudes Cola de solicitudes
 * @return 1 si se procesó una solicitud, 0 si la cola está vacía
 */
int ciclo_simulacion(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], 
                    char mapa[SIZE_MAP][SIZE_MAP], int mapa_taxis[SIZE_MAP][SIZE_MAP], Cola *solicitudes);

#endif /* SIMULACION_H */
