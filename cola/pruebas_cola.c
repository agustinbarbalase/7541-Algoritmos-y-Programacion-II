#include "cola.h"
#include "pila.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>

static void prueba_cola_vacia(void) {
  printf("\nINICIO DE PRUEBAS DE COLA VACIA\n");
  cola_t *cola = cola_crear();

  print_test("Crear cola", cola != NULL);
  print_test("Cola recien creada es verdadero (Cond. Borde)", cola_esta_vacia(cola));
  print_test("Primero de cola vacia devuelve NULL", cola_ver_primero(cola) == NULL);
  print_test("Desencolar elemento de cola vacia devuelve NULL", 
    cola_ver_primero(cola) == NULL
  );

  cola_destruir(cola, free);
}

static void prueba_cola_elementos(void) {
  printf("\nINICIO DE PRUEBAS DE COLA ELEMENTOS\n");
  cola_t *cola = cola_crear();

  print_test("Crear cola", cola != NULL);

  /* Encolando elementos */
  int* primer_elemento = malloc(sizeof(int));
  int* segundo_elemento = malloc(sizeof(int));
  *primer_elemento = 5;
  *segundo_elemento = 7;
  print_test("Encolar elemento", cola_encolar(cola, primer_elemento));
  print_test("La cola no debe ser vacia", !cola_esta_vacia(cola));
  print_test("El primero debe ser el primer elemento", 
    cola_ver_primero(cola) == primer_elemento
  );
  print_test("Encolar elemento segundo elemento es valido", 
    cola_encolar(cola, segundo_elemento)
  );
  print_test("La cola no debe ser vacia", !cola_esta_vacia(cola));
  print_test("El primero debe seguir siendo el primer elemento", 
    cola_ver_primero(cola) == primer_elemento
  );

  /* Desencolando elementos */
  print_test("Desencolando elemento y comprobando invariaza", 
    cola_desencolar(cola) == primer_elemento
  );
  print_test("La cola no debe ser vacia", !cola_esta_vacia(cola));
  print_test("El primero debe ser el segundo elemento", 
    cola_ver_primero(cola) == segundo_elemento
  );

  print_test("Desencolando elemento y comprobando invariaza", 
    cola_desencolar(cola) == segundo_elemento
  );
  print_test("La cola debe ser vacia", cola_esta_vacia(cola));
  print_test("El primero debe ser NULL", 
    cola_ver_primero(cola) == NULL
  );

  free(primer_elemento);
  free(segundo_elemento);
  cola_destruir(cola, free);  
}

static void prueba_de_volumen(void) {
  printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

  cola_t *cola = cola_crear();

  print_test("Crear cola", cola != NULL);

  int VOLUMEN = 10000;

  /* Creamos vector para testear */
  int* vector = malloc(VOLUMEN * sizeof(int));
  for(int i = 0; i < VOLUMEN; i++) {
    vector[i] = i;
  }
  
  /* Encolando elementos */
  bool ok_encolar = true;
  int* primer_elemento = vector;
  for(int i = 0; i < VOLUMEN; i++) {
    ok_encolar &= cola_encolar(cola, vector + i);
    ok_encolar &= *((int *)cola_ver_primero(cola)) == *primer_elemento; // Comparando por valor
    ok_encolar &= cola_ver_primero(cola) == primer_elemento; // Comparando por direccion
    ok_encolar &= !cola_esta_vacia(cola);
  }
  print_test("Encolando muchos elementos", ok_encolar);

  /* Desencolando elementos */
  bool ok_desencolar = true;
  for(int i = 0; i < VOLUMEN; i++) {
    int* primero = cola_ver_primero(cola);
    int* elemento_desencolado = cola_desencolar(cola);

    if (i == VOLUMEN - 1) {
      ok_desencolar &= cola_esta_vacia(cola);
      ok_desencolar &= cola_ver_primero(cola) == NULL;
    } else {
      ok_desencolar &= !cola_esta_vacia(cola);
      ok_desencolar &= *primero == vector[i]; // Comparando por valor
      ok_desencolar &= primero == vector + i; // Comparando por direccion
      ok_desencolar &= *elemento_desencolado == vector[i]; // Comparando por valor
      ok_desencolar &= elemento_desencolado == vector + i; // Comparando por direccion
      ok_desencolar &= cola_ver_primero(cola) != NULL;
    }

  }
  print_test("Desencolando muchos elementos", ok_desencolar);
  
  free(vector);
  cola_destruir(cola, free);
}

static void prueba_cond_borde(void) {
  printf("\nINICIO DE PRUEBAS DE CONDICIONES BORDE\n");

  cola_t *primera_cola = cola_crear();

  print_test("La acción de esta_vacía en una cola recién creada es verdadero", 
    cola_esta_vacia(primera_cola)
  );
  print_test("Las acciones de desencolar y ver_primero", 
    cola_desencolar(primera_cola) == NULL && cola_ver_primero(primera_cola) == NULL
  );
  
  /*
    Comprobar que al desencolar hasta que está vacía hace que la cola se 
    comporte como recién creada.
  */
  print_test("cola es vacia", cola_esta_vacia(primera_cola));
  /*
    Las acciones de desencolar y ver_primero en 
    una cola a la que se le apiló y desapiló hasta estar vacía son inválidas.
  */
  print_test("Ver primero es nulo", cola_ver_primero(primera_cola) == NULL);
  print_test("Desencolar es nulo", cola_desencolar(primera_cola) == NULL);

  cola_t *segunda_cola = cola_crear();

  print_test("Crear cola", segunda_cola != NULL);

  /* El encolamiento del elemento NULL es válido */
  print_test("Encolar elemento NULL es valido", cola_encolar(segunda_cola, NULL));
  print_test("La cola no debe ser vacia", !cola_esta_vacia(segunda_cola));
  print_test("El primero debe ser NULL", cola_ver_primero(segunda_cola) == NULL);

  /* Desencolando elementos */
  print_test("Desencolando elemento y comprobando invariaza", 
    cola_desencolar(segunda_cola) == NULL
  );
  print_test("La cola no debe ser vacia", cola_esta_vacia(segunda_cola));

  cola_destruir(primera_cola, free);
  cola_destruir(segunda_cola, free);
}

/* Wrapper función pila_destruir() */
void pila_destruir_wrapper(void* pila) { pila_destruir(pila); }

static void prueba_destruccion(void) {
  printf("\nINICIO DE PRUEBAS DE DESTRUCCION\n");

  /* NULL con elementos */
  cola_t* primera_cola = cola_crear();
  int uno = 1;
  int dos = 2;

  print_test("Cola es vacia", cola_esta_vacia(primera_cola));
  print_test("Encolar 'uno'", cola_encolar(primera_cola, &uno));
  print_test("Encolar 'dos'", cola_encolar(primera_cola, &dos));

  cola_destruir(primera_cola, NULL);

  /* NULL sin elementos */
  cola_t* segunda_cola = cola_crear();

  print_test("Cola es vacia", cola_esta_vacia(segunda_cola));
  print_test("Encolar 'uno'", cola_encolar(segunda_cola, &uno));
  print_test("Encolar 'dos'", cola_encolar(segunda_cola, &dos));
  print_test("Desencolar 'uno'", cola_desencolar(segunda_cola) == &uno);
  print_test("Desencolar 'dos'", cola_desencolar(segunda_cola) == &dos);  

  cola_destruir(segunda_cola, NULL);

  /* Destruccion con una estructura de datos */
  cola_t* tercera_cola = cola_crear();
  pila_t* primera_pila = pila_crear();
  pila_t* segunda_pila = pila_crear();

  int VOLUMEN = 10;
  int* valor = malloc(sizeof(int));
  *valor = 10;
  for(int i = 0; i < VOLUMEN; i++) {
    pila_apilar(primera_pila, valor);
    pila_apilar(segunda_pila, valor);
  }

  print_test("Encolar 'Pila uno'", cola_encolar(tercera_cola, primera_pila));
  print_test("Encolar 'Pila dos'", cola_encolar(tercera_cola, segunda_pila));

  cola_destruir(tercera_cola, pila_destruir_wrapper);
  free(valor);

  /* Free sin elementos */
  cola_t* cuarta_cola = cola_crear();

  print_test("Cola es vacia", cola_esta_vacia(cuarta_cola));

  cola_destruir(cuarta_cola, free);
}

void pruebas_cola_estudiante() {
  prueba_cola_vacia();
  prueba_cola_elementos();
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
  pruebas_pila_estudiante();
  pruebas_cola_estudiante();
  return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
