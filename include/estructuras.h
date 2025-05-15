#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include "constantes.h"

// Struct para el taxi
typedef struct {
    int id;
    int x;
    int y;
    int estado;
} Taxi;


/**
 * @brief Estructura que representa una solicitud de viaje
 */
typedef struct {
    int x_origen;    // Nodo de origen del pasajero
    int y_origen;   // Nodo de destino del pasajero
    int x_destino;   // Nodo de destino del pasajero
    int y_destino;   // Nodo de destino del pasajero
    int id;             // Identificador único de la solicitud
} Solicitud;



#endif


