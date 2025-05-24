#include "../include/archivos.h"
#include <stdio.h>
#include <stdlib.h>



void cargar_mapa(char mapa[SIZE_MAP][SIZE_MAP], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
    }

    // Inicializar el mapa con ceros
    for (int i = 0; i < SIZE_MAP; i++) {
        for (int j = 0; j < SIZE_MAP; j++) {
            mapa[i][j] = '.';
        }
    }

    int x1, y1, x2, y2, peso, id;
    
    // Leer entradas del archivo y guardar en el mapa y calles
    while (fscanf(archivo, "%d %d %d %d %d", &x1, &y1, &x2, &y2, &peso) == 5) {
        int id_1 = -1, id_2 = -1;
        int pos1 = y1 * SIZE_MAP + x1;
        int pos2 = y2 * SIZE_MAP + x2;

        // Buscar los IDs correspondientes a las posiciones en nodos_ids
        for (int i = 0; i < MAX_NODOS; i++) {
            if (nodos_ids[i] == pos1) {
                id_1 = i;
            }
            if (nodos_ids[i] == pos2) {
                id_2 = i;
            }
            if (id_1 != -1 && id_2 != -1) {
                break;
            }
        }

        // Si se encontraron ambos IDs, crear las aristas
        if (id_1 != -1 && id_2 != -1) {
            calles[id_1][id_2] = peso;
            calles[id_2][id_1] = peso; // Grafo no dirigido
            mapa[y1][x1] = 'X';
            mapa[y2][x2] = 'X';
        }
    }
    fclose(archivo);

    return;
};


// Inicializar el mapa de calles con ceros
void inicializar_calles(int calles[MAX_NODOS][MAX_NODOS]) {
    for(int i = 0; i < MAX_NODOS; i++) {
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
        printf("Saliendo del programa...\n");
        exit(1);
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
    *taxis_totales = id - 1;
    return;
};

// Cargar las solicitudes desde el archivo
void cargar_solicitudes(Cola *solicitudes, char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
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

void cargar_nodos(int nodos_ids[MAX_NODOS], char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
        return;
    }

    // Inicializar el array de nodos
    for(int i = 0; i < MAX_NODOS; i++) {
        nodos_ids[i] = -1;
    }

    int x, y;
    int id = 0;

    while (fscanf(archivo, "%d %d", &x, &y) == 2 && id < MAX_NODOS) {
        nodos_ids[id] = y * SIZE_MAP + x;
        id++;
    };

    fclose(archivo);
    return;
};

/**
 * Agrega un nuevo taxi al sistema
 */
int agregar_taxi(Taxi *taxis, int mapa_taxis[SIZE_MAP][SIZE_MAP], char mapa[SIZE_MAP][SIZE_MAP], int *taxis_totales) {
    // Verificar si hay espacio para más taxis
    if (*taxis_totales >= MAX_TAXIS) {
        printf("Error: No se pueden agregar más taxis. Máximo alcanzado (%d/%d).\n", *taxis_totales, MAX_TAXIS);
        return 0;
    }
    
    int x, y;
    
    printf("\n===== Agregar Nuevo Taxi =====\n");
    printf("Ingrese las coordenadas (x,y) para el nuevo taxi.\n");
    
    // Solicitar y validar coordenada x
    do {
        printf("Coordenada X (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &x) != 1) {
            // Limpiar el buffer de entrada si no se ingresó un número
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            x = -1;
            continue;
        }
        
        if (x < 0 || x >= SIZE_MAP) {
            printf("Error: La coordenada X debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (x < 0 || x >= SIZE_MAP);
    
    // Solicitar y validar coordenada y
    do {
        printf("Coordenada Y (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &y) != 1) {
            // Limpiar el buffer de entrada si no se ingresó un número
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            y = -1;
            continue;
        }
        
        if (y < 0 || y >= SIZE_MAP) {
            printf("Error: La coordenada Y debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (y < 0 || y >= SIZE_MAP);
    
    // Verificar que la posición no esté ocupada por otro taxi
    if (mapa_taxis[x][y] != -1) {
        printf("Error: Ya hay un taxi en la posición (%d, %d).\n", x, y);
        return 0;
    }
    
    // Verificar que la posición sea válida en el mapa (debe ser un nodo)
    if (mapa[y][x] != 'X') {
        printf("Error: La posición (%d, %d) no es un nodo válido en el mapa.\n", x, y);
        printf("El taxi debe colocarse en un nodo (marcado con 'X' en el mapa).\n");
        return 0;
    }
    
    // Crear el nuevo taxi
    int nuevo_id = *taxis_totales + 1;
    taxis[*taxis_totales].id = nuevo_id;
    taxis[*taxis_totales].x = x;
    taxis[*taxis_totales].y = y;
    taxis[*taxis_totales].estado = 0; // Disponible
    taxis[*taxis_totales].tiempo = 0;
    taxis[*taxis_totales].x_destino = 0;
    taxis[*taxis_totales].y_destino = 0;
    
    // Actualizar el mapa de taxis
    mapa_taxis[x][y] = nuevo_id - 1; // El índice en el arreglo es id-1
    
    // Incrementar el contador de taxis
    (*taxis_totales)++;
    
    printf("Taxi #%d agregado correctamente en la posición (%d, %d).\n", nuevo_id, x, y);
    printf("Total de taxis: %d/%d\n", *taxis_totales, MAX_TAXIS);
    
    return 1;
}

/**
 * Agrega una nueva solicitud de viaje
 */
int agregar_solicitud(Cola *solicitudes, char mapa[SIZE_MAP][SIZE_MAP]) {
    // Verificar si hay espacio para más solicitudes
    if (esta_llena(solicitudes)) {
        printf("Error: No se pueden agregar más solicitudes. Máximo alcanzado (%d/%d).\n", 
               solicitudes->cantidad, MAX_SOLICITUDES);
        return 0;
    }
    
    int x_origen, y_origen, x_destino, y_destino;
    
    printf("\n===== Agregar Nueva Solicitud =====\n");
    printf("Ingrese las coordenadas de origen y destino.\n");
    
    // Solicitar y validar coordenadas de origen (x, y)
    printf("\n--- Coordenadas de ORIGEN ---\n");
    
    // Solicitar y validar coordenada x de origen
    do {
        printf("Coordenada X de origen (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &x_origen) != 1) {
            // Limpiar el buffer de entrada si no se ingresó un número
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            x_origen = -1;
            continue;
        }
        
        if (x_origen < 0 || x_origen >= SIZE_MAP) {
            printf("Error: La coordenada X debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (x_origen < 0 || x_origen >= SIZE_MAP);
    
    // Solicitar y validar coordenada y de origen
    do {
        printf("Coordenada Y de origen (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &y_origen) != 1) {
            // Limpiar el buffer de entrada si no se ingresó un número
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            y_origen = -1;
            continue;
        }
        
        if (y_origen < 0 || y_origen >= SIZE_MAP) {
            printf("Error: La coordenada Y debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (y_origen < 0 || y_origen >= SIZE_MAP);
    
    // Verificar que la posición de origen sea un nodo válido
    if (mapa[y_origen][x_origen] != 'X') {
        printf("Error: La posición de origen (%d, %d) no es un nodo válido en el mapa.\n", x_origen, y_origen);
        printf("El origen debe ser un nodo (marcado con 'X' en el mapa).\n");
        return 0;
    }
    
    // Solicitar y validar coordenadas de destino (x, y)
    printf("\n--- Coordenadas de DESTINO ---\n");
    
    // Solicitar y validar coordenada x de destino
    do {
        printf("Coordenada X de destino (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &x_destino) != 1) {
            // Limpiar el buffer de entrada si no se ingresó un número
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            x_destino = -1;
            continue;
        }
        
        if (x_destino < 0 || x_destino >= SIZE_MAP) {
            printf("Error: La coordenada X debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (x_destino < 0 || x_destino >= SIZE_MAP);
    
    // Solicitar y validar coordenada y de destino
    do {
        printf("Coordenada Y de destino (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &y_destino) != 1) {
            // Limpiar el buffer de entrada si no se ingresó un número
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            y_destino = -1;
            continue;
        }
        
        if (y_destino < 0 || y_destino >= SIZE_MAP) {
            printf("Error: La coordenada Y debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (y_destino < 0 || y_destino >= SIZE_MAP);
    
    // Verificar que la posición de destino sea un nodo válido
    if (mapa[y_destino][x_destino] != 'X') {
        printf("Error: La posición de destino (%d, %d) no es un nodo válido en el mapa.\n", x_destino, y_destino);
        printf("El destino debe ser un nodo (marcado con 'X' en el mapa).\n");
        return 0;
    }
    
    // Verificar que el origen y destino sean diferentes
    if (x_origen == x_destino && y_origen == y_destino) {
        printf("Error: El origen y el destino deben ser diferentes.\n");
        return 0;
    }
    
    // Crear nueva solicitud
    Solicitud nueva_solicitud;
    nueva_solicitud.x_origen = x_origen;
    nueva_solicitud.y_origen = y_origen;
    nueva_solicitud.x_destino = x_destino;
    nueva_solicitud.y_destino = y_destino;
    
    // Determinar el ID para la nueva solicitud
    // Usamos la cantidad actual + 1 como ID
    nueva_solicitud.id = solicitudes->cantidad + 1;
    
    // Agregar la solicitud a la cola
    if (!apilar(solicitudes, nueva_solicitud)) {
        printf("Error: No se pudo agregar la solicitud a la cola.\n");
        return 0;
    }
    
    printf("Solicitud #%d agregada correctamente.\n", nueva_solicitud.id);
    printf("Origen: (%d, %d), Destino: (%d, %d)\n", x_origen, y_origen, x_destino, y_destino);
    printf("Total de solicitudes: %d/%d\n", solicitudes->cantidad, MAX_SOLICITUDES);
    
    return 1;
}

/**
 * Muestra los nodos cargados en el programa
 */
void mostrar_nodos(int nodos_ids[MAX_NODOS]) {
    printf("\n===== Nodos Cargados =====\n");
    printf("ID\tPosición (X,Y)\n");
    printf("------------------------\n");
    
    int contador = 0;
    
    for (int i = 0; i < MAX_NODOS; i++) {
        if (nodos_ids[i] != -1) {
            // Convertir el ID codificado de vuelta a coordenadas X,Y
            int y = nodos_ids[i] / SIZE_MAP;
            int x = nodos_ids[i] % SIZE_MAP;
            
            printf("%d\t(%d, %d)\n", i+1, x, y);
            contador++;
        }
    }
    
    printf("------------------------\n");
    printf("Total de nodos: %d\n", contador);
}