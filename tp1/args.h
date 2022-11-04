#ifndef ARGS_H
#define ARGS_H

#include <stdlib.h>

struct args;
typedef struct args args_t;

/*
  Función que nos crea un struct args_t que nos permitira manejar los argumentos
  Post: Nos devuelve el puntero al struct o NULL, en caso de que no se haya creado
  correctamente
*/
args_t* create_args(int argc, char* argv[]);

/*
  Función que nos retorna la cantidad de argumentos que tiene nuestro programa
  Pre: No debe ser un puntero a NULL
  Post: Retorna la cantidad
*/
int return_numbers_args(args_t* args);

/*
  Función que nos retorna un argumento dada una posicion
  Pre: No debe ser un puntero a NULL
  Post: Retorna el puntero de nuestro argumento
*/
char* return_arg(args_t* args, size_t pos);

/*
  Función que nos permite destruir el struct que creamos
  Post: Destruye el struct args, liberando su memoria
*/
void destroy_args(args_t* args);

#endif