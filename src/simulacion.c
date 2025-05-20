#include <stdio.h>
#include "../include/simulacion.h"
#include "../include/dijasktra.h"
#include <stdlib.h>

/**
 * Procesa una solicitud de viaje, asignando el taxi más cercano
 */
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

/**
 * Actualiza el estado de todos los taxis, decrementando el tiempo de los ocupados
 */
int actualizar_taxis(Taxi taxis[MAX_TAXIS], int mapa_taxis[SIZE_MAP][SIZE_MAP]) {
    int taxis_completados = 0;
    
    printf("\n===== Actualizando Estado de Taxis =====\n");
    
    for (int i = 0; i < MAX_TAXIS; i++) {
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

/**
 * Muestra la información de los taxis
 */
void mostrar_info_taxis(Taxi taxis[MAX_TAXIS]) {
    printf("\n===== Estado Actual de Taxis =====\n");
    
    int disponibles = 0;
    int ocupados = 0;
    
    for (int i = 0; i < MAX_TAXIS; i++) {
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

/**
 * Ejecuta la simulación
 */
void ejecutar_simulacion(Taxi taxis[MAX_TAXIS], int nodos_ids[MAX_NODOS], int calles[MAX_NODOS][MAX_NODOS], 
                        char mapa[SIZE_MAP][SIZE_MAP], int mapa_taxis[SIZE_MAP][SIZE_MAP], Cola *solicitudes) {
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
            for (int i = 0; i < MAX_TAXIS; i++) {
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

