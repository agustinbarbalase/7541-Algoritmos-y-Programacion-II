#define _POSIX_C_SOURCE 200809L
#include "post.h"
#include <string.h>
#include "usuario.h"

typedef struct feed {
  size_t usuario_id;
  size_t creador_id;
  size_t post_id;
} feed_t;

struct usuario {
  size_t id;
  heap_t* feed;
};

/*
  Funcion que crear un struct feed_t
  Pre
  Post: Devuelve un struct feed_t, caso contrario NULL
*/
feed_t* feed_crear(usuario_t* creador, size_t receptor_id, size_t post_id) {
  feed_t* nuevo_feed = malloc(sizeof(feed_t));
  if(!nuevo_feed) return NULL;

  nuevo_feed->usuario_id = receptor_id;
  nuevo_feed->creador_id = creador->id;
  nuevo_feed->post_id = post_id;

  return nuevo_feed;
}

usuario_t* usuario_crear(size_t id, cmp_func_t cmp) {
  usuario_t* nuevo_usuario = malloc(sizeof(usuario_t));
  if(!nuevo_usuario) return NULL;

  nuevo_usuario->feed = heap_crear(cmp);
  if(!nuevo_usuario->feed) {
    free(nuevo_usuario);
    return NULL;
  }

  nuevo_usuario->id = id;

  return nuevo_usuario;
}

bool usuario_insertar_feed(usuario_t* usuario, usuario_t* creador, size_t post_id) {
  feed_t* nuevo_feed = feed_crear(creador, usuario->id, post_id);
  if(!nuevo_feed) return false;

  if(!heap_encolar(usuario->feed, nuevo_feed)) return false;

  return true;
}

/*
  Funcion que destruye el struct feed_t.
  Pre: El feed no debe ser NULL
  Post: Libera la memoria que ocupa el struct
*/
void feed_destruir(void* e) {
  feed_t* feed = (feed_t *) e;
  free(feed);
}

int usuario_devolver_feed(usuario_t* usuario) {
  feed_t* feed = heap_desencolar(usuario->feed);
  if(!feed) return -1;
  size_t post_id = feed->post_id;
  feed_destruir(feed);
  return (int) post_id;
}

void usuario_destruir(usuario_t* usuario) {
  heap_destruir(usuario->feed, feed_destruir);
  free(usuario);
}

/*
  Funcion que calcula la distancia entre dos numeros
  Post: Devuele la distancia
*/
int calc_distancia(const int a, const int b) {
  if((b - a) < 0) return -(b - a);
  return b - a;
}

int cmp_afinidad(const void* a, const void* b) {
  feed_t* feed_a = (feed_t*) a;
  feed_t* feed_b = (feed_t*) b;

  int a_creador = calc_distancia((int) feed_a->usuario_id, (int) feed_a->creador_id);
  int b_creador = calc_distancia((int) feed_b->usuario_id, (int) feed_b->creador_id);

  if(a_creador < b_creador) return 1;
  else if(a_creador > b_creador) return -1;
  
  if(feed_a->post_id < feed_b->post_id) return 1;
  return -1;
}