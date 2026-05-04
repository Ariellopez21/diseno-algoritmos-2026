/*
 * binsearch.c
 * Análisis de Algoritmos — Búsqueda Binaria
 * Se verá la versión: iterativa y recursiva.
 */

#include <stdio.h>
#include <stdbool.h>
/* ─── Versión ITERATIVA ─────────────────────────────────────────────────── */
/*
 * Precondición: arr[] debe estar ordenado de menor a mayor.
 * Retorna el índice del elemento si lo encuentra, -1 en caso contrario.
 */
int binsearch_iterativo(int arr[], int n, int objetivo) {
	int izq = 0, der = n, mid;
	bool encontrado = false;
	while( izq <= der ){
		mid = (izq + der) / 2;
		printf("izq = %d\tder = %d\n",izq, der);
		if(arr[mid] == objetivo){	
			encontrado = true;
			return mid;
		}
		else if( arr[mid] < objetivo )
			izq = mid + 1;
		else if( arr[mid] > objetivo )
			der = mid - 1;
	}
	if(!encontrado)
		return -1;
}

/* ─── Versión RECURSIVA ─────────────────────────────────────────────────── */
/*
 * Precondición: arr[] debe estar ordenado de menor a mayor.
 * Llamada inicial: binsearch_recursivo(arr, 0, n-1, objetivo)
 * Retorna el índice del elemento si lo encuentra, -1 en caso contrario.
 */
int binsearch_recursivo(int arr[], int izq, int der, int objetivo) {
    printf("Aun no implementado...\n");
}

/* ─── Main ──────────────────────────────────────────────────────────────── */

int main(void) {
    int arr[]  = {1, 42, 51};
    int n      = sizeof(arr) / sizeof(arr[0]);
    int objetivo =1;

    /* Llamada a la versión iterativa */
    int resultado = binsearch_iterativo(arr, n - 1, objetivo);

    /* resultado == índice encontrado  (ó -1 si no existe) */
    printf("Objetivo: arr[%d] = %d\n", resultado, arr[resultado]);
    return 0;
}

