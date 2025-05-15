#include <stdio.h>
#include "../include/cola.h"

/**
 * @brief Inicializa una cola vacía
 * @param cola Puntero a la cola a inicializar
 */
void inicializar_cola(Cola *cola) {
    cola->frente = 0;
    cola->final = -1;
    cola->cantidad = 0;
}

/**
 * @brief Verifica si la cola está vacía
 * @param cola Puntero a la cola a verificar
 * @return 1 si la cola está vacía, 0 en caso contrario
 */
int esta_vacia(Cola *cola) {
    return cola->cantidad == 0;
}

/**
 * @brief Verifica si la cola está llena
 * @param cola Puntero a la cola a verificar
 * @return 1 si la cola está llena, 0 en caso contrario
 */
int esta_llena(Cola *cola) {
    return cola->cantidad == MAX_SOLICITUDES;
}

/**
 * @brief Agrega una solicitud al final de la cola
 * @param cola Puntero a la cola
 * @param solicitud Solicitud a agregar
 * @return 1 si se pudo agregar, 0 si la cola está llena
 */
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

/**
 * @brief Elimina y retorna la solicitud del frente de la cola
 * @param cola Puntero a la cola
 * @param solicitud Puntero donde se almacenará la solicitud eliminada
 * @return 1 si se pudo eliminar, 0 si la cola está vacía
 */
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

/**
 * @brief Retorna la solicitud del frente de la cola sin eliminarla
 * @param cola Puntero a la cola
 * @param solicitud Puntero donde se almacenará la solicitud del frente
 * @return 1 si se pudo obtener, 0 si la cola está vacía
 */
int frente(Cola *cola, Solicitud *solicitud) {
    if (esta_vacia(cola)) {
        return 0; // Cola vacía, no hay frente
    }
    
    // Obtiene la solicitud del frente sin eliminarla
    *solicitud = cola->elementos[cola->frente];
    
    return 1; // Operación exitosa
}

/**
 * @brief Muestra todas las solicitudes en la cola
 * @param cola Puntero a la cola a mostrar
 */
void mostrar_cola(Cola *cola) {
    if (esta_vacia(cola)) {
        printf("La cola de solicitudes está vacía.\n");
        return;
    }
    
    printf("\n--- COLA DE SOLICITUDES ---\n");
    printf("Total de solicitudes: %d\n\n", cola->cantidad);
    
    int indice = cola->frente;
    for (int i = 0; i < cola->cantidad; i++) {
        Solicitud solicitud = cola->elementos[indice];
        printf("Solicitud %d:\n", solicitud.id);
        printf("  Origen: Nodo %d\n", solicitud.nodo_origen);
        printf("  Destino: Nodo %d\n\n", solicitud.nodo_destino);
        
        // Avanza de forma circular
        indice = (indice + 1) % MAX_SOLICITUDES;
    }
    
    printf("-------------------------\n");
}
