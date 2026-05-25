# Explicación de Algoritmos de Scheduling

## Introducción: ¿Qué es el problema de Scheduling de Intervalos?

El problema de **scheduling de intervalos** consiste en lo siguiente: dado un conjunto de actividades o intervalos, donde cada uno tiene un tiempo de inicio (`s`) y un tiempo de fin (`f`), encontrar el subconjunto más grande (o el de mayor peso/valor) de intervalos **mutuamente compatibles**, es decir, que ningún par se solape en el tiempo.

**Dos intervalos se solapan** si uno empieza antes de que el otro termine *y* viceversa, de forma estricta: `f[j] > s[i] && f[i] > s[j]`. Si uno termina exactamente cuando otro empieza (ej. A termina en 2 y D empieza en 2), **no** se consideran solapados.

**Ejemplo compartido por todos los archivos:**

```
Intervalo  Inicio  Fin  Peso
    A          0     2     4
    B          0     5     6
    C          3     6     4
    D          2     4     1
    E          6     8     3
```

Visualizado en una línea de tiempo:
```
Tiempo: 0    1    2    3    4    5    6    7    8
    A:  [=========]
    B:  [========================]
    C:                 [==============]
    D:            [=========]
    E:                                [=========]
```

- A y D: A termina en 2 y D empieza en 2 → **compatibles** (no hay solapamiento estricto).
- A y B: ambos empiezan en 0, se solapan → **incompatibles**.
- El óptimo **sin peso** (mayor cantidad de intervalos): `{A, D, E}` = 3 intervalos.
- El óptimo **con peso** (mayor suma de valores): `{A, C, E}` = peso 4+4+3 = **11**.

---
---

## Archivo 1: `schedulling_naive.c` — Fuerza Bruta Sin Peso

### 1. Explicación general del algoritmo

Este archivo implementa la solución más directa al problema: **probar absolutamente todos los subconjuntos posibles** de intervalos y quedarse con el subconjunto válido más grande. Se llama "fuerza bruta" (brute force, de ahí el prefijo `bf_`) porque no usa ningún atajo inteligente; simplemente lo prueba todo de forma exhaustiva.

Si hay `n` intervalos, existen exactamente **2ⁿ subconjuntos** posibles (incluyendo el vacío), porque cada intervalo puede estar o no estar en la solución de forma independiente. Por ejemplo, con 5 intervalos hay 2⁵ = 32 subconjuntos. Para cada uno se verifica si todos sus intervalos son compatibles entre sí. El subconjunto válido con más intervalos es la respuesta.

El archivo contiene dos funciones algorítmicas:
- **`bf_unweighted`**: solo devuelve el número máximo de intervalos compatibles.
- **`bf_backtrack`**: hace lo mismo pero además recuerda exactamente cuáles intervalos seleccionó (para poder mostrarlos).

### 2. Orden algorítmico: O(2ⁿ · n²)

La complejidad viene de dos niveles de trabajo multiplicados:

| Nivel | Qué hace | Costo |
|-------|----------|-------|
| Externo | Recorre todos los subconjuntos posibles | 2ⁿ iteraciones |
| Interno | Para cada subconjunto, revisa todos los pares de intervalos activos | hasta n² comparaciones |

**¿Dónde están los ciclos que determinan el orden?** En `bf_unweighted` (líneas 30–43):

```c
for (int mask = 0; mask < (1 << n); mask++) {     // ciclo externo: 2^n iteraciones
    for (int i = 0; i < n && ok; i++) {            // ciclo medio: hasta n veces
        for (int j = 0; j < i && ok; j++) {        // ciclo interno: hasta n veces
            // verificar solapamiento entre i y j
        }
    }
}
```

Son **tres ciclos anidados**. El externo es el que manda: recorre 2ⁿ subconjuntos. Por eso el algoritmo es **impracticable para n grande**: con n=20 ya son más de un millón de subconjuntos; con n=30 serían más de mil millones. El código tiene `#define N_MAX 20` precisamente por esto.

### 3. Declaración de estructuras y variables en `main`

No se define ninguna estructura especial en este archivo: trabaja directamente con arreglos separados.

```c
int  s[]   = {0, 0, 3, 2, 6};
int  f[]   = {2, 5, 6, 4, 8};
int  w[]   = {4, 6, 4, 1, 3};
char lbl[] = {'A', 'B', 'C', 'D', 'E'};
int  n     = 5;
int  sel[5];
```

- **`s[]`** *(start)*: tiempos de inicio de cada intervalo. `s[i]` es el inicio del intervalo `i`. La letra `s` es la abreviación estándar en libros de algoritmos para "start time".
- **`f[]`** *(finish)*: tiempos de fin. `f[i]` es el fin del intervalo `i`. De igual forma, `f` es la abreviación de "finish time". Los arreglos `s` y `f` son **paralelos**: el intervalo 0 va de `s[0]=0` a `f[0]=2`, el intervalo 1 va de `s[1]=0` a `f[1]=5`, etc.
- **`w[]`** *(weight)*: pesos o valores de cada intervalo. Existe **únicamente con fines visuales** para que la tabla impresa en pantalla se vea igual que la de los demás archivos. La función `bf_unweighted` ignora completamente este arreglo; el sufijo "unweighted" (sin peso) indica que el objetivo es solo contar cuántos intervalos caben, no cuánto valen.
- **`lbl[]`** *(labels)*: etiquetas de texto ('A', 'B', 'C'...). Sirven para que la salida en consola sea legible con letras en vez de números. No tienen ningún papel en el algoritmo.
- **`n = 5`**: cantidad de intervalos en el ejemplo demo. Es fijo aquí, pero en la parte de medición de tiempos varía dinámicamente desde `N_MIN` hasta `N_MAX`.
- **`sel[5]`** *(selected)*: arreglo de flags (ceros y unos). Después de llamar a `bf_backtrack`, `sel[i] = 1` significa que el intervalo `i` fue incluido en la mejor solución; `sel[i] = 0` significa que fue descartado. Se inicializa vacío y se rellena dentro de la función.

**Constantes `#define` al inicio del archivo:**

```c
#define MAX_N  50    // tamaño máximo del arreglo para la medición de tiempos
#define N_MIN   5    // n mínimo para la medición
#define N_MAX  20    // n máximo: 2^20 ≈ 1 millón de subconjuntos, límite práctico
#define K_REPS 500   // repeticiones para promediar el tiempo
```

`N_MAX = 20` existe porque con n=20 ya se evalúan ~1 millón de subconjuntos por llamada, y subir más haría la medición extremadamente lenta. `K_REPS = 500` es menor que en los otros archivos (que usan 5000) porque aquí cada llamada ya hace un trabajo gigantesco.

### 4. Desglose del algoritmo principal

#### La variable `mask` — representación de subconjuntos con bits

La idea más importante de este archivo es la variable `mask`. El desafío es: ¿cómo representar y recorrer todos los subconjuntos posibles sin crear arreglos auxiliares para cada uno?

La solución es ingeniosa: se usa un **número entero como si fuera una lista de checks activados/desactivados**. Cada bit del número corresponde a un intervalo. Si el bit está en `1`, el intervalo está "incluido" en el subconjunto; si está en `0`, está "excluido".

Con n=5 (intervalos A, B, C, D, E):

```
Número   Bits (E D C B A)   Subconjunto representado
  0      0 0 0 0 0          {}              (vacío)
  1      0 0 0 0 1          {A}
  2      0 0 0 1 0          {B}
  3      0 0 0 1 1          {A, B}
  5      0 0 1 0 1          {A, C}
 21      1 0 1 0 1          {A, C, E}
 31      1 1 1 1 1          {A, B, C, D, E}  (todos)
```

Al iterar `for (mask = 0; mask < (1 << n); mask++)`, se recorren todos los números del 0 al 2ⁿ-1, que son exactamente todos los subconjuntos posibles. Es un truco muy compacto: en vez de generar los subconjuntos explícitamente, se usan los propios números binarios para representarlos.

**¿Cómo saber si el intervalo `i` está en el subconjunto `mask`?**

Se usa la expresión `mask & (1 << i)`. En palabras: "¿está el bit número `i` encendido en `mask`?". Si el resultado es 0, el intervalo `i` no está en este subconjunto y se salta. Si es distinto de 0, sí está incluido.

Ejemplo: `mask = 21 = 0b10101`. Para ver si el intervalo 2 (C) está incluido:
- `1 << 2 = 4 = 0b00100`
- `21 & 4 = 0b10101 & 0b00100 = 0b00100 = 4` → distinto de 0 → C **está incluido** ✓

Para ver si el intervalo 1 (B) está incluido:
- `1 << 1 = 2 = 0b00010`
- `21 & 2 = 0b10101 & 0b00010 = 0b00000 = 0` → es cero → B **no está incluido** ✓

#### La función `bf_unweighted`

```c
int bf_unweighted(int s[], int f[], int n) {
    int best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int cnt = 0, ok = 1;
        for (int i = 0; i < n && ok; i++) {
            if (!(mask & (1 << i))) continue;   // intervalo i no está en este subconjunto → saltar
            cnt++;                               // contar cuántos intervalos activos hay
            for (int j = 0; j < i && ok; j++) {
                if (!(mask & (1 << j))) continue;
                if (f[j] > s[i] && f[i] > s[j]) ok = 0;  // se solapan → subconjunto inválido
            }
        }
        if (ok && cnt > best) best = cnt;       // si es válido y mejor, actualizar
    }
    return best;
}
```

- **`best`**: el mejor resultado encontrado hasta ahora (mayor cantidad de intervalos compatibles). Empieza en 0.
- **`cnt`**: cuántos intervalos tiene el subconjunto actual.
- **`ok`**: bandera que indica si el subconjunto actual es válido (sin solapamientos). Comienza en 1 (verdadero) y se apaga si se detecta un solapamiento.
- **`&& ok`** en los ciclos medios e internos: si ya se detectó un solapamiento (`ok = 0`), los ciclos se cortan inmediatamente. Esto es una optimización: no tiene sentido seguir revisando un subconjunto que ya es inválido.
- **Condición de solapamiento**: `f[j] > s[i] && f[i] > s[j]` — el intervalo j termina después de que i empieza, Y el intervalo i termina después de que j empieza. Ambas condiciones juntas significan solapamiento. Si solo una se cumple, no se solapan.
- Al final del ciclo externo: `if (ok && cnt > best) best = cnt` — solo actualiza `best` si el subconjunto fue válido y tiene más intervalos que el mejor previo.

#### La función `bf_backtrack`

Idéntica a `bf_unweighted` en su lógica, con dos diferencias:

1. Guarda `best_mask` (la máscara del mejor subconjunto encontrado):
   ```c
   if (ok && cnt > best) { best = cnt; best_mask = mask; }
   ```

2. Al terminar, convierte la máscara ganadora al arreglo `selected[]`:
   ```c
   for (int i = 0; i < n; i++)
       selected[i] = (best_mask & (1 << i)) ? 1 : 0;
   ```
   Para cada intervalo `i`, verifica si su bit está encendido en `best_mask`. Si sí, marca `selected[i] = 1`. Esto "decodifica" el número entero de vuelta a un arreglo legible de flags.

#### La función `generar_intervalos`

```c
void generar_intervalos(int s[], int f[], int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        s[i] = rand() % (2 * n);
        f[i] = s[i] + rand() % n + 1;
    }
}
```

Genera intervalos aleatorios para la parte de medición de tiempos. El parámetro `seed` (semilla aleatoria) fija el generador de números aleatorios: con la misma semilla siempre se produce la misma secuencia de números, lo que hace que los experimentos sean **reproducibles** (siempre se miden los mismos datos). La expresión `f[i] = s[i] + rand() % n + 1` garantiza que el fin sea siempre mayor que el inicio (el `+1` evita intervalos de duración cero).

---
---

## Archivo 2: `schedulling_greedy.c` — Greedy Sin Peso (Activity Selection)

### 1. Explicación general del algoritmo

Este archivo implementa la solución **óptima** al problema sin pesos usando una estrategia **greedy** (codiciosa). En vez de probar todos los subconjuntos como la fuerza bruta, sigue una sola regla local sencilla aplicada de forma repetida:

> **"Siempre elige el siguiente intervalo compatible que termine más temprano."**

La intuición es clara: al elegir siempre el que termina antes, dejamos el mayor espacio libre posible en la línea de tiempo para los intervalos siguientes, maximizando las oportunidades de incluir más. 

Este enfoque produce el mismo resultado que la fuerza bruta (`{A, D, E}`) pero es drásticamente más eficiente: O(n log n) vs O(2ⁿ · n²). Y no es solo una heurística: existe una prueba matemática llamada **argumento de intercambio** (exchange argument) que demuestra que este greedy siempre da la solución óptima para el problema sin pesos.

### 2. Orden algorítmico: O(n log n)

| Operación | Costo |
|-----------|-------|
| `qsort`: ordenar todos los intervalos por finish time | O(n log n) |
| Recorrido lineal para seleccionar intervalos | O(n) |
| **Total** | **O(n log n)** (dominado por el ordenamiento) |

**¿Dónde están los ciclos que determinan el orden?** En `greedy_unweighted` (líneas 40–51):

```c
qsort(ac, n, sizeof(Act), cmp_finish);    // O(n log n): el paso dominante
for (int i = 0; i < n; i++) {            // O(n): un solo recorrido lineal
    if (ac[i].s >= last_f) { ... }
}
```

Solo hay **un ciclo** (más el sort). No hay ciclos anidados de complejidad cuadrática. Cada intervalo se examina exactamente una vez.

### 3. Declaración de estructuras y variables en `main`

Este archivo introduce la primera **estructura** del laboratorio para agrupar los datos de un intervalo:

**Estructura `Act`** (línea 21):
```c
typedef struct { int s, f, orig; } Act;
```
- **`s`** *(start)*: tiempo de inicio del intervalo.
- **`f`** *(finish)*: tiempo de fin del intervalo.
- **`orig`** *(original index)*: el índice del intervalo **antes de ordenar**. Este campo es fundamental: cuando `qsort` reordena el arreglo por finish time, los elementos cambian de posición. Sin `orig`, no sabríamos decirle al usuario "el intervalo A fue elegido"; solo podríamos decir "el elemento en la posición 0 del arreglo ordenado fue elegido", lo cual no tiene sentido para el usuario. `orig` preserva la identidad del intervalo a lo largo de todo el algoritmo.

**Variables en `main`:**
```c
Act  ac[]  = {{0,2,0}, {0,5,1}, {3,6,2}, {2,4,3}, {6,8,4}};
```
Cada elemento `{s, f, orig}`. El tercer valor es el índice original (0=A, 1=B, 2=C, 3=D, 4=E).

```c
int  w[]   = {4, 6, 4, 1, 3};   // pesos, solo para la tabla visual
char lbl[] = {'A', 'B', 'C', 'D', 'E'};
int  n     = 5;
int  sel[5];     // sel[i]=1 si el intervalo con índice original i fue seleccionado
Act  ac_s[5];    // copia de ac para trabajar sin modificar el original
```

- **`ac_s`** *(ac sorted / copia para sort)*: `qsort` modifica el arreglo **en su lugar** (lo reordena directamente). Si llamáramos `greedy_unweighted(ac, ...)` sin copiar, el arreglo `ac` quedaría reordenado y no podríamos imprimir la tabla en el orden original. Por eso se hace `memcpy(ac_s, ac, sizeof(ac))` para crear una copia de trabajo y se opera sobre `ac_s`, dejando `ac` intacto.

**Constantes:**
```c
#define MAX_N   500   // tamaño máximo (el greedy escala bien, puede manejar 500)
#define N_MIN    10
#define N_MAX   500
#define K_REPS 5000   // muchas más repeticiones que en naive: O(n log n) es muy rápido
```

`K_REPS = 5000` es diez veces mayor que el `K_REPS = 500` del naive, porque O(n log n) es tan rápido que con pocas repeticiones la medición sería demasiado ruidosa.

### 4. Desglose del algoritmo principal

#### El comparador `cmp_finish`

```c
int cmp_finish(const void *a, const void *b) {
    return ((Act *)a)->f - ((Act *)b)->f;
}
```

Esta función se pasa a `qsort` para indicarle cómo comparar dos elementos. El contrato de `qsort` es: si la función devuelve un número **negativo**, `a` va antes que `b`; si devuelve **positivo**, `b` va antes que `a`; si devuelve **cero**, son iguales. La resta `a->f - b->f` logra exactamente eso: si `a` termina antes que `b`, el resultado es negativo y `a` queda primero → orden ascendente por finish time.

#### La función `greedy_unweighted`

```c
int greedy_unweighted(Act ac[], int n, int selected[]) {
    qsort(ac, n, sizeof(Act), cmp_finish);                   // ordenar por fin
    if (selected) for (int i = 0; i < n; i++) selected[i] = 0;  // limpiar

    int count = 0, last_f = -1;
    for (int i = 0; i < n; i++) {
        if (ac[i].s >= last_f) {              // ¿el intervalo empieza cuando o después del último elegido?
            count++;
            last_f = ac[i].f;                // actualizar cuándo termina el último elegido
            if (selected) selected[ac[i].orig] = 1;
        }
    }
    return count;
}
```

**`last_f = -1`**: representa "todavía no hemos elegido ningún intervalo". Se inicializa en -1 (antes de cualquier tiempo posible) para que el primer intervalo siempre sea aceptado, sin importar cuándo empiece.

**`count`**: contador de intervalos seleccionados.

**La condición `ac[i].s >= last_f`**: un intervalo es compatible con el último elegido si empieza en el momento en que el anterior termina o después. La condición `>=` (mayor o igual, no estricto) refleja que si un intervalo termina en 2 y el siguiente empieza en 2, **no** se solapan.

**Paso a paso con el ejemplo** — después de ordenar por fin: A(f=2), D(f=4), B(f=5), C(f=6), E(f=8):

| i | Intervalo | s | f | s >= last_f?      | Acción        | last_f tras acción |
|---|-----------|---|---|-------------------|---------------|--------------------|
| 0 | A         | 0 | 2 | 0 >= -1  →  ✓    | Seleccionar A | 2                  |
| 1 | D         | 2 | 4 | 2 >= 2   →  ✓    | Seleccionar D | 4                  |
| 2 | B         | 0 | 5 | 0 >= 4   →  ✗    | Saltar B      | 4 (sin cambio)     |
| 3 | C         | 3 | 6 | 3 >= 4   →  ✗    | Saltar C      | 4 (sin cambio)     |
| 4 | E         | 6 | 8 | 6 >= 4   →  ✓    | Seleccionar E | 8                  |

Resultado: `{A, D, E}` = 3 intervalos. Coincide exactamente con el óptimo de la fuerza bruta.

**¿Por qué el greedy es óptimo aquí?** Porque el intervalo que termina más temprano nunca "bloquea" una mejor solución: si A termina en 2 y existe una solución óptima que no usa A sino otro intervalo X con finish time >= 2, podemos intercambiar X por A y la solución no empeora (A libera igual o más espacio que X). Este argumento se puede repetir para toda la selección greedy.

**El parámetro `selected` puede ser `NULL`**: en la medición de tiempos se llama como `greedy_unweighted(ac, n, NULL)` porque no importa cuáles se eligieron, solo el conteo total. El `if (selected)` verifica antes de escribir en el arreglo para evitar una escritura en un puntero nulo (lo que provocaría un error de segmento de memoria).

> [!tip] Para DP sin peso
> Si todos los pesos son 1, el greedy sin peso (`greedy_unweighted`) es equivalente a correr DP con `w[i] = 1`. Es más simple y tiene la misma complejidad: no hace falta DP.

---
---

## Archivo 3: `schedulling_weighted_dp.c` — Programación Dinámica Con Peso

### 1. Explicación general del algoritmo

Cuando los intervalos tienen **pesos distintos**, el greedy de "el que termina antes" ya no garantiza la solución óptima. Un intervalo que termina antes puede valer muy poco; podría ser mejor elegir uno que termina un poco más tarde pero vale mucho más, o elegir varios de menor valor que en conjunto superan al de mayor valor.

Este archivo usa **Programación Dinámica (DP)**, una técnica que descompone el problema en subproblemas más pequeños, los resuelve en orden y guarda los resultados intermedios para no recalcularlos. La clave es la siguiente idea:

> Para cada intervalo `j` (con los intervalos ordenados por finish time, numerados 1 a n), hay exactamente dos opciones:
>
> **Opción 1 — Incluirlo:** se gana su peso `w_j`, pero hay que respetar que no se solape con el último que se tomó. El mejor compañero compatible para `j` es el último intervalo que termina antes o cuando `j` empieza, llamado `p(j)`. Entonces el valor total de incluir `j` es: `w_j + OPT[p(j)]`.
>
> **Opción 2 — Excluirlo:** simplemente no se toma. El valor es el mejor resultado sin contar `j`: `OPT[j-1]`.
>
> El óptimo es el máximo de ambas opciones:
> `OPT[j] = max( w_j + OPT[p(j)],  OPT[j-1] )`

Al llenar esta tabla de izquierda a derecha (de j=1 hasta j=n), cuando llegamos a `OPT[n]` tenemos la respuesta óptima. Luego, con una segunda pasada hacia atrás (**backtracking**), se reconstruye cuáles intervalos se incluyeron.

Este algoritmo es **óptimo** para el problema con pesos y tiene la misma complejidad que el greedy sin pesos: O(n log n).

### 2. Orden algorítmico: O(n log n)

| Operación | Costo |
|-----------|-------|
| `qsort`: ordenar por finish time | O(n log n) |
| `find_p`: búsqueda binaria para cada j (n veces) | O(n log n) total |
| Llenar la tabla `OPT` con el ciclo | O(n) |
| Backtracking para reconstruir la solución | O(n) |
| **Total** | **O(n log n)** |

**¿Dónde están los ciclos que determinan el orden?** En `dp_weighted` y `dp_solve`:

```c
qsort(iv, n, sizeof(Inv), cmp_finish);     // O(n log n)
for (int j = 1; j <= n; j++) {             // O(n): un solo ciclo lineal
    int p = find_p(iv, j);                 // O(log n) cada vez → total O(n log n)
    ...
}
```

**No hay ciclos anidados cuadráticos.** La búsqueda binaria dentro del ciclo hace que sea O(log n) por iteración.

### 3. Declaración de estructuras y variables en `main`

**Estructura `Inv`** (línea 21):
```c
typedef struct { int s, f, w, orig; } Inv;
```
- **`s`** *(start)*: tiempo de inicio.
- **`f`** *(finish)*: tiempo de fin.
- **`w`** *(weight)*: peso o valor del intervalo. Aquí **sí importa**, a diferencia de los archivos anteriores donde era solo visual.
- **`orig`** *(original index)*: índice original antes de ordenar, igual que en el greedy.

El nombre `Inv` viene de **Interval** (en algunos libros se abrevia así para distinguirlo del `Act` del greedy sin peso, aunque conceptualmente son iguales con el campo `w` extra).

**Variables en `main`:**
```c
Inv  iv[]  = {
    {0, 2, 4, 0},   // A: s=0, f=2, w=4, orig=0
    {0, 5, 6, 1},   // B: s=0, f=5, w=6, orig=1
    {3, 6, 4, 2},   // C: s=3, f=6, w=4, orig=2
    {2, 4, 1, 3},   // D: s=2, f=4, w=1, orig=3
    {6, 8, 3, 4},   // E: s=6, f=8, w=3, orig=4
};
Inv  iv_s[5];   // copia de iv para trabajar (qsort modifica in-place)
char lbl[] = {'A', 'B', 'C', 'D', 'E'};
int  n     = 5;
int  sel[5];    // sel[i]=1 si el intervalo con índice original i fue seleccionado
```

- **`iv_s`**: copia de trabajo, igual que `ac_s` en el greedy. Se hace `memcpy(iv_s, iv, sizeof(iv))` para preservar `iv` intacto (necesario para la impresión de la tabla en el orden original A, B, C, D, E).

**Variables internas en `dp_solve` (no en `main`, pero importantes):**
```c
int OPT[MAX_N + 1];  // OPT[j] = peso máximo usando solo los primeros j intervalos
int P[MAX_N + 1];    // P[j] = el p(j) calculado para el intervalo j (guardado para el backtracking)
```
- **`OPT`**: la "tabla de DP". Tiene tamaño n+1 porque `OPT[0] = 0` es el caso base (con 0 intervalos el peso óptimo es 0). Los índices van de 1 a n, uno por intervalo.
- **`P`**: guarda el valor `p(j)` de cada intervalo para reutilizarlo en el backtracking. Si no lo guardáramos, habría que recalcular `find_p` para cada paso de la reconstrucción, lo cual no es incorrecto pero sería redundante.

### 4. Desglose del algoritmo principal

#### La función `find_p(iv, j)` — Búsqueda binaria del "último compatible"

```c
int find_p(Inv iv[], int j) {
    int lo = 0, hi = j - 2, best = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (iv[mid].f <= iv[j-1].s) { best = mid; lo = mid + 1; }
        else                          hi = mid - 1;
    }
    return best + 1;   // convierte de índice base-0 a base-1
}
```

**¿Qué hace `find_p`?** Dado el intervalo número `j` (en numeración 1 a n), encuentra el **último intervalo del arreglo ordenado que termina en o antes de que `j` comience**. Es decir, el mejor "socio" que podemos combinar con `j` sin que se solapten.

Como el arreglo ya está ordenado por finish time, se puede usar **búsqueda binaria** en vez de recorrerlo linealmente, lo que reduce el costo de O(n) a O(log n) por llamada.

**¿Por qué `hi = j - 2`?** Porque solo buscamos entre los intervalos anteriores a `j`. En índice base-0, el intervalo `j` es `iv[j-1]`, entonces los anteriores van de `iv[0]` a `iv[j-2]`, o sea `hi = j - 2`.

**¿Por qué `best + 1` al final?** La búsqueda trabaja en índices base-0 (0, 1, 2...), pero la tabla `OPT` usa índices base-1 (1, 2, 3...) para que `OPT[0] = 0` sea el caso base vacío. Se suma 1 para convertir entre los dos sistemas.

**Ejemplo:** después de ordenar, el arreglo es `[A(f=2), D(f=4), B(f=5), C(f=6), E(f=8)]`. Para j=5 (intervalo E, que empieza en s=6):
- ¿Qué intervalos terminan en ≤ 6? A(f=2)✓, D(f=4)✓, B(f=5)✓, C(f=6)✓.
- La búsqueda binaria encuentra que el **último** que califica es C (índice base-0 = 3).
- `find_p` devuelve 3+1 = **4**.
- Esto significa: si incluimos E, podemos combinarla con `OPT[4]`, el óptimo de los primeros 4 intervalos.

#### La función `dp_weighted` / `dp_solve` — Llenado de la tabla OPT

```c
OPT[0] = 0;   // caso base: con 0 intervalos, el peso óptimo es 0
for (int j = 1; j <= n; j++) {
    int p   = find_p(iv, j);               // el último compatible antes de j
    int inc = iv[j-1].w + OPT[p];          // opción 1: INCLUIR el intervalo j
    int exc = OPT[j-1];                    // opción 2: EXCLUIR el intervalo j
    OPT[j]  = inc > exc ? inc : exc;       // tomar el máximo
}
```

- **`inc`** *(include)*: el valor si se incluye el intervalo j. Es su propio peso `iv[j-1].w` más la mejor solución para todos los intervalos compatibles anteriores, que es `OPT[p]`.
- **`exc`** *(exclude)*: el valor si no se incluye j. Simplemente hereda el mejor resultado sin j, que es `OPT[j-1]`.
- La elección es `max(inc, exc)`.

**Traza completa con el ejemplo** (orden post-sort: A, D, B, C, E):

| j | Intervalo | w | s  | p(j) | inc = w + OPT[p] | exc = OPT[j-1] | OPT[j] |
|---|-----------|---|----|------|------------------|----------------|--------|
| 0 | —         | — | —  | —    | —                | —              | **0**  |
| 1 | A         | 4 | 0  | 0    | 4 + OPT[0] = 4   | OPT[0] = 0     | **4**  |
| 2 | D         | 1 | 2  | 1    | 1 + OPT[1] = 5   | OPT[1] = 4     | **5**  |
| 3 | B         | 6 | 0  | 0    | 6 + OPT[0] = 6   | OPT[2] = 5     | **6**  |
| 4 | C         | 4 | 3  | 1    | 4 + OPT[1] = 8   | OPT[3] = 6     | **8**  |
| 5 | E         | 3 | 6  | 4    | 3 + OPT[4] = 11  | OPT[4] = 8     | **11** |

La respuesta óptima es `OPT[5] = 11`.

Algunas notas sobre los valores de p(j):
- p(A)=0: A empieza en s=0; ningún intervalo termina en ≤0 → p=0 (caso base vacío).
- p(D)=1: D empieza en s=2; A termina en f=2 (2≤2✓), D termina en f=4 (4≤2✗) → el último compatible es A → p=1.
- p(B)=0: B empieza en s=0; ningún intervalo termina en ≤0 → p=0.
- p(C)=1: C empieza en s=3; A termina en f=2 (2≤3✓), D termina en f=4 (4≤3✗) → el último compatible es A → p=1.
- p(E)=4: E empieza en s=6; C termina en f=6 (6≤6✓) → el último compatible es C → p=4.

#### Backtracking — Reconstruir la solución desde la tabla OPT

Una vez llenada la tabla, se "lee hacia atrás" para descubrir qué intervalos se incluyeron:

```c
int j = n;
while (j > 0) {
    if (iv[j-1].w + OPT[P[j]] >= OPT[j-1]) {   // ¿fue j incluido?
        selected[iv[j-1].orig] = 1;              // sí: marcarlo
        j = P[j];                                // saltar al último compatible antes de j
    } else {
        j--;                                     // no: simplemente retroceder uno
    }
}
```

**¿Cómo saber si j fue incluido?** Al reconstruir, para cada posición j se verifica si la decisión de "incluir" fue la ganadora: `iv[j-1].w + OPT[P[j]] >= OPT[j-1]`. Si esto es cierto, significa que en el momento de llenar la tabla, el `inc` fue mayor o igual que el `exc`, así que j fue incluido.

**Traza del backtracking** con el ejemplo:

| j | ¿w + OPT[P[j]] >= OPT[j-1]?                | Decisión       | Nuevo j   |
|---|---------------------------------------------|----------------|-----------|
| 5 | 3 + OPT[4]=8 = 11 >= OPT[4]=8  →  **Sí** | Incluir E      | j = P[5]=4|
| 4 | 4 + OPT[1]=4 = 8  >= OPT[3]=6  →  **Sí** | Incluir C      | j = P[4]=1|
| 1 | 4 + OPT[0]=0 = 4  >= OPT[0]=0  →  **Sí** | Incluir A      | j = P[1]=0|
| 0 | — (loop termina)                            | —              | —         |

Seleccionados: E, C, A → `{A, C, E}` con peso 4+4+3 = **11**. ✓

---
---

## Archivo 4: `schedulling_weighted_greedy.c` — Greedy Heurístico Con Peso

### 1. Explicación general del algoritmo

Este archivo implementa una estrategia greedy **heurística** para el problema con pesos. La idea es simple e intuitiva:

> **"Ordena los intervalos de mayor a menor peso y ve tomando el más valioso que no se solape con ninguno de los ya elegidos."**

La palabra **heurística** es clave: a diferencia del greedy sin peso (que es óptimo) y el DP con peso (que también es óptimo), este greedy **no garantiza encontrar la mejor solución**. A veces la encuentra, a veces no. El archivo lo demuestra con el mismo ejemplo: aquí se obtiene peso 9 (`{B, E}`), mientras que el óptimo del DP es 11 (`{A, C, E}`).

**¿Por qué falla?** Porque elegir el intervalo de mayor peso puede bloquear varios intervalos de menor peso cuya **suma** supera al elegido. En el ejemplo: B tiene peso 6 (el mayor), pero bloquea a A (w=4) y C (w=4). La suma de A y C es 8 > 6, pero el greedy elige B primero sin ver eso.

Este algoritmo tiene valor práctico en casos donde el DP es demasiado costoso o complejo de implementar, y una solución "buena pero no perfecta" es suficiente. Es la comparación negativa del laboratorio: muestra por qué el DP es necesario.

### 2. Orden algorítmico: O(n²)

| Operación | Costo |
|-----------|-------|
| `qsort`: ordenar por peso descendente | O(n log n) |
| Ciclo externo: revisar cada intervalo | O(n) |
| Ciclo interno: comparar con todos los ya elegidos | O(n) por intervalo |
| **Total** | **O(n²)** (dominado por los ciclos anidados) |

**¿Dónde están los ciclos que determinan el orden?** En `greedy_weighted` (líneas 53–63):

```c
for (int i = 0; i < n; i++) {                         // O(n): ciclo externo
    int ok = 1;
    for (int k = 0; k < npicked && ok; k++)            // O(n): ciclo interno
        if (solapan(&iv[i], &iv[picked[k]])) ok = 0;
    ...
}
```

Son **dos ciclos anidados**: por cada intervalo candidato (ciclo externo), se compara con todos los ya elegidos (ciclo interno). En el peor caso, si todos los intervalos son compatibles y se eligen todos, el ciclo interno crece linealmente → O(n²) total.

Esto contrasta con el DP: ambos resuelven el problema con pesos, pero el DP lo hace en O(n log n) y de forma óptima, mientras que este greedy lo hace en O(n²) y de forma subóptima.

### 3. Declaración de estructuras y variables en `main`

**Estructura `Inv`** (idéntica al archivo DP, línea 24):
```c
typedef struct { int s, f, w, orig; } Inv;
```
Mismos campos que en el DP: `s`, `f`, `w`, `orig`.

**Variables en `main`:**
```c
Inv  iv[]  = {
    {0, 2, 4, 0},   // A  (s=0, f=2, w=4)
    {0, 5, 6, 1},   // B  (s=0, f=5, w=6)  ← mayor peso, pero bloquea A y C
    {3, 6, 4, 2},   // C  (s=3, f=6, w=4)
    {2, 4, 1, 3},   // D  (s=2, f=4, w=1)
    {6, 8, 3, 4},   // E  (s=6, f=8, w=3)
};
Inv  iv_s[5];   // copia de trabajo
char lbl[] = {'A', 'B', 'C', 'D', 'E'};
int  n     = 5;
int  sel[5];    // sel[i]=1 si el intervalo con índice original i fue seleccionado
```

**Variables internas en `greedy_weighted` (importantes):**
```c
int picked[MAX_N];   // índices (en el arreglo YA ORDENADO) de los intervalos elegidos
int npicked = 0;     // cuántos intervalos se han elegido hasta ahora
int total   = 0;     // suma acumulada de pesos
```

- **`picked[]`** *(picked intervals)*: arreglo que guarda los **índices** (dentro del arreglo `iv` ya reordenado por peso) de los intervalos que se han seleccionado hasta el momento. Se usa para poder comparar cada candidato nuevo contra todos los ya elegidos y verificar que no haya solapamiento.
- **`npicked`** *(number picked)*: el contador de cuántos intervalos están en `picked`. Empieza en 0 y crece con cada intervalo aceptado.
- **`total`**: la suma de pesos de todos los intervalos elegidos. Es el valor de retorno de la función.

**Constantes:**
```c
#define MAX_N   300   // menor que en los otros archivos: O(n^2) se vuelve costoso antes
#define N_MAX   300
#define K_REPS 5000
```

`MAX_N = 300` (no 500 como en greedy sin peso y DP) porque O(n²) crece más rápido y medir hasta n=300 ya es suficientemente representativo.

### 4. Desglose del algoritmo principal

#### El comparador `cmp_weight_desc`

```c
int cmp_weight_desc(const void *a, const void *b) {
    return ((Inv *)b)->w - ((Inv *)a)->w;   // nota: b - a (no a - b)
}
```

Igual que `cmp_finish` pero **invertido**: devuelve `b->w - a->w` (en vez de `a->w - b->w`). Esto ordena de **mayor a menor** peso (orden descendente). Si b pesa más que a, el resultado es positivo, lo que le indica a `qsort` que b debe ir antes.

#### La función `solapan`

```c
static int solapan(const Inv *a, const Inv *b) {
    return a->f > b->s && b->f > a->s;
}
```

Función auxiliar que devuelve 1 si dos intervalos se solapan, 0 si no. La condición es la misma que en el naive: dos intervalos [a.s, a.f] y [b.s, b.f] se solapan si `a.f > b.s` (a termina después de que b empieza) **y** `b.f > a.s` (b termina después de que a empieza). Ambas condiciones juntas garantizan cruce real. El `static` indica que esta función es privada al archivo (no visible desde otros archivos).

#### La función `greedy_weighted`

```c
int greedy_weighted(Inv iv[], int n, int selected[]) {
    qsort(iv, n, sizeof(Inv), cmp_weight_desc);      // ordenar por peso descendente
    if (selected) for (int i = 0; i < n; i++) selected[i] = 0;

    int picked[MAX_N], npicked = 0, total = 0;

    for (int i = 0; i < n; i++) {
        int ok = 1;
        for (int k = 0; k < npicked && ok; k++)      // comparar con cada elegido previo
            if (solapan(&iv[i], &iv[picked[k]])) ok = 0;
        if (ok) {
            picked[npicked++] = i;    // agregar al conjunto de elegidos
            total += iv[i].w;         // sumar su peso
            if (selected) selected[iv[i].orig] = 1;
        }
    }
    return total;
}
```

**Paso a paso con el ejemplo** — después de ordenar por peso descendente: B(w=6), A(w=4), C(w=4), E(w=3), D(w=1):

| i | Intervalo | w | ¿Solapa con algún picked? | ok? | Acción       | picked[]  | total |
|---|-----------|---|---------------------------|-----|--------------|-----------|-------|
| 0 | B         | 6 | npicked=0, no hay con quién | ✓  | Elegir B     | [0]       | 6     |
| 1 | A         | 4 | ¿A solapa B? A[0,2], B[0,5]: 2>0 && 5>0 → sí | ✗ | Saltar A | [0]  | 6     |
| 2 | C         | 4 | ¿C solapa B? C[3,6], B[0,5]: 6>0 && 5>3 → sí | ✗ | Saltar C | [0]  | 6     |
| 3 | E         | 3 | ¿E solapa B? E[6,8], B[0,5]: 8>0 && 5>6 → **NO** (5≤6) | ✓ | Elegir E | [0,3] | 9 |
| 4 | D         | 1 | ¿D solapa B? D[2,4], B[0,5]: 4>0 && 5>2 → sí | ✗ | Saltar D | [0,3] | 9 |

Resultado: `{B, E}` con peso total **9**. El óptimo real es **11** (`{A, C, E}`).

**¿Por qué saltar A cuando solapa con B?** Porque en la verificación, A[0,2] y B[0,5]: `a.f=2 > b.s=0` (✓) y `b.f=5 > a.s=0` (✓) → se solapan. El greedy elimina A del conjunto posible para siempre, sin poder "arrepentirse" de haber elegido B antes.

**El problema de fondo:** el greedy tomó B (w=6) de forma codiciosa. Pero B bloquea A(w=4) y C(w=4). La suma de ambos (8) supera el peso de B (6). Para detectar esto, se necesitaría evaluar todas las combinaciones posibles, que es exactamente lo que el DP hace de forma eficiente.

> [!warning] Contraejemplo: el greedy con peso NO es siempre óptimo
>
> | Intervalo | Inicio | Fin | Peso |
> | --------- | ------ | --- | ---- |
> | A         | 0      | 2   | 4    |
> | **B**     | 0      | 5   | **6** ← mayor peso |
> | C         | 3      | 6   | 4    |
> | D         | 2      | 4   | 1    |
> | E         | 6      | 8   | 3    |
>
> El greedy ordena por peso desc y elige **B (w=6)**, luego E (w=3) → **total = 9**
>
> El óptimo (DP) elige **A + C + E** (4 + 4 + 3) → **total = 11**
>
> **¿Por qué falla?** B bloquea a A y a C. La suma de A+C es 8, que supera el peso de B (6). El greedy hace una elección localmente óptima sin ver que ese intervalo "caro" destruye una combinación aún más valiosa.


---
---

## Comparación Final entre los 4 Algoritmos

| Archivo | Estrategia | ¿Con peso? | Complejidad | ¿Óptimo? |
|---------|------------|------------|-------------|----------|
| `schedulling_naive.c` | Fuerza bruta (todos los subconjuntos) | No | O(2ⁿ · n²) | Sí (lo prueba todo) |
| `schedulling_greedy.c` | Greedy por finish time más temprano | No | O(n log n) | Sí (demostrable) |
| `schedulling_weighted_dp.c` | Programación Dinámica | Sí | O(n log n) | Sí (demostrable) |
| `schedulling_weighted_greedy.c` | Greedy por mayor peso | Sí | O(n²) | **No** (heurístico) |

**Con el ejemplo de los 5 intervalos:**

| Algoritmo | Selección | Resultado |
|-----------|-----------|-----------|
| Naive (fuerza bruta sin peso) | {A, D, E} | 3 intervalos |
| Greedy sin peso | {A, D, E} | 3 intervalos |
| DP con peso | {A, C, E} | peso 11 (óptimo) |
| Greedy con peso (heurístico) | {B, E} | peso 9 (subóptimo) |

La progresión del laboratorio es pedagógica:
1. **Naive** demuestra el problema y su solución correcta, pero inviable para n grande.
2. **Greedy sin peso** resuelve el caso sin pesos de forma elegante y óptima.
3. **Greedy con peso** muestra que la intuición greedy **falla** cuando se introducen pesos.
4. **DP con peso** resuelve correctamente el caso con pesos, con la misma complejidad que el greedy sin peso.


> [!info] ¿Cuándo usar cada uno?
> - **Sin peso → Greedy.** Es óptimo, más simple que DP y tiene la misma complejidad O(n log n). No hay razón para usar DP o fuerza bruta.
> - **Con peso → DP.** El greedy puede dar resultados muy subóptimos con pesos arbitrarios. DP evalúa explícitamente ambas opciones para cada intervalo (incluir o excluir) apoyándose en los subproblemas ya resueltos, garantizando el óptimo global en O(n log n).

> [!info] ¿Por qué greedy falla con pesos pero no sin ellos?
> Sin peso, todos los intervalos valen lo mismo: elegir el que termina antes siempre es al menos tan bueno como cualquier otra elección (argumento de intercambio). Con pesos, un intervalo corto y ligero puede terminar antes pero valer mucho menos que una combinación de intervalos solapados con él. El greedy no tiene información para anticipar esa pérdida; DP sí, porque resuelve cada subproblema óptimamente antes de avanzar.
