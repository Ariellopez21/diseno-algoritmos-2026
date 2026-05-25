/*
 * schedulling_weighted_greedy.c  —  Greedy Heurístico Con Peso
 *
 * Compilar : gcc -O2 -o schedulling_weighted_greedy schedulling_weighted_greedy.c
 * Ejecutar : ./schedulling_weighted_greedy
 *
 * Estrategia  : ordenar por peso DESC, seleccionar si no solapa con los ya elegidos
 * Complejidad : O(n^2)  —  la verificación de solapamiento es lineal por intervalo
 *
 * IMPORTANTE: esta heurística NO garantiza optimalidad.
 * El main muestra un contraejemplo donde el greedy obtiene 9 y el óptimo (DP) es 11.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N   300
#define N_MIN    10
#define N_MAX   300
#define K_REPS 5000

typedef struct { int s, f, w, orig; } Inv;

/* ─────────────────────────────────────────────────────────────────
   COMPARADOR — ordena por peso descendente
   ───────────────────────────────────────────────────────────────── */
int cmp_weight_desc(const void *a, const void *b) {
    return ((Inv *)b)->w - ((Inv *)a)->w;
}

/* ─── ¿Se solapan dos intervalos? ─── */
static int solapan(const Inv *a, const Inv *b) {
    return a->f > b->s && b->f > a->s;
}

/* ─────────────────────────────────────────────────────────────────
   ALGORITMO GREEDY HEURÍSTICO
   Ordena por peso descendente. Toma codiciamente el de mayor peso
   que no solape con ninguno de los ya elegidos.
   FALLA cuando un intervalo pesado bloquea varios intervalos
   cuya suma de pesos supera al intervalo elegido.
   Contraejemplo: B(w=6) bloquea A+C (w=4+4=8 > 6).
   selected[orig] = 1 si fue elegido (puede ser NULL).
   ───────────────────────────────────────────────────────────────── */
int greedy_weighted(Inv iv[], int n, int selected[]) {
    qsort(iv, n, sizeof(Inv), cmp_weight_desc);      /* O(n log n) */
    if (selected) for (int i = 0; i < n; i++) selected[i] = 0;

    int picked[MAX_N], npicked = 0, total = 0;

    for (int i = 0; i < n; i++) {
        int ok = 1;
        for (int k = 0; k < npicked && ok; k++)      /* O(n) por intervalo */
            if (solapan(&iv[i], &iv[picked[k]])) ok = 0;
        if (ok) {
            picked[npicked++] = i;
            total += iv[i].w;
            if (selected) selected[iv[i].orig] = 1;
        }
    }
    return total;
}

/* ─────────────────────────────────────────────────────────────────
   MEDICIÓN DE TIEMPO  (basado en tool4.c)
   greedy_weighted modifica iv via qsort: se copia antes de cada llamada.
   reps adaptativas: O(n^2) crece más rápido, se reducen para n grande.
   ───────────────────────────────────────────────────────────────── */
double medir_promedio_ns(Inv iv_orig[], int n, int reps) {
    struct timespec ini, fin;
    long long dummy = 0;
    Inv iv[MAX_N];
    clock_gettime(CLOCK_MONOTONIC, &ini);
    for (int k = 0; k < reps; k++) {
        memcpy(iv, iv_orig, n * sizeof(Inv));
        dummy += greedy_weighted(iv, n, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &fin);
    (void)dummy;
    return ((fin.tv_sec - ini.tv_sec) * 1e9
          + (fin.tv_nsec - ini.tv_nsec)) / reps;
}

/* ─── Genera intervalos aleatorios reproducibles ─── */
void generar_intervalos(Inv iv[], int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        iv[i].s    = rand() % (2 * n);
        iv[i].f    = iv[i].s + rand() % n + 1;
        iv[i].w    = rand() % 10 + 1;
        iv[i].orig = i;
    }
}

/* ─────────────────────────────────────────────────────────────────
   EXPORTAR CSV
   reps adaptativas para mitigar el costo O(n^2) en n grande.
   ───────────────────────────────────────────────────────────────── */
void exportar_tiempos_csv(const char *archivo) {
    FILE *fp = fopen(archivo, "w");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "n,tiempo_ns\n");
    Inv iv[MAX_N];
    for (int n = N_MIN; n <= N_MAX; n += (n < 100 ? 10 : 50)) {
        generar_intervalos(iv, n, 42);
        int reps = (n >= 200) ? 500 : (n >= 100) ? 2000 : K_REPS;
        fprintf(fp, "%d,%.2f\n", n, medir_promedio_ns(iv, n, reps));
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
     * Mismo problema demo que los otros 3 archivos.
     *
     * Greedy por peso: elige B(w=6), luego E(w=3) → total 9
     * Optimo DP      : elige A(w=4) + C(w=4) + E(w=3) → total 11
     *
     * ¿Por qué falla? B bloquea A y C, cuya suma (8) supera el peso de B (6).
     */
    Inv  iv[]  = {
        {0, 2, 4, 0},   /* A */
        {0, 5, 6, 1},   /* B  <- mayor peso, pero bloquea A y C */
        {3, 6, 4, 2},   /* C */
        {2, 4, 1, 3},   /* D */
        {6, 8, 3, 4},   /* E */
    };
    Inv  iv_s[5];
    char lbl[] = {'A', 'B', 'C', 'D', 'E'};
    int  n     = 5;
    int  sel[5];

    sep();
    printf("  SCHEDULING — GREEDY HEURISTICO (Weighted)\n");
    sep();
    printf("  %-4s  %6s  %6s  %6s\n", "Int", "Inicio", "Fin", "Peso");
    sep();
    for (int i = 0; i < n; i++)
        printf("   %c    %6d  %6d  %6d\n", lbl[i], iv[i].s, iv[i].f, iv[i].w);
    sep();

    for (int i = 0; i < n; i++) sel[i] = 0;
    memcpy(iv_s, iv, sizeof(iv));
    int res = greedy_weighted(iv_s, n, sel);

    printf("  Peso total (greedy heuristico): %d\n", res);
    printf("  Seleccionados: ");
    for (int i = 0; i < n; i++) if (sel[i]) printf("%c ", lbl[i]);
    printf("\n\n");
    printf("  Optimo real (DP)  : 11  ->  {A, C, E}  (4+4+3)\n");
    printf("  Greedy obtiene    : %2d  ->  SUBOPTIMO en este caso\n\n", res);
    printf("  [!] B(w=6) bloquea A+C (w=4+4=8). El greedy no lo detecta.\n");
    printf("  [!] Para pesos arbitrarios usar DP: O(n log n).\n");
    sep();

    sep();
    printf("  EXPORTANDO TIEMPOS -> db/schedulling_weighted_greedy.csv\n");
    sep();
    exportar_tiempos_csv("db/schedulling_weighted_greedy.csv");
    printf("\n  Listo.\n");
    sep();

    return 0;
}
