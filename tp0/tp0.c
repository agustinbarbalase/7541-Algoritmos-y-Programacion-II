#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int aux;
    aux = *x;
    *x = *y;
    *y = aux;
}

int maximo(int vector[], int n) {
    if (n < 1) return -1;
    int max = n - 1;
    for(int i = 0; i < n - 1; i++) {
        if(vector[i] > vector[max]) {
            max = i;
        }
    }
    return max;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    int i = 0;
    int j = 0;

    while((i < n1) && (j < n2)) {
        if(vector1[i] < vector2[i]) {
            return -1;
        } else if(vector1[i] > vector2[i]) {
            return 1;
        }
        i++;
        j++;
    }

    if(n1 < n2) {
        return -1;
    } else if(n1 > n2) {
        return 1;
    }

    return 0;
}

void seleccion(int vector[], int n) {
    int pos_max;

    for(int i = n; i > 0; i--) {
        pos_max = maximo(vector, i);
        swap(&vector[pos_max], &vector[i - 1]);
    }
}
