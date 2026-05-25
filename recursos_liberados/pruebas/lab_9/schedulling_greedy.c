/*
 * schedulling_greedy.c  —  Greedy Sin Peso (Activity Selection)
 *
 * Compilar : gcc -O2 -o schedulling_greedy schedulling_greedy.c
 * Ejecutar : ./schedulling_greedy
 *
 * Estrategia  : ordenar por finish time, elegir el primero compatible
 * Complejidad : O(n log n)  —  OPTIMO para scheduling sin peso
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_N   500
#define N_MIN    10
#define N_MAX   500
#define K_REPS 5000

typedef struct { int s, f, orig; } Act;

/* ─────────────────────────────────────────────────────────────────
   COMPARADOR — ordena por finish time ascendente
   ───────────────────────────────────────────────────────────────── */
int cmp_finish(const void *a, const void *b) {
    return ((Act *)a)->f - ((Act *)b)->f;
}

/* ─────────────────────────────────────────────────────────────────
   ALGORITMO GREEDY  —  Activity Selection
   Idea central: siempre elegir el intervalo compatible que termina
   más temprano. Esto maximiza el espacio disponible para los siguientes.
   El argumento de intercambio (exchange argument) garantiza optimalidad:
   si una solución óptima no contiene al primer intervalo elegido por
   greedy, se puede intercambiar sin perder ningún intervalo del resto.
   selected[] (opcional): selected[orig] = 1 si fue elegido.
   ───────────────────────────────────────────────────────────────── */
int greedy_unweighted(Act ac[], int n, int selected[]) {
    qsort(ac, n, sizeof(Act), cmp_finish);         /* O(n log n) */
    if (selected) for (int i = 0; i < n; i++) selected[i] = 0;

    int count = 0, last_f = -1;
    for (int i = 0; i < n; i++) {                  /* O(n) */
        if (ac[i].s >= last_f) {                   /* compatible: empieza cuando termina el último */
            count++;
            last_f = ac[i].f;
            if (selected) selected[ac[i].orig] = 1;
        }
    }
    return count;
}

/* ─────────────────────────────────────────────────────────────────
   MEDICIÓN DE TIEMPO  (basado en tool4.c)
   greedy_unweighted modifica ac via qsort: se copia antes de cada llamada.
   ───────────────────────────────────────────────────────────────── */
double medir_promedio_ns(Act ac_orig[], int n, int reps) {
    struct timespec ini, fin;
    long long dummy = 0;
    Act ac[MAX_N];
    clock_gettime(CLOCK_MONOTONIC, &ini);
    for (int k = 0; k < reps; k++) {
        memcpy(ac, ac_orig, n * sizeof(Act));
        dummy += greedy_unweighted(ac, n, NULL);   /* NULL: no rastrear seleccion */
    }
    clock_gettime(CLOCK_MONOTONIC, &fin);
    (void)dummy;
    return ((fin.tv_sec - ini.tv_sec) * 1e9
          + (fin.tv_nsec - ini.tv_nsec)) / reps;
}

/* ─── Genera intervalos aleatorios reproducibles ─── */
void generar_intervalos(Act ac[], int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        ac[i].s    = rand() % (2 * n);
        ac[i].f    = ac[i].s + rand() % n + 1;
        ac[i].orig = i;
    }
}

/* ─────────────────────────────────────────────────────────────────
   EXPORTAR CSV
   ───────────────────────────────────────────────────────────────── */
void exportar_tiempos_csv(const char *archivo) {
    FILE *fp = fopen(archivo, "w");
    if (!fp) { perror("fopen"); return; }
    fprintf(fp, "n,tiempo_ns\n");
    Act ac[MAX_N];
    for (int n = N_MIN; n <= N_MAX; n += (n < 100 ? 10 : 50)) {
        generar_intervalos(ac, n, 42);
        fprintf(fp, "%d,%.2f\n", n, medir_promedio_ns(ac, n, K_REPS));
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
     * Los pesos se muestran solo como referencia; greedy_unweighted no los usa.
     *
     * Orden por finish: A(f=2) D(f=4) B(f=5) C(f=6) E(f=8)
     * Greedy selecciona: A (last_f=2), D (s=2>=2 OK, last_f=4),
     *                    B (s=0<4 skip), C (s=3<4 skip), E (s=6>=4 OK)
     * Resultado: {A, D, E} = 3 intervalos  <- OPTIMO
     */
    Act  ac[]  = {{0,2,0},{0,5,1},{3,6,2},{2,4,3},{6,8,4}};
    int  w[]   = {4, 6, 4, 1, 3};   /* solo visual */
    char lbl[] = {'A', 'B', 'C', 'D', 'E'};
    int  n     = 5;
    int  sel[5];
    Act  ac_s[5];

    sep();
    printf("  SCHEDULING — GREEDY (Unweighted)\n");
    sep();
    printf("  %-4s  %6s  %6s  %6s\n", "Int", "Inicio", "Fin", "Peso");
    sep();
    for (int i = 0; i < n; i++)
        printf("   %c    %6d  %6d  %6d\n", lbl[i], ac[i].s, ac[i].f, w[i]);
    sep();

    memcpy(ac_s, ac, sizeof(ac));
    int res = greedy_unweighted(ac_s, n, sel);

    printf("  Max. intervalos (greedy): %d\n", res);
    printf("  Seleccionados: ");
    for (int i = 0; i < n; i++) if (sel[i]) printf("%c ", lbl[i]);
    printf("\n\n");
    printf("  [OK] Greedy es OPTIMO para scheduling sin peso.\n");
    printf("  [OK] O(n log n) vs O(2^n * n^2) de fuerza bruta.\n");
    printf("  [OK] Mismo resultado que bf_unweighted, fraccion del costo.\n");
    sep();

    sep();
    printf("  EXPORTANDO TIEMPOS -> db/schedulling_greedy.csv\n");
    sep();
    exportar_tiempos_csv("db/schedulling_greedy.csv");
    printf("\n  Listo.\n");
    sep();

    return 0;
}
