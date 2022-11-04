#define _POSIX_C_SOURCE 200809L 
#include "abb.h"
#include "pila.h"
#include <string.h>
#include <stdlib.h>

typedef struct nodo nodo_t;

struct nodo {
  nodo_t* izq;
  nodo_t* der;
  char* clave;
  void* dato;
};

struct abb {
  nodo_t* raiz;
  abb_comparar_clave_t cmp;
  abb_destruir_dato_t destruir_dato;
  size_t cantidad;
};

struct abb_iter {
  const abb_t* arbol;
  pila_t* pila;
};

nodo_t* crear_nodo(const char *clave, void *dato) {
  nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
  if(!nuevo_nodo) return NULL;

  char* clave_duplicada = strdup(clave);
  if(!clave_duplicada) {
    free(nuevo_nodo);
    return NULL;
  }
  nuevo_nodo->izq = NULL;
  nuevo_nodo->der = NULL;
  nuevo_nodo->clave = clave_duplicada;
  nuevo_nodo->dato = dato;

  return nuevo_nodo;
}

nodo_t* buscar_reemplazante(nodo_t* nodo) {
  nodo_t* nodo_reemplazante = nodo->izq;

  while(nodo_reemplazante->der) {
    nodo_reemplazante = nodo_reemplazante->der;
  }   

  return nodo_reemplazante;
}

nodo_t* abb_buscar(nodo_t* nodo, nodo_t** padre, abb_comparar_clave_t cmp, const char* clave) {
  if(!nodo) return NULL;
  if(cmp(nodo->clave, clave) == 0) return nodo;

  *padre = nodo;

  if(cmp(nodo->clave, clave) > 0) 
    return abb_buscar(nodo->izq, padre, cmp, clave);
  return abb_buscar(nodo->der, padre, cmp, clave);
}

void _abb_destruir(nodo_t* nodo, abb_destruir_dato_t destruir_dato) {
  if(!nodo) return;

  _abb_destruir(nodo->izq, destruir_dato);
  _abb_destruir(nodo->der, destruir_dato);
  if(destruir_dato) destruir_dato(nodo->dato);
  free(nodo->clave);
  free(nodo);
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
  abb_t* nuevo_abb = malloc(sizeof(abb_t));
  if(!nuevo_abb) return NULL;

  nuevo_abb->raiz = NULL;
  nuevo_abb->cmp = cmp;
  nuevo_abb->destruir_dato = destruir_dato;
  nuevo_abb->cantidad = 0;

  return nuevo_abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
  nodo_t* padre = NULL;
  nodo_t* nodo_buscado = abb_buscar(arbol->raiz, &padre, arbol->cmp, clave);
  
  if(nodo_buscado) {
    if(arbol->destruir_dato) arbol->destruir_dato(nodo_buscado->dato);
    nodo_buscado->dato = dato; 
  } else {
    nodo_t* nuevo_nodo = crear_nodo(clave, dato);
    if(!nuevo_nodo) return false;
    if(arbol->cantidad == 0) arbol->raiz = nuevo_nodo;
    else {
      if(arbol->cmp(padre->clave, clave) > 0) padre->izq = nuevo_nodo;
      else padre->der = nuevo_nodo;
    }
    arbol->cantidad++;
  }

  return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
  if(!abb_pertenece(arbol, clave)) return NULL;

  nodo_t* padre = NULL;
  nodo_t* nodo_buscado = abb_buscar(arbol->raiz, &padre, arbol->cmp, clave);
  void* dato = nodo_buscado->dato;

  // Caso con dos hijos
  if(nodo_buscado->der && nodo_buscado->izq) {
    nodo_t* nodo_reemplazante = buscar_reemplazante(nodo_buscado);

    char* clave_reemplazante = strdup(nodo_reemplazante->clave); 
    void* dato_reemplazante = abb_borrar(arbol, nodo_reemplazante->clave);
    
    free(nodo_buscado->clave);
    nodo_buscado->clave = clave_reemplazante;
    nodo_buscado->dato = dato_reemplazante;
  } else {
    nodo_t* nodo_hijo = (nodo_buscado->izq) ? nodo_buscado->izq : nodo_buscado->der;
    
    if(!padre) {
      arbol->raiz = nodo_hijo;
    } else {
      if(padre->izq && arbol->cmp(padre->izq->clave, nodo_buscado->clave) == 0) {
        padre->izq = nodo_hijo;
      } else if(padre->der) {
        padre->der = nodo_hijo;
      }
    }

    free(nodo_buscado->clave);
    free(nodo_buscado);
    arbol->cantidad--;
  }
  
  return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
  nodo_t* padre = NULL;
  nodo_t* nodo_buscado = abb_buscar(arbol->raiz, &padre, arbol->cmp, clave);
  if(!nodo_buscado) return NULL;
  return nodo_buscado->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
  nodo_t* padre = NULL;
  return abb_buscar(arbol->raiz, &padre, arbol->cmp, clave) != NULL;
}

size_t abb_cantidad(const abb_t *arbol) {
  return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
  _abb_destruir(arbol->raiz, arbol->destruir_dato);
  free(arbol);
}

bool _abb_in_order(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
  if(!nodo) return true;
  if(!_abb_in_order(nodo->izq, visitar, extra)) return false;
  if(!visitar(nodo->clave, nodo->dato, extra)) return false;
  return _abb_in_order(nodo->der, visitar, extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
  if(!visitar) return;
  _abb_in_order(arbol->raiz, visitar, extra);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
  abb_iter_t* nuevo_iterador = malloc(sizeof(abb_iter_t));
  if(!nuevo_iterador) return NULL;
  
  nuevo_iterador->pila = pila_crear(); 
  if(!nuevo_iterador->pila) {
    free(nuevo_iterador);
    return NULL;
  }
  nuevo_iterador->arbol = arbol;

  nodo_t* actual = arbol->raiz;
  while(actual != NULL) {
    pila_apilar(nuevo_iterador->pila, actual);
    actual = actual->izq;
  }

  return nuevo_iterador;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
  if(abb_iter_in_al_final(iter)) return false; 

  nodo_t* actual = ((nodo_t *) pila_desapilar(iter->pila));
  if(actual->der) {
    actual = actual->der;
    while(actual != NULL) {
      pila_apilar(iter->pila, actual);
      actual = actual->izq;
    }
  }

  return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
  if(abb_iter_in_al_final(iter)) return NULL;
  return ((nodo_t *) pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
  return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
  pila_destruir(iter->pila);
  free(iter);
}
