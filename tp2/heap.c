#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL    10
#define FACTOR_REDIMENSION    2
#define DIFERENCIA_CAPACIDAD  4

struct heap {
  void** arreglo;
  size_t capacidad;
  size_t cantidad;
  cmp_func_t cmp;
};

void swap(void** x, void** y) {
  void* aux = *x;
  *x = *y;
  *y = aux;
}

size_t calcular_hijo_izq(size_t i) {
  return (2 * i) + 1;
}

size_t calcular_hijo_der(size_t i) {
  return (2 * i) + 2;
}

size_t calcular_padre(size_t i) {
  return (i - 1) / 2;
}

bool heap_redimensionar(heap_t* heap, size_t nueva_dim) {
  void** nuevo_arreglo = realloc(heap->arreglo, nueva_dim * sizeof(void *));
  if(!nuevo_arreglo) return false;

  heap->arreglo = nuevo_arreglo;
  heap->capacidad = nueva_dim;

  return true;
}

void upheap(void** arreglo, size_t pos, cmp_func_t cmp) {
  if(pos <= 0) return;

  size_t pos_padre = calcular_padre(pos);
  if(cmp(arreglo[pos_padre], arreglo[pos]) < 0) {
    swap(&arreglo[pos_padre], &arreglo[pos]);
    upheap(arreglo, pos_padre, cmp);
  }
}

void downheap(void** arreglo, size_t n, size_t pos, cmp_func_t cmp) {
  if(pos >= n) return;
  
  size_t hijo_mayor = pos;
  size_t pos_izq = calcular_hijo_izq(pos);
  size_t pos_der = calcular_hijo_der(pos);

  if(pos_izq < n && cmp(arreglo[hijo_mayor], arreglo[pos_izq]) < 0) {
    hijo_mayor = pos_izq;
  }
  
  if(pos_der < n && cmp(arreglo[hijo_mayor], arreglo[pos_der]) < 0) {
    hijo_mayor = pos_der;
  }

  if(pos != hijo_mayor) {
    swap(&arreglo[pos], &arreglo[hijo_mayor]);
    downheap(arreglo, n, hijo_mayor, cmp);
  }
}

void heapify(void** arreglo, size_t n, cmp_func_t cmp) {
  for(int i = (int) n; i >= 0; i--) {
    downheap(arreglo, n, i, cmp);
  }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
  heapify(elementos, cant, cmp);

  for(size_t i = cant - 1; i > 0; i--) {
    swap(&elementos[i], &elementos[0]);
    downheap(elementos, i, 0, cmp);
  }
}

heap_t *heap_crear(cmp_func_t cmp){
  heap_t* nuevo_heap = malloc(sizeof(heap_t));
  if(!nuevo_heap) return NULL;

  nuevo_heap->arreglo = malloc(CAPACIDAD_INICIAL * sizeof(void *));
  if(!nuevo_heap->arreglo) {
    free(nuevo_heap);
    return NULL;
  }

  nuevo_heap->capacidad = CAPACIDAD_INICIAL;
  nuevo_heap->cantidad = 0;
  nuevo_heap->cmp = cmp;

  return nuevo_heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
  heap_t* nuevo_heap = malloc(sizeof(heap_t));
  if(!nuevo_heap) return NULL;

  void** nuevo_arreglo = malloc(2 * n * sizeof(void *));
  if(!nuevo_arreglo) {
    free(nuevo_heap);
    return NULL;
  }

  for(int i = 0; i < n; i++) {
    nuevo_arreglo[i] = arreglo[i];
  }

  heapify(nuevo_arreglo, n, cmp);

  nuevo_heap->arreglo = nuevo_arreglo;
  nuevo_heap->capacidad = 2 * n;
  nuevo_heap->cantidad = n;
  nuevo_heap->cmp = cmp;

  return nuevo_heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
  if(destruir_elemento) {
    for(size_t i = 0; i < heap->cantidad; i++) {
      destruir_elemento(heap->arreglo[i]);
    }
  }
  free(heap->arreglo);
  free(heap);
}

size_t heap_cantidad(const heap_t *heap){
  return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
  return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
  if(heap->cantidad == heap->capacidad) {
    if(!heap_redimensionar(heap, heap->capacidad * FACTOR_REDIMENSION)) { 
      return false;
    }
  }

  heap->arreglo[heap->cantidad] = elem;
  upheap(heap->arreglo, heap->cantidad, heap->cmp);
  heap->cantidad++;

  return true;
}

void *heap_ver_max(const heap_t *heap){
  if(heap_esta_vacio(heap)) return NULL;
  return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap){
  if(heap_esta_vacio(heap)) return NULL;

  void* maximo = heap->arreglo[0];
  heap->cantidad--;
  
  swap(&(heap->arreglo[0]), &(heap->arreglo[heap->cantidad]));
  downheap(heap->arreglo, heap->cantidad, 0, heap->cmp);

  size_t nueva_dim = heap->capacidad / FACTOR_REDIMENSION;
  if(
    (heap->cantidad * DIFERENCIA_CAPACIDAD <= heap->capacidad) &&
    (nueva_dim >= CAPACIDAD_INICIAL)
  ) {
    heap_redimensionar(heap, nueva_dim);
  }  

  return maximo;
}