#include <stdio.h>
#include <string.h>

#define MAX_N 50

/*
 * Función main con los 4 algoritmos de fibonacci implementados
 * con un contador global que cada función incrementa en su
 * operación principal para ilustrar la cantidad de
 * llamadas (para las versiones recursivas) y las
 * operaciones (para las versiones iterativas).
 */

/* ── Contador global ── */
long long contador = 0;

/* ── fib_iter ── cuenta cada iteración del loop */
long long fib_iter(int n) {
    contador = 0;
    if (n <= 1) return n;
    long long a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        contador++;
        c = a + b; a = b; b = c;
    }
    return b;
}

/* ── fib_rec ── cuenta CADA llamada recursiva (incluye repetidas) */
long long fib_rec(int n) {
    contador++;
    if (n <= 1) return n;
    return fib_rec(n - 1) + fib_rec(n - 2);
}

/* ── fib_memo ── cuenta solo llamadas que NO fueron cacheadas */
long long memo[MAX_N];

long long fib_memo(int n) {
    contador++;                        // se cuenta al entrar
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n]; // retorna sin recursión adicional
    memo[n] = fib_memo(n - 1) + fib_memo(n - 2);
    return memo[n];
}

/* ── fib_tab ── cuenta cada celda llenada en la tabla */
long long fib_tab(int n) {
    contador = 0;
    if (n <= 1) return n;
    long long tab[MAX_N];
    tab[0] = 0; tab[1] = 1;
    for (int i = 2; i <= n; i++) {
        contador++;
        tab[i] = tab[i - 1] + tab[i - 2];
    }
    return tab[n];
}

int main(void) {
    int n = 10;
    long long r;

    /* fib_iter */
    r = fib_iter(n);
    printf("fib_iter(%d)  = %lld | operaciones: %lld\n", n, r, contador);

    /* fib_rec */
    contador = 0;
    r = fib_rec(n);
    printf("fib_rec(%d)   = %lld | llamadas:    %lld\n", n, r, contador);

    /* fib_memo */
    memset(memo, -1, sizeof(memo));
    contador = 0;
    r = fib_memo(n);
    printf("fib_memo(%d)  = %lld | llamadas:    %lld\n", n, r, contador);

    /* fib_tab */
    r = fib_tab(n);
    printf("fib_tab(%d)   = %lld | operaciones: %lld\n", n, r, contador);

    return 0;
}