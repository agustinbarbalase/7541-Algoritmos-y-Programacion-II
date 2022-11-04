#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "testing.h"

int cmp_datos(const void *a, const void *b) {
  return *((int *) a) > *((int *) b) ? 1 : -1;
}

static void prueba_heap_vacio(void) {
  printf("\nPRUEBAS DE HEAP VACIO\n");
  heap_t *heap = heap_crear(cmp_datos);

  print_test("Crear heap", heap != NULL);
  print_test("heap recien creada es verdadero (Cond. Borde)", heap_esta_vacio(heap));
  print_test("El maximo de heap vacio devuelve NULL", heap_ver_max(heap) == NULL);
  print_test("Desencolar elemento de heap vacio devuelve NULL", 
    heap_desencolar(heap) == NULL
  );

  heap_destruir(heap, free);
}

static void prueba_heap_elementos(void) {
  printf("\nPRUEBAS DE HEAP ELEMENTOS\n");
  heap_t *heap = heap_crear(cmp_datos);

  print_test("Crear heap", heap != NULL);

  /* Encolando elementos */
  int* primer_elemento = malloc(sizeof(int));
  int* segundo_elemento = malloc(sizeof(int));
  *primer_elemento = 5;
  *segundo_elemento = 7;

  print_test("Encolar elemento", heap_encolar(heap, primer_elemento));
  print_test("El heap no debe ser vacio", !heap_esta_vacio(heap));
  print_test("El primero debe ser el maximo", 
    (int *) heap_ver_max(heap) == primer_elemento
  );
  print_test("La cantidad debe ser 1", heap_cantidad(heap) == 1);

  print_test("Encolar elemento segundo elemento es valido", 
    heap_encolar(heap, segundo_elemento)
  );
  print_test("El heap no debe ser vacio", !heap_esta_vacio(heap));
  print_test("La cantidad debe ser 2", heap_cantidad(heap) == 2);
  print_test("El maximo debe ser el segundo elemento", 
    *((int *) heap_ver_max(heap)) == *segundo_elemento
  );

  /* Desencolando elementos */
  print_test("Desencolando elemento y comprobando invariaza", 
    heap_desencolar(heap) == segundo_elemento
  );
  print_test("El heap no debe ser vacio", !heap_esta_vacio(heap));
  print_test("El primero debe ser el primer elemento", 
    *((int *) heap_ver_max(heap)) == *primer_elemento
  );

  print_test("Desencolando elemento y comprobando invariaza", 
    heap_desencolar(heap) == primer_elemento
  );
  print_test("El heap debe ser vacio", heap_esta_vacio(heap));
  print_test("El primero debe ser NULL", 
    (int *) heap_ver_max(heap) == NULL
  );

  *primer_elemento = 7;
  *segundo_elemento = 5;

  print_test("Encolar elemento", heap_encolar(heap, primer_elemento));
  print_test("El heap no debe ser vacio", !heap_esta_vacio(heap));
  print_test("El primero debe ser el maximo", 
    (int *) heap_ver_max(heap) == primer_elemento
  );
  print_test("La cantidad debe ser 1", heap_cantidad(heap) == 1);

  print_test("Encolar elemento segundo elemento es valido", 
    heap_encolar(heap, segundo_elemento)
  );
  print_test("El heap no debe ser vacio", !heap_esta_vacio(heap));
  print_test("La cantidad debe ser 2", heap_cantidad(heap) == 2);
  print_test("El maximo debe ser el primer elemento", 
    *((int *) heap_ver_max(heap)) == *primer_elemento
  );

  /* Desencolando elementos */
  print_test("Desencolando elemento y comprobando invariaza", 
    heap_desencolar(heap) == primer_elemento
  );
  print_test("El heap no debe ser vacio", !heap_esta_vacio(heap));
  print_test("El primero debe ser el segundo elemento", 
    *((int *) heap_ver_max(heap)) == *segundo_elemento
  );

  print_test("Desencolando elemento y comprobando invariaza", 
    heap_desencolar(heap) == segundo_elemento
  );
  print_test("El heap debe ser vacio", heap_esta_vacio(heap));
  print_test("El primero debe ser NULL", 
    (int *) heap_ver_max(heap) == NULL
  );

  free(primer_elemento);
  free(segundo_elemento);
  heap_destruir(heap, free);  
}

static void prueba_de_volumen(void) {
  printf("\nPRUEBAS DE VOLUMEN\n");

  heap_t *heap = heap_crear(cmp_datos);

  print_test("Crear heap", heap != NULL);

  int VOLUMEN = 10000;

  /* Creamos vector para testear */
  int* vector = malloc(VOLUMEN * sizeof(int));
  for(int i = 0; i < VOLUMEN; i++) {
    vector[i] = i;
  }
  
  /* Encolando elementos */
  bool ok_encolar = true;
  int* elemento_maximo = vector;
  for(int i = 0; i < VOLUMEN; i++) {
    elemento_maximo = vector + i;
    ok_encolar &= heap_encolar(heap, vector + i);
    ok_encolar &= *((int *)(int *) heap_ver_max(heap)) == *elemento_maximo; // Comparando por valor
    ok_encolar &= (int *) heap_ver_max(heap) == elemento_maximo; // Comparando por direccion
    ok_encolar &= !heap_esta_vacio(heap);
  }
  print_test("Encolando muchos elementos", ok_encolar);

  /* Desencolando elementos */
  bool ok_desencolar = true;
  for(int i = VOLUMEN - 1; i >= 0; i--) {
    int* primero = (int *) heap_ver_max(heap);
    int* elemento_desencolado = heap_desencolar(heap);

    if (i == 0) {
      ok_desencolar &= heap_esta_vacio(heap);
      ok_desencolar &= (int *) heap_ver_max(heap) == NULL;
    } else {
      ok_desencolar &= !heap_esta_vacio(heap);
      ok_desencolar &= *primero == vector[i]; // Comparando por valor
      ok_desencolar &= primero == vector + i; // Comparando por direccion
      ok_desencolar &= *elemento_desencolado == vector[i]; // Comparando por valor
      ok_desencolar &= elemento_desencolado == vector + i; // Comparando por direccion
      ok_desencolar &= (int *) heap_ver_max(heap) != NULL;
    }

  }
  print_test("Desencolando muchos elementos", ok_desencolar);
  
  free(vector);
  heap_destruir(heap, NULL);
}

static void prueba_cond_borde(void) {
  printf("\nPRUEBAS DE CONDICIONES BORDE\n");

  heap_t *primer_heap = heap_crear(cmp_datos);

  print_test("La acción de esta_vacía en una heap recién creada es verdadero", 
    heap_esta_vacio(primer_heap)
  );
  print_test("Las acciones de desencolar y ver_primero", 
    heap_desencolar(primer_heap) == NULL && (int *) heap_ver_max(primer_heap) == NULL
  );
  
  /*
    Comprobar que al desencolar hasta que está vacía hace que El heap se 
    comporte como recién creada.
  */
  print_test("heap es vacio", heap_esta_vacio(primer_heap));
  /*
    Las acciones de desencolar y ver_primero en 
    una heap a la que se le apiló y desapiló hasta estar vacía son inválidas.
  */
  print_test("Ver primero es nulo", (int *) heap_ver_max(primer_heap) == NULL);
  print_test("Desencolar es nulo", heap_desencolar(primer_heap) == NULL);

  heap_t *segundo_heap = heap_crear(cmp_datos);

  print_test("Crear heap", segundo_heap != NULL);

  /* El encolamiento del elemento NULL es válido */
  print_test("Encolar elemento NULL es valido", heap_encolar(segundo_heap, NULL));
  print_test("El heap no debe ser vacio", !heap_esta_vacio(segundo_heap));
  print_test("El primero debe ser NULL", (int *) heap_ver_max(segundo_heap) == NULL);

  /* Desencolando elementos */
  print_test("Desencolando elemento y comprobando invariaza", 
    heap_desencolar(segundo_heap) == NULL
  );
  print_test("El heap no debe ser vacio", heap_esta_vacio(segundo_heap));

  heap_destruir(primer_heap, free);
  heap_destruir(segundo_heap, free);
}

static void prueba_destruccion(void) {
  printf("\nPRUEBAS DE DESTRUCCION\n");

  /* NULL con elementos */
  heap_t* primer_heap = heap_crear(cmp_datos);
  int uno = 1;
  int dos = 2;

  print_test("heap es vacio", heap_esta_vacio(primer_heap));
  print_test("Encolar 'uno'", heap_encolar(primer_heap, &uno));
  print_test("Encolar 'dos'", heap_encolar(primer_heap, &dos));

  heap_destruir(primer_heap, NULL);

  /* NULL sin elementos */
  heap_t* segundo_heap = heap_crear(cmp_datos);

  print_test("heap es vacio", heap_esta_vacio(segundo_heap));
  print_test("Encolar 'uno'", heap_encolar(segundo_heap, &uno));
  print_test("Encolar 'dos'", heap_encolar(segundo_heap, &dos));
  print_test("Desencolar debe ser 'dos'", 
    heap_desencolar(segundo_heap) == &dos
  );
  print_test("Desencolar debe ser 'uno'", 
    heap_desencolar(segundo_heap) == &uno
  );  

  heap_destruir(segundo_heap, NULL);

  /* Free sin elementos */
  heap_t* cuarto_heap = heap_crear(cmp_datos);

  print_test("heap es vacio", heap_esta_vacio(cuarto_heap));

  heap_destruir(cuarto_heap, free);
}

void pruebas_heap_estudiante() {
  prueba_heap_vacio();
  prueba_heap_elementos();
  prueba_de_volumen();
  prueba_cond_borde();
  prueba_destruccion();
  printf("\n");
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
  pruebas_heap_estudiante();
  return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif