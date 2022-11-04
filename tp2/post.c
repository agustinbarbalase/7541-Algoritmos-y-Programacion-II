#define _POSIX_C_SOURCE 200809L
#include "post.h"
#include <string.h>
#include "abb.h"

struct post {
  size_t id;
  char* informacion;
  char* nombre_usuario;
  abb_t* likes;
};

post_t* post_crear(size_t id, char* informacion, char* nombre_usuario) {
  post_t* nuevo_post = malloc(sizeof(post_t));
  if(!nuevo_post) return NULL;

  nuevo_post->likes = abb_crear(strcmp, NULL);
  if(!nuevo_post->likes) {
    free(nuevo_post);
    return NULL;
  }

  nuevo_post->id = id;
  nuevo_post->informacion = informacion;
  nuevo_post->nombre_usuario = nombre_usuario;

  return nuevo_post;
}

bool post_insertar_likes(post_t* post, const char* nombre_usuario) {
  return abb_guardar(post->likes, nombre_usuario, NULL);
}

size_t post_obtener_id(post_t* post) {
  return post->id;
}

size_t post_obtener_cantidad_likes(post_t* post) {
  return abb_cantidad(post->likes);
}

void post_imprimir(post_t* post, FILE* salida) {
  fprintf(stdout, "Post ID %ld\n", post->id);
  fprintf(stdout, "%s dijo: %s\n", 
    post->nombre_usuario,
    post->informacion
  );
  fprintf(stdout, "Likes: %ld\n",
    abb_cantidad(post->likes)
  );
} 

/*
  Funcion que sirve para visitar en el abb de los likes de un post
*/
bool post_visitar_likes(const char* clave, void* dato, void* extra) {
  fprintf(extra, "\t%s\n", clave);
  return true;
}

void post_imprimir_likes(post_t* post, FILE* salida) {
  fprintf(salida, "El post tiene %ld likes:\n", 
    post_obtener_cantidad_likes(post)
  );
  abb_in_order(post->likes, post_visitar_likes, salida);
}

void post_destruir(post_t* post) {
  abb_destruir(post->likes);
  free(post->informacion);
  free(post->nombre_usuario);
  free(post);
}