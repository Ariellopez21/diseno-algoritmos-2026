/*
 * schedulling_weighted_dp.c  —  Programación Dinámica Con Peso
 *
 * Compilar : gcc -O2 -o schedulling_weighted_dp schedulling_weighted_dp.c
 * Ejecutar : ./schedulling_weighted_dp
 *
 * Estrategia  : OPT[j] = max( w_j + OPT[p(j)],  OPT[j-1] )
 * Complejidad : O(n log n)  —  OPTIMO para scheduling con peso
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N   500
#define N_MIN    10
#define N_MAX   500
#define K_REPS 5000

typedef struct { int s, f, w, orig; } Inv;

/* ─────────────────────────────────────────────────────────────────
   COMPARADOR — ordena por finish time ascendente (para qsort)
   ───────────────────────────────────────────────────────────────── */
int cmp_finish(const void *a, const void *b) {
    return ((Inv *)a)->f - ((Inv *)b)->f;
}

/* ─────────────────────────────────────────────────────────────────
   find_p(iv, j)  —  búsqueda binaria sobre arreglo 1-based
   Retorna el mayor p < j tal que iv[p-1].f <= iv[j-1].s
   (el último intervalo que termina antes o exactamente cuando j empieza).
   Retorna 0 si ninguno califica.
   ───────────────────────────────────────────────────────────────── */
int find_p(Inv iv[], int j) {
    int lo = 0, hi = j - 2, best = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (iv[mid].f <= iv[j-1].s) { best = mid; lo = mid + 1; }
        else                          hi = mid - 1;
    }
    return best + 1;   /* convierte a índice 1-based */
}

/* ─────────────────────────────────────────────────────────────────
   ALGORITMO DP — devuelve el peso máximo (modifica iv via qsort)
   Usado en la medición de tiempo: no necesita reconstruir la solución.
   ───────────────────────────────────────────────────────────────── */
int dp_weighted(Inv iv[], int n) {
    qsort(iv, n, sizeof(Inv), cmp_finish);

    int OPT[MAX_N + 1];
    OPT[0] = 0;
    for (int j = 1; j <= n; j++) {
        int p   = find_p(iv, j);
        int inc = iv[j-1].w + OPT[p];   /* incluir intervalo j */
        int exc = OPT[j-1];              /* excluir intervalo j */
        OPT[j]  = inc > exc ? inc : exc;
    }
    return OPT[n];
}

/* ─────────────────────────────────────────────────────────────────
   dp_solve — igual que dp_weighted + reconstruye la selección.
   Supone que iv ya está ordenado por finish time.
   selected[orig] = 1 si el intervalo fue elegido.
   ───────────────────────────────────────────────────────────────── */
int dp_solve(Inv iv[], int n, int selected[]) {
    int OPT[MAX_N + 1], P[MAX_N + 1];
    OPT[0] = 0;
    for (int j = 1; j <= n; j++) {
        P[j]   = find_p(iv, j);
        int inc = iv[j-1].w + OPT[P[j]];
        int exc = OPT[j-1];
        OPT[j]  = inc > exc ? inc : exc;
    }

    /* Backtracking: recorrer la tabla de atrás hacia adelante */
    for (int i = 0; i < n; i++) selected[i] = 0;
    int j = n;
    while (j > 0) {
        if (iv[j-1].w + OPT[P[j]] >= OPT[j-1]) {
            selected[iv[j-1].orig] = 1;   /* incluir: guardar índice original */
            j = P[j];
        } else {
            j--;                           /* excluir */
        }
    }
    return OPT[n];
}

/* ─────────────────────────────────────────────────────────────────
   MEDICIÓN DE TIEMPO  (basado en tool4.c)
   dp_weighted usa qsort y modifica iv: se copia el arreglo antes
   de cada llamada para que cada repetición parta del mismo estado.
   El memcpy incluye el costo de la copia en la medición, pero es
   despreciable frente al algoritmo y garantiza correctitud.
   ───────────────────────────────────────────────────────────────── */
double medir_promedio_ns(Inv iv_orig[], int n, int reps) {
    struct timespec ini, fin;
    long long dummy = 0;
    Inv iv[MAX_N];
    clock_gettime(CLOCK_MONOTONIC, &ini);
    for (int k = 0; k < reps; k++) {
        memcpy(iv, iv_orig, n * sizeof(Inv));
        dummy += dp_weighted(iv, n);
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
   Rango n=10..500, paso 10 hasta n<100, luego paso 50.
   ───────────────────────────────────────────────────────────────── */
void exportar_tiempos_csv(const char *archivo) {
    FILE *fp = fopen(archivo, "w");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "n,tiempo_ns\n");
    Inv iv[MAX_N];
    for (int n = N_MIN; n <= N_MAX; n += (n < 100 ? 10 : 50)) {
        generar_intervalos(iv, n, 42);
        fprintf(fp, "%d,%.2f\n", n, medir_promedio_ns(iv, n, K_REPS));
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
     * Después de sort por finish: A(f=2) D(f=4) B(f=5) C(f=6) E(f=8)
     * OPT[5] = 11  →  backtracking selecciona {A, C, E}
     *
     * Comparar con schedulling_weighted_greedy: solo obtiene 9 ({B, E}).
     */
    Inv  iv[]  = {
        {0, 2, 4, 0},   /* A */
        {0, 5, 6, 1},   /* B */
        {3, 6, 4, 2},   /* C */
        {2, 4, 1, 3},   /* D */
        {6, 8, 3, 4},   /* E */
    };
    Inv  iv_s[5];
    char lbl[] = {'A', 'B', 'C', 'D', 'E'};
    int  n     = 5;
    int  sel[5];

    sep();
    printf("  SCHEDULING — PROGRAMACION DINAMICA (Weighted)\n");
    sep();
    printf("  %-4s  %6s  %6s  %6s\n", "Int", "Inicio", "Fin", "Peso");
    sep();
    for (int i = 0; i < n; i++)
        printf("   %c    %6d  %6d  %6d\n", lbl[i], iv[i].s, iv[i].f, iv[i].w);
    sep();

    memcpy(iv_s, iv, sizeof(iv));
    qsort(iv_s, n, sizeof(Inv), cmp_finish);
    int res = dp_solve(iv_s, n, sel);

    printf("  Peso optimo (DP): %d\n", res);
    printf("  Seleccionados: ");
    for (int i = 0; i < n; i++) if (sel[i]) printf("%c ", lbl[i]);
    printf("\n\n");
    printf("  Comparar con greedy weighted: obtiene solo 9 ({B, E})\n");
    printf("  Complejidad: O(n log n)\n");
    sep();

    sep();
    printf("  EXPORTANDO TIEMPOS -> db/schedulling_weighted_dp.csv\n");
    sep();
    exportar_tiempos_csv("db/schedulling_weighted_dp.csv");
    printf("\n  Listo.\n");
    sep();

    return 0;
}
