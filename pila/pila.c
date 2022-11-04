#include "pila.h"
#include <stdlib.h>

#define CAPACIDAD_INICIAL 10

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
    si la nueva capacidad es menor a la CAPACIDAD_INICIAL por defecto la variable
    nueva_capacidad pasa a ser CAPACIDAD_INICIAL
*/
bool pila_redimensionar(pila_t* pila, size_t nueva_capacidad) {
    if(nueva_capacidad < CAPACIDAD_INICIAL) nueva_capacidad = CAPACIDAD_INICIAL;

    void **pila_dato_nuevo = realloc(pila->datos, nueva_capacidad * sizeof(void *));
    if(!pila_dato_nuevo) return false; 

    pila->datos = pila_dato_nuevo;
    pila->capacidad = nueva_capacidad;
    return true;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
    return;
}

bool pila_esta_vacia(const pila_t *pila) {
    if(pila->cantidad == 0) return true;
    return false;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if(pila->cantidad == pila->capacidad) {
        if(!pila_redimensionar(pila, pila->capacidad * 2)) return false;
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

    if(pila->cantidad * 4 <= pila->capacidad) { 
        pila_redimensionar(pila, pila->capacidad / 2);
    };
    
    return ultimo_elemento;
}
