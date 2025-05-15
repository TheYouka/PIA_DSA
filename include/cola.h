#ifndef COLA_H
#define COLA_H

#include "estructuras.h"

#define MAX_SOLICITUDES 50

/**
 * @brief Estructura que representa una cola de solicitudes
 */
typedef struct {
    Solicitud elementos[MAX_SOLICITUDES];  // Array de solicitudes
    int frente;                            // Índice del frente de la cola
    int final;                             // Índice del final de la cola
    int cantidad;                          // Cantidad de elementos en la cola
} Cola;

/**
 * @brief Inicializa una cola vacía
 * @param cola Puntero a la cola a inicializar
 */
void inicializar_cola(Cola *cola);

/**
 * @brief Verifica si la cola está vacía
 * @param cola Puntero a la cola a verificar
 * @return 1 si la cola está vacía, 0 en caso contrario
 */
int esta_vacia(Cola *cola);

/**
 * @brief Verifica si la cola está llena
 * @param cola Puntero a la cola a verificar
 * @return 1 si la cola está llena, 0 en caso contrario
 */
int esta_llena(Cola *cola);

/**
 * @brief Agrega una solicitud al final de la cola
 * @param cola Puntero a la cola
 * @param solicitud Solicitud a agregar
 * @return 1 si se pudo agregar, 0 si la cola está llena
 */
int apilar(Cola *cola, Solicitud solicitud);

/**
 * @brief Elimina y retorna la solicitud del frente de la cola
 * @param cola Puntero a la cola
 * @param solicitud Puntero donde se almacenará la solicitud eliminada
 * @return 1 si se pudo eliminar, 0 si la cola está vacía
 */
int desapilar(Cola *cola, Solicitud *solicitud);

/**
 * @brief Retorna la solicitud del frente de la cola sin eliminarla
 * @param cola Puntero a la cola
 * @param solicitud Puntero donde se almacenará la solicitud del frente
 * @return 1 si se pudo obtener, 0 si la cola está vacía
 */
int frente(Cola *cola, Solicitud *solicitud);

/**
 * @brief Muestra todas las solicitudes en la cola
 * @param cola Puntero a la cola a mostrar
 */
void mostrar_cola(Cola *cola);

#endif /* COLA_H */
