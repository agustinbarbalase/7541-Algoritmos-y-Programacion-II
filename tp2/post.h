#ifndef POST_H
#define POST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
  El struct post_t funciona como los posts de la aplicacion. 
  Contiene informacion sobre quien es su autor y que es lo
  que dijo ese autor entre otras cosas. Es una forma, de
  representar al post
*/
typedef struct post post_t;

/*
  Funcion que crear un struct post_t
  Pre: La informacion y el nombre del usuario no deberian ser
  NULL
  Post: Devuelve un struct post_t, caso contrario NULL
*/
post_t* post_crear(size_t id, char* informacion, char* nombre_usuario);

/*
  Funcion que inserta un nuevo like. A partir del nombre 
  del usuario.
  Pre: Si el nombre ya fue ingresado, no se computa dos veces
  Post: Devuelve true si el nombre fue ingresado con exito, false
  caso contrario
*/
bool post_insertar_likes(post_t* post, const char* nombre_usuario);

/*
  Funcion que devuelve el id de un post
  Pre: El post no debe ser NULL
  Post: Devuelve el numero
*/
size_t post_obtener_id(post_t* post);

/*
  Funcion que devuelve la cantidad de likes que tiene un post
  Pre: El post no debe ser NULL
  Post: Devuelve el numero, incluso si no tiene likes
*/
size_t post_obtener_cantidad_likes(post_t* post);

/*
  Funcion imprime por un archivo que le pasemos, la informacion
  del post
  Pre: El post no debe ser NULL
  Post: Devuelve la informacion
*/
void post_imprimir(post_t* post, FILE* salida);

/*
  Funcion imprime por un archivo que le pasemos, los usuarios
  que le dieron like al post
  Pre: El post no debe ser NULL
  Post: Devuelve la informacion
*/
void post_imprimir_likes(post_t* post, FILE* salida);

/*
  Funcion que destruye el struct post_t
  Pre: El post no debe ser NULL
  Post: Libera la memoria que ocupa el struct
*/
void post_destruir(post_t* post);

#endif