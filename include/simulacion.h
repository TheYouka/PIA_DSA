#ifndef SIMULACION_H
#define SIMULACION_H

#include "constantes.h"
#include "estructuras.h"
#include "cola.h"
#include "dijasktra.h"

/**
 * @brief Procesa una solicitud de viaje, asignando el taxi más cercano
 * @param taxis Arreglo de taxis
 * @param nodos_ids Arreglo de nodos
 * @param calles Matriz de adyacencia
 * @param solicitud Solicitud a procesar
 * @return 1 si se procesó correctamente, 0 en caso contrario
 */
int procesar_solicitud(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], Solicitud solicitud);

/**
 * @brief Actualiza el estado de todos los taxis, decrementando el tiempo de los ocupados
 * @param taxis Arreglo de taxis
 * @param mapa_taxis Matriz que representa la posición de los taxis en el mapa
 * @return Cantidad de taxis que completaron su viaje
 */
int actualizar_taxis(Taxi taxis[MAX_TAXIS], int mapa_taxis[SIZE_MAP][SIZE_MAP]);

/**
 * @brief Ejecuta la simulación
 * @param taxis Arreglo de taxis
 * @param nodos_ids Arreglo de nodos
 * @param calles Matriz de adyacencia
 * @param mapa Representación visual del mapa
 * @param mapa_taxis Matriz que representa la posición de los taxis en el mapa
 * @param solicitudes Cola de solicitudes
 */
void ejecutar_simulacion(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], 
                        char mapa[SIZE_MAP][SIZE_MAP], int mapa_taxis[SIZE_MAP][SIZE_MAP], Cola *solicitudes);

#endif /* SIMULACION_H */
