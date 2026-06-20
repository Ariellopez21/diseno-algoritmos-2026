#include <stdio.h>
#include <string.h>

int buscar_bf(char *T, char *P) {
    int n = strlen(T);
    int m = strlen(P);

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && T[i + j] == P[j]) {
            j++;
        }
        if (j == m) {
            return i;  // primera ocurrencia
        }
    }
    return -1;  // no encontrado
}

int main() {
    char T[] = "AABABC";
    char P[] = "AB";

    int pos = buscar_bf(T, P);

    if (pos != -1)
        printf("Patron encontrado en posicion %d\n", pos);
    else
        printf("Patron no encontrado\n");

    return 0;
}
