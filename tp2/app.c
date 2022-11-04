#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <string.h>
#include "app.h"
#include "hash.h"

struct app {
  char* usuario_loggeado;
  hash_t* usuarios;
  hash_t* posts;
};

/*
  Funcion que sirve como wrapper para el hash de los usuarios
*/
void usuario_destruir_wrapper(void* usuario) {
  usuario_destruir((usuario_t *) usuario);
}

/*
  Funcion que sirve como wrapper para el hash de los posts
*/
void post_destruir_wrapper(void* post) {
  post_destruir((post_t *) post);
}

app_t* app_crear() {
  app_t* nueva_app = malloc(sizeof(app_t));
  if(!nueva_app) return NULL;

  nueva_app->usuarios = hash_crear(usuario_destruir_wrapper);
  if(!nueva_app->usuarios) {
    free(nueva_app);
    return NULL;
  }

  nueva_app->posts = hash_crear(post_destruir_wrapper);
  if(!nueva_app->posts) {
    hash_destruir(nueva_app->usuarios);
    free(nueva_app);
    return NULL;
  }

  nueva_app->usuario_loggeado = NULL;

  return nueva_app;
}

bool app_existe_usuario(app_t* app, const char* nombre) {
  return hash_obtener(app->usuarios, nombre) != NULL;
}

bool app_hay_loggeado(app_t* app) {
  return app->usuario_loggeado != NULL;
}

bool app_insertar_usuario(app_t* app, char* nombre) {
  usuario_t* nuevo_usuario = usuario_crear(
    hash_cantidad(app->usuarios),
    cmp_afinidad
  );
  hash_guardar(app->usuarios, nombre, nuevo_usuario);
  return true;
}

/*
  Funcion que recorre cada uno de los usuarios de la app
  y añade un post nuevo en sus feed
  Pre: La app y el nuevo_post no deben ser NULL
  Post: Todos los usuarios tienen ese nuevo post en sus feeds
*/
void app_publicar_feed(app_t* app, post_t* nuevo_post) {
  usuario_t* usuario_creador = hash_obtener(app->usuarios, app->usuario_loggeado);
  hash_iter_t* hash_iter = hash_iter_crear(app->usuarios);
  
  while(!hash_iter_al_final(hash_iter)) {
    const char* clave_actual = hash_iter_ver_actual(hash_iter);
    if(strcmp(clave_actual, app->usuario_loggeado) != 0) {
      usuario_t* usuario_actual = hash_obtener(app->usuarios, clave_actual);
      usuario_insertar_feed(
        usuario_actual,
        usuario_creador,
        post_obtener_id(nuevo_post)
      );
    }
    hash_iter_avanzar(hash_iter);
  }
  
  hash_iter_destruir(hash_iter);
}

bool app_insertar_post(app_t* app, char* informacion) {
  if(!app_hay_loggeado(app)) return false;
  post_t* nuevo_post = post_crear(
    hash_cantidad(app->posts),
    informacion,
    strdup(app->usuario_loggeado)
  );

  char post_id_char[10];
  sprintf(post_id_char, "%ld", post_obtener_id(nuevo_post));
  hash_guardar(app->posts, post_id_char, nuevo_post);

  app_publicar_feed(app, nuevo_post);

  return true;
}

usuario_t* app_devolver_usuario(app_t* app, const char* nombre) {
  return hash_obtener(app->usuarios, nombre);
}

const char* app_devolver_loggeado(app_t* app) {
  if(!app_hay_loggeado(app)) return NULL;
  return app->usuario_loggeado;
}

post_t* app_devolver_post(app_t* app, size_t post_id) {
  char post_id_char[10];
  sprintf(post_id_char, "%ld", post_id);
  post_t* post = hash_obtener(app->posts, post_id_char);
  return post;
}

bool app_login(app_t* app, char* nombre) {
  if(!app_existe_usuario(app, nombre)) return false;
  app->usuario_loggeado = nombre;
  return true;
}

bool app_logout(app_t* app) {
  if(!app_hay_loggeado(app)) return false;
  free(app->usuario_loggeado);
  app->usuario_loggeado = NULL;
  return true;
}

void app_destruir(app_t* app) {
  hash_destruir(app->usuarios);
  hash_destruir(app->posts);
  free(app);
}