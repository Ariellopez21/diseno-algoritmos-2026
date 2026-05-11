#include <stdio.h>
#include <string.h>

#define MAX_N 50

/*
 * Función main con los 2 algoritmos de fibonacci recursivos
 * con un contador global y variables de profundidad.
 * Con el objetivo de ilustrar la profundidad de llamadas
 * recursivas que realiza cada algoritmo.
 */

/* ── Contador global ── */
long long contador = 0;

/* Variables globales de profundidad */
int prof_actual = 0;
int prof_max    = 0;

long long fib_rec(int n) {
    prof_actual++;
    if (prof_actual > prof_max) prof_max = prof_actual;
    contador++;
    //printf("[%lld]\n",contador);
    if (n <= 1) { prof_actual--; return n; }
    long long r = fib_rec(n - 1) + fib_rec(n - 2);
    prof_actual--;
    return r;
}

/* ── fib_memo ── cuenta solo llamadas que NO fueron cacheadas */
long long memo[MAX_N];

long long fib_memo(int n) {
    prof_actual++;
    if (prof_actual > prof_max) prof_max = prof_actual;
    contador++;
    if (n <= 1) { prof_actual--; return n; }
    if (memo[n] != -1) { prof_actual--; return memo[n]; }
    memo[n] = fib_memo(n - 1) + fib_memo(n - 2);
    prof_actual--;
    return memo[n];
}

int main(void) {
    int n = 35;
    long long r;

    /* fib_rec */
    contador = 0;
    r = fib_rec(n);
    printf("fib_rec(%d)   = %lld | llamadas:    %lld | prof. max de pila: %d\n", n, r, contador, prof_max);

    /* fib_memo */
    memset(memo, -1, sizeof(memo));
    contador = 0;
    prof_actual = 0, prof_max = 0;
    r = fib_memo(n);
    printf("fib_memo(%d)  = %lld | llamadas:    %lld | prof. max de pila: %d\n", n, r, contador, prof_max);

    return 0;
}
