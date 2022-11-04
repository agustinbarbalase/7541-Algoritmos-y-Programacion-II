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
    int* segundo_elemento = malloc(sizeof(int));
    *primer_elemento = 5;
    *segundo_elemento = 11;
    print_test("Apilar primer elemento", pila_apilar(pila, &primer_elemento));
    print_test("La pila no debe ser vacia", !pila_esta_vacia(pila));
    print_test("El tope debe ser el primer elemento", 
        pila_ver_tope(pila) == &primer_elemento
    );

    print_test("Apilar segundo elemento", pila_apilar(pila, &segundo_elemento));
    print_test("La pila no debe ser vacia", !pila_esta_vacia(pila));
    print_test("El tope debe ser el segundo elemento", 
        pila_ver_tope(pila) == &segundo_elemento
    );

    /* Desapilando elementos */
    print_test("Desapilando elemento y comprobando invariaza", 
        pila_desapilar(pila) == &segundo_elemento
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
    free(segundo_elemento);
    pila_destruir(pila);
}

static void prueba_de_volumen(void) {
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");
    pila_t *pila = pila_crear();

    print_test("Crear pila", pila != NULL);

    int VOLUMEN = 10000;

    /* Creamos vector para testear */
    int* vector = malloc(VOLUMEN * sizeof(int));
    for(int i = 0; i < VOLUMEN; i++) {
        vector[i] = i;
    }
    
    /* Apilando elementos */
    bool ok_apilar = true;
    for(int i = 0; i < VOLUMEN; i++) {
        ok_apilar &= pila_apilar(pila, vector + i);
        ok_apilar &= *((int *)pila_ver_tope(pila)) == vector[i]; // Comparando por valor
        ok_apilar &= pila_ver_tope(pila) == vector + i; // Comparando por direccion
        ok_apilar &= !pila_esta_vacia(pila);
    }
    print_test("Apilando muchos elementos", ok_apilar);

    /* Desapilando elementos */
    bool ok_desapilar = true;
    for(int i = VOLUMEN - 1; i >= 0; i--) {
        int* tope = pila_ver_tope(pila);
        int* elemento_desapilado = pila_desapilar(pila);

        ok_desapilar &= *tope == vector[i]; // Comparando por valor
        ok_desapilar &= tope == vector + i; // Comparando por direccion
        ok_desapilar &= *elemento_desapilado == vector[i]; // Comparando por valor
        ok_desapilar &= elemento_desapilado == vector + i; // Comparando por direccion

        if (i == 0) {
            ok_desapilar &= pila_esta_vacia(pila);
            ok_desapilar &= pila_ver_tope(pila) == NULL;
        } else {
            ok_desapilar &= !pila_esta_vacia(pila);
            ok_desapilar &= pila_ver_tope(pila) != NULL;
        }
    }
    print_test("Desapilando muchos elementos", ok_desapilar);
    
    free(vector);
    pila_destruir(pila);
}

static void prueba_cond_borde(void) {
    printf("\nINICIO DE PRUEBAS DE CONDICIONES BORDE\n");

    pila_t *primera_pila = pila_crear();

    print_test("La acción de esta_vacía en una pila recién creada es verdadero", 
        pila_esta_vacia(primera_pila)
    );
    print_test("Las acciones de desapilar y ver_tope", 
        pila_desapilar(primera_pila) == NULL && pila_ver_tope(primera_pila) == NULL
    );
    
    /*
        Comprobar que al desapilar hasta que está vacía hace que la pila se 
        comporte como recién creada.
    */
    print_test("Pila es vacia", pila_esta_vacia(primera_pila));
    /*
        Las acciones de desapilar y ver_tope en 
        una pila a la que se le apiló y desapiló hasta estar vacía son inválidas.
    */
    print_test("Ver tope es nulo", pila_ver_tope(primera_pila) == NULL);
    print_test("Desapilar es nulo", pila_desapilar(primera_pila) == NULL);

    pila_t *segunda_pila = pila_crear();

    /* Apilar el elemento NULL es valido */
    print_test("Apilar NULL", pila_apilar(segunda_pila, NULL));
    print_test("La pila no debe ser vacia", !pila_esta_vacia(segunda_pila));
    print_test("El tope debe ser NULL", 
        pila_ver_tope(segunda_pila) == NULL
    );
    print_test("Desapilando elemento", 
        pila_desapilar(segunda_pila) == NULL
    );

    pila_destruir(primera_pila);
    pila_destruir(segunda_pila);
}

void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_pila_elementos();
    prueba_de_volumen();
    prueba_cond_borde();
    printf("\n");
}
