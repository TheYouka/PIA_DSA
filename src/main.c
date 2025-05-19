#include <stdio.h>
#include "../include/visualizacion.h"
#include "../include/cola.h"
#include "../include/archivos.h"
#include "../include/constantes.h"
#include "../include/dijasktra.h"

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
        }
    }

    // Array que dado el id del nodo guarda su ubicacion en el mapa de forma absoluta, y*SIZE_MAP + x
    int nodos_ids[MAX_NODOS];

    // Cargar los datos iniciales
    cargar_nodos(nodos_ids, "data/nodos.txt");
    cargar_taxis(taxis, mapa_taxis, "data/taxis.txt", &taxis_totales);
    cargar_mapa(mapa, nodos_ids, calles, "data/mapa.txt");

    // Inicializar cola para las solicitudes
    Cola solicitudes;
    inicializar_cola(&solicitudes);

    // Cargar las solicitudes desde el archivo
    cargar_solicitudes(&solicitudes, "data/solicitudes.txt");
    
    do {
        opcion = mostrar_menu_principal();
        
        switch (opcion) {
            case 1:
                printf("\nIniciando simulación...\n");
                // Aquí podríamos llamar a la función de simulación cuando esté lista
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
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
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
                mostrar_cola(&solicitudes);
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 7:
                printf("\nVisualizando mapa...\n");
                imprimir_mapa(mapa);
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
            case 8:
                printf("\nSaliendo del programa. ¡Hasta pronto!\n");
                break;
            default:
                printf("\nOpción no válida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 8);  // Corregido: salir en la opción 8, no en la 7

    return 0;
}