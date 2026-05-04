#include <stdio.h>

#define MAX_N 50

/*
 * Función main con la estimación de uso en memoria
 * que posee cada algoritmo de fibonacci implementado.
 */

void mostrar_memoria(int n) {
       printf("\n=== Estimación de memoria para n=%d ===\n", n);

       /* fib_iter: solo 3 variables long long */
       printf("fib_iter : ~%zu bytes (constante)\n",
              3 * sizeof(long long));

       /* fib_rec: n marcos de pila, cada uno con ~2 long long + overhead */
       printf("fib_rec  : ~%d bytes (pila, profundidad n)\n",
              n * (int)(2 * sizeof(long long) + 16)); // 16 = ret addr + frame ptr aprox

       /* fib_memo: arreglo memo + pila de profundidad n */
       printf("fib_memo : ~%zu bytes (arreglo) + ~%d bytes (pila)\n",
              (n + 1) * sizeof(long long),
              n * (int)(2 * sizeof(long long) + 16));

       /* fib_tab: solo el arreglo, sin pila recursiva */
       printf("fib_tab  : ~%zu bytes (arreglo, sin pila)\n",
              (n + 1) * sizeof(long long));
}

int main(void) {
       int n = 10;

       mostrar_memoria(n);

       return 0;
}