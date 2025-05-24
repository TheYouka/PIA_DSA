/*
 * Proyecto.c - Implementación completa del sistema de taxis
 * Este archivo combina todas las funcionalidades del proyecto en un solo archivo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>  // Para INT_MAX

/* Constantes del sistema */
#define MAX_NODOS 50
#define MAX_SOLICITUDES 50
#define SIZE_MAP 25
#define MAX_TAXIS 50
#define DISTANCIA_UNIDAD 10

/* Estructuras del sistema */
typedef struct {
    int id;
    int x;
    int y;
    int estado;      // 0: disponible, 1: ocupado
    int tiempo;      // Tiempo restante hasta que el taxi complete su viaje
    int x_destino;   // Coordenada x de destino cuando está ocupado
    int y_destino;   // Coordenada y de destino cuando está ocupado
} Taxi;

typedef struct {
    int x_origen;    // Nodo de origen del pasajero
    int y_origen;    // Nodo de destino del pasajero
    int x_destino;   // Nodo de destino del pasajero
    int y_destino;   // Nodo de destino del pasajero
    int id;          // Identificador único de la solicitud
} Solicitud;

typedef struct {
    Solicitud elementos[MAX_SOLICITUDES];  // Array de solicitudes
    int frente;                            // Índice del frente de la cola
    int final;                             // Índice del final de la cola
    int cantidad;                          // Cantidad de elementos en la cola
} Cola;

/* Prototipos de funciones */
// Funciones de cola
void inicializar_cola(Cola *cola);
int esta_vacia(Cola *cola);
int esta_llena(Cola *cola);
int apilar(Cola *cola, Solicitud solicitud);
int desapilar(Cola *cola, Solicitud *solicitud);
int frente(Cola *cola, Solicitud *solicitud);
void mostrar_cola(Cola *cola);

// Funciones de visualización
void imprimir_mapa(char (*mapa)[SIZE_MAP]);
int mostrar_menu_principal(void);
void mostrar_nodos(int *nodos_ids);

// Funciones de archivos
void cargar_nodos(int *nodos_ids, const char *nombre_archivo);
void cargar_taxis(Taxi *taxis, int (*mapa_taxis)[SIZE_MAP], const char *nombre_archivo, int *taxis_totales);
void cargar_mapa(char (*mapa)[SIZE_MAP], int *nodos_ids, int (*calles)[MAX_NODOS], const char *nombre_archivo);
void cargar_solicitudes(Cola *solicitudes, const char *nombre_archivo);
int agregar_taxi(Taxi *taxis, int (*mapa_taxis)[SIZE_MAP], char (*mapa)[SIZE_MAP], int *taxis_totales);
int agregar_solicitud(Cola *solicitudes, char (*mapa)[SIZE_MAP]);

// Funciones de Dijkstra
void inicializar_calles(int (*calles)[MAX_NODOS]);
void dijkstra(int (*calles)[MAX_NODOS], int inicio, int *distancias, int *predecesores);
int obtener_ruta(int (*calles)[MAX_NODOS], int *nodos_ids, 
                int x_origen, int y_origen, int x_destino, int y_destino, 
                int *ruta, int *longitud);

// Funciones de simulación
void ejecutar_simulacion(Taxi *taxis, int *nodos_ids, int (*calles)[MAX_NODOS], 
                        char (*mapa)[SIZE_MAP], int (*mapa_taxis)[SIZE_MAP], Cola *solicitudes);

int encontrar_taxi_cercano(Taxi *taxis, int *nodos_ids, int (*calles)[MAX_NODOS], int x_origen, int y_origen);

/* Implementación de funciones */

/* Implementación de funciones de cola */
void inicializar_cola(Cola *cola) {
    cola->frente = 0;
    cola->final = -1;
    cola->cantidad = 0;
}

int esta_vacia(Cola *cola) {
    return cola->cantidad == 0;
}

int esta_llena(Cola *cola) {
    return cola->cantidad == MAX_SOLICITUDES;
}

int apilar(Cola *cola, Solicitud solicitud) {
    if (esta_llena(cola)) {
        return 0; // Cola llena, no se puede agregar
    }
    
    // Incrementa el índice final de forma circular
    cola->final = (cola->final + 1) % MAX_SOLICITUDES;
    
    // Agrega la solicitud al final de la cola
    cola->elementos[cola->final] = solicitud;
    
    // Incrementa la cantidad de elementos
    cola->cantidad++;
    
    return 1; // Operación exitosa
}

int desapilar(Cola *cola, Solicitud *solicitud) {
    if (esta_vacia(cola)) {
        return 0; // Cola vacía, no se puede eliminar
    }
    
    // Obtiene la solicitud del frente
    *solicitud = cola->elementos[cola->frente];
    
    // Incrementa el índice frente de forma circular
    cola->frente = (cola->frente + 1) % MAX_SOLICITUDES;
    
    // Decrementa la cantidad de elementos
    cola->cantidad--;
    
    return 1; // Operación exitosa
}

int frente(Cola *cola, Solicitud *solicitud) {
    if (esta_vacia(cola)) {
        return 0; // Cola vacía, no hay frente
    }
    
    // Obtiene la solicitud del frente sin eliminarla
    *solicitud = cola->elementos[cola->frente];
    
    return 1; // Operación exitosa
}

void mostrar_cola(Cola *cola) {
    if (esta_vacia(cola)) {
        printf("La cola de solicitudes está vacía.\n");
        return;
    }
    
    printf("\n--- COLA DE SOLICITUDES ---\n");
    printf("Total de solicitudes: %d\n\n", cola->cantidad);
    
    int indice = cola->frente;
    int i;
    for (i = 0; i < cola->cantidad; i++) {
        Solicitud solicitud = cola->elementos[indice];
        printf("Solicitud %d:\n", solicitud.id);
        printf("  Origen: X : %d  Y: %d\n", solicitud.x_origen, solicitud.y_origen);
        printf("  Destino: X : %d  Y : %d \n\n", solicitud.x_destino, solicitud.y_destino);
        
        // Avanza de forma circular
        indice = (indice + 1) % MAX_SOLICITUDES;
    }
    
    printf("-------------------------\n");
}

/* Implementación de funciones de visualización */
int mostrar_menu_principal() {
    int opcion = 0;
    
    while (opcion < 1 || opcion > 8) {
        // Limpiar pantalla
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        
        printf("\033[1;33m"); // Texto amarillo brillante
        printf("╔══════════════════════════════════════════════════╗\n");
        printf("║              \033[1;36mMINI-UBER SIMULATION\033[1;33m                ║\n");
        printf("╠══════════════════════════════════════════════════╣\n");
        printf("║                                                  ║\n");
        printf("║  \033[1;37m1.\033[0m\033[1;32m Iniciar Simulación\033[1;33m                           ║\n");
        printf("║  \033[1;37m2.\033[0m\033[1;32m Agregar Taxis\033[1;33m                                ║\n");
        printf("║  \033[1;37m3.\033[0m\033[1;32m Agregar Solicitudes\033[1;33m                          ║\n");
        printf("║  \033[1;37m4.\033[0m\033[1;32m Ver Taxis\033[1;33m                                    ║\n");
        printf("║  \033[1;37m5.\033[0m\033[1;32m Ver Mapa\033[1;33m                                     ║\n");
        printf("║  \033[1;37m6.\033[0m\033[1;32m Ver Solicitudes\033[1;33m                              ║\n");
        printf("║  \033[1;37m7.\033[0m\033[1;32m Ver nodos\033[1;33m                                    ║\n");
        printf("║  \033[1;37m8.\033[0m\033[1;31m Salir\033[1;33m                                        ║\n");
        printf("║                                                  ║\n");
        printf("╚══════════════════════════════════════════════════╝\n");
        printf("\033[0m"); // Resetear color
        
        printf("\n\033[1;36mSeleccione una opción (1-8): \033[0m");
        if (scanf("%d", &opcion) != 1) {
            // Limpiar el buffer de entrada en caso de entrada no válida
            while (getchar() != '\n');
            opcion = 0; // Forzar repetición del bucle
        }
        
        if (opcion < 1 || opcion > 8) {
            printf("\n\033[1;31m[ERROR] Opción inválida. Presione Enter para continuar...\033[0m");
            getchar(); // Capturar el Enter después del número
            getchar(); // Esperar otro Enter para continuar
        }
    }
    
    return opcion;
}

void imprimir_mapa(char (*mapa)[SIZE_MAP]) {
    int i, j;
    for (i = 0; i < SIZE_MAP; i++) {
        for (j = 0; j < SIZE_MAP; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

void mostrar_nodos(int *nodos_ids) {
    int i, x, y;
    printf("\n--- NODOS DISPONIBLES ---\n");
    for (i = 0; i < MAX_NODOS; i++) {
        if (nodos_ids[i] != -1) {
            x = nodos_ids[i] % SIZE_MAP;
            y = nodos_ids[i] / SIZE_MAP;
            printf("Nodo %d: (%d, %d)\n", i, x, y);
        }
    }
    printf("-------------------------\n");
}

/* Implementación de funciones de Dijkstra */
#define INFINITO INT_MAX

int cords_a_absoluto(int x, int y) {
    return y * SIZE_MAP + x;
}

void absoluto_a_coords(int absolute, int *x, int *y) {
    *y = absolute / SIZE_MAP;
    *x = absolute % SIZE_MAP;
}

int buscar_id_nodo(int nodos_ids[MAX_NODOS], int x, int y) {
    int pos_absoluta = cords_a_absoluto(x, y);
    int i;
    for (i = 0; i < MAX_NODOS; i++) {
        if (nodos_ids[i] == pos_absoluta) {
            return i;
        }
    }
    
    // No se encontró el nodo
    return -1;
}

int minima_distancia(int distancias[MAX_NODOS], int visitado[MAX_NODOS]) {
    int min = INFINITO;
    int min_index = -1;
    int i;
    for (i = 0; i < MAX_NODOS; i++) {
        if (visitado[i] == 0 && distancias[i] < min) {
            min = distancias[i];
            min_index = i;
        }
    }
    
    return min_index;
}

void dijkstra(int (*calles)[MAX_NODOS], int inicio, int *distancias, int *predecesores) {
    int visitado[MAX_NODOS];
    int i, count, u, v;
    
    // Inicializar distancias, predecesores y visitado
    for (i = 0; i < MAX_NODOS; i++) {
        distancias[i] = INFINITO;
        predecesores[i] = -1;
        visitado[i] = 0;
    }
    
    // La distancia del nodo inicial a sí mismo es 0
    distancias[inicio] = 0;
    
    // Encontrar el camino más corto para todos los nodos
    for (count = 0; count < MAX_NODOS - 1; count++) {
        // Elegir el nodo con la distancia mínima del conjunto de nodos no visitados
        u = minima_distancia(distancias, visitado);
        
        // Si no hay más nodos alcanzables, salir del bucle
        if (u == -1) {
            break;
        }
        
        // Marcar el nodo elegido como visitado
        visitado[u] = 1;
        
        // Actualizar las distancias de los nodos adyacentes al nodo elegido
        for (v = 0; v < MAX_NODOS; v++) {
            // Actualizar distancia[v] solo si:
            // 1. Hay una arista de u a v (calles[u][v] != 0)
            // 2. v no ha sido visitado
            // 3. La distancia total a través de u es menor que la distancia actual de v
            if (calles[u][v] != 0 && visitado[v] == 0 && 
                distancias[u] != INFINITO && 
                distancias[u] + calles[u][v] < distancias[v]) {
                distancias[v] = distancias[u] + calles[u][v];
                predecesores[v] = u;
            }
        }
    }
}

int obtener_ruta(int (*calles)[MAX_NODOS], int *nodos_ids, 
                int x_origen, int y_origen, int x_destino, int y_destino, 
                int *ruta, int *longitud) {
    int id_nodo_origen, id_nodo_destino;
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    int temp_ruta[MAX_NODOS];
    int temp_longitud = 0;
    int actual, i;
    
    id_nodo_origen = buscar_id_nodo(nodos_ids, x_origen, y_origen);
    id_nodo_destino = buscar_id_nodo(nodos_ids, x_destino, y_destino);
    
    // Si alguno de los nodos no existe, no hay ruta
    if (id_nodo_origen == -1 || id_nodo_destino == -1) {
        *longitud = 0;
        return -1;
    }
    
    // Aplicar Dijkstra desde el nodo de origen
    dijkstra(calles, id_nodo_origen, distancias, predecesores);
    
    // Si no hay ruta al destino
    if (distancias[id_nodo_destino] == INFINITO) {
        *longitud = 0;
        return -1;
    }
    
    // Reconstruir la ruta desde el destino hacia el origen
    actual = id_nodo_destino;
    
    while (actual != -1) {
        temp_ruta[temp_longitud++] = actual;
        actual = predecesores[actual];
    }
    
    // Invertir la ruta (de origen a destino)
    *longitud = temp_longitud;
    for (i = 0; i < temp_longitud; i++) {
        ruta[i] = temp_ruta[temp_longitud - 1 - i];
    }
    
    return distancias[id_nodo_destino];
}

void inicializar_calles(int (*calles)[MAX_NODOS]) {
    int i, j;
    for (i = 0; i < MAX_NODOS; i++) {
        for (j = 0; j < MAX_NODOS; j++) {
            calles[i][j] = 0;
        }
    }
}

/* Implementación de funciones de simulación */
int procesar_solicitud(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], Solicitud solicitud) {
    printf("\n===== Procesando Solicitud #%d =====\n", solicitud.id);
    printf("Origen: (%d, %d), Destino: (%d, %d)\n", solicitud.x_origen, solicitud.y_origen, solicitud.x_destino, solicitud.y_destino);
    
    // Buscar el taxi más cercano al origen del pasajero
    int id_taxi = encontrar_taxi_cercano(taxis, nodos_ids, calles, solicitud.x_origen, solicitud.y_origen);
    
    if (id_taxi == -1) {
        printf("No hay taxis disponibles para atender la solicitud.\n");
        return 0;
    }
    
    // El índice en el array de taxis es id_taxi - 1 (ya que los IDs empiezan en 1)
    int indice_taxi = id_taxi - 1;
    
    printf("Taxi #%d asignado. Ubicación actual: (%d, %d)\n", 
           taxis[indice_taxi].id, taxis[indice_taxi].x, taxis[indice_taxi].y);
    
    // Calcular ruta del taxi al pasajero y luego al destino
    int ruta_taxi_pasajero[MAX_NODOS];
    int longitud_taxi_pasajero;
    int distancia_taxi_pasajero = obtener_ruta(calles, nodos_ids, 
                                              taxis[indice_taxi].x, taxis[indice_taxi].y, 
                                              solicitud.x_origen, solicitud.y_origen,
                                              ruta_taxi_pasajero, &longitud_taxi_pasajero);
    
    if (distancia_taxi_pasajero == -1) {
        printf("No hay ruta disponible del taxi al pasajero.\n");
        return 0;
    }
    
    // Calcular ruta del pasajero a su destino
    int ruta_pasajero_destino[MAX_NODOS];
    int longitud_pasajero_destino;
    int distancia_pasajero_destino = obtener_ruta(calles, nodos_ids, 
                                                 solicitud.x_origen, solicitud.y_origen, 
                                                 solicitud.x_destino, solicitud.y_destino,
                                                 ruta_pasajero_destino, &longitud_pasajero_destino);
    
    if (distancia_pasajero_destino == -1) {
        printf("No hay ruta disponible del origen al destino del pasajero.\n");
        return 0;
    }
    
    // Tiempo total = distancia al pasajero + distancia del pasajero al destino
    int tiempo_total = (distancia_taxi_pasajero + distancia_pasajero_destino)/DISTANCIA_UNIDAD;
    
    // Actualizar el estado del taxi
    taxis[indice_taxi].estado = 1; // 1 = ocupado
    taxis[indice_taxi].tiempo = tiempo_total;
    taxis[indice_taxi].x_destino = solicitud.x_destino;
    taxis[indice_taxi].y_destino = solicitud.y_destino;
    
    printf("Tiempo total del viaje: %d unidades\n", tiempo_total);

    // Imprimir la distancia de la ruta del taxi al pasajero y la distancia del pasajero al destino
    printf("Distancia del taxi al pasajero: %d\n", distancia_taxi_pasajero);
    printf("Distancia del pasajero al destino: %d\n", distancia_pasajero_destino);
    printf("El taxi estará ocupado durante %d etapas.\n", tiempo_total);
    
    return 1;
}

int actualizar_taxis(Taxi taxis[MAX_TAXIS], int mapa_taxis[SIZE_MAP][SIZE_MAP]) {
    int taxis_completados = 0;
    int i;
    printf("\n===== Actualizando Estado de Taxis =====\n");
    
    for (i = 0; i < MAX_TAXIS; i++) {
        if (taxis[i].id == -1) continue; // Taxi no inicializado
        
        if (taxis[i].estado == 1) { // Taxi ocupado
            // Reducir el tiempo restante
            taxis[i].tiempo--;
            
            printf("Taxi #%d: Tiempo restante: %d\n", taxis[i].id, taxis[i].tiempo);
            
            // Si el tiempo llegó a cero, el taxi ha completado su viaje
            if (taxis[i].tiempo <= 0) {
                printf("Taxi #%d ha completado su viaje. Nueva posición: (%d, %d)\n", 
                       taxis[i].id, taxis[i].x_destino, taxis[i].y_destino);
                
                // Eliminar el taxi de su posición anterior en el mapa
                mapa_taxis[taxis[i].x][taxis[i].y] = -1;
                
                // Actualizar coordenadas y estado
                taxis[i].x = taxis[i].x_destino;
                taxis[i].y = taxis[i].y_destino;
                taxis[i].estado = 0; // Disponible de nuevo
                
                // Actualizar el mapa de taxis
                mapa_taxis[taxis[i].x][taxis[i].y] = taxis[i].id;
                
                taxis_completados++;
            }
        }
    }
    
    return taxis_completados;
}

void mostrar_info_taxis(Taxi taxis[MAX_TAXIS]) {
    printf("\n===== Estado Actual de Taxis =====\n");
    
    int disponibles = 0;
    int ocupados = 0;
    int i;
    for (i = 0; i < MAX_TAXIS; i++) {
        if (taxis[i].id != -1) {
            printf("Taxi #%d: Posición (%d, %d), Estado: %s", 
                   taxis[i].id, taxis[i].x, taxis[i].y, 
                   taxis[i].estado == 0 ? "Disponible" : "Ocupado");
            
            if (taxis[i].estado == 1) {
                printf(", Destino: (%d, %d), Tiempo restante: %d",
                       taxis[i].x_destino, taxis[i].y_destino, taxis[i].tiempo);
                ocupados++;
            } else {
                disponibles++;
            }
            
            printf("\n");
        }
    }
    
    printf("\nTaxis disponibles: %d, Taxis ocupados: %d\n", disponibles, ocupados);
}

void ejecutar_simulacion(Taxi *taxis, int *nodos_ids, int (*calles)[MAX_NODOS], 
                        char (*mapa)[SIZE_MAP], int (*mapa_taxis)[SIZE_MAP], Cola *solicitudes) {
    int etapa = 1;
    int simulacion_completa = 0;
    
    printf("\n===== INICIANDO SIMULACIÓN =====\n");
    
    while (!simulacion_completa) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        printf("\n===== ETAPA %d =====\n", etapa);
        
        // 1. Actualizar el estado de los taxis
        int taxis_completados = actualizar_taxis(taxis, mapa_taxis);
        if (taxis_completados > 0) {
            printf("%d taxi(s) completaron su viaje en esta etapa.\n", taxis_completados);
        }
        
        // 2. Procesar una solicitud si hay alguna pendiente
        if (!esta_vacia(solicitudes)) {
            Solicitud solicitud;
            if (frente(solicitudes, &solicitud)) {
                int resultado = procesar_solicitud(taxis, nodos_ids, calles, solicitud);
                if (resultado) {
                    // Si se procesó correctamente, desapilar la solicitud
                    desapilar(solicitudes, &solicitud);
                    printf("Solicitud #%d procesada correctamente.\n", solicitud.id);
                } else {
                    printf("No se pudo procesar la solicitud #%d. Se intentará en la siguiente etapa.\n", solicitud.id);
                }
            }
        } else {
            printf("No hay más solicitudes pendientes.\n");
            
            // Verificar si todos los taxis están disponibles
            int todos_disponibles = 1;
            int i;
            for (i = 0; i < MAX_TAXIS; i++) {
                if (taxis[i].id != -1 && taxis[i].estado == 1) {
                    todos_disponibles = 0;
                    break;
                }
            }
            
            if (todos_disponibles) {
                printf("Todos los taxis están disponibles. Simulación completa.\n");
                simulacion_completa = 1;
            }
        }
        
        // 3. Mostrar información actual
        mostrar_info_taxis(taxis);
        
        if (!simulacion_completa) {
            // Esperar a que el usuario presione ENTER para continuar
            printf("\nPresione ENTER para avanzar a la siguiente etapa...");
            getchar();
            etapa++;
        }
    }
    
    printf("\n===== SIMULACIÓN FINALIZADA =====\n");
    printf("Total de etapas: %d\n", etapa);
}

/* Implementación de funciones de archivos */
void cargar_mapa(char (*mapa)[SIZE_MAP], int *nodos_ids, int (*calles)[MAX_NODOS], const char *nombre_archivo) {
    FILE *archivo;
    int i, j, x1, y1, x2, y2, peso;
    int id_1, id_2, pos1, pos2;
    
    archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
    }

    // Inicializar el mapa con ceros
    for (i = 0; i < SIZE_MAP; i++) {
        for (j = 0; j < SIZE_MAP; j++) {
            mapa[i][j] = '.';
        }
    }
    
    // Leer entradas del archivo y guardar en el mapa y calles
    while (fscanf(archivo, "%d %d %d %d %d", &x1, &y1, &x2, &y2, &peso) == 5) {
        id_1 = -1;
        id_2 = -1;
        pos1 = y1 * SIZE_MAP + x1;
        pos2 = y2 * SIZE_MAP + x2;

        // Buscar los IDs correspondientes a las posiciones en nodos_ids
        for (i = 0; i < MAX_NODOS; i++) {
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
}

void cargar_taxis(Taxi *taxis, int (*mapa_taxis)[SIZE_MAP], const char *nombre_archivo, int *taxis_totales) {
    FILE *archivo;
    int i, x, y, id;
    
    archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
    }

    // Inicializar el array de taxis
    for(i = 0; i < MAX_TAXIS; i++) {
        taxis[i].id = -1;
        taxis[i].x = -1;
        taxis[i].y = -1;
        taxis[i].estado = -1;
    }

    id = 1;
    
    // Leer coordenadas del archivo y cargar los taxis
    while (fscanf(archivo, "%d %d", &x, &y) == 2 && id <= MAX_TAXIS) {
        taxis[id-1].id = id;
        taxis[id-1].x = x;
        taxis[id-1].y = y;
        taxis[id-1].estado = 0; // 0 indica taxi libre
        id++;

        // Guardar el id del taxi en la posicion del mapa
        mapa_taxis[x][y] = id-1;
    }

    fclose(archivo);
    *taxis_totales = id - 1;
}

void cargar_solicitudes(Cola *solicitudes, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
    }

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
    }

    fclose(archivo);
}

void cargar_nodos(int *nodos_ids, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombre_archivo);
        printf("Saliendo del programa...\n");
        exit(1);
    }

    // Inicializar el array de nodos
    int i;
    for(i = 0; i < MAX_NODOS; i++) {
        nodos_ids[i] = -1;
    }

    int x, y;
    int id = 0;

    while (fscanf(archivo, "%d %d", &x, &y) == 2 && id < MAX_NODOS) {
        nodos_ids[id] = y * SIZE_MAP + x;
        id++;
    }

    fclose(archivo);
}

int agregar_taxi(Taxi *taxis, int (*mapa_taxis)[SIZE_MAP], char (*mapa)[SIZE_MAP], int *taxis_totales) {
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

int agregar_solicitud(Cola *solicitudes, char (*mapa)[SIZE_MAP]) {
    // Verificar si hay espacio para más solicitudes
    if (esta_llena(solicitudes)) {
        printf("Error: No se pueden agregar más solicitudes. Máximo alcanzado (%d/%d).\n", 
               solicitudes->cantidad, MAX_SOLICITUDES);
        return 0;
    }
    
    int x_origen, y_origen, x_destino, y_destino;
    
    printf("\n===== Agregar Nueva Solicitud =====\n");
    printf("Ingrese las coordenadas de origen y destino.\n");
    
    // Solicitar y validar coordenadas de origen
    do {
        printf("Origen X (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &x_origen) != 1) {
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            x_origen = -1;
            continue;
        }
        
        if (x_origen < 0 || x_origen >= SIZE_MAP) {
            printf("Error: La coordenada X debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (x_origen < 0 || x_origen >= SIZE_MAP);
    
    do {
        printf("Origen Y (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &y_origen) != 1) {
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            y_origen = -1;
            continue;
        }
        
        if (y_origen < 0 || y_origen >= SIZE_MAP) {
            printf("Error: La coordenada Y debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (y_origen < 0 || y_origen >= SIZE_MAP);
    
    // Verificar que el origen sea un nodo válido
    if (mapa[y_origen][x_origen] != 'X') {
        printf("Error: El origen (%d, %d) no es un nodo válido en el mapa.\n", x_origen, y_origen);
        return 0;
    }
    
    // Solicitar y validar coordenadas de destino
    do {
        printf("Destino X (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &x_destino) != 1) {
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            x_destino = -1;
            continue;
        }
        
        if (x_destino < 0 || x_destino >= SIZE_MAP) {
            printf("Error: La coordenada X debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (x_destino < 0 || x_destino >= SIZE_MAP);
    
    do {
        printf("Destino Y (0-%d): ", SIZE_MAP - 1);
        if (scanf("%d", &y_destino) != 1) {
            while (getchar() != '\n');
            printf("Error: Ingrese un número válido.\n");
            y_destino = -1;
            continue;
        }
        
        if (y_destino < 0 || y_destino >= SIZE_MAP) {
            printf("Error: La coordenada Y debe estar entre 0 y %d.\n", SIZE_MAP - 1);
        }
    } while (y_destino < 0 || y_destino >= SIZE_MAP);
    
    // Verificar que el destino sea un nodo válido
    if (mapa[y_destino][x_destino] != 'X') {
        printf("Error: El destino (%d, %d) no es un nodo válido en el mapa.\n", x_destino, y_destino);
        return 0;
    }
    
    // Verificar que origen y destino sean diferentes
    if (x_origen == x_destino && y_origen == y_destino) {
        printf("Error: El origen y destino no pueden ser el mismo punto.\n");
        return 0;
    }
    
    // Crear la nueva solicitud
    Solicitud solicitud;
    solicitud.x_origen = x_origen;
    solicitud.y_origen = y_origen;
    solicitud.x_destino = x_destino;
    solicitud.y_destino = y_destino;
    solicitud.id = solicitudes->cantidad + 1;
    
    // Agregar la solicitud a la cola
    if (apilar(solicitudes, solicitud)) {
        printf("Solicitud #%d agregada correctamente.\n", solicitud.id);
        printf("Origen: (%d, %d), Destino: (%d, %d)\n", 
               solicitud.x_origen, solicitud.y_origen, 
               solicitud.x_destino, solicitud.y_destino);
        return 1;
    } else {
        printf("Error al agregar la solicitud.\n");
        return 0;
    }
}

int encontrar_taxi_cercano(Taxi *taxis, int *nodos_ids, int (*calles)[MAX_NODOS], int x_origen, int y_origen) {
    int id_nodo_origen = buscar_id_nodo(nodos_ids, x_origen, y_origen);
    
    // Si no existe el nodo de origen, no se puede buscar un taxi
    if (id_nodo_origen == -1) {
        return -1;
    }
    
    // Aplicar Dijkstra desde el nodo de origen
    int distancias[MAX_NODOS];
    int predecesores[MAX_NODOS];
    dijkstra(calles, id_nodo_origen, distancias, predecesores);
    
    // Buscar el taxi más cercano
    int min_distancia = INFINITO;
    int id_taxi_cercano = -1;
    int i;
    for (i = 0; i < MAX_TAXIS; i++) {
        // Solo considerar taxis disponibles
        if (taxis[i].id != -1 && taxis[i].estado == 0) {
            int id_nodo_taxi = buscar_id_nodo(nodos_ids, taxis[i].x, taxis[i].y);
            
            // Si el taxi está en un nodo válido y es alcanzable
            if (id_nodo_taxi != -1 && distancias[id_nodo_taxi] != INFINITO) {
                if (distancias[id_nodo_taxi] < min_distancia) {
                    min_distancia = distancias[id_nodo_taxi];
                    id_taxi_cercano = taxis[i].id;
                }
            }
        }
    }
    
    // Regresa el id del taxi más cercano del array de taxis
    return id_taxi_cercano;
}

int main() {
    int opcion, i, j;
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
    for (i = 0; i < SIZE_MAP; i++) {
        for (j = 0; j < SIZE_MAP; j++) {
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
    
    // Inicializar los nuevos campos de los taxis
    for(i = 0; i < MAX_TAXIS; i++) {
        if (taxis[i].id != -1) {
            taxis[i].tiempo = 0;
            taxis[i].x_destino = 0;
            taxis[i].y_destino = 0;
        }
    }
    
    do {
        opcion = mostrar_menu_principal();
        
        switch (opcion) {
            case 1:
                printf("\nIniciando simulación por etapas...\n");
                printf("Presione ENTER para comenzar.\n");
                getchar();
                getchar();
                
                // Ejecutar la simulación
                ejecutar_simulacion(taxis, nodos_ids, calles, mapa, mapa_taxis, &solicitudes);
                
                printf("Simulación finalizada. Presione ENTER para continuar...");
                getchar();
                break;
                
            case 2:
                printf("\nAgregar taxis...\n");
                
                // Mostrar el mapa para que el usuario vea los nodos disponibles
                printf("Mostrando mapa (los nodos están marcados con 'X'):\n");
                imprimir_mapa(mapa);
                
                // Intentar agregar un nuevo taxi
                agregar_taxi(taxis, mapa_taxis, mapa, &taxis_totales);
                
                printf("\nPresione Enter para continuar...");
                getchar();
                getchar();
                break;
                
            case 3:
                printf("\nAgregar solicitudes...\n");
                agregar_solicitud(&solicitudes, mapa);
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
                
            case 4:
                printf("\nMostrando taxis disponibles...\n");
                // Mostrar información de taxis
                for (i = 0; i < MAX_TAXIS; i++) {
                    if (taxis[i].id != -1) {
                        printf("Taxi #%d: Posición (%d, %d), Estado: %s\n", 
                               taxis[i].id, taxis[i].x, taxis[i].y, 
                               taxis[i].estado == 0 ? "Disponible" : "Ocupado");
                    }
                }
                printf("Presione Enter para continuar...");
                getchar();
                getchar();
                break;
                
            case 5:
                printf("\nMostrando mapa...\n");
                imprimir_mapa(mapa);
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
                printf("\nMostrando nodos...\n");
                mostrar_nodos(nodos_ids);
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
    } while (opcion != 8);  // Salir en la opción 8

    return 0;
} 