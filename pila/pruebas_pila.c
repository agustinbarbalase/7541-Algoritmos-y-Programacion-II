#include "pila.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>

static void prueba_pila_vacia(void) {
    printf("\nINICIO DE PRUEBAS CON PILA VACIA\n");
    pila_t *pila = pila_crear();

    print_test("Crear pila", pila != NULL);
    print_test("Pila recien creada es verdadero (Cond. Borde)", pila_esta_vacia(pila));
    print_test("Tope de pila vacia devuelve NULL", pila_ver_tope(pila) == NULL);
    print_test("Desapilar elemento pila vacia devuelve NULL", 
        pila_desapilar(pila) == NULL
    );

    pila_destruir(pila);
}

static void prueba_pila_elementos(void) {
    printf("\nINICIO DE PRUEBAS CON PILA CON ELEMENTOS\n");
    pila_t *pila = pila_crear();

    print_test("Crear pila", pila != NULL);

    /* Apilando elementos */
    int* primer_elemento = malloc(sizeof(int));
    *primer_elemento = 5;
    print_test("Apilar elemento", pila_apilar(pila, &primer_elemento));
    print_test("La pila no debe ser vacia", !pila_esta_vacia(pila));
    print_test("El tope debe ser el primer elemento", 
        pila_ver_tope(pila) == &primer_elemento
    );

    /* El apilamiento del elemento NULL es válido */
    print_test("Apilar elemento NULL es valido", pila_apilar(pila, NULL));
    print_test("La pila no debe ser vacia", !pila_esta_vacia(pila));
    print_test("El tope debe ser NULL", pila_ver_tope(pila) == NULL);

    /* Desapilando elementos */
    print_test("Desapilando elemento y comprobando invariaza", 
        pila_desapilar(pila) == NULL
    );
    print_test("La pila no debe ser vacia", !pila_esta_vacia(pila));
    print_test("El tope debe ser el primer elemento", 
        pila_ver_tope(pila) == &primer_elemento
    );

    print_test("Desapilando elemento y comprobando invariaza", 
        pila_desapilar(pila) == &primer_elemento
    );
    print_test("La pila debe ser vacia", pila_esta_vacia(pila));
    print_test("El tope debe ser NULL", 
        pila_ver_tope(pila) == NULL
    );

    free(primer_elemento);
    pila_destruir(pila);
}

static void prueba_de_volumen(void) {
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");
    pila_t *pila = pila_crear();

    print_test("Crear pila", pila != NULL);

    int VOLUMEN = 10000;

    /* Creamos vector para testear */
    int* VECTOR = malloc(VOLUMEN * sizeof(int));
    for(int i = 0; i < VOLUMEN; i++) {
        VECTOR[i] = i;
    }
    
    /* Apilando elementos */
    bool ok_apilar = true;
    for(int i = 0; i < VOLUMEN; i++) {
        ok_apilar &= pila_apilar(pila, VECTOR + i);
        ok_apilar &= *((int *)pila_ver_tope(pila)) == VECTOR[i]; // Comparando por valor
        ok_apilar &= pila_ver_tope(pila) == VECTOR + i; // Comparando por direccion
        ok_apilar &= !pila_esta_vacia(pila);
    }
    print_test("Apilando muchos elementos", ok_apilar);

    /* Desapilando elementos */
    bool ok_desapilar = true;
    for(int i = VOLUMEN - 1; i >= 0; i--) {
        int* tope = pila_ver_tope(pila);
        int* elemento_desapilado = pila_desapilar(pila);

        ok_desapilar &= *tope == VECTOR[i]; // Comparando por valor
        ok_desapilar &= tope == VECTOR + i; // Comparando por direccion
        ok_desapilar &= *elemento_desapilado == VECTOR[i]; // Comparando por valor
        ok_desapilar &= elemento_desapilado == VECTOR + i; // Comparando por direccion

        if (i == 0) {
            ok_desapilar &= pila_esta_vacia(pila);
            ok_desapilar &= pila_ver_tope(pila) == NULL;
        } else {
            ok_desapilar &= !pila_esta_vacia(pila);
            ok_desapilar &= pila_ver_tope(pila) != NULL;
        }
    }
    print_test("Desapilando muchos elementos", ok_desapilar);
    
    free(VECTOR);
    pila_destruir(pila);
}

static void prueba_cond_borde(void) {
    printf("\nINICIO DE PRUEBAS DE CONDICIONES BORDE\n");

    pila_t *pila = pila_crear();

    print_test("La acción de esta_vacía en una pila recién creada es verdadero", 
        pila_esta_vacia(pila)
    );
    print_test("Las acciones de desapilar y ver_tope", 
        pila_desapilar(pila) == NULL && pila_ver_tope(pila) == NULL
    );

    int VOLUMEN = 25;
    bool ok_apilar = true;
    int valor = 0;
    for(int i = 0; i < VOLUMEN; i++) {
        ok_apilar &= pila_apilar(pila, &valor);
    }
    print_test("Apilar elementos", ok_apilar);

    bool ok_desapilar = true;
    for(int i = 0; i < VOLUMEN; i++) {
        ok_desapilar &= pila_desapilar(pila) == &valor;
    }
    print_test("Desapilar elementos", ok_desapilar);
    
    /*
        Comprobar que al desapilar hasta que está vacía hace que la pila se 
        comporte como recién creada.
    */
    print_test("Pila es vacia", pila_esta_vacia(pila));
    /*
        Las acciones de desapilar y ver_tope en 
        una pila a la que se le apiló y desapiló hasta estar vacía son inválidas.
    */
    print_test("Ver tope es nulo", pila_ver_tope(pila) == NULL);
    print_test("Desapilar es nulo", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
}

void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_pila_elementos();
    prueba_de_volumen();
    prueba_cond_borde();
    printf("\n");
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
