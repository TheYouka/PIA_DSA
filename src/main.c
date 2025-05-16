#include <stdio.h>
#include "../include/visualizacion.h"
#include "../include/cola.h"
#include "../include/archivos.h"
#include "../include/constantes.h"


int main() {
    int opcion;
    // Inicializar el mapa con espacios
    char mapa[SIZE_MAP][SIZE_MAP];

    // Inicializar el mapa de calles con ceros
    int calles[SIZE_MAP][SIZE_MAP];
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

    
    // Cargar los taxis desde el archivo
    cargar_taxis(taxis, mapa_taxis, "data/taxis.txt", &taxis_totales);

    // Cargar el mapa desde el archivo e inicializar el mapa con .
    cargar_mapa(mapa, calles, "data/mapa.txt");

    // Inicializar cola para las solicitudes
    Cola Solicitudes;
    inicializar_cola(&Solicitudes);

    // Cargar las solicitudes desde el archivo
    cargar_solicitudes(&Solicitudes, "data/solicitudes.txt");


    
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