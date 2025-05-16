#include "../include/archivos.h"
#include <stdio.h>

void cargar_mapa(char mapa[SIZE_MAP][SIZE_MAP], int calles[SIZE_MAP][SIZE_MAP], char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        return;
    }

    // Inicializar el mapa con ceros
    for (int i = 0; i < SIZE_MAP; i++) {
        for (int j = 0; j < SIZE_MAP; j++) {
            mapa[i][j] = '.';
        }
    }

    int x1, y1, x2, y2, peso;
    
    // Leer entradas del archivo y guardar en el mapa y calles
    while (fscanf(archivo, "%d %d %d %d %d", &x1, &y1, &x2, &y2, &peso) == 5) {
        calles[x1][y1] = peso;
        calles[x2][y2] = peso;
        mapa[x1][y1] = 'X';
        mapa[x2][y2] = 'X';
    }
    fclose(archivo);

    return;
};


// Inicializar el mapa de calles con ceros
void inicializar_calles(int calles[SIZE_MAP][SIZE_MAP]) {
    for(int i = 0; i < SIZE_MAP; i++) {
        for(int j = 0; j < SIZE_MAP; j++) {
            calles[i][j] = 0;
        }
    }
};  


// Cargar los taxis desde el archivo
void cargar_taxis(Taxi *taxis, int mapa_taxis[SIZE_MAP][SIZE_MAP], char *nombre_archivo,int *taxis_totales) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        return;
    }

    // Inicializar el array de taxis
    for(int i = 0; i < MAX_TAXIS; i++) {
        taxis[i].id = -1;
        taxis[i].x = -1;
        taxis[i].y = -1;
        taxis[i].estado = -1;
    };

    int x, y;
    int id = 1;
    
    // Leer coordenadas del archivo y cargar los taxis
    while (fscanf(archivo, "%d %d", &x, &y) == 2 && id <= MAX_TAXIS) {
        taxis[id-1].id = id;
        taxis[id-1].x = x;
        taxis[id-1].y = y;
        taxis[id-1].estado = 0; // 0 indica taxi libre
        id++;

        // Guardar el id del taxi en la posicion del mapa
        mapa_taxis[x][y] = id-1;
    };

    fclose(archivo);
    return;
};

// Cargar las solicitudes desde el archivo
void cargar_solicitudes(Cola *solicitudes, char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        return;
    };

    int x_origen, y_origen, x_destino, y_destino;
    int id = 1;

    // Leer coordenadas del archivo y cargar las solicitudes
    while (fscanf(archivo, "%d %d %d %d", &x_origen, &y_origen, &x_destino, &y_destino) == 4 && id <= MAX_SOLICITUDES) {
        Solicitud solicitud;
        solicitud.x_origen = x_origen;
        solicitud.y_origen = y_origen;
        solicitud.x_destino = x_destino;
        solicitud.y_destino = y_destino;
        solicitud.id = id;
        apilar(solicitudes, solicitud);
        id++;
    };

    fclose(archivo);
    return;
};



