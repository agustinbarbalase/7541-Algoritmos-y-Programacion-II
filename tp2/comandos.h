#ifndef COMANDOS_H
#define COMANDOS_H

#include <stdio.h>
#include <stdbool.h>
#include "app.h"

/*
  Funcion que a partir de una direccion de archivo, intenta leer ese 
  archivo con un formato especifico y carga esa informacion en el struct
  app_t. La informacion corresponde a los usuarios de la aplicacion
  Pre: La app no debe ser NULL y la direccion de archivo debe poder ser
  ejecutada por el programa. El formato del archivo debe ser por cada linea,
  contener un nombre sin repetir de los usuarios
  Post: Devuelve true y la app con todos los usuarios cargados, caso contrario
  devuelve false
*/
bool cargar_usuarios(app_t* app, char* dir_archivo);

/*
  Funcion que lee por stdin la entrada que se pase, hasta que termine 
  el stdin y esos comandos pasa a otra funcion para que los ejecute
  Pre: La app no debe ser NULL
  Post: Lee y ejecuta los comandos pasados por stdin
*/
void recibir_comando(app_t* app);

#endif