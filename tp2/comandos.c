#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "comandos.h"

#define ERR_MSG_LOGGEADO            "Ya habia un usuario loggeado"
#define ERR_MSG_INEXISTENTE         "usuario no existente"
#define ERR_MSG_NO_LOGGEADO         "no habia usuario loggeado"
#define ERR_MSG_VER_FEED            "Usuario no loggeado o no hay mas posts para ver"
#define ERR_MSG_VER_POST            "Usuario no loggeado o Post inexistente"
#define ERR_MSG_NO_POST_O_SIN_LIKES "Post inexistente o sin likes"

/*
  Funcion que devulve un string sin el \n, solo del final
  Pre: El string no debe ser NULL
  Post: Si el \n existe lo borra, caso contrario no hace nada
*/
void obtener_cadena_sin_salto(char* cadena) {	
  if (cadena[strlen(cadena) - 1] == '\n') cadena[strlen(cadena) - 1] = '\0';
}

/*
  Funcion que lee comandos de la entrada estandar una sola vez
  Post: Devuelve lo que se ingreso
*/
char* obtener_dato() {
  char* dato = NULL;
  size_t tamanio_buffer = 0;
  tamanio_buffer = getline(&dato, &tamanio_buffer, stdin);
  obtener_cadena_sin_salto(dato);
  return dato;
}

/*
  Funcion que corresponde al comando login. El objetivo es loggear a
  un usuario
  Post: Si el usuario no hay usuario loggeado y el usuario existe
  lo pone como loggeado e imprime mensaje, caso contrario imprime 
  errores tanto si esta loggeado como si no existe
*/
void login(app_t* app) {
  char* nombre = obtener_dato();
  if(app_hay_loggeado(app)) {
    fprintf(stdout, "Error: %s\n", ERR_MSG_LOGGEADO);
    free(nombre);
    return;
  }
  if(!app_login(app, nombre)) { 
    fprintf(stdout, "Error: %s\n", ERR_MSG_INEXISTENTE);
    free(nombre);
    return;
  }
  fprintf(stdout, "Hola %s\n", nombre);
}

/*
  Funcion que corresponde al comando logout. El objetivo es desloggear a
  un usuario
  Post: Si hay usuario loggeado lo fija como desloggeado e imprime mensaje, 
  caso contrario imprime error si no hay usuario loggeado
*/
void logout(app_t* app) {
  if(app_logout(app)) fprintf(stdout, "Adios\n");
  else fprintf(stdout, "Error: %s\n", ERR_MSG_NO_LOGGEADO);
}

/*
  Funcion que corresponde al comando publicar. El objetivo es publicar a
  un post
  Post: Si hay usuario loggeado y se le pasa informacion se inserta el post
  y se imprime un mensaje, caso contrario se imprime un error
*/
void publicar_post(app_t* app) {
  char* informacion = obtener_dato();
  if(!app_insertar_post(app, informacion)) {
    fprintf(stdout, "Error: %s\n", ERR_MSG_NO_LOGGEADO);
    free(informacion);
  } else {
    fprintf(stdout, "Post publicado\n");
  }
}

/*
  Funcion que corresponde al comando ver_siguiente_feed. El objetivo es mostrarle
  al usuario un post relevante para el
  Post: Si hay usuario loggeado y hay posts par ver se imprime el post, 
  caso contrario se imprime si no hay usuario loggeado o no hay mas post
  para ver
*/
void proximo_post(app_t* app) {
  if(!app_hay_loggeado(app)) {
    fprintf(stdout, "%s\n", ERR_MSG_VER_FEED);
    return;
  }
  const char* nombre_loggeado = app_devolver_loggeado(app);
  usuario_t* usuario = app_devolver_usuario(app, nombre_loggeado);
  int post_id = usuario_devolver_feed(usuario);
  if(post_id == -1) {
    fprintf(stdout, "%s\n", ERR_MSG_VER_FEED);
    return;
  }
  post_t* post = app_devolver_post(app, post_id);
  post_imprimir(post, stdin);
}

/*
  Funcion que corresponde al comando likear_post. El objetivo es likear un post
  Post: Si hay usuario loggeado y se pasa un id existen se likea ese post, 
  caso contrario se imprime si no hay usuario loggeado o si el id pasado no
  corresponde a un post
*/
void likear_post(app_t* app) {
  int post_id;  
  if(fscanf(stdin, "%i\n", &post_id) == 0) return;
  post_t* post = app_devolver_post(app, post_id);
  if(!post || !app_hay_loggeado(app)) {
    fprintf(stdout, "Error: %s\n", ERR_MSG_VER_POST);
    return;
  }
  post_insertar_likes(post, app_devolver_loggeado(app));
  fprintf(stdout, "Post likeado\n");
}

/*
  Funcion que corresponde al comando mostrar_likes. El objetivo es mostrar
  quienes le dieron like a un post
  Post: Si se pasa un id existen se imprime queines le dieron like, 
  caso contrario si el id pasado no corresponde a un post
*/
void mostrar_likes(app_t* app) {
  int post_id;
  if(fscanf(stdin, "%i\n", &post_id) == 0) return;
  post_t* post = app_devolver_post(app, post_id);
  if(!post || post_obtener_cantidad_likes(post) == 0) {
    fprintf(stdout, "Error: %s\n", ERR_MSG_NO_POST_O_SIN_LIKES);
    return;
  }
  post_imprimir_likes(post, stdout);
}

/*
  Funcion que a partir de un nombre de comando pasado por parametro, lo busca
  e intenta ejecutar
  Pre: La app no debe ser NULL
  Post: Llama al comando si existe, caso contrario imprime un error por stdout
*/
void llamar_comando(app_t* app, char* comando) {
  if(strcmp(comando, "login") == 0) login(app); 
  else if(strcmp(comando, "logout") == 0) logout(app);
  else if(strcmp(comando, "publicar") == 0) publicar_post(app);
  else if(strcmp(comando, "ver_siguiente_feed") == 0) proximo_post(app);
  else if(strcmp(comando, "likear_post") == 0) likear_post(app);
  else if(strcmp(comando, "mostrar_likes") == 0) mostrar_likes(app);
  else fprintf(stdout, "Error: comando no reconocido\n");
}

bool cargar_usuarios(app_t* app, char* dir_archivo) {
  FILE* archivo = fopen(dir_archivo, "r");
	
  if (!archivo) return false;
	
  char* linea = NULL;
  size_t tamanio_buffer = 0;
	
  while((getline(&linea, &tamanio_buffer, archivo)) != EOF){
    obtener_cadena_sin_salto(linea);
    app_insertar_usuario(app, linea);
  }
	
  free(linea);
  fclose(archivo);
  return true;
}

void recibir_comando(app_t* app){
  char* comando = NULL;
  size_t tamanio_buffer = 0;
	
  while((getline(&comando, &tamanio_buffer, stdin)) != EOF){
    obtener_cadena_sin_salto(comando);
    llamar_comando(app, comando);
  }

  free(comando);
}