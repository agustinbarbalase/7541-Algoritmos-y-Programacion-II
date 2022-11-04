#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/*
  Funcion que dado un archivo lee su informacion y la guarda en una cola
  Pre: El archivo no debe ser NULL
  Post: Nos devuelve la cola con la informacion ya estando leeida
*/
queue_t *read_file(int size_memory, FILE *file);

/*
  Funcion que dada una cola, vuelca su informacion hacia el stdout
  Pre: La cola no debe estar vacia
  Post: Devuelve a traves del stdout la informacion de la cola
*/
void write_output(queue_t* queue);

#endif