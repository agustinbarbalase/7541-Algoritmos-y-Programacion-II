#include "pila.h"
#include <stdlib.h>

#define CAPACIDAD_INICIAL   10
#define FACTOR_REDIMENSION   2
#define DIFERENCIA_CAPACIDAD 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if(pila == NULL) return NULL;
    
    pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void *));
    if(pila->datos == NULL) {
        free(pila);
        return NULL;
    };

    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    return pila;
}

/*
    Funcion que recibe una struct pila y que varia la capacidad de memoria del
    atributo datos
    Pre: el struct de pila no debe ser vacia
    Post: Devuelve true si el cambio de memoria fue exitoso y false en caso contrario.
*/
bool pila_redimensionar(pila_t* pila, size_t nueva_capacidad) {
    void **pila_dato_nuevo = realloc(pila->datos, nueva_capacidad * sizeof(void *));
    if(!pila_dato_nuevo) return false; 

    pila->datos = pila_dato_nuevo;
    pila->capacidad = nueva_capacidad;
    return true;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if(pila->cantidad == pila->capacidad) {
        if(!pila_redimensionar(pila, pila->capacidad * FACTOR_REDIMENSION)) {
            return false;
        }
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;

    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if(pila_esta_vacia(pila)) return NULL;
    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila) {
    if(pila_esta_vacia(pila)) return NULL;

    pila->cantidad--;
    void *ultimo_elemento = pila->datos[pila->cantidad];

    if(pila->cantidad * DIFERENCIA_CAPACIDAD <= pila->capacidad) { 
        pila_redimensionar(pila, pila->capacidad / FACTOR_REDIMENSION);
    };
    
    return ultimo_elemento;
}
