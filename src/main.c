#include <stdio.h>
#include "../include/visualizacion.h"
#include "../include/cola.h"
#include "../include/archivos.h"
#include "../include/constantes.h"


int main() {
    int opcion;
    // Inicializar el mapa con espacios
    char mapa[SIZE_MAP][SIZE_MAP];

    // Inicializar el mapa de calles con ceros, para que cada posicion i,j guarde la distancia entre el nodo_ID i y el nodo_ID j
    int calles[MAX_NODOS][MAX_NODOS];
    inicializar_calles(calles);

    // Inicializar array para rastrear los taxis
    Taxi taxis[MAX_TAXIS];
    int taxis_totales = 0;

    // Array que guarda el id del taxi en cualquiera posicion del mapa
    int mapa_taxis[SIZE_MAP][SIZE_MAP];
    for (int i = 0; i < SIZE_MAP; i++) {
        for (int j = 0; j < SIZE_MAP; j++) {
            mapa_taxis[i][j] = -1;
        };
    };

    // Array que dado el id del nodo guarda su ubicacion en el mapa de forma absoluta, y*SIZE_MAP + x
    int nodos_ids[MAX_NODOS];

    
    cargar_nodos(nodos_ids, "data/nodos.txt");

    // Cargar los taxis desde el archivo
    cargar_taxis(taxis, mapa_taxis, "data/taxis.txt", &taxis_totales);

    // Cargar el mapa desde el archivo e inicializar el mapa con .
    cargar_mapa(mapa, nodos_ids, calles, "data/mapa.txt");

    // Inicializar cola para las solicitudes
    Cola Solicitudes;
    inicializar_cola(&Solicitudes);

    // Cargar las solicitudes desde el archivo
    cargar_solicitudes(&Solicitudes, "data/solicitudes.txt");

    
   
    

    // Contador para la última posición agregada
    int ultima_pos = 0;

    // Iterar sobre el mapa buscando 'X'
    for (int y = 0; y < SIZE_MAP; y++) {
        for (int x = 0; x < SIZE_MAP; x++) {
            if (mapa[y][x] == 'X') {
                // Agregar forma absoluta de la coordenada
                nodos_ids[ultima_pos] = y * SIZE_MAP + x;
                ultima_pos++;
            }
        }
    }


    
    do {
        opcion = mostrar_menu_principal();
        
        switch (opcion) {
            case 1:
                printf("\nIniciando simulación...\n");
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 2:
                printf("\nAgregar taxis...\n");
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 3:
                printf("\nAgregar solicitudes...\n");
                break;
            case 4:
                printf("\nMostrando taxis disponibles...\n");
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 5:
                printf("\nMostrando mapa...\n");
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 6:
                printf("\nMostrando solicitudes pendientes...\n");
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 7:
                imprimir_mapa(mapa);
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 8:
                printf("\nSaliendo del programa. ¡Hasta pronto!\n");
                break;
        }
    } while (opcion != 7);

    return 0;
}