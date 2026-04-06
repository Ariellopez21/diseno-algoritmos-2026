**`Ariel López S.`**
---

# Reforzamiento C

---
En esta práctica veremos a grandes rasgos contenidos aprendidos en el ramo anterior **Estructura de datos**, específicamente hablaremos de **estructuras**, archivo **Makefile**, archivos **encabezados** y determinaremos una estructura de archivos que pueden seguir para la realización de sus prácticas a lo largo del presente ramo.

Además, realizaremos implementación de **algoritmos** y los compararemos aprendiendo a tomar **tiempos de ejecución** con librerías especializadas de lenguaje C y luego las exportaremos utilizando **manejo de archivos** con punteros.

---

## Bloque 1  `typedef` y `struct`

### 1. `typedef` — renombrar un tipo

`typedef` no crea nada nuevo. Solo le da un **nombre alternativo** a un tipo que ya existe.

```c
typedef int Entero;
typedef double Real;

Entero x = 5;    /* exactamente igual que: int x = 5    */
Real   y = 3.14; /* exactamente igual que: double y = 3.14 */
```

#### ¿Para qué sirve?

**Legibilidad semántica** — el nombre comunica intención.

**Portabilidad** — si mañana necesitas cambiar el tipo base, lo cambias en un solo lugar:

```c
typedef int ElementType;
/* si necesitas floats en vez de ints, solo cambias esta línea */
/* todo el código que usa ElementType se adapta solo           */
```

---

### 2. `struct` — agrupar variables bajo un nombre

Un `struct` agrupa variables de distintos tipos en una sola unidad.

```c
struct Punto {
    int x;
    int y;
};
```

Para declarar una variable de ese tipo, en C puro hay que escribir la palabra `struct`:

```c
struct Punto p;   /* sin typedef, se necesita "struct" cada vez */
p.x = 3;
p.y = 7;
```

---

### 3. Formas de declarar un `struct`

#### Forma 1 — `struct` con nombre (sin typedef)

```c
struct Nodo {
    int          valor;
    struct Nodo *siguiente;  /* referencia a sí mismo */
};

struct Nodo n1;   /* se necesita escribir "struct" siempre */
```

> Necesaria cuando el struct se referencia **a sí mismo** (listas enlazadas, árboles). El nombre `Nodo` existe desde la primera línea, antes de cerrar la llave.

---

#### Forma 2 — `struct` anónimo con `typedef`

```c
typedef struct {
    int x;
    int y;
} Punto;

Punto p;   /* ya no se necesita escribir "struct" */
p.x = 3;
```

Sin nombre después de `struct`. Funciona cuando el struct **no necesita referenciarse a sí mismo**.

> No podría usarse para listas enlazadas porque dentro del struct el alias `Punto` todavía no existe.

---

#### Forma 3 — `struct` con nombre y `typedef` (estilo explícito)

```c
typedef struct Nodo {
    int          valor;
    struct Nodo *siguiente;   /* usa el nombre "struct Nodo", no el alias */
} Nodo;

Nodo *n = NULL;   /* el alias "Nodo" está disponible fuera */
```

Dos nombres para lo mismo: `struct Nodo` (nombre interno) y `Nodo` (alias externo). Adentro del struct se usa `struct Nodo *` porque el alias aún no está definido.

---

#### Forma 4 — estilo Weiss (typedef oculta el puntero)

Este es el patrón del libro de Weiss. El objetivo es que el **cliente no vea el puntero**.

```c
typedef int           ElementType;
typedef struct Nodo  *PtrToNodo;
typedef PtrToNodo     Lista;
typedef PtrToNodo     Position;

struct Nodo {
    ElementType Element;
    Position    Next;
};
```

Quien usa esta lista escribe:

```c
Lista    l = NULL;    /* en realidad es un puntero, pero no se ve */
Position p = l;
```

> **Ventaja:** el código cliente queda limpio, sin asteriscos visibles. **Riesgo:** quien no conoce la implementación no sabe que `Lista` es un puntero. `Lista a, b` declara dos punteros — correcto, porque `Lista` ya incluye el `*`.

---

#### Comparación de los cuatro estilos

|Estilo|Sintaxis al usar|Auto-referencia|Puntero visible|
|---|---|---|---|
|Sin typedef|`struct Nodo n`|Sí|Sí|
|Anónimo + typedef|`Punto p`|No|Sí|
|Nombre + typedef|`Nodo *n`|Sí|Sí|
|Weiss|`Lista l`|Sí|No|

---

### 4. Struct dentro de struct

Un struct puede contener otro struct como campo.

```c
typedef struct {
    int x;
    int y;
} Punto;

typedef struct {
    Punto origen;    /* struct como campo */
    Punto destino;
    float grosor;
} Linea;

Linea l;
l.origen.x  = 0;
l.origen.y  = 0;
l.destino.x = 10;
l.destino.y = 5;
```

---

### 5. Puntero a struct — operadores `.` y `->`

Con una variable normal se usa `.` para acceder a los campos:

```c
Punto p;
p.x = 3;       /* p es la variable directa */
```

Con un puntero se usa `->`:

```c
Punto *ptr = &p;
ptr->x = 3;    /* ptr es un puntero */

/* estas dos líneas son equivalentes: */
ptr->x = 3;
(*ptr).x = 3;  /* desreferenciar y luego acceder al campo */
```

> `->` es solo azúcar sintáctica para `(*ptr).campo`. Existe porque escribir `(*ptr).campo` todo el tiempo es incómodo.

---

### 6. Pasar struct a una función — por valor vs por puntero

#### Por valor — se copia todo el struct

```c
void imprimir_punto(Punto p) {   /* recibe una copia */
    printf("(%d, %d)\n", p.x, p.y);
    p.x = 99;   /* modifica la copia, no el original */
}

Punto p = {3, 7};
imprimir_punto(p);
printf("%d\n", p.x);   /* imprime 3 — el original no cambió */
```

#### Por puntero — se comparte el original

```c
void mover_punto(Punto *p, int dx, int dy) {   /* recibe la dirección */
    p->x += dx;
    p->y += dy;
}

Punto p = {3, 7};
mover_punto(&p, 1, 2);
printf("%d\n", p.x);   /* imprime 4 — el original sí cambió */
```

#### ¿Cuándo usar cada uno?

|Situación|Usar|
|---|---|
|Solo leer el struct, struct pequeño|Por valor|
|Modificar el struct|Por puntero|
|Struct grande (muchos campos)|Por puntero — evita copiar todo|

> Un struct con un arreglo de 1000 elementos pesa mucho. Pasarlo por valor copia esos 1000 elementos en cada llamada. Pasarlo por puntero siempre copia solo una dirección de memoria (8 bytes en sistemas de 64 bits).

---

### 7. `typedef` con arreglos y punteros simples

`typedef` no es exclusivo de structs. Puede usarse con cualquier tipo:

```c
/* arreglo */
typedef int Vector[3];
Vector v = {1, 2, 3};   /* equivale a: int v[3] = {1, 2, 3} */

/* puntero */
typedef int *PtrInt;
PtrInt p = NULL;        /* equivale a: int *p = NULL */
```

> El caso del puntero en typedef tiene la misma implicación que vimos con Weiss: `PtrInt a, b` declara **dos punteros** porque `PtrInt` ya incluye el `*`. `int *a, b` declara **un puntero y un entero** — el `*` pertenece al nombre.

---

### 8. Resumen visual

```
typedef  int  ElementType;
│        │    └─ tipo base
│        └─ tipo al que se le da alias
└─ palabra clave


typedef  struct Nodo  *PtrToNodo;
│        │            └─ el asterisco forma parte del alias
│        └─ struct con nombre (puede auto-referenciarse)
└─ palabra clave


struct Nodo {
    ElementType  Element;    ← campo de tipo simple
    struct Nodo *Next;       ← auto-referencia (no puede usar el alias aún)
};


Punto p;       → p es el struct completo en memoria
Punto *ptr;    → ptr es una dirección, apunta a un Punto en memoria


p.campo        → acceso directo
ptr->campo     → acceso a través de puntero (= (*ptr).campo)
```

---

## Bloque 2 Estructura de proyecto + Makefile

> [!note] El archivo `Makefile` será compartido por pregrado virtual.

Este  Makefile gestiona un proyecto que tiene **dos programas distintos**:

- `program.out` → el programa principal (algoritmos)
- `programdb.out` → el programa que genera la base de datos

Ambos comparten los mismos headers, pero tienen sus propios directorios de fuentes.

### Estructura de directorios que asume este Makefile

```
proyecto/
├── src/          ← .c del programa principal
├── srcdb/        ← .c del programa de base de datos
├── obj/          ← .o generados al compilar (no se tocan a mano)
├── incs/         ← headers .h
├── build/        ← ejecutables finales
├── docs/         ← documentación
├── db/           ← archivos de datos
└── plots/        ← carpetas para gráficos por algoritmo
```

> [!info] Punto clave para mencionar
> Esta separación no es capricho. Cada directorio tiene una responsabilidad. Los archivos `.o` van a `obj/` para no contaminar el código fuente. Los ejecutables van a `build/` para no mezclarlos con nada más.

### Variables de configuración

```makefile
CC=gcc
EXEC=program.out
EXECDB=programdb.out
GRUPO=G1
NTAR=2
```

| Variable | Qué es                                 | Por qué importa                                              |
| -------- | -------------------------------------- | ------------------------------------------------------------ |
| `CC`     | El compilador a usar                   | Cambiar a `clang` o cruzar compiladores es cambiar una línea |
| `EXEC`   | Nombre del ejecutable principal        |                                                              |
| `EXECDB` | Nombre del ejecutable de base de datos |                                                              |
| `GRUPO`  | Identificador del grupo                | Se usa para las entregas de tareas                           |
| `NTAR`   | Número de tarea                        | Se combina con `GRUPO` al momento de enviar                  |

> [!question] Pregunta
> ¿Por qué conviene tener el nombre del compilador en una variable en vez de escribir `gcc` directamente en cada regla?

### Directorios y archivos fuente

```makefile
SRC_DIR=src
SRCDB_DIR=srcdb
OBJ_DIR=obj

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
SRCDB_FILES=$(wildcard $(SRCDB_DIR)/*.c)
```

 #### `wildcard`

`$(wildcard patron)` le pregunta al sistema operativo: _"¿qué archivos existen que coincidan con este patrón?"_

```makefile
SRC_FILES=$(wildcard src/*.c)
# resultado: src/main.c src/fibonacci.c src/timer.c
# (lo que realmente exista en la carpeta)
```

> [!success] Ventaja clave
> Si agregas un archivo `.c` nuevo a `src/`, el Makefile lo detecta automáticamente. No tienes que modificar nada.

### Transformar `.c` en `.o`

```makefile
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
OBJDB_FILES=$(patsubst $(SRCDB_DIR)/%.c,$(OBJ_DIR)/%_srcdb.o,$(SRCDB_FILES))
```

#### `patsubst` — sustitución de patrones

`$(patsubst patron_origen, patron_destino, lista)`

Toma cada elemento de la lista, y si coincide con el patrón origen, lo transforma al patrón destino.

**Ejemplo concreto:**

```
SRC_FILES  = src/main.c  src/fibonacci.c  src/timer.c
                 ↓               ↓               ↓
OBJ_FILES  = obj/main.o  obj/fibonacci.o  obj/timer.o
```

Para los archivos de base de datos, el sufijo `_srcdb` evita colisiones de nombres en `obj/`:

```
srcdb/createdb.c  →  obj/createdb_srcdb.o
```

### Headers y librerías

```makefile
INCLUDE=-I./incs/
LIBS= -lm
```

|Flag|Qué hace|
|---|---|
|`-I./incs/`|Le dice a gcc dónde buscar los archivos `.h`|
|`-lm`|Enlaza la librería matemática (`math.h`) — necesaria para `sqrt`, `pow`, `log`, etc.|

### Flags de compilación

```makefile
CFLAGS=-Wall -Wextra -Wpedantic -O3
LDFLAGS= -Wall -lm
```

### Flags de advertencia

|Flag|Qué activa|
|---|---|
|`-Wall`|Advertencias comunes: variables sin usar, comparaciones sospechosas, etc.|
|`-Wextra`|Advertencias adicionales que `-Wall` no cubre|
|`-Wpedantic`|Exige cumplimiento estricto del estándar C. Sin extensiones de gcc.|

> [!info] Buenas prácticas
> Estos flags no cambian el programa, pero hacen que el compilador sea más estricto con nosotros. En este ramo van a compilar **siempre** con `-Wall` al menos. Si el compilador advierte, hay que corregir.

### Flag de optimización

| Flag  | Qué hace                                                                        |
| ----- | ------------------------------------------------------------------------------- |
| `-O0` | Sin optimización (default). Lo que escribiste es lo que se ejecuta.             |
| `-O1` | Optimizaciones básicas                                                          |
| `-O2` | Optimizaciones agresivas sin comprometer depuración                             |
| `-O3` | Máxima optimización. Puede reordenar instrucciones, vectorizar, hacer inlining. |


### Reglas de compilación

#### Regla principal: `all`

```makefile
all: $(OBJDB_FILES) $(OBJ_FILES)
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ_FILES) $(INCLUDE) $(LIBS) $(LDFLAGS)
```

Cuando escribes `make` o `make all`:

1. Make verifica que existan todos los `.o` en `$(OBJDB_FILES)` y `$(OBJ_FILES)`
2. Si alguno no existe o está desactualizado, lo construye
3. Finalmente enlaza todos los `.o` del programa principal en `build/program.out`

#### Regla genérica: compilar `.c` → `.o`

```makefile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)
```

El `%` es un comodín. Esta regla dice: _"Para construir cualquier `obj/ALGO.o`, necesito `src/ALGO.c`"_

|Variable automática|Qué significa|
|---|---|
|`$@`|El target (lo que se va a producir): `obj/fibonacci.o`|
|`$^`|Todas las dependencias: `src/fibonacci.c`|

El flag `-c` le dice a gcc: _"compila pero no enlaces todavía, solo produce el `.o`"_

#### Regla para los archivos de base de datos

```makefile
$(OBJ_DIR)/%_srcdb.o: $(SRCDB_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)
```

Mismo concepto, pero el patrón agrega `_srcdb` al nombre del `.o` para distinguirlo:

```
srcdb/createdb.c  →  obj/createdb_srcdb.o
```

####  Regla `.PHONY`

```makefile
.PHONY: clean folders send
```

Le dice a Make que `clean`, `folders` y `send` son **nombres de reglas**, no nombres de archivos.

> **¿Por qué importa?** Si existiera un archivo llamado `clean` en el directorio, Make pensaría que ya está construido y no ejecutaría la regla. `.PHONY` previene esa confusión.

#### Regla `clean`

```makefile
clean:
	rm -f $(OBJ_FILES) $(OBJDB_FILES)
	rm -f build/$(EXEC) build/$(EXECDB)
```

Elimina todos los `.o` y los ejecutables. Fuerza una recompilación completa en el próximo `make`.

```bash
make clean   # limpia
make         # recompila todo desde cero
```

####  Regla `folders`

```makefile
folders:
	mkdir -p src srcdb obj incs build docs db
	mkdir -p plots/binarysearch plots/bubblesort plots/comparatives \
	         plots/insertionsort plots/sequentialsearch plots/selectionsort
```

Crea toda la estructura de directorios del proyecto de una sola vez.

```bash
make folders   # primera vez que se clona o inicia el proyecto
```


#### Regla `send`

```makefile
send:
	tar czf $(GRUPO)-$(NTAR).tgz --transform 's,^,$(GRUPO)-$(NTAR)/,' \
	    Makefile src srcdb incs docs
```

Empaqueta el proyecto para entrega. Con `GRUPO` y `NTAR`:

```bash
make send
# Ejemplo: genera: G1-2.tgz
```

El flag `--transform` renombra los archivos dentro del `.tgz` para que queden dentro de una carpeta `G1-2/` al descomprimir.

| Flag de `tar` | Qué hace                           |
| ------------- | ---------------------------------- |
| `c`           | Crear archivo                      |
| `z`           | Comprimir con gzip                 |
| `f`           | Especificar nombre del archivo     |
| `--transform` | Renombrar paths dentro del archivo |

> [!important] Notar qué 
> No se incluye `obj/` y `build/`; Quedan fuera del paquete. Se entregan solo fuentes, no binarios.

#### Reglas `run` y `run-db`

```makefile
run: build/$(EXEC)
	./build/$(EXEC)

run-db: build/$(EXECDB)
	./build/$(EXECDB)
```

Ejecutan el programa correspondiente. Si el ejecutable no existe o está desactualizado, Make lo construye primero.

```bash
make run      # compila si es necesario y ejecuta program.out
make run-db   # compila si es necesario y ejecuta programdb.out
```

### Flujo completo de trabajo

```
Primera vez:
  make folders    → crea directorios
  make            → compila todo
  make run        → ejecuta

Ciclo normal:
  [editar .c]
  make run        → recompila solo lo necesario y ejecuta

Se cambió un .h:
  make clean
  make run

Entregar tarea:
  make send       → genera G1-2.tgz
```

---

## Bloque 3 Implementación de Fibonacci

### La sucesión

$$T(n)=T(n-1)+T(n-2); \quad T(0)=0,\ T(1)=1$$

Cada término es la suma de los dos anteriores:

```
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...
```

---

### Tres formas de implementarlo

Existen varias formas de implementar el algoritmo de Fibonacci, pero en este caso veremos tres y nos enfocaremos en dos, pues, mencionaremos el tercero para tenerlo en cuenta más adelante en el ramo.

| Versión                | Complejidad | Descripción                                        |
| ---------------------- | ----------- | -------------------------------------------------- |
| Iterativa              | O(n)        | Recorre la secuencia de menor a mayor con un ciclo |
| Recursiva ingenua      | O(2ⁿ)       | Se llama a sí misma dos veces por cada nivel       |
| Dinámica (memoización) | O(n)        | Recursiva pero guarda resultados ya calculados     |

> [!question] ¿Por qué recursiva ingenua?
> Nos referimos a que el algoritmo es la traducción más directa posible de la definición matemática, sin optimización alguna, pudiendo aprovechar los cálculos realizados con anterioridad.

> [!tip] Algoritmos de optimización
> La versión dinámica es tan eficiente como la iterativa, pero su importancia es conceptual: introduce la idea de **no recalcular lo que ya se sabe**.

El objetivo de hoy es ver empíricamente la diferencia entre O(n) y O(2ⁿ) — no solo en teoría, sino midiendo tiempos reales, es decir, experimentalmente.

---

### Estructura del proyecto

El proyecto tiene tres archivos con responsabilidades separadas:

```
base.h        ← definiciones: struct, constantes, declaraciones
fibonacci.c   ← toda la lógica: algoritmos, medición, exportación
main.c        ← punto de entrada: solo inicializa y llama a run_experiment()
```

---

### `base.h` — definiciones compartidas

```c
#define NUM_VALUES  11
#define DATA_DIR    "data/"

typedef struct {
    int    n;
    double time_iter;
    double time_rec;
} ExecResult;

void      run_experiment(void);
long long fib_iterativo(int n);
long long fib_recursivo(int n);
```

Tres responsabilidades en un solo lugar:

**`#define NUM_VALUES`** — controla cuántos valores de n se van a medir. Si se quiere agregar o quitar puntos al experimento, se cambia este número y el arreglo `valores[]` en `fibonacci.c`. Nada más.

**`#define DATA_DIR`** — centraliza la ruta de salida. Si los archivos deben ir a otro directorio, se cambia en un solo lugar.

**`ExecResult`** — agrupa en un struct los tres datos que nos interesan por cada medición: el valor de n, el tiempo del algoritmo iterativo y el tiempo del recursivo. Tener ambos tiempos en el mismo struct hace que comparar y exportar sea directo.

---

### `main.c` — punto de entrada

```c
#include "base.h"

int main(void) {
    srand(time(NULL));
    run_experiment();
    return 0;
}
```

`main.c` no sabe nada de Fibonacci ni de archivos. Solo inicializa la semilla aleatoria y delega todo a `run_experiment()`. Así si mañana se quiere cambiar el experimento, `main.c` no se toca.

---

### `fibonacci.c` — lógica del experimento

#### Variables de configuración

```c
static const int valores[NUM_VALUES] = {
    1, 5, 10, 15, 20, 25, 30, 35, 38, 40, 42
};
```

`static` significa que este arreglo es **privado** de `fibonacci.c` — ningún otro archivo puede accederlo directamente. Es la lista de valores de n que se van a medir.

#### Los dos algoritmos

`fibonacci.c` implementa `fib_iterativo` y `fib_recursivo`. 

```c
long long fib_iterativo(int n) {  
    if (n <= 1) return n;  
    long long a = 0, b = 1, c;  
    for (int i = 2; i <= n; i++) 
    { c = a + b;  a = b;  b = c; }  
    return b; 
}
```

```c
long long fib_recursivo(int n) {  
    if (n <= 1) return n;  
    return fib_recursivo(n - 1) + fib_recursivo(n - 2);  
}
```

#### Medición de tiempo

```c
void run_experiment(void) {
...
/* medir iterativo */  
ini      = clock();  
res_iter = fib_iterativo(n);  
fin      = clock();  
resultados[i].time_iter = (double)(fin - ini) / CLOCKS_PER_SEC;  
  
/* medir recursivo */  
ini     = clock();  
res_rec = fib_recursivo(n);  
fin     = clock();  
resultados[i].time_rec = (double)(fin - ini) / CLOCKS_PER_SEC;
...
}
```

#### El struct `ExecResult` en acción

```c
ExecResult resultados[NUM_VALUES];

/* para cada n: */
resultados[i].n         = n;
resultados[i].time_iter = (double)(fin - ini) / CLOCKS_PER_SEC;
resultados[i].time_rec  = (double)(fin - ini) / CLOCKS_PER_SEC;
```

El struct permite almacenar los tres datos de cada medición juntos. Al final del experimento, `resultados[]` tiene todo lo necesario para exportar — se recorre una vez y se escribe el CSV. Sin el struct habría que mantener tres arreglos paralelos sincronizados, lo que es propenso a errores.

#### Verificación de correctitud

```c
char check = (res_iter == res_rec) ? 'v' : 'X';
```

Antes de comparar tiempos, hay que confirmar que ambos algoritmos calculan lo mismo. Esta línea imprime `[v]` si coinciden o `[X]` si no — un chequeo rápido que descarta bugs antes de interpretar los resultados.

---

### Salida esperada

```
  n     iterativo(s)    recursivo(s)    resultado
  ----  --------------  --------------  ---------
  1     0.00000100      0.00000100      1 [v]
  10    0.00000100      0.00000100      55 [v]
  20    0.00000100      0.00001800      6765 [v]
  30    0.00000100      0.00201400      832040 [v]
  38    0.00000101      0.18401200      39088169 [v]
  42    0.00000101      1.28943100      267914296 [v]
```

El iterativo permanece prácticamente constante en todos los casos. El recursivo crece de forma visible a partir de n=30 y se vuelve dominante desde n=38 en adelante.

---

## Bloque 4 Graficar con gnuplot

### El script completo

```gnuplot
set terminal png size 800,500
set output "data/fibonacci_times.png"

set title  "Fibonacci — Iterativo O(n) vs Recursivo O(2^n)"
set xlabel "n"
set ylabel "Tiempo (segundos)"

# CSV usa coma como separador
set datafile separator ","
  
set grid  
set key left top  
  
plot "data/exec_time.csv" using 1:2 with linespoints title "iterativo", \  
     "data/exec_time.csv" using 1:3 with linespoints title "recursivo",
```

Ejecutar desde terminal:

```bash
gnuplot plot_f1_f4.gp
```

Resultado: `data/fibonacci_times.png`

---

### Partes importantes

**`set terminal pngcairo size 800,500`** — le dice a gnuplot que en vez de abrir una ventana, exporte a un PNG de 800×500 píxeles. `pngcairo` es el motor de renderizado — produce imágenes más nítidas que el `png` básico.

**`set output`** — el nombre del archivo que va a generar. Sin esta línea gnuplot no sabe dónde guardar.

**`set datafile separator ","`** — por defecto gnuplot asume columnas separadas por espacios. Esta línea le dice que use coma, necesario para cualquier CSV.

**`every ::1`** — le indica a gnuplot que empiece desde la fila 1, saltando la fila 0 que es el encabezado `n,time_iter,time_rec`.

**`using 1:2` y `using 1:3`** — selecciona qué columnas usar como ejes X e Y. Columna 1 es `n`, columna 2 es `time_iter`, columna 3 es `time_rec`. El mismo archivo CSV se lee dos veces, una curva por algoritmo.

**`with linespoints`** — dibuja línea entre puntos y marca cada dato con un punto. Con 11 valores de n es la opción más clara.