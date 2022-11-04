#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL   10
#define FACTOR_AUMENTO       2
#define FACTOR_REDUCCION     2
#define MAX_FACTOR_CARGA   0.7f
#define MIN_FACTOR_CARGA   0.3f

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/
 
typedef enum {
  VACIO,
  BORRADO,
  OCUPADO
} estado_t;

typedef struct campo {
  estado_t estado;
  char* clave;
  void* dato;
} campo_t;

struct hash {
  campo_t* tabla;
  size_t cantidad;
  size_t cantidad_borrado;
  size_t capacidad;
  hash_destruir_dato_t destruir_dato;
};

struct hash_iter {
  const hash_t* hash;
  size_t actual;
};

/* *****************************************************************
 *                       FUNCIONES AUXILIARES
 * *****************************************************************/
 
// Jenkins Hashing - https://youtu.be/BAYgiWVFZ4A?t=1555
size_t funcion_hashing(const char *clave, size_t largo) {
  size_t hash, i;
  for(hash = 0, i = 0; i < strlen(clave); i++) {
    hash += (size_t)clave[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash % largo;
} 

// Asigna el estado de VACIO a todos los campos de una tabla
void tabla_asignar_vacio(campo_t* tabla, size_t capacidad) {
  for(size_t i = 0; i < capacidad; i++) {
	  tabla[i].estado = VACIO;
  }
}

/* Busca en el hash dado la clave pasada por parametros.
 * En caso de no estar devuelve la posición vacía más cercana al 
 * indice dado por la funcion de hashing*/
size_t hash_buscar(const hash_t* hash, const char* clave) {
  size_t pos_clave = funcion_hashing(clave, hash->capacidad);
	
  while(hash->tabla[pos_clave].estado != VACIO) {
    if (hash->tabla[pos_clave].estado == OCUPADO) {
      if (strcmp(hash->tabla[pos_clave].clave, clave) == 0) break;
    }
    pos_clave++;
    if (pos_clave == hash->capacidad) pos_clave = 0;
  }

  return pos_clave;
}

// Redimensiona la tabla de un hash
bool hash_redimensionar(hash_t* hash, size_t nueva_capacidad) {
  campo_t* nueva_tabla = malloc(nueva_capacidad * sizeof(campo_t));
  if(nueva_tabla == NULL) return false;
	
  tabla_asignar_vacio(nueva_tabla, nueva_capacidad);
	
  campo_t* tabla_auxiliar = hash->tabla;
  hash->tabla = nueva_tabla;
  hash->cantidad = 0;
  hash->cantidad_borrado = 0;
	
  size_t capacidad_anterior = hash->capacidad;
  hash->capacidad = nueva_capacidad;

  for(size_t i = 0; i < capacidad_anterior; i++) {
    if (tabla_auxiliar[i].estado == OCUPADO) {
      hash_guardar(hash, tabla_auxiliar[i].clave, tabla_auxiliar[i].dato);
      free(tabla_auxiliar[i].clave);
    }
  }
	
  free(tabla_auxiliar);
	
  return true;
}

// Devuelve el valor del factor de carga de un hash cerrado
float hash_factor_de_carga(hash_t* hash) {
  float n = (float) hash->cantidad + (float) hash->cantidad_borrado;
  return n / (float) hash->capacidad;
}

/* *****************************************************************
 *                       FUNCIONES HASH
 * *****************************************************************/
 
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
  hash_t* nuevo_hash = malloc(sizeof(hash_t));
  if(!nuevo_hash) return NULL;

  nuevo_hash->capacidad = CAPACIDAD_INICIAL;
  nuevo_hash->cantidad = 0;
  nuevo_hash->destruir_dato = destruir_dato;
  nuevo_hash->cantidad_borrado = 0;

  campo_t* nueva_tabla = malloc(CAPACIDAD_INICIAL * sizeof(campo_t));
  if(!nueva_tabla) {
    free(nuevo_hash);
    return NULL;
  }

  nuevo_hash->tabla = nueva_tabla;

  // Asignamos a toda la tabla el estado VACIO
  tabla_asignar_vacio(nueva_tabla, CAPACIDAD_INICIAL);

  return nuevo_hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
  // Factor de redimension
  if (hash_factor_de_carga(hash) > MAX_FACTOR_CARGA) {
	  if(!hash_redimensionar(hash, hash->capacidad * FACTOR_AUMENTO)) 
      return false;
  }
  
  size_t pos_clave = hash_buscar(hash, clave);
	
  // No está en el hash
  if (hash->tabla[pos_clave].estado == VACIO) {
    hash->tabla[pos_clave].clave = strdup(clave);
    hash->tabla[pos_clave].dato = dato;
    hash->tabla[pos_clave].estado = OCUPADO;
    hash->cantidad++;
    return true;
  } else if (hash->destruir_dato) {
	  hash->destruir_dato(hash->tabla[pos_clave].dato);
  }

  hash->tabla[pos_clave].dato = dato;
  return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
  if(!hash_pertenece(hash, clave)) return NULL;
  size_t pos_clave = hash_buscar(hash, clave);
	
  void* dato = hash->tabla[pos_clave].dato;
  free(hash->tabla[pos_clave].clave);
  hash->tabla[pos_clave].estado = BORRADO;
  hash->cantidad--;
  hash->cantidad_borrado++;
	
  return dato;
	
  // Redimension
  if(
    hash->capacidad > CAPACIDAD_INICIAL &&
    hash_factor_de_carga(hash) < MIN_FACTOR_CARGA
  ) {
	  hash_redimensionar(hash, hash->capacidad / FACTOR_REDUCCION);
  }
}

void *hash_obtener(const hash_t *hash, const char *clave) {
  size_t pos_clave = hash_buscar(hash, clave);
  if(!hash_pertenece(hash, clave)) return NULL;
  return hash->tabla[pos_clave].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
  size_t pos_clave = hash_buscar(hash, clave);
  return hash->tabla[pos_clave].estado == OCUPADO;
}

size_t hash_cantidad(const hash_t *hash) {
  return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
  for(size_t i = 0; i < hash->capacidad; i++) {
    campo_t campo_actual = hash->tabla[i];
    if(campo_actual.estado == OCUPADO) {
      free(campo_actual.clave);
      if(hash->destruir_dato) hash->destruir_dato(campo_actual.dato); 
    }
  }
  free(hash->tabla);
  free(hash);
}

/* *****************************************************************
 *                       FUNCIONES ITER HASH
 * *****************************************************************/
 
hash_iter_t *hash_iter_crear(const hash_t *hash) {
  hash_iter_t* nuevo_iter = malloc(sizeof(hash_iter_t));
  if(!nuevo_iter) return NULL;

  nuevo_iter->hash = hash;

  if(hash->cantidad == 0) {
    nuevo_iter->actual = hash->capacidad;
  } else {
    size_t pos_actual = 0;
    while(hash->tabla[pos_actual].estado != OCUPADO) pos_actual++;
    nuevo_iter->actual = pos_actual;
  }

  return nuevo_iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
  iter->actual++;
  if(iter->actual >= iter->hash->capacidad) return false;

  while(iter->hash->tabla[iter->actual].estado != OCUPADO) {
    iter->actual++;
    if(hash_iter_al_final(iter)) return false;
  }

  return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
  if(hash_iter_al_final(iter)) return NULL;
  return iter->hash->tabla[iter->actual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
  return iter->actual >= iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t *iter) {
  free(iter);
}
