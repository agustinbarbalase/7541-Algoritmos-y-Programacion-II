/*
 * prueba_abb.c
 * Pruebas para el tipo de dato abstracto ABB
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "testing.h"
#include "abb.h"

static void prueba_crear_abb_vacio() {
  printf("\nPRUEBA CREAR ABB VACIO\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  print_test("Prueba abb crear abb vacio", abb);
  print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
  print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
  print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
  print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

  abb_destruir(abb);
}

static void prueba_iterar_abb_vacio() {
  printf("\nPRUEBA ITERAR ABB VACIO\n");

  abb_t* abb = abb_crear(strcmp, NULL);
  abb_iter_t* iter = abb_iter_in_crear(abb);
  print_test("Prueba abb iter crear iterador abb vacio", iter);
  print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
  print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
  print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

  abb_iter_in_destruir(iter);
  abb_destruir(abb);
}

static void prueba_abb_insertar() {
  printf("\nPRUEBA ABB INSERTAR\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  char *clave1 = "perro", *valor1 = "guau";
  char *clave2 = "gato", *valor2 = "miau";
  char *clave3 = "vaca", *valor3 = "mu";

  /* Inserta 1 valor y luego lo borra */
  print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
  print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
  print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
  print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
  print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
  print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

  /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
  print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
  print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
  print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
  print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

  print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
  print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
  print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
  print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

  abb_destruir(abb);
}

static void prueba_abb_reemplazar() {
  printf("\nPRUEBA ABB REEMPLAZAR\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
  char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

  /* Inserta 2 valores y luego los reemplaza */
  print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
  print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
  print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
  print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
  print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
  print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
  print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

  print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
  print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
  print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
  print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
  print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
  print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
  print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

  abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir() {
  printf("\nPRUEBA ABB REEMPLAZAR CON DESTRUIR\n");

  abb_t* abb = abb_crear(strcmp, free);

  char *clave1 = "perro", *valor1a, *valor1b;
  char *clave2 = "gato", *valor2a, *valor2b;

  /* Pide memoria para 4 valores */
  valor1a = malloc(10 * sizeof(char));
  valor1b = malloc(10 * sizeof(char));
  valor2a = malloc(10 * sizeof(char));
  valor2b = malloc(10 * sizeof(char));

  /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
  print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
  print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
  print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
  print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
  print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
  print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
  print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

  print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
  print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
  print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
  print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
  print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
  print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
  print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

  /* Se destruye el abb (se debe liberar lo que quedó dentro) */
  abb_destruir(abb);
}

static void prueba_abb_borrar() {
  printf("\nPRUEBA ABB BORRAR\n");

  char *clave1 = "PERRO_A", *valor1 = "GUAU_A";
  char *clave2 = "PERRO_B", *valor2 = "GUAU_B";
  char *clave3 = "PERRO_C", *valor3 = "GUAU_C";
  char *clave4 = "PERRO_D", *valor4 = "GUAU_D";
  char *clave5 = "PERRO_E", *valor5 = "GUAU_E";
  char *clave6 = "PERRO_F", *valor6 = "GUAU_F";
  
  /* Sin hijos */
  abb_t* abb = abb_crear(strcmp, NULL);
  print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, valor4));
  print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
  print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, valor5));
  print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
  print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
  print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, valor6));
  print_test("Prueba abb la cantidad de elementos es 6", abb_cantidad(abb) == 6);
  print_test("Prueba abb pertenece clave1, es verdadero", 
    abb_pertenece(abb, clave1)
  );
  print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
  print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave1));
  print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
  print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
  print_test("Prueba abb la cantidad de elementos es 5", abb_cantidad(abb) == 5);
  abb_destruir(abb);

  /* Con 1 hijo */
  abb = abb_crear(strcmp, NULL);
  print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, valor4));
  print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
  print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, valor5));
  print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
  print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
  print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, valor6));
  print_test("Prueba abb la cantidad de elementos es 6", abb_cantidad(abb) == 6);
  print_test("Prueba abb pertenece clave5, es verdadero", 
    abb_pertenece(abb, clave5)
  );
  print_test("Prueba abb borrar clave5, es valor5", abb_borrar(abb, clave5) == valor5);
  print_test("Prueba abb borrar clave5, es NULL", !abb_borrar(abb, clave5));
  print_test("Prueba abb pertenece clave5, es falso", !abb_pertenece(abb, clave5));
  print_test("Prueba abb obtener clave5, es NULL", !abb_obtener(abb, clave5));
  print_test("Prueba abb la cantidad de elementos es 5", abb_cantidad(abb) == 5);
  abb_destruir(abb);

  /* Con 2 hijos */
  abb = abb_crear(strcmp, NULL);
  print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, valor4));
  print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
  print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, valor5));
  print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
  print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
  print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, valor6));
  print_test("Prueba abb la cantidad de elementos es 6", abb_cantidad(abb) == 6);
  print_test("Prueba abb pertenece clave2, es verdadero", 
    abb_pertenece(abb, clave2)
  );
  print_test("Prueba abb borrar clave2, es valor5", abb_borrar(abb, clave2) == valor2);
  print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave2));
  print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
  print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
  print_test("Prueba abb la cantidad de elementos es 5", abb_cantidad(abb) == 5);
  abb_destruir(abb);
}

static void prueba_abb_clave_vacia() {
  printf("\nPRUEBA ABB CLAVE VACIA\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  char *clave = "", *valor = "";

  print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
  print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
  print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
  print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
  print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
  print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

  abb_destruir(abb);
}

static void prueba_abb_valor_null() {
  printf("\nPRUEBA ABB VALOR NULL\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  char *clave = "", *valor = NULL;

  /* Inserta 1 valor y luego lo borra */
  print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
  print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
  print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
  print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
  print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
  print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

  abb_destruir(abb);
}

void swap(void* x, void* y) {
  void* aux = x;
  x = y;
  y = aux;
}

static void prueba_abb_volumen(size_t largo, bool debug) {
  printf("\nPRUEBA ABB VOLUMEN\n");

  clock_t start = clock();

  srand((unsigned) time(NULL));

  const size_t largo_clave = 10;
  char (*claves)[largo_clave] = malloc(largo * largo_clave);

  unsigned* valores[largo];

  for(unsigned i = 0; i < largo; i++) {
    valores[i] = malloc(sizeof(int));
    sprintf(claves[i], "%08d", i);
    *valores[i] = i;
  }

  for(size_t i = 0; i < largo; i++) {
    size_t pos_uno = (size_t)rand() % largo;
    size_t pos_dos = (size_t)rand() % largo;
    
    if(pos_uno == pos_dos) continue;

    swap(valores[pos_uno], valores[pos_dos]);
    swap(claves[pos_uno], claves[pos_dos]);
  }

  abb_t* abb = abb_crear(strcmp, NULL);

  bool ok = true;
  for(unsigned i = 0; i < largo; i++) {
    ok = abb_guardar(abb, claves[i], valores[i]);
    if (!ok) break;
  }

  if(debug) {
    print_test("Prueba abb almacenar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", 
      abb_cantidad(abb) == largo
    );
  }

  /* Verifica que devuelva los valores correctos */
  for(size_t i = 0; i < largo; i++) {
    ok = abb_pertenece(abb, claves[i]);
    if (!ok) break;
    ok = abb_obtener(abb, claves[i]) == valores[i];
    if (!ok) break;
  }

  if(debug) {
    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", 
      abb_cantidad(abb) == largo
    );
  } 

  /* Verifica que borre y devuelva los valores correctos */
  for(size_t i = 0; i < largo; i++) {
    ok = abb_borrar(abb, claves[i]) == valores[i];
    if (!ok) break;
  }

  if(debug) {
    print_test("Prueba abb borrar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es 0", 
      abb_cantidad(abb) == 0
    );
  }

  /* Destruye el abb y crea uno nuevo que sí libera */
  abb_destruir(abb);
  abb = abb_crear(strcmp, free);

  /* Inserta 'largo' parejas en el abb */
  ok = true;
  for(size_t i = 0; i < largo; i++) {
    ok = abb_guardar(abb, claves[i], valores[i]);
    if (!ok) break;
  }

  free(claves);

  /* Destruye el abb - debería liberar los enteros */
  abb_destruir(abb);

  clock_t finish = clock();

  printf("\nDuracion total: %.2f s\n", (float) (finish - start) / 1000);
}

static int buscar(const char* clave, char* claves[], int largo) {
  for(int i = 0; i < largo; i++) {
    if(strcmp(clave, claves[i]) == 0) return i;
  }
  return -1;
}

static void prueba_abb_iterar() {
  printf("\nPRUEBA ABB ITERAR\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  char *claves[] = {"perro", "gato", "vaca"};
  char *valores[] = {"guau", "miau", "mu"};

  /* Inserta 3 valores */
  print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
  print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
  print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

  // Prueba de iteración sobre las claves almacenadas.
  abb_iter_t* iter = abb_iter_in_crear(abb);
  const char *clave;
  int indice;

  print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

  /* Primer valor */
  clave = abb_iter_in_ver_actual(iter);
  indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
  print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
  print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
  print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
  print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

  /* Segundo valor */
  clave = abb_iter_in_ver_actual(iter);
  indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
  print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
  print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
  print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
  print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

  /* Tercer valor */
  clave = abb_iter_in_ver_actual(iter);
  indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
  print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
  print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
  abb_iter_in_avanzar(iter);
  print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

  /* Vuelve a tratar de avanzar, por las dudas */
  print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
  print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
  print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

  abb_iter_in_destruir(iter);
  abb_destruir(abb);
}

static void prueba_abb_iterar_volumen(size_t largo) {
  printf("\nPRUEBA ABB ITERAR VOLUMEN\n");

  clock_t start = clock();

  srand((unsigned) time(NULL));
  const size_t largo_clave = 10;
  char (*claves)[largo_clave] = malloc(largo * largo_clave);

  size_t valores[largo];

  for(unsigned i = 0; i < largo; i++) {
    sprintf(claves[i], "%08d", i);
    valores[i] = i;
  }

  for(size_t i = 0; i < largo; i++) {
    size_t pos_uno = (size_t)rand() % largo;
    size_t pos_dos = (size_t)rand() % largo;
    
    if(pos_uno == pos_dos) continue;

    swap(valores + pos_uno, valores + pos_dos);
    swap(claves[pos_uno], claves[pos_dos]);
  }

  abb_t* abb = abb_crear(strcmp, NULL);

  bool ok = true;
  for(unsigned i = 0; i < largo; i++) {
    ok = abb_guardar(abb, claves[i], &valores[i]);
    if (!ok) break;
  }

  // Prueba de iteración sobre las claves almacenadas.
  abb_iter_t* iter = abb_iter_in_crear(abb);
  print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

  ok = true;
  unsigned i;
  const char *clave;
  size_t *valor;

  for (i = 0; i < largo; i++) {
    if(abb_iter_in_al_final(iter)) {
      ok = false;
      break;
    }
    clave = abb_iter_in_ver_actual(iter);
    if(clave == NULL) {
      ok = false;
      break;
    }
    valor = abb_obtener(abb, clave);
    if(valor == NULL) {
      ok = false;
      break;
    }
    *valor = largo;
    abb_iter_in_avanzar(iter);
  }
  print_test("Prueba abb iteración en volumen", ok);
  print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
  print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

  ok = true;
  for(i = 0; i < largo; i++) {
    if(valores[i] != largo) {
      ok = false;
      break;
    }
  }
  print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

  free(claves);
  abb_iter_in_destruir(iter);
  abb_destruir(abb);

  clock_t finish = clock();

  printf("\nDuracion total: %.2f s\n", (float) (finish - start) / 1000);
}

bool sumar_elementos(const char* clave, void* dato, void* extra) {
  int* suma = (int *) extra;
  *suma += *((int *) dato);
  return true;
}

bool sumar_elementos_con_corte(const char* clave, void* dato, void* extra) {
  if(strcmp(clave, "vaca") == 0) return false;
  int* suma = (int *) extra;
  *suma += *((int *) dato);
  return true;
}

static void prueba_abb_iterar_interno() {
  printf("\nPRUEBA ABB ITERAR INTERNO\n");

  abb_t* abb = abb_crear(strcmp, NULL);

  char *claves[] = {"perro", "gato", "vaca"};
  int valores[3] = {1, 2, 3};

  /* Inserta 3 valores */
  print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], &valores[0]));
  print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], &valores[1]));
  print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], &valores[2]));

  /* Prueba sin corte */
  int extra_uno = 0;
  abb_in_order(abb, sumar_elementos, &extra_uno);
  print_test("Prueba abb el extra_uno debe ser 6", extra_uno == 6); 

  /* Prueba con corte */
  int extra_dos = 0;
  abb_in_order(abb, sumar_elementos_con_corte, &extra_dos);
  print_test("Prueba abb el extra_dos debe ser 3", extra_dos == 3); 

  abb_destruir(abb);
}

/*
  Funcion que devuelve la suma de los primeros N naturales
*/
size_t suma_de_los_primeros_n(size_t n) {
  return (n * (n + 1)) / 2;
}

bool sumar_elementos_con_corte_dos(const char* clave, void* dato, void* extra) {
  if(strcmp(clave, "00000301") == 0) return false;
  int* suma = (int *) extra;
  *suma += *((int *) dato);
  return true;
}

static void prueba_abb_iterar_interno_volumen() {
  printf("\nPRUEBA ABB ITERAR INTERNO VOLUMEN\n");

  int LARGO = 1000;
  clock_t start = clock();

  srand((unsigned) time(NULL));
  const size_t largo_clave = 10;
  char (*claves)[largo_clave] = malloc(LARGO * largo_clave);

  size_t valores[LARGO];

  for(unsigned i = 0; i < LARGO; i++) {
    sprintf(claves[i], "%08d", i + 1);
    valores[i] = i + 1;
  }

  for(size_t i = 0; i < LARGO; i++) {
    size_t pos_uno = (size_t)rand() % LARGO;
    size_t pos_dos = (size_t)rand() % LARGO;
    
    if(pos_uno == pos_dos) continue;

    swap(valores + pos_uno, valores + pos_dos);
    swap(claves[pos_uno], claves[pos_dos]);
  }

  abb_t* abb = abb_crear(strcmp, NULL);

  bool ok = true;
  for(unsigned i = 0; i < LARGO; i++) {
    ok = abb_guardar(abb, claves[i], &valores[i]);
    if (!ok) break;
  }

  /* Prueba sin corte */
  int extra_uno = 0;
  abb_in_order(abb, sumar_elementos, &extra_uno);
  print_test("Prueba abb el extra_uno debe ser la suma de los primeros 1000 naturales", 
    extra_uno == suma_de_los_primeros_n(LARGO)
  );

  /* Prueba con corte */
  int extra_dos = 0;
  abb_in_order(abb, sumar_elementos_con_corte_dos, &extra_dos);
  print_test("Prueba abb el extra_dos debe ser la suma de los primeros 300 naturales", 
    extra_dos == suma_de_los_primeros_n(300)
  );

  free(claves);
  abb_destruir(abb);

  clock_t finish = clock();

  printf("\nDuracion total: %.2f s\n", (float) (finish - start) / 1000);
}

void pruebas_abb_estudiante(void) {
  prueba_crear_abb_vacio();
  prueba_iterar_abb_vacio();
  prueba_abb_insertar();
  prueba_abb_reemplazar();
  prueba_abb_reemplazar_con_destruir();
  prueba_abb_borrar();
  prueba_abb_clave_vacia();
  prueba_abb_valor_null();
  prueba_abb_volumen(1000, true);
  prueba_abb_iterar();
  prueba_abb_iterar_volumen(1000);
  prueba_abb_iterar_interno();
  prueba_abb_iterar_interno_volumen();
  printf("\n");
}

#ifndef CORRECTOR

int main() {
  pruebas_abb_estudiante();
  return failure_count() > 0;
}

#endif
