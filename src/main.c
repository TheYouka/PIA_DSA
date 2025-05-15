#include <stdio.h>
#include "../include/visualizacion.h"
#include "../include/cola.h"
#include "../include/archivos.h"
#include "../include/constantes.h"


int main() {
    int opcion;

    int mapa[SIZE_MAP][SIZE_MAP];
    cargar_mapa(mapa, "data/mapa.txt");
    
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