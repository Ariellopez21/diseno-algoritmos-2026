#include <stdio.h>

int main() {
    int cost[] = {1, 1, 100, 100, 100, 1};
    int n = 6;
    int total = 0;
    int i = 0;  // puedes empezar en 0 o 1

    while (i < n) {
        total += cost[i];

        // ¿Hay dos opciones disponibles?
        if (i + 2 < n) {
            if (cost[i+1] <= cost[i+2])
                i += 1;
            else
                i += 2;
        } else {
            i += 1;  // solo queda un paso posible
        }
    }

    printf("Costo total (greedy): %d\n", total);
    return 0;
}
