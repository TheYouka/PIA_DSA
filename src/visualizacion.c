#include <stdio.h>
#include <stdlib.h>
#include "../include/visualizacion.h"

/**
 * @brief Muestra el menú principal del Mini-Uber con estilo visual
 * @return int Opción seleccionada por el usuario
 */
int mostrar_menu_principal() {
    int opcion = 0;
    
    while (opcion < 1 || opcion > 7) {
        // Limpiar pantalla
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        
        printf("\033[1;33m"); // Texto amarillo brillante
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║              \033[1;36mMINI-UBER SIMULATION\033[1;33m               ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║                                                  ║\n");
        printf("║  \033[1;37m1.\033[0m\033[1;32m Iniciar Simulación\033[1;33m                        ║\n");
        printf("║  \033[1;37m2.\033[0m\033[1;32m Agregar Taxis\033[1;33m                             ║\n");
        printf("║  \033[1;37m3.\033[0m\033[1;32m Agregar Solicitudes\033[1;33m                       ║\n");
        printf("║  \033[1;37m4.\033[0m\033[1;32m Ver Taxis\033[1;33m                                 ║\n");
        printf("║  \033[1;37m5.\033[0m\033[1;32m Ver Mapa\033[1;33m                                  ║\n");
        printf("║  \033[1;37m6.\033[0m\033[1;32m Ver Solicitudes\033[1;33m                           ║\n");
        printf("║  \033[1;37m7.\033[0m\033[1;32m Imprimir Mapa\033[1;33m                             ║\n");
        printf("║  \033[1;37m7.\033[0m\033[1;31m Salir\033[1;33m                                     ║\n");
        printf("║                                                  ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");
        printf("\033[0m"); // Resetear color
        
        printf("\n\033[1;36mSeleccione una opción (1-7): \033[0m");
        if (scanf("%d", &opcion) != 1) {
            // Limpiar el buffer de entrada en caso de entrada no válida
            while (getchar() != '\n');
            opcion = 0; // Forzar repetición del bucle
        }
        
        if (opcion < 1 || opcion > 7) {
            printf("\n\033[1;31m[ERROR] Opción inválida. Presione Enter para continuar...\033[0m");
            getchar(); // Capturar el Enter después del número
            getchar(); // Esperar otro Enter para continuar
        }
    }
    
    return opcion;
};

int imprimir_mapa(char mapa[SIZE_MAP][SIZE_MAP]) {
    for (int i = 0; i < SIZE_MAP; i++) {
        for (int j = 0; j < SIZE_MAP; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
    return 0;
}