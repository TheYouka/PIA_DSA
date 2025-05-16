#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "constantes.h"
#include "cola.h"

void cargar_mapa(char mapa[SIZE_MAP][SIZE_MAP], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], char *nombre_archivo);

void cargar_solicitudes(Cola *solicitudes, char *nombre_archivo);

void inicializar_calles(int calles[MAX_NODOS][MAX_NODOS]);

void cargar_taxis(Taxi *taxis, int mapa_taxis[SIZE_MAP][SIZE_MAP], char *nombre_archivo, int *taxis_totales);

void cargar_nodos(int nodos_ids[MAX_NODOS], char *nombre_archivo);


#endif
