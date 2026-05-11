
---
### Código C — fuerza bruta

```c
/* O(2^n * n²): sin peso → pasar w[] con todos en 1 */
int bf_weighted(int s[], int f[], int w[], int n) {
    int best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int total = 0, ok = 1;
        for (int i = 0; i < n && ok; i++) {
            if (!(mask & (1 << i))) continue;  /* i no está en el subconjunto */
            total += w[i];
            for (int j = 0; j < i && ok; j++) {
                if (!(mask & (1 << j))) continue;
                /* ¿i y j se solapan? */
                if (f[j] > s[i] && f[i] > s[j]) ok = 0;
            }
        }
        if (ok && total > best) best = total;
    }
    return best;
}
```

> [!info] Para entender la máscara de bits 
> Si `n=4` y `mask=0101` en binario (`mask=5`), significa que los intervalos **1** y **3** (**A** y **C**) están en el subconjunto. Dentro del 3er for: `mask & (1<<i)` pregunta si el bit `i` está activo.

**Complejidades de fuerza bruta**

| Operación                                  | Complejidad    | Razón                    |
| ------------------------------------------ | -------------- | ------------------------ |
| Generar subconjuntos                       | O(2ⁿ)          | 2ⁿ máscaras posibles     |
| Verificar compatibilidad de un subconjunto | O(n²)          | Comparar todos los pares |
| **TOTAL**                                  | **O(2ⁿ × n²)** |                          |

> [!warning] Para n=20: ~400 millones de ops. Para n=30: ~1 trillón. Impracticable.

---

### Código C — DP

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct { int s, f, w; } Inv;

/* Comparador para ordenar por finish time */
int cmp_f(const void *a, const void *b) {
    return ((Inv*)a)->f - ((Inv*)b)->f;
}
```

```c
/* Búsqueda binaria: mayor p < j con iv[p-1].f <= iv[j-1].s
   Retorna índice 1-based (0 = ninguno) */
int find_p(Inv iv[], int j) {
    int lo = 0, hi = j - 2, best = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (iv[mid].f <= iv[j-1].s) { best = mid; lo = mid + 1; }
        else                          hi = mid - 1;
    }
    return best + 1;  /* convierte a 1-based */
}
```

```c
int dp_weighted(Inv iv[], int n) {
    qsort(iv, n, sizeof(Inv), cmp_f);   /* O(n log n) */

    int OPT[n + 1];
    OPT[0] = 0;

    for (int j = 1; j <= n; j++) {
        int p   = find_p(iv, j);         /* O(log n) */
        int inc = iv[j-1].w + OPT[p];   /* incluir j */
        int exc = OPT[j-1];              /* excluir j */
        OPT[j]  = inc > exc ? inc : exc;
    }
    return OPT[n];
}
```

> [!tip] Para DP sin peso
> Inicializa todos los `iv[i].w = 1` antes de llamar a la función.

**Complejidades de DP**

| Operación                | Complejidad    | Razón                       |
| ------------------------ | -------------- | --------------------------- |
| Ordenar por finish       | **O(n log n)** | -                           |
| Calcular p(j) para todos | **O(n log n)** | búsqueda binaria por cada j |
| Llenar tabla OPT         | **O(n)**       | 1 operación por celda       |
| Backtracking             | **O(n)**       | -                           |
| **TOTAL**                | **O(n log n)** | -                           |

---

### Comparación algorítmica

| Operación                                | Fuerza bruta    | DP                                           |
| ---------------------------------------- | --------------- | -------------------------------------------- |
| Ordenar por finish                       | — (no necesita) | **O(n log n)**                               |
| Generar/evaluar subconjuntos             | **O(2ⁿ)**       | —                                            |
| Verificar compatibilidad por subconjunto | **O(n²)**       | —                                            |
| Calcular p(j) para todos                 | —               | **O(n log n)** — búsqueda binaria por cada j |
| Llenar tabla OPT                         | —               | **O(n)** — 1 operación por celda             |
| Backtracking                             | —               | **O(n)**                                     |
| **TOTAL**                                | **O(2ⁿ · n²)**  | **O(n log n)**                               |
> [!info] Para n = 30
> fuerza bruta ~1 trillón de ops
> DP ~150 ops.

