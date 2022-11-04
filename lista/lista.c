#include "lista.h"
#include <stdlib.h>

/* ========================== ESTRUCTURAS =========================*/
typedef struct nodo nodo_t;

struct nodo {
  void* dato;
  nodo_t* proximo;
};

struct lista {
  nodo_t* primero;
  nodo_t* ultimo;
  size_t largo;
};

struct lista_iter {
  nodo_t* anterior;
  nodo_t* actual;
  lista_t* lista;
};

nodo_t* crear_nodo(void* dato) {
  nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
  if(!nuevo_nodo) return NULL;

  nuevo_nodo->dato = dato;
  nuevo_nodo->proximo = NULL;

  return nuevo_nodo;
}

/* ====================== FUNCIONES LISTA =========================*/

lista_t *lista_crear(void) {
  lista_t* nueva_lista = malloc(sizeof(lista_t));
  if(!nueva_lista) return NULL;

  nueva_lista->primero = NULL;
  nueva_lista->ultimo = NULL;
  nueva_lista->largo = 0;

  return nueva_lista;
}

bool lista_esta_vacia(const lista_t *lista) {
  return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
  nodo_t* nuevo_nodo = crear_nodo(dato);
  if(!nuevo_nodo) return false;

  if(lista_esta_vacia(lista)) lista->ultimo = nuevo_nodo;
  nuevo_nodo->proximo = lista->primero;
  lista->primero = nuevo_nodo;
  
  lista->largo++;

  return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
  nodo_t* nuevo_nodo = crear_nodo(dato);
  if(!nuevo_nodo) return false;

  if(lista_esta_vacia(lista)) lista->primero = nuevo_nodo;
  else lista->ultimo->proximo = nuevo_nodo;
  lista->ultimo = nuevo_nodo;

  lista->largo++;

  return true;
}

void *lista_borrar_primero(lista_t *lista) {
  if(lista_esta_vacia(lista)) return NULL;

  void* dato = lista_ver_primero(lista);
  nodo_t* primer_elemento = lista->primero;

  lista->primero = primer_elemento->proximo;
  
  if(lista->largo == 1) lista->ultimo = NULL;
  free(primer_elemento);

  lista->largo--;

  return dato;
}

void *lista_ver_primero(const lista_t *lista) {
  if(lista_esta_vacia(lista)) return NULL;
  return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
  if(lista_esta_vacia(lista)) return NULL;
  return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
  return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
  while(!lista_esta_vacia(lista)) {
    void* dato = lista_borrar_primero(lista);
    if (destruir_dato) destruir_dato(dato);
  }
  free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
  if(!visitar) return;
  
  nodo_t* nodo_actual = lista->primero;
  while(nodo_actual != NULL) {
    void* actual = nodo_actual->dato;
    if(!visitar(actual, extra)) break;
    nodo_actual = nodo_actual->proximo;
  }

}

/* ============== FUNCIONES ITERADOR EXTERNO ======================*/

lista_iter_t *lista_iter_crear(lista_t *lista) {
  lista_iter_t* nuevo_iter = malloc(sizeof(lista_iter_t));
  if(!nuevo_iter) return NULL;

  nuevo_iter->anterior = NULL;
  nuevo_iter->actual = lista->primero;
  nuevo_iter->lista = lista;

  return nuevo_iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
  if(lista_iter_al_final(iter)) return false;

  iter->anterior = iter->actual;
  iter->actual = iter->actual->proximo;
  
  return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
  if(lista_iter_al_final(iter)) return NULL;
  return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
  return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
  free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
  if(!iter->anterior || !iter->actual) {
    bool insercion_correcta = !iter->anterior ? 
      lista_insertar_primero(iter->lista, dato) : 
      lista_insertar_ultimo(iter->lista, dato);
    if(!insercion_correcta) return false;
    iter->actual = !iter->anterior ? iter->lista->primero : iter->lista->ultimo;
    return true;
  }

  nodo_t* nuevo_nodo = crear_nodo(dato);
  if(!nuevo_nodo) return false;
  
  iter->anterior->proximo = nuevo_nodo;
  nuevo_nodo->proximo = iter->actual;
  iter->actual = nuevo_nodo;

  iter->lista->largo++;
  return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
  if(lista_iter_al_final(iter)) return NULL;
  
  nodo_t* nodo_actual = iter->actual; 
  void* dato = nodo_actual->dato;

  if(iter->anterior) iter->anterior->proximo = nodo_actual->proximo;
  else iter->lista->primero = nodo_actual->proximo;

  if(!iter->actual->proximo) iter->lista->ultimo = iter->anterior;

  iter->actual = nodo_actual->proximo;
  free(nodo_actual);

  iter->lista->largo--;

  return dato;
}
