#include <stdio.h>
#include <string.h>

#define MAX 1000

int memo[MAX];
int cost[MAX];
int n;

int minCosto(int i) {
    // Caso base: llegamos al tope
    if (i >= n) return 0;

    // Si ya fue calculado, retorna directo
    if (memo[i] != -1) return memo[i];

    // Recurrencia
    int salto1 = minCosto(i + 1);
    int salto2 = minCosto(i + 2);

    memo[i] = cost[i] + (salto1 < salto2 ? salto1 : salto2);
    return memo[i];
}

int main() {
    int arr[] = {1, 1, 100, 100, 100, 1};
    n = 6;

    for (int i = 0; i < n; i++)
        cost[i] = arr[i];

    memset(memo, -1, sizeof(memo));

    int resultado = minCosto(0) < minCosto(1)
                  ? minCosto(0)
                  : minCosto(1);

    printf("Costo minimo (memo): %d\n", resultado);
    return 0;
}
