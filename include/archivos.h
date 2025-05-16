#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "constantes.h"
#include "cola.h"

void cargar_mapa(char mapa[SIZE_MAP][SIZE_MAP], int calles[SIZE_MAP][SIZE_MAP], char *nombre_archivo);

void cargar_solicitudes(Cola *solicitudes, char *nombre_archivo);

void inicializar_calles(int calles[SIZE_MAP][SIZE_MAP]);

void cargar_taxis(Taxi *taxis, int mapa_taxis[SIZE_MAP][SIZE_MAP], char *nombre_archivo, int *taxis_totales);


#endif
