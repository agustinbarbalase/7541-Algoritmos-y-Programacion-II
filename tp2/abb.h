#include <stdlib.h>
#include <stdbool.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);

typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                DEFINICION DE LAS FUNCIONES DEL ABB
 * *****************************************************************/

/* Crea un arbol binario de busqueda, con funciones de comparacion de claves
 * y una función de destrucción.
 * Pre: Las funciones de comparación y destrucción son funciones válidas.
 * Post: Se devuelve un puntero a un nuevo abb en caso de funcionar. De lo
 * contrario devuelve NULL.*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda o reemplaza el dato-clave en el abb.
 * Pre: El abb fue creado.
 * Post: devuelve True si se pudo guardar el nuevo elemento, false en
 * caso contrario.*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra el dato y la clave del árbol, devuelve al usuario dicho dato.
 * Pre: El árbol fue creado.
 * Post: Se borró el elemento del arbol, se devolvió el dato de existir,
 * NULL en caso contrario.*/
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor pedido encontrado en el árbol.
 * Pre: El árbol fue creado.
 * Post: Se devuelve el dato pedido, NULL en caso de que la clave no
 * exista en el árbol.*/
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Indica si el elemento pertenece al árbol o no.
 * Pre: El árbol fue creado.
 * Post: Se devuelve true en caso de que el elemento exista, false en caso
 * contario.*/
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos en el árbol.ç
 * Pre: El árbol existe.*/
size_t abb_cantidad(const abb_t *arbol);

/* Destruye el árbol y todos sus elementos.*/
void abb_destruir(abb_t *arbol);

/* Itera el árbol con el método in order. Aplica una función visitante
 * a sus elementos y se itera hasta que la misma de false o se iteren todos
 * los elementos
 * Pre: El árbol fue creado, la función visitar es válida.
 * Post: Se iteraron los elementos necesarios y se aplicó la función visitar
 * a cada uno de ellos.*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* *****************************************************************
 *            DEFINICION DE LAS FUNCIONES DEL ITERADOR
 * *****************************************************************/

/* Crea un iterador de un abb.
 * Pre: El árbol fue creado.
 * Post: Se devuelve un puntero a iterador de un abb especificado.
 * En caso de no poderse crear devuelve NULL.*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza al siguiente elemento del abb.
 * Pre: El iterador fue creado.
 * Post: Se avanzó a la siguiente posición del árbol. Devuelve true si 
 * se pudo avanzar, false en caso contrario.*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Devuelve la clave a la que apunta el iterador.
 * Pre: el iterador fue creado.
 * Post: Devuelve NULL si el iterador se encuentra al final, en caso 
 * contrario devuelve la clave*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Indica si se llegó al final del abb.
 * Pre: el iterador fue creado.
 * Post: devuelve true si se está al final, false dado lo contrario.*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador.
 * Pre: el iterador fue creado.
 * Post: el iterador es eliminado y se liberó toda su memoria.*/
void abb_iter_in_destruir(abb_iter_t* iter);