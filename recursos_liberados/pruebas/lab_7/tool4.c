#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_N    50       // Para tamaño de arreglos que almacenan valores de fib.
#define N_MIN    5        // Para función exportar_tiempos_csv
#define N_MAX    35       // Para función exportar_tiempos_csv; fib_rec se vuelve inviable ~40+
#define K_REPS   100000   // Para función exportar_tiempos_csv
#define N_CONTADOR 30     // Para función exportar_contadores_csv.

/*
 * Herramienta completa.
 * Conecta:
 * - Los 4 algoritmos de fibonacci
 * - Mediciones temporales en nanosegs
 * - Exports en csv para contadores y tiempo.
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

/*
 * -------- medir_promedio_ns -----------
 * clock_gettime: Función para tomar el tiempo actual con precisión de nanosegundos
   y lo guarda en un struct timespec, que tiene dos campos: tv_sec (segundos) y tv_nsec (nanosegundos).
   Se llama dos veces, antes y después del loop, para medir el tiempo transcurrido entre ambos puntos.

 * CLOCK_MONOTONIC: Es el reloj que se le pasa como fuente.
   "Monotónico" significa que nunca retrocede: no se ve afectado si el sistema
   cambia la hora del sistema operativo (actualizaciones NTP, cambio manual, etc.).
   Para medir intervalos es siempre preferible sobre CLOCK_REALTIME.

 * dummy: El compilador con -O2 o superior es capaz de detectar que los resultados de fib_iter(n)
   nunca se usan y eliminar las llamadas completamente, por lo tanto, el loop desaparecería y
   mediría tiempo cero. Acumular el resultado en dummy fuerza al compilador a ejecutar cada llamada.
   La línea (void)dummy al final le dice al compilador "sé que no la uso más, no es un error",
   evitando un warning.
*/
double medir_promedio_ns(int algoritmo, int n, int reps) {
    struct timespec ini, fin;
    long long dummy = 0;

    clock_gettime(CLOCK_MONOTONIC, &ini);
    for (int k = 0; k < reps; k++) {
        switch (algoritmo) {
            case 0: dummy += fib_iter(n); break;
            case 1: dummy += fib_rec(n);  break;
            case 2:
                memset(memo, -1, sizeof(memo));
                dummy += fib_memo(n);
                break;
            case 3: dummy += fib_tab(n);  break;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &fin);

    /* dummy evita que el compilador elimine las llamadas por optimización */
    (void)dummy;

    double total_ns = (fin.tv_sec  - ini.tv_sec)  * 1e9
                    + (fin.tv_nsec - ini.tv_nsec);
    return total_ns / reps;
}

/*
 * -------- exportar_contadores_csv -----------
 * Ejecuta los 4 algoritmos.
   Su objetivo es capturar el valor de contador.
   Esto es una forma de calcular la complejidad operacional.
   NO se calcula el uso en memoria.
 */
void exportar_contadores_csv(const char *archivo) {
    FILE *f = fopen(archivo, "w");
    fprintf(f, "n,fib_iter,fib_rec,fib_memo,fib_tab\n");

    for (int n = 1; n <= N_CONTADOR; n++) {
        long long c_iter, c_rec, c_memo, c_tab;

        /* iter */
        fib_iter(n); c_iter = contador;

        /* rec */
        contador = 0; fib_rec(n); c_rec = contador;

        /* memo */
        memset(memo, -1, sizeof(memo));
        contador = 0; fib_memo(n); c_memo = contador;

        /* tab */
        fib_tab(n); c_tab = contador;

        fprintf(f, "%d,%lld,%lld,%lld,%lld\n",
                n, c_iter, c_rec, c_memo, c_tab);
    }
    fclose(f);
    printf("CSV exportado: %s\n", archivo);
}

/*
 * -------- exportar_tiempos_csv -----------
 * Ejecuta los 4 algoritmos.
 * reps_rec: Es un operador que reduce las repeticiones de fib_rec a medida que n crece,
   porque su costo es O(2ⁿ) y con n=35 y 100.000 repeticiones el programa tardaría minutos.
   Los otros tres algoritmos siempre usan K_REPS completo porque son O(n)
   y no tienen ese problema.
   En otras palabras,
   fib_rec necesita menos repeticiones para medir bien precisamente porque ya es lento
   por sí solo. Los otros necesitan muchas repeticiones para que el promedio sea
   estadísticamente significativo, ya que cada llamada dura nanosegundos.
 */
void exportar_tiempos_csv(const char *archivo) {
    FILE *f = fopen(archivo, "w");
    fprintf(f, "n,iter_ns,rec_ns,memo_ns,tab_ns\n");

    for (int n = N_MIN; n <= N_MAX; n++) {
        int reps_rec = (n >= 28) ? 10 : (n >= 20) ? 1000 : K_REPS;
        fprintf(f, "%d,%.2f,%.2f,%.2f,%.2f\n",
                n,
                medir_promedio_ns(0, n, K_REPS),
                medir_promedio_ns(1, n, reps_rec),
                medir_promedio_ns(2, n, K_REPS),
                medir_promedio_ns(3, n, K_REPS));
    }
    fclose(f);
    printf("CSV exportado: %s\n", archivo);
}
/* ════════════════════════════════════════════════════════════════
   MAIN
   ════════════════════════════════════════════════════════════════ */

/* Imprime una línea separadora */
static void separador(void) {
    printf("─────────────────────────────────────────────────────────────\n");
}

/* Tabla de contadores para un n dado — resetea contador manualmente */
static void fila_contadores(int n) {
    long long c_iter, c_rec, c_memo, c_tab;

    fib_iter(n);          c_iter = contador;
    contador = 0;
    fib_rec(n);           c_rec  = contador;
    memset(memo, -1, sizeof(memo));
    contador = 0;
    fib_memo(n);          c_memo = contador;
    fib_tab(n);           c_tab  = contador;

    printf("  n = %2d │ iter: %6lld │ rec: %10lld │ memo: %5lld │ tab: %5lld\n",
           n, c_iter, c_rec, c_memo, c_tab);
}

int main(void) {

    /* ── Bloque 1: tabla de contadores ── */
    separador();
    printf("  COMPARACIÓN DE OPERACIONES POR ALGORITMO\n");
    separador();
    printf("  n      │ fib_iter │    fib_rec    │ fib_memo │ fib_tab\n");
    separador();

    int ns_demo[] = {5, 10, 15, 20, 25, 30};
    int total     = sizeof(ns_demo) / sizeof(ns_demo[0]);
    for (int i = 0; i < total; i++)
        fila_contadores(ns_demo[i]);

    separador();
    printf("  * fib_rec cuenta llamadas totales (incluye repetidas)\n");
    printf("  * fib_memo cuenta TODAS las entradas a la función (con y sin caché)\n");
    printf("  * fib_iter / fib_tab cuentan iteraciones del loop\n\n");

    /* ── Bloque 2: exportar CSVs ── */
    separador();
    printf("  EXPORTANDO DATOS A CSV\n");
    separador();
    exportar_contadores_csv("contadores.csv");
    exportar_tiempos_csv("tiempos.csv");
    printf("\n  ¡Listo! Archivos generados en el directorio actual.\n\n");

    /* ── Bloque 3: instrucciones para gnuplot (ejecuta tu mismo) ── */
    separador();
    printf("  PARA GENERAR LOS GRÁFICOS\n");
    separador();
    printf("  gnuplot contadores.gp\n");
    printf("  gnuplot tiempos.gp\n\n");
    printf("  Imágenes de salida:\n");
    printf("    → contadores_fibonacci.png\n");
    printf("    → tiempos_fibonacci.png\n");
    separador();

    return 0;
}