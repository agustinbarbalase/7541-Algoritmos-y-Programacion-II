#ifndef COLA_H
#define COLA_H

#include <stdbool.h>

struct queue;
typedef struct queue queue_t;

/*
  Crea una cola.
  Post: devuelve una nueva cola vacía.
*/
queue_t *create_queue(void);

/*
  Destruye la cola. Si se recibe la función destruir_dato por parámetro,
  para cada uno de los elementos de la cola llama a destruir_dato.
  Pre: la cola fue creada. destruir_dato es una función capaz de destruir
  los datos de la cola, o NULL en caso de que no se la utilice.
  Post: se eliminaron todos los elementos de la cola.
*/
void destroy_queue(queue_t *queue, void (*destroy_fn)(void *));

/*
  Devuelve verdadero si la cola no tiene elementos encolados, false en caso contrario.
  Pre: la cola fue creada.
*/
bool is_empty_queue(const queue_t *queue);

/*
  Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
  Pre: la cola fue creada.
  Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
  de la cola.
*/
bool add_element_queue(queue_t *queue, void *data);

/*
  Obtiene el valor del primer elemento de la cola. Si la cola tiene
  elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
  Pre: la cola fue creada.
  Post: se devolvió el primer elemento de la cola, cuando no está vacía.
*/
void *return_first_queue(const queue_t *queue);

/*
  Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
  primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
  Pre: la cola fue creada.
  Post: se devolvió el valor del primer elemento anterior, la cola
  contiene un elemento menos, si la cola no estaba vacía.
*/
void *delete_element_queue(queue_t *queue);

#endif