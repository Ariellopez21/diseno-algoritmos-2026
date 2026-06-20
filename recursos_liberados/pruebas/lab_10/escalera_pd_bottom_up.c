#include <stdio.h>

int main() {
    int cost[] = {1, 1, 100, 100, 100, 1};
    int n = 6;
    int dp[n + 1];

    dp[n] = 0;

    for (int i = n - 1; i >= 0; i--) {
        int salto1 = dp[i + 1];
        int salto2 = (i + 2 <= n) ? dp[i + 2] : dp[n];
        dp[i] = cost[i] + (salto1 < salto2 ? salto1 : salto2);
    }

    int resultado = dp[0] < dp[1] ? dp[0] : dp[1];
    printf("Costo minimo (PD): %d\n", resultado);
    return 0;
}
