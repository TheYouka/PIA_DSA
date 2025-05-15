#include "../include/archivos.h"
#include <stdio.h>

void cargar_mapa(int mapa[SIZE_MAP][SIZE_MAP], char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        return;
    }

    // Inicializar el mapa con ceros
    for (int i = 0; i < SIZE_MAP; i++) {
        for (int j = 0; j < SIZE_MAP; j++) {
            mapa[i][j] = 0;
        }
    }

    int x1, y1, x2, y2, peso;
    
    // Leer entradas del 



    
    fclose(archivo);
}