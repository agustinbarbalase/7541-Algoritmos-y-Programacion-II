#ifndef USUARIO_H
#define USUARIO_H

#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"

/*
  El struct usuario_t funciona como los usuarios de la aplicacion. 
  Contiene informacion como su nombre y que posts tiene para ver,
  entre otras cosas. Es una forma, de representar al usuario
*/
typedef struct usuario usuario_t;

/*
  Funcion que crear un struct usuario_t
  Pre
  Post: Devuelve un struct usuario_t, caso contrario NULL
*/
usuario_t* usuario_crear(size_t id, cmp_func_t cmp);

/*
  Funcion que crea e inserta un feed. El feed es informacion que esta
  asociado con un post y su usuario creador, y que permite relacionarse
  con el usuario que lo recibe
  Post: Devuelve true si fue ingresado con exito, false
  caso contrario
*/
bool usuario_insertar_feed(usuario_t* usuario, usuario_t* creador, size_t post_id);

/*
  Funcion que devuelve el id de un post, que se encuentra en un feed. Es
  decir, el id del post mas relevante para un usuario en ese momento
  Pre: el usuario no debe ser NULL
  Post: Devuelve el numero del id en caso que haya para ver, caso 
  contrario devuelve -1
*/
int usuario_devolver_feed(usuario_t* usuario);

/*
  Funcion que destruye el struct usuario_t
  Pre: El usuario no debe ser NULL
  Post: Libera la memoria que ocupa el struct
*/
void usuario_destruir(usuario_t* usuario);

/*
  Funcion que compara dos post y determina cual es mas relevante. Todo esto
  a partir de la afinidad
  Post:
    Si a > b -> devuelve 1
    Si a < b -> devuelve -1
    Caso contario devuelve 0
*/
int cmp_afinidad(const void* a, const void* b);

#endif