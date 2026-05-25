/*
 * schedulling_naive.c  —  Fuerza Bruta Sin Peso
 *
 * Compilar : gcc -O2 -o schedulling_naive schedulling_naive.c
 * Ejecutar : ./schedulling_naive
 *
 * Estrategia  : probar todos los 2^n subconjuntos posibles
 * Complejidad : O(2^n * n^2)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N  50
#define N_MIN   5
#define N_MAX  20    /* 2^20 ≈ 1 M subconjuntos: límite práctico */
#define K_REPS  500  /* reducido: cada llamada ya evalúa 2^n subconjuntos */

/* ─────────────────────────────────────────────────────────────────
   ALGORITMO PRINCIPAL
   Itera sobre todas las máscaras de bits posibles (2^n).
   Cada bit activo en 'mask' indica que ese intervalo está en el subconjunto.
   Verifica compatibilidad de cada par (i, j) ambos activos.
   Dos intervalos solapan si f[j] > s[i] && f[i] > s[j] (condición estricta).
   ───────────────────────────────────────────────────────────────── */
int bf_unweighted(int s[], int f[], int n) {
    int best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int cnt = 0, ok = 1;
        for (int i = 0; i < n && ok; i++) {
            if (!(mask & (1 << i))) continue;
            cnt++;
            for (int j = 0; j < i && ok; j++) {
                if (!(mask & (1 << j))) continue;
                if (f[j] > s[i] && f[i] > s[j]) ok = 0;  /* solapan */
            }
        }
        if (ok && cnt > best) best = cnt;
    }
    return best;
}

/* ─────────────────────────────────────────────────────────────────
   BACKTRACKING
   Igual que bf_unweighted, pero además guarda la máscara ganadora
   para reconstruir cuáles intervalos fueron seleccionados.
   selected[i] = 1  si el intervalo i fue elegido.
   ───────────────────────────────────────────────────────────────── */
int bf_backtrack(int s[], int f[], int n, int selected[]) {
    int best = 0, best_mask = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int cnt = 0, ok = 1;
        for (int i = 0; i < n && ok; i++) {
            if (!(mask & (1 << i))) continue;
            cnt++;
            for (int j = 0; j < i && ok; j++) {
                if (!(mask & (1 << j))) continue;
                if (f[j] > s[i] && f[i] > s[j]) ok = 0;
            }
        }
        if (ok && cnt > best) { best = cnt; best_mask = mask; }
    }
    for (int i = 0; i < n; i++)
        selected[i] = (best_mask & (1 << i)) ? 1 : 0;
    return best;
}

/* ─────────────────────────────────────────────────────────────────
   MEDICIÓN DE TIEMPO  (basado en tool4.c)
   clock_gettime(CLOCK_MONOTONIC): reloj monótono, no retrocede.
   dummy: evita que el compilador elimine las llamadas por optimización.
   bf_unweighted no modifica s[] ni f[]: no se necesita copia previa.
   ───────────────────────────────────────────────────────────────── */
double medir_promedio_ns(int s[], int f[], int n, int reps) {
    struct timespec ini, fin;
    long long dummy = 0;
    clock_gettime(CLOCK_MONOTONIC, &ini);
    for (int k = 0; k < reps; k++)
        dummy += bf_unweighted(s, f, n);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    (void)dummy;
    return ((fin.tv_sec - ini.tv_sec) * 1e9
          + (fin.tv_nsec - ini.tv_nsec)) / reps;
}

/* ─── Genera intervalos aleatorios reproducibles (semilla fija) ─── */
void generar_intervalos(int s[], int f[], int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        s[i] = rand() % (2 * n);
        f[i] = s[i] + rand() % n + 1;   /* garantiza f > s */
    }
}

/* ─────────────────────────────────────────────────────────────────
   EXPORTAR CSV
   reps adaptativas: 2^n crece exponencialmente.
     n >= 18 → 2 reps    (2^18 = 262 144 subconjuntos por llamada)
     n >= 15 → 10 reps
     n >= 12 → 100 reps
     resto   → K_REPS
   ───────────────────────────────────────────────────────────────── */
void exportar_tiempos_csv(const char *archivo) {
    FILE *fp = fopen(archivo, "w");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "n,tiempo_ns\n");
    int s[MAX_N], f[MAX_N];
    for (int n = N_MIN; n <= N_MAX; n++) {
        generar_intervalos(s, f, n, 42);
        int reps = (n >= 18) ? 2 : (n >= 15) ? 10 : (n >= 12) ? 100 : K_REPS;
        fprintf(fp, "%d,%.2f\n", n, medir_promedio_ns(s, f, n, reps));
    }
    fclose(fp);
    printf("  CSV exportado: %s\n", archivo);
}

/* ════════════════════════════════════════════════════════════════
   MAIN
   ════════════════════════════════════════════════════════════════ */
static void sep(void) {
    printf("─────────────────────────────────────────────────────────────\n");
}

int main(void) {
    /*
     * Problema demo — 5 intervalos fijos, mismo conjunto en los 4 archivos.
     *
     *   Óptimo unweighted  : {A, D, E} = 3 intervalos   (este archivo)
     *   Óptimo weighted DP : {A, C, E} = peso 11         (schedulling_weighted_dp)
     *   Greedy weighted    : {B, E}    = peso  9          (schedulling_weighted_greedy)
     *
     * Los pesos se muestran solo como referencia visual; bf_unweighted no los usa.
     */
    int  s[]   = {0, 0, 3, 2, 6};
    int  f[]   = {2, 5, 6, 4, 8};
    int  w[]   = {4, 6, 4, 1, 3};   /* solo visual */
    char lbl[] = {'A', 'B', 'C', 'D', 'E'};
    int  n     = 5;
    int  sel[5];

    sep();
    printf("  SCHEDULING — FUERZA BRUTA (Unweighted)\n");
    sep();
    printf("  %-4s  %6s  %6s  %6s\n", "Int", "Inicio", "Fin", "Peso");
    sep();
    for (int i = 0; i < n; i++)
        printf("   %c    %6d  %6d  %6d\n", lbl[i], s[i], f[i], w[i]);
    sep();

    int res = bf_backtrack(s, f, n, sel);

    printf("  Max. intervalos compatibles: %d\n", res);
    printf("  Seleccionados: ");
    for (int i = 0; i < n; i++) if (sel[i]) printf("%c ", lbl[i]);
    printf("\n\n");
    printf("  Subconjuntos evaluados: 2^%d = %d\n", n, 1 << n);
    printf("  Complejidad: O(2^n * n^2)\n");
    sep();

    sep();
    printf("  EXPORTANDO TIEMPOS -> db/schedulling_naive.csv\n");
    sep();
    exportar_tiempos_csv("db/schedulling_naive.csv");
    printf("\n  Listo.\n");
    sep();

    return 0;
}
