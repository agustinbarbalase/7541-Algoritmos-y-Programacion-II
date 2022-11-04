#ifndef APP_H
#define APP_H

#include <stdlib.h>
#include <stdbool.h>
#include "post.h"
#include "usuario.h"

/*
  El struct app_t contiene a la aplicacion. Basicamente es un 
  estado de la aplicacion. Que varia en cada comando.
*/
typedef struct app app_t;

/*
  Funcion que crear un struct app_t
  Post: Devuelve un struct app_t, caso contrario NULL
*/
app_t* app_crear();

/*
  Funcion que devuelve si un usuario esta en la base de datos
  Post: Devuelve true si existe, caso contrario devuelve false
*/
bool app_existe_usuario(app_t* app, const char* nombre);

/*
  Funcion que devuelve si un usuario hay un usuario loggeado
  Post: Devuelve true si hay alguien, caso contrario devuelve false
*/
bool app_hay_loggeado(app_t* app);

/*
  Funcion que crea e inserta un nuevo usuario. La forma que se crea
  es con un struct usuario_t y es eso lo que se inserta. Los nombres
  junto con los ids son unicos
  Pre: Si el usuario ya fue ingresado, sera sobreescrito
  Post: Devuelve true si el usuario fue ingresado con exito, false
  caso contrario
*/
bool app_insertar_usuario(app_t* app, char* nombre);

/*
  Funcion que crea e inserta un nuevo post. El autor del post
  se define como al usuario loggeado en ese momento.
  Pre: Debe haber un usuario loggeado
  Post: Devuelve true si el post fue ingresado con exito. En caso de
  de que no haya usuario loggeado o no se haya creado con exito 
  se devuelve false
*/
bool app_insertar_post(app_t* app, char* informacion);

/*
  Funcion que devuelve a partir de un nombre, devuelve al usuario
  Post: Si el usuario existe lo devuelve, caso contrario devuelve NULL
*/
usuario_t* app_devolver_usuario(app_t* app, const char* nombre);

/*
  Funcion que devuelve el usuario que este loggeado
  Post: Si hay usuario loggeado lo devuelve, caso contrario devuelve NULL
*/
const char* app_devolver_loggeado(app_t* app);

/*
  Funcion que devuelve a partir de un id, devuelve al post
  Post: Si el post existe lo devuelve, caso contrario devuelve NULL
*/
post_t* app_devolver_post(app_t* app, size_t post_id);

/*
  Funcion que designa como usuario loggeado al que pasamos por parametro
  Pre: El usuario deberia existir
  Post: Si el usuario existe, lo loggea y devuelve true, caso contrario
  no hace nada y devuelve false
*/
bool app_login(app_t* app, char* nombre);

/*
  Funcion que borra al usuario loggeado de ese momento
  Post: Si hay usuario loggeado, designa al espacio como NULL y devuelve true,
  caso contrario no hace nada y devuelve fals
*/
bool app_logout(app_t* app);

/*
  Funcion que destruye el struct app_t
  Pre: El app no debe ser NULL
  Post: Libera la memoria que ocupa el struct
*/
void app_destruir(app_t* app);

#endif
