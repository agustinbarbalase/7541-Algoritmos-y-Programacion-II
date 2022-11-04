#include "cola.h"
#include <stdlib.h>

typedef struct nodo nodo_t;

struct nodo {
  void* dato;
  nodo_t* proximo;
};

struct cola {
  nodo_t* primero;
  nodo_t* ultimo;
};

/*
  Función que sirve para crear un nuevo nodo para ser añadido a una cola
  Post: devuelve el nodo. En caso de que no se haya creado correctamente
  devuelve NULL
*/
nodo_t* crear_nodo(void *valor) {
  nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
  if(!nuevo_nodo) return NULL;
  
  nuevo_nodo->dato = valor;
  nuevo_nodo->proximo = NULL;

  return nuevo_nodo;
}

cola_t *cola_crear(void) {
  cola_t* nueva_cola = malloc(sizeof(cola_t));
  if(!nueva_cola) return NULL;

  nueva_cola->primero = NULL;
  nueva_cola->ultimo = NULL;

  return nueva_cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
  while(!cola_esta_vacia(cola)) {
    void* dato = cola_desencolar(cola);
    if(destruir_dato) destruir_dato(dato);
  }
  free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
  return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
  nodo_t* nuevo_nodo = crear_nodo(valor);
  if(!nuevo_nodo) return false;

  if(cola_esta_vacia(cola)) cola->primero = nuevo_nodo;
  else cola->ultimo->proximo = nuevo_nodo;
  cola->ultimo = nuevo_nodo;

  return true;
}

void *cola_ver_primero(const cola_t *cola) {
  if(cola_esta_vacia(cola)) return NULL;
  return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola) {
  if(cola_esta_vacia(cola)) return NULL;

  nodo_t* primer_elemento = cola->primero;
  void* dato = cola_ver_primero(cola);

  cola->primero = primer_elemento->proximo;
  if(!cola->primero) cola->ultimo = NULL;
  free(primer_elemento);

  return dato;
}