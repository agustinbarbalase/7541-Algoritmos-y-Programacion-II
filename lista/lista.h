#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
 // Estructura que permite guardar datos de tipo void*
typedef struct lista lista_t;

 // Estructura que permite iterar sobre una lista
typedef struct lista_iter lista_iter_t;


/* *****************************************************************
 *              DEFINICION DE LAS FUNCIONES DE LA LISTA
 * *****************************************************************/

/* Crea una lista vacía.
 * Post: Devuelve una lista vacía, en caso de fallar devuelve NULL*/
lista_t *lista_crear(void);

/* Revisa si la lista está vacía.
 * Pre: La lista fue creada.
 * Post: Devuelve true si la lista no tiene elementos.*/
bool lista_esta_vacia(const lista_t *lista);

/* Inserta un elemento en la primera posición.
 * Pre: la lista fue creada.
 * Post: Se agregó el elemento en la primera posición, devuelve true,
 * false en caso de fallar.*/
bool lista_insertar_primero(lista_t *lista, void *dato);

/* Inserta un elemento en la última posición.
 * Pre: la lista fue creada.
 * Post: Se agregó el elemento en la última posición, devuelve true,
 * false en caso de fallar.*/
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/* Elimina y devuelve el elemento que se encuentra en la primera posición y
 * mueve todos los demás elementos.
 * Pre: La lista fue creada.
 * Post: El primer elemento fue eliminado y la función devuelve el dato 
 * correspondiente. En caso de que la lista esté vacía devuelve NULL.*/
void *lista_borrar_primero(lista_t *lista);

/* Devuelve el dato que se encuentra en la primera posición.
 * Pre: La lista fue creada.
 * Post: Se devuelve el dato, de no haberlo, es decir, que la lista está
 * vacía, devuelve NULL.*/
void *lista_ver_primero(const lista_t *lista);

/* Devuelve el dato que se encuentra en la última posición.
 * Pre: La lista fue creada.
 * Post: Se devuelve el dato, de no haberlo, es decir, que la lista está
 * vacía, devuelve NULL.*/
void *lista_ver_ultimo(const lista_t* lista);

/* Devuelve la cantidad de elementos en la lista.
 * Pre: La lista fue creada.*/
size_t lista_largo(const lista_t *lista);

/* Destruye la lista y todos sus elementos.
 * Pre: La lista fue creada y destruir dato es una función de destrucción
 * válida.
 * Post: La lista y todos sus elementos se eliminaron. Se liberó toda
 * la memoria.*/
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* Itera toda la lista y aplica una función visitante a todos sus elementos.
 * Pre: La lista fue creada, y visitar es una función válida.
 * Post: Se iteró sobre toda la lista y se aplicó la función visitar a
 * cada uno de sus elementos.*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* *****************************************************************
 *           DEFINICION DE LAS FUNCIONES DEL ITERADOR EXTERNO
 * *****************************************************************/

/* Crea un iterador de una lista dada.
 * Pre: La lista existe.
 * Post: Devuelve un puntero al iterador de la lista. En caso de fallar
 * devuelve NULL.*/
lista_iter_t *lista_iter_crear(lista_t *lista);

/* Avanza al siguiente elemento de la lista.
 * Pre: El iterador y la lista han sido creados. 
 * Post: Se avanzó un elemento en la lista y ahora el iterador apunta a 
 * ese elemento. Devuelve true, en caso de avanzar, false si se llegó
 * al final o si la lista se encuentra vacía.*/
bool lista_iter_avanzar(lista_iter_t *iter);

/* Devuelve el elemento que se encuentra en la posición del iterador.
 * Pre: El iterador y la lista han sido creados.*/
void *lista_iter_ver_actual(const lista_iter_t *iter);

/* Revisa si el iterador se encuentra al final de la lista o no.
 * Pre: El iterador y la lista han sido creados.
 * Post: Devuelve true en caso de haber llegado al final.*/
bool lista_iter_al_final(const lista_iter_t *iter);


/* Destruye al iterador y libera su memoria.
 * Pre: El iterador ha sido creado.
 * Post: Se ha eliminado el iterador.*/
void lista_iter_destruir(lista_iter_t *iter);

/* Agrega el dato pasado por parametro a la lista en la posición
 * actual del iterador.
 * Pre: El iterador y la lista han sido creados.
 * Post: Se ha agregado el nuevo elemento a la lista en la
 * posición del iterador, este apunta al nuevo elemento. Los demás
 * elentos se corren hacia adelante. Devuelve true en caso de funcionar.*/
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/* Borra y devuelve el elemento actual de la lista
 * Pre: El iterador y la lista han sido creados.
 * Post: El elemento ya no se encuentra en la lista, y se devolvió
 * correctamente.*/
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                     FUNCIONES DE PRUEBAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);

#endif
