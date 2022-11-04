#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL   10
#define FACTOR_REDIMENSION   2
#define DIFERENCIA_CAPACIDAD 4

struct heap {
  void** arreglo;
  cmp_func_t cmp;
  size_t cantidad;
  size_t capacidad;
};

void swap(void** x, void** y) {
  void* aux = *x;
  *x = *y;
  *y = aux;
}

size_t calcular_padre(size_t i) {
  return (i - 1) / 2;
}

size_t calcular_hijo_izq(size_t i) {
  return (2 * i) + 1; 
}

size_t calcular_hijo_der(size_t i) {
  return (2 * i) + 2; 
}

void up_heap(heap_t* heap, size_t pos) {
  if(pos == 0) return;

  size_t pos_padre = calcular_padre(pos);
  void* padre_actual = heap->arreglo[pos_padre];
  void* actual = heap->arreglo[pos];

  if(heap->cmp(padre_actual, actual) >= 0) return;
  swap(&heap->arreglo[pos_padre], &heap->arreglo[pos]);

  return up_heap(heap, pos_padre);
}

size_t hijo_mayor(heap_t* heap, size_t pos) {
  size_t hijo_mayor = pos;
  size_t pos_hijo_izq = calcular_hijo_izq(pos);
  size_t pos_hijo_der = calcular_hijo_der(pos);

  if(pos_hijo_izq < heap->cantidad) {
    int diff_izq = heap->cmp(heap->arreglo[hijo_mayor], heap->arreglo[pos_hijo_izq]);
    if(diff_izq < 0) hijo_mayor = pos_hijo_izq;
  }    
  
  if(pos_hijo_der < heap->cantidad) {
    int diff_der = heap->cmp(heap->arreglo[hijo_mayor], heap->arreglo[pos_hijo_der]);
    if(diff_der < 0) hijo_mayor = pos_hijo_der;
  }
  
  return hijo_mayor;
}

void down_heap(heap_t* heap, size_t pos) {
  if(pos >= heap->cantidad) return;

  size_t pos_hijo_mayor = hijo_mayor(heap, pos);
  if(pos == pos_hijo_mayor) return;  
  swap(&heap->arreglo[pos_hijo_mayor], &heap->arreglo[pos]);

  return down_heap(heap, pos_hijo_mayor);
}

bool heap_redimensionar(heap_t* heap, size_t nueva_cap) {
  void** nuevo_arreglo = realloc(heap->arreglo, nueva_cap * sizeof(void *));
  if(!nuevo_arreglo) return false;

  heap->arreglo = nuevo_arreglo;
  heap->capacidad = nueva_cap;

  return true;
}

heap_t *heap_crear(cmp_func_t cmp) {
  heap_t* nuevo_heap = malloc(sizeof(heap_t));
  if(!nuevo_heap) return NULL;

  void** nuevo_arreglo = malloc(CAPACIDAD_INICIAL * sizeof(void *));
  if(!nuevo_arreglo) {
    free(nuevo_heap);
    return NULL;
  }

  nuevo_heap->arreglo = nuevo_arreglo;
  nuevo_heap->cmp = cmp;
  nuevo_heap->cantidad = 0;
  nuevo_heap->capacidad = CAPACIDAD_INICIAL;

  return nuevo_heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
  heap_t* nuevo_heap = malloc(sizeof(heap_t));
  if(!nuevo_heap) return NULL;

  void** nuevo_arreglo = malloc(n * sizeof(void *));
  if(!nuevo_arreglo) {
    free(nuevo_heap);
    return NULL;
  }

  nuevo_heap->arreglo = nuevo_arreglo;
  nuevo_heap->cmp = cmp;
  nuevo_heap->cantidad = 0;
  nuevo_heap->capacidad = n;

  for(int i = 0; i < n; i++) {
    nuevo_heap->arreglo[i] = arreglo[i];    
  }

  return nuevo_heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
  if(destruir_elemento) {
    for(size_t i = 0; i < heap->cantidad; i++) {
      void* actual = heap->arreglo[i];
      destruir_elemento(actual);
    }
  }
  free(heap->arreglo);
  free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
  return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
  return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
  if(heap->cantidad == heap->capacidad) {
    if(!heap_redimensionar(heap, heap->capacidad * FACTOR_REDIMENSION)) {
      return false;
    }
  }

  heap->arreglo[heap->cantidad] = elem;
  heap->cantidad++;
  
  up_heap(heap, heap->cantidad - 1);

  return true;
}

void *heap_ver_max(const heap_t *heap) { 
  if(heap_esta_vacio(heap)) return NULL;
  return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap) {
  if(heap_esta_vacio(heap)) return NULL;

  void* raiz = heap->arreglo[0];
  swap(&heap->arreglo[0], &heap->arreglo[heap->cantidad - 1]);
  heap->cantidad--;

  down_heap(heap, 0);

  size_t nueva_dim = heap->capacidad / FACTOR_REDIMENSION;
  if(
    (heap->cantidad * DIFERENCIA_CAPACIDAD <= heap->capacidad) && 
    (nueva_dim >= CAPACIDAD_INICIAL)
  ) { 
    heap_redimensionar(heap, nueva_dim);
  };
    
  return raiz;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {

}