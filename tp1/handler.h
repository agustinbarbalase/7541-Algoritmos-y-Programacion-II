#ifndef HANDLER_H
#define HANDLER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "args.h"

#define FILE_STDIN 2
#define FILE_PATH  3

#define ERROR_MSG_FILE "Error: archivo fuente inaccesible"
#define ERROR_MSG_ARGS "Error: Cantidad erronea de parametros"

/*
  Función que nos dice si los argumentos para nuestor programa son validos o no
  Post: Si los arguementos son validos devuelve True. Caso Contrario: False
*/
bool are_valid_args(args_t* args);

/*
  Función que a partir de los argumentos nos dice que archivo tenemos que leer
  para manejar el programa, ya sea un archivo del directorio o el stdin
  Post: Nos devuelve el archivo que debemos leer para que el programa funcione
*/
FILE* switch_file(args_t* args);

/*
  Función que dado un mensaje por parametro imprime ese error por el stderr. Y nos
  sale del programa con un codigo de estado 1.
  Post: Retorna el error por stderr
*/
void error_handler(const char* message);

/*
  Función que nos devulve la cantidad de memoria que debe ocupar un buffer del archivo
  como maximo. Todo esto lo determina a partir de los argumentos
  Post: Devulve el tamaño de la memoria
*/
int calc_size_memory(args_t* args);

#endif