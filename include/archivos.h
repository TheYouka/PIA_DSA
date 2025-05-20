#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "constantes.h"
#include "cola.h"

/**
 * @brief Carga el mapa desde un archivo
 * @param mapa Matriz que representa el mapa
 * @param nodos_ids Arreglo de nodos
 * @param calles Matriz de adyacencia
 * @param nombre_archivo Nombre del archivo a cargar
 */
void cargar_mapa(char mapa[SIZE_MAP][SIZE_MAP], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], char *nombre_archivo);

/**
 * @brief Inicializa la matriz de calles con ceros
 * @param calles Matriz de adyacencia a inicializar
 */
void inicializar_calles(int calles[MAX_NODOS][MAX_NODOS]);

/**
 * @brief Carga los taxis desde un archivo
 * @param taxis Arreglo de taxis
 * @param mapa_taxis Matriz que representa la posición de los taxis en el mapa
 * @param nombre_archivo Nombre del archivo a cargar
 * @param taxis_totales Puntero para almacenar la cantidad de taxis cargados
 */
void cargar_taxis(Taxi *taxis, int mapa_taxis[SIZE_MAP][SIZE_MAP], char *nombre_archivo, int *taxis_totales);

/**
 * @brief Carga las solicitudes desde un archivo
 * @param solicitudes Cola de solicitudes
 * @param nombre_archivo Nombre del archivo a cargar
 */
void cargar_solicitudes(Cola *solicitudes, char *nombre_archivo);

/**
 * @brief Carga los nodos desde un archivo
 * @param nodos_ids Arreglo de nodos
 * @param nombre_archivo Nombre del archivo a cargar
 */
void cargar_nodos(int nodos_ids[MAX_NODOS], char *nombre_archivo);

/**
 * @brief Agrega un nuevo taxi al sistema
 * @param taxis Arreglo de taxis
 * @param mapa_taxis Matriz que representa la posición de los taxis en el mapa
 * @param mapa Matriz que representa el mapa
 * @param taxis_totales Puntero a la cantidad total de taxis
 * @return 1 si se agregó correctamente, 0 en caso contrario
 */
int agregar_taxi(Taxi *taxis, int mapa_taxis[SIZE_MAP][SIZE_MAP], char mapa[SIZE_MAP][SIZE_MAP], int *taxis_totales);

/**
 * @brief Agrega una nueva solicitud de viaje
 * @param solicitudes Cola de solicitudes
 * @param mapa Matriz que representa el mapa
 * @return 1 si se agregó correctamente, 0 en caso contrario
 */
int agregar_solicitud(Cola *solicitudes, char mapa[SIZE_MAP][SIZE_MAP]);

#endif
