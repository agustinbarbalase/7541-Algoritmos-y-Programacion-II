#include "pila.h"
#include "lista.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void prueba_lista_vacia(void) {
  printf("\nINICIO DE PRUEBAS DE LISTA VACIA\n");
  lista_t *lista = lista_crear();

  print_test("Crear lista", lista != NULL);
  print_test("Lista recien creada es verdadero (Cond. Borde)", lista_esta_vacia(lista));
  print_test("Primero de lista vacia devuelve NULL", lista_ver_primero(lista) == NULL);
  print_test("Borrar primero elemento de lista vacia devuelve NULL", 
    lista_ver_primero(lista) == NULL
  );

  lista_destruir(lista, free);
}

static void prueba_lista_elementos(void) {
  printf("\nINICIO DE PRUEBAS DE LISTA ELEMENTOS\n");
  lista_t *lista = lista_crear();

  print_test("Crear lista", lista != NULL);

  /* Insertando elementos al final */
  int* primer_elemento = malloc(sizeof(int));
  int* segundo_elemento = malloc(sizeof(int));
  *primer_elemento = 5;
  *segundo_elemento = 7;
  print_test("Insertar primer elemento", lista_insertar_ultimo(lista, primer_elemento));
  print_test("La lista no debe ser vacia", !lista_esta_vacia(lista));
  print_test("El primero debe ser el primer elemento", 
    lista_ver_primero(lista) == primer_elemento
  );
  print_test("Inserta primero elemento segundo elemento es valido", 
    lista_insertar_ultimo(lista, segundo_elemento)
  );
  print_test("La lista no debe ser vacia", !lista_esta_vacia(lista));
  print_test("El primero debe seguir siendo el primer elemento", 
    lista_ver_primero(lista) == primer_elemento
  );

  /* Borrando elementos */
  print_test("Desenlistando elemento y comprobando invariaza", 
    lista_borrar_primero(lista) == primer_elemento
  );
  print_test("La lista no debe ser vacia", !lista_esta_vacia(lista));
  print_test("El primero debe ser el segundo elemento", 
    lista_ver_primero(lista) == segundo_elemento
  );

  print_test("Desenlistando elemento y comprobando invariaza", 
    lista_borrar_primero(lista) == segundo_elemento
  );
  print_test("La lista debe ser vacia", lista_esta_vacia(lista));
  print_test("El primero debe ser NULL", 
    lista_ver_primero(lista) == NULL
  );

  free(primer_elemento);
  free(segundo_elemento);
  lista_destruir(lista, free);  
}

static void prueba_de_volumen(void) {
  printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");

  lista_t *lista = lista_crear();

  print_test("Crear lista", lista != NULL);

  int VOLUMEN = 10000;

  /* Creamos vector para testear */
  int* vector = malloc((size_t)VOLUMEN * sizeof(int));
  for(int i = 0; i < VOLUMEN; i++) {
    vector[i] = i;
  }
  
  /* Insertando elementos al final */
  bool ok_insertar_final = true;
  int* primer_elemento = vector;
  for(int i = 0; i < VOLUMEN; i++) {
    ok_insertar_final &= lista_insertar_ultimo(lista, vector + i);
    // Comparando por valor
    ok_insertar_final &= *((int *)lista_ver_primero(lista)) == *primer_elemento;
    // Comparando por direccion
    ok_insertar_final &= lista_ver_primero(lista) == primer_elemento;
    ok_insertar_final &= !lista_esta_vacia(lista);
  }
  print_test("Insertando muchos elementos al final", ok_insertar_final);

  /* Borrando elementos */
  bool ok_borrar_final = true;
  for(int i = 0; i < VOLUMEN; i++) {
    int* primero = lista_ver_primero(lista);
    int* elemento_borrado = lista_borrar_primero(lista);

    if (i == VOLUMEN - 1) {
      ok_borrar_final &= lista_esta_vacia(lista);
      ok_borrar_final &= lista_ver_primero(lista) == NULL;
    } else {
      ok_borrar_final &= !lista_esta_vacia(lista);
      ok_borrar_final &= *primero == vector[i]; // Comparando por valor
      ok_borrar_final &= primero == vector + i; // Comparando por direccion
      ok_borrar_final &= *elemento_borrado == vector[i]; // Comparando por valor
      ok_borrar_final &= elemento_borrado == vector + i; // Comparando por direccion
      ok_borrar_final &= lista_ver_primero(lista) != NULL;
    }

  }
  print_test("Borrando muchos elementos", ok_borrar_final);
  
  /* Insertando elementos al principio */
  bool ok_insertar_prin = true;
  for(int i = 0; i < VOLUMEN; i++) {
    ok_insertar_prin &= lista_insertar_primero(lista, vector + i);
    // Comparando por valor
    ok_insertar_prin &= *((int *)lista_ver_primero(lista)) == vector[i];
    // Comparando por direccion
    ok_insertar_prin &= lista_ver_primero(lista) == vector + i;
    ok_insertar_prin &= !lista_esta_vacia(lista);
  }
  print_test("Insertando muchos elementos al principio", ok_insertar_prin);


  /* Borrando elementos */
  bool ok_borrar_prin = true;
  for(int i = VOLUMEN - 1; i > 0; i--) {
    int* primero = lista_ver_primero(lista);
    int* elemento_borrado = lista_borrar_primero(lista);

    if (i == 0) {
      ok_borrar_prin &= lista_esta_vacia(lista);
      ok_borrar_prin &= lista_ver_primero(lista) == NULL;
    } else {
      ok_borrar_prin &= !lista_esta_vacia(lista);
      ok_borrar_prin &= *primero == vector[i]; // Comparando por valor
      ok_borrar_prin &= primero == vector + i; // Comparando por direccion
      ok_borrar_prin &= *elemento_borrado == vector[i]; // Comparando por valor
      ok_borrar_prin &= elemento_borrado == vector + i; // Comparando por direccion
      ok_borrar_prin &= lista_ver_primero(lista) != NULL;
    }

  }
  print_test("Borrando muchos elementos", ok_borrar_prin);

  free(vector);
  lista_destruir(lista, NULL);
}

static void prueba_cond_borde(void) {
  printf("\nINICIO DE PRUEBAS DE CONDICIONES BORDE\n");

  lista_t *primera_lista = lista_crear();

  print_test("La acción de esta_vacía en una lista recién creada es verdadero", 
    lista_esta_vacia(primera_lista)
  );
  print_test("Las acciones de borrar primero y ver_primero", 
    lista_borrar_primero(primera_lista) == NULL && lista_ver_primero(primera_lista) == NULL
  );
  
  /*
    Comprobar que al borrar primero hasta que está vacía hace que la lista se 
    comporte como recién creada.
  */
  print_test("Lista es vacia", lista_esta_vacia(primera_lista));
  /*
    Las acciones de borrar primero y ver_primero en 
    una lista a la que se le insertó y eliminó hasta estar vacía son inválidas.
  */
  print_test("Ver primero es nulo", lista_ver_primero(primera_lista) == NULL);
  print_test("Borrar primero es nulo", lista_borrar_primero(primera_lista) == NULL);

  lista_t *segunda_lista = lista_crear();

  print_test("Crear lista", segunda_lista != NULL);

  /* El insertado del elemento NULL es válido */
  print_test("Inserta primero elemento NULL es valido", 
    lista_insertar_ultimo(segunda_lista, NULL)
  );
  print_test("La lista no debe ser vacia", 
    !lista_esta_vacia(segunda_lista)
  );
  print_test("El primero debe ser NULL", 
    lista_ver_primero(segunda_lista) == NULL
  );

  /* Borrando elementos */
  print_test("Borrando elemento y comprobando invariaza", 
    lista_borrar_primero(segunda_lista) == NULL
  );
  print_test("La lista no debe ser vacia", lista_esta_vacia(segunda_lista));

  lista_destruir(primera_lista, free);
  lista_destruir(segunda_lista, free);
}

/* Wrapper función pila_destruir() */
void pila_destruir_wrapper(void* pila) { pila_destruir(pila); }

static void prueba_destruccion(void) {
  printf("\nINICIO DE PRUEBAS DE DESTRUCCION\n");

  /* NULL con elementos */
  lista_t* primera_lista = lista_crear();
  int uno = 1;
  int dos = 2;

  print_test("Lista es vacia", lista_esta_vacia(primera_lista));
  print_test("Inserta primero 'uno'", lista_insertar_ultimo(primera_lista, &uno));
  print_test("Inserta primero 'dos'", lista_insertar_ultimo(primera_lista, &dos));

  lista_destruir(primera_lista, NULL);

  /* NULL sin elementos */
  lista_t* segunda_lista = lista_crear();

  print_test("Lista es vacia", lista_esta_vacia(segunda_lista));
  print_test("Inserta primero 'uno'", lista_insertar_ultimo(segunda_lista, &uno));
  print_test("Inserta primero 'dos'", lista_insertar_ultimo(segunda_lista, &dos));
  print_test("Borrar primero 'uno'", lista_borrar_primero(segunda_lista) == &uno);
  print_test("Borrar primero 'dos'", lista_borrar_primero(segunda_lista) == &dos);  

  lista_destruir(segunda_lista, NULL);

  /* Destruccion con una estructura de datos */
  lista_t* tercera_lista = lista_crear();
  pila_t* primera_pila = pila_crear();
  pila_t* segunda_pila = pila_crear();

  int VOLUMEN = 10;
  int* valor = malloc(sizeof(int));
  *valor = 10;
  for(int i = 0; i < VOLUMEN; i++) {
    pila_apilar(primera_pila, valor);
    pila_apilar(segunda_pila, valor);
  }

  print_test("Inserta primero 'Pila uno'", 
    lista_insertar_ultimo(tercera_lista, primera_pila)
  );
  print_test("Inserta primero 'Pila dos'", 
    lista_insertar_ultimo(tercera_lista, segunda_pila)
  );

  lista_destruir(tercera_lista, pila_destruir_wrapper);
  free(valor);

  /* Free sin elementos */
  lista_t* cuarta_lista = lista_crear();

  print_test("Lista es vacia", lista_esta_vacia(cuarta_lista));

  lista_destruir(cuarta_lista, free);
}

bool ver_item(void* dato, void* extra) {
  (*(int *) extra)++;
  return true;
}

static void prueba_lista_iterador_interno() {
  printf("\nINICIO PRUEBAS ITERADOR INTERNO\n");

  lista_t *lista = lista_crear();

  print_test("Lista se creó", lista != NULL);

  bool ok_insertar_primera = true;
  ok_insertar_primera &= lista_insertar_ultimo(lista, "leche");
  ok_insertar_primera &= lista_insertar_ultimo(lista, "huevos");
  ok_insertar_primera &= lista_insertar_ultimo(lista, "pan");
  ok_insertar_primera &= lista_insertar_ultimo(lista, "mermelada");
  print_test("Se insertaron todos los elementos", ok_insertar_primera);

  int num_items = 0;
  lista_iterar(lista, ver_item, &num_items);
  print_test("La cantidad de elementos debe ser 4", num_items == 4);

  lista_destruir(lista, NULL);
}

bool ver_item_con_corte(void* dato, void* extra) {
  (*(int *) extra)++;
  if(strcmp(dato, "pan") == 0) return false;
  return true;
}

static void prueba_lista_iterador_interno_con_corte() {
  printf("\nINICIO PRUEBAS ITERADOR INTERNO CON CORTE\n");

  lista_t *lista = lista_crear();

  print_test("Lista se creó", lista != NULL);

  bool ok_insertar = true;
  ok_insertar &= lista_insertar_ultimo(lista, "leche");
  ok_insertar &= lista_insertar_ultimo(lista, "huevos");
  ok_insertar &= lista_insertar_ultimo(lista, "pan");
  ok_insertar &= lista_insertar_ultimo(lista, "mermelada");
  print_test("Se insertaron todos los elementos", ok_insertar);

  int num_items = 0;
  lista_iterar(lista, ver_item_con_corte, &num_items);
  print_test("La cantidad de elementos debe ser 3", num_items == 3);

  lista_destruir(lista, NULL);
}

static void prueba_lista_iterador_externo() {
  printf("\nINICIO DE PRUEBAS DE ITERADOR EXTERNO\n");
  
  lista_t* lista = lista_crear();
  
  int primero = 10;
  int segundo = 20;
  int tercero = 20;

  bool ok_insertar = true;
  ok_insertar &= lista_insertar_ultimo(lista, &primero);
  ok_insertar &= lista_insertar_ultimo(lista, &segundo);
  ok_insertar &= lista_insertar_ultimo(lista, &tercero);

  lista_iter_t* lista_iter = lista_iter_crear(lista);

  print_test("Lista se creó", lista != NULL);
  print_test("Se insertaron todos los elementos", ok_insertar);
  print_test("Iterador se creó", lista_iter != NULL);

  print_test("Actual debe ser primero",    
    lista_iter_ver_actual(lista_iter) == &primero
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("Actual debe ser segundo",    
    lista_iter_ver_actual(lista_iter) == &segundo
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("Actual debe ser tercero",
    lista_iter_ver_actual(lista_iter) == &tercero
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("Actual debe ser NULL",
    lista_iter_ver_actual(lista_iter) == NULL
  );
  print_test("El iterador no avanza y esta al final", 
    !lista_iter_avanzar(lista_iter) && lista_iter_al_final(lista_iter)
  );

  lista_destruir(lista, NULL);
  lista_iter_destruir(lista_iter);
}

static void prueba_lista_iterador_externo_inserciones() {
  printf("\nINICIO DE PRUEBAS DE ITERADOR EXTERNO CON INSERCIONES\n");
  
  lista_t* lista = lista_crear();
  
  int primero = 10;
  int segundo = 20;
  int tercero = 30;
  int cuarto = 40;
  int quinto = 50;

  bool ok_insertar = true;
  ok_insertar &= lista_insertar_ultimo(lista, &segundo);
  ok_insertar &= lista_insertar_ultimo(lista, &cuarto);

  lista_iter_t* lista_iter = lista_iter_crear(lista);
  print_test("Lista se creó", lista != NULL);
  print_test("Se insertaron todos los elementos", ok_insertar);
  print_test("Iterador se creó", lista_iter != NULL);
  
  print_test("El actual y primero es el segundo", 
    lista_ver_primero(lista) == &segundo && 
    lista_iter_ver_actual(lista_iter) == &segundo
  );
  print_test("Insertamos el primero", 
    lista_iter_insertar(lista_iter, &primero)
  );
  print_test("El actual y primero es el primero", 
    lista_ver_primero(lista) == &primero && 
    lista_iter_ver_actual(lista_iter) == &primero
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual es el segundo", 
		lista_iter_ver_actual(lista_iter) == &segundo
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual es el cuarto", 
		lista_iter_ver_actual(lista_iter) == &cuarto
  );
  print_test("Insertamos el tercero", 
    lista_iter_insertar(lista_iter, &tercero)
  );
  print_test("El actual es el tercero", 
		lista_iter_ver_actual(lista_iter) == &tercero
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual y el ultimo debe ser cuarto",
    lista_iter_ver_actual(lista_iter) == &cuarto &&
    lista_ver_ultimo(lista) == &cuarto
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual debe ser NULL",
    lista_iter_ver_actual(lista_iter) == NULL
  );
  print_test("El iterador no avanza y esta al final", 
    !lista_iter_avanzar(lista_iter) && lista_iter_al_final(lista_iter)
  );
  print_test("Insertamos el quinto", 
    lista_iter_insertar(lista_iter, &quinto)
  );
  print_test("El actual y el ultimo debe ser quinto",
    lista_iter_ver_actual(lista_iter) == &quinto &&
    lista_ver_ultimo(lista) == &quinto
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual debe ser NULL",
    lista_iter_ver_actual(lista_iter) == NULL
  );
  print_test("El iterador no avanza y esta al final", 
    !lista_iter_avanzar(lista_iter) && lista_iter_al_final(lista_iter)
  );
  
  lista_iter_destruir(lista_iter);
  lista_destruir(lista, NULL);
}

static void prueba_lista_iterador_externo_borrados() {
  printf("\nINICIO DE PRUEBAS DE ITERADOR EXTERNO CON BORRADOS\n");
  
  lista_t* lista = lista_crear();
  
  int primero = 10;
  int segundo = 20;
  int tercero = 30;
  int cuarto = 40;
  int quinto = 50;

  bool ok_insertar = true;
  ok_insertar &= lista_insertar_ultimo(lista, &primero);
  ok_insertar &= lista_insertar_ultimo(lista, &segundo);
  ok_insertar &= lista_insertar_ultimo(lista, &tercero);
  ok_insertar &= lista_insertar_ultimo(lista, &cuarto);
  ok_insertar &= lista_insertar_ultimo(lista, &quinto);

  lista_iter_t* lista_iter = lista_iter_crear(lista);
  print_test("Lista se creó", lista != NULL);
  print_test("Se insertaron todos los elementos", ok_insertar);
  print_test("Iterador se creó", lista_iter != NULL);
  
  print_test("El actual y primero es el primero", 
    lista_ver_primero(lista) == &primero && 
    lista_iter_ver_actual(lista_iter) == &primero
  );
  print_test("Borramos el primero", 
    lista_iter_borrar(lista_iter) == &primero
  );
  print_test("El actual y primero es el segundo", 
    lista_ver_primero(lista) == &segundo && 
    lista_iter_ver_actual(lista_iter) == &segundo
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual es el tercero", 
		lista_iter_ver_actual(lista_iter) == &tercero
  );
  print_test("Borramos el tercero", 
    lista_iter_borrar(lista_iter) == &tercero
  );
  print_test("El actual es el cuarto", 
		lista_iter_ver_actual(lista_iter) == &cuarto
  );
  print_test("Avanzamos el iterador", lista_iter_avanzar(lista_iter));
  print_test("El actual es el quinto", 
		lista_iter_ver_actual(lista_iter) == &quinto
  );
  print_test("Borramos el quinto", 
    lista_iter_borrar(lista_iter) == &quinto
  );
  print_test("El actual debe ser NULL y el ultimo debe ser cuarto",
    lista_iter_ver_actual(lista_iter) == NULL &&
    lista_ver_ultimo(lista) == &cuarto
  );
  print_test("El iterador no avanza y esta al final", 
    !lista_iter_avanzar(lista_iter) && lista_iter_al_final(lista_iter)
  );
  
  lista_iter_destruir(lista_iter);
  lista_destruir(lista, NULL);
}

static void prueba_lista_iterador_externo_multiples_operaciones() {
  printf("\nINICIO DE PRUEBAS DE ITERADOR EXTERNO CON MULTIPLES OPERACIONES\n");
  
  lista_t* lista = lista_crear();
  lista_iter_t* lista_iter = lista_iter_crear(lista);
  
  print_test("Creamos una lista y esta vacia", lista_esta_vacia(lista));
  print_test("Se pudo crear un iterador externo", lista_iter != NULL);
  
  int primero = 10;
  int segundo = 20;
  int tercero = 30;
  int medio = 40;

  print_test("Se pudo insertar un elemento con el iterador", 
		lista_iter_insertar(lista_iter, &primero)
  );
  print_test("La lista ya no esta vacia", !lista_esta_vacia(lista));
  print_test("El elemento insertado es el primero", 
    lista_ver_primero(lista) == &primero
  );
  print_test("Se puede avanzar el iterador", 
    lista_iter_avanzar(lista_iter)
  );
  print_test("El elemento actual del iterador apunta a NULL", 
		lista_iter_ver_actual(lista_iter) == NULL
  );
  print_test("Puedo volver a insertar un elemento", 
    lista_iter_insertar(lista_iter, &segundo)
  );
  print_test("El elemento se insertó en la posicion correcta", 
		lista_ver_ultimo(lista) == &segundo
  );	
  print_test("Puedo borrar el elemento actual y se devuelve el dato esperado",
		lista_iter_borrar(lista_iter) == &segundo
  );	
  print_test("El nuevo ultimo es el elemento esperado", 
    lista_ver_ultimo(lista) == &primero
  );
  print_test("El iterador ahora está al final", 
    lista_iter_al_final(lista_iter)
  );
  print_test("Agrego un segundo elemento con el iterador", 
    lista_iter_insertar(lista_iter, &segundo)
  );
  print_test("El iterador apunta al segundo elemento", 
    lista_iter_ver_actual(lista_iter) == &segundo
  );
  print_test("Agrego un tercer elemento con primitivas de lista", 
    lista_insertar_ultimo(lista, &tercero)
  );
  print_test("El iterador sigue apuntando al segundo elemento", 
    lista_iter_ver_actual(lista_iter) == &segundo
  );
  print_test("Puedo avanzar al iterador", lista_iter_avanzar(lista_iter));
  print_test("El iterador ahora apunta al tercer elemento", 
    lista_iter_ver_actual(lista_iter) == &tercero
  );
  print_test("Puedo agregar un elemento en el medio con el iterador", 
    lista_iter_insertar(lista_iter, &medio)
  );
  print_test("El iterador apunta a medio", 
    lista_iter_ver_actual(lista_iter) == &medio
  );
  print_test("Puedo borrar el elemento del medio", 
    lista_iter_borrar(lista_iter) == &medio
  );
  print_test("Ahora el iterador apunta al siguiente", 
    lista_iter_ver_actual(lista_iter) == &tercero
  );
  print_test("Puedo avanzar el iterador", lista_iter_avanzar(lista_iter));
  print_test("El iterador llego al final", lista_iter_al_final(lista_iter));

  lista_iter_destruir(lista_iter);
  lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante(void) {
  prueba_lista_vacia();
  prueba_lista_elementos();
  prueba_de_volumen();
  prueba_cond_borde();
  prueba_destruccion();
  prueba_lista_iterador_interno();
  prueba_lista_iterador_interno_con_corte();
  prueba_lista_iterador_externo();
  prueba_lista_iterador_externo_inserciones();
  prueba_lista_iterador_externo_borrados();
  prueba_lista_iterador_externo_multiples_operaciones();
  printf("\n");
}

#ifndef CORRECTOR

int main() {
  pruebas_lista_estudiante();
  return failure_count() > 0;
}

#endif
