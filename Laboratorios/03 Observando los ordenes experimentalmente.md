**`Ariel López S.`**
---

# Ordenes algorítmicos en la práctica

---
En el laboratorio anterior [[02 Reforzamiento C]] reforzamos contenidos de C, gnuplot y Makefile, condensando una **gran cantidad de información en una sola sesión**.

Sin embargo, no llegamos a poner en práctica los resultados obtenidos. En esta ocasión **experimentaremos directamente con la teoría vista en clases**.

---

## La teoría

Cuando estudiamos el comportamiento de un algoritmo, no nos interesa el tiempo exacto que tarda para un valor particular de `n`, sino **cómo crece ese tiempo a medida que `n` aumenta**. Esa es la esencia del análisis de complejidad algorítmica.

### ¿Por qué podemos generalizar con O(n), O(n²), O(n log n)?

Si un algoritmo recorre `n` elementos una vez, realiza `n` operaciones. Si lo recorre tres veces, realiza `3n`. Si lo recorre diez veces, `10n`. En todos los casos, **la cantidad de operaciones crece proporcionalmente a `n`**, por lo que el orden es el mismo: $O(n)$.

La notación $O$ descarta las constantes y los términos de menor peso porque, para valores grandes de `n`, lo que determina el comportamiento es el término dominante. En concreto:

$$2n, \quad 30n, \quad 100n, \quad 100000n \quad \Rightarrow \quad O(n)$$

Todos representan crecimiento lineal. La diferencia entre ellos es únicamente la **pendiente**, no el orden.

### ¿Cómo se llega a estos resultados?

De forma simplificada: contamos cuántas veces se ejecuta la operación principal en función de `n`.

- Un ciclo de `0` a `n` → $O(n)$
- Dos ciclos **anidados**, cada uno de `0` a `n` → $O(n^2)$
- Un ciclo que divide su índice a la mitad en cada iteración → $O(\log n)$
- Ciclo externo lineal con interno logarítmico → $O(n \log n)$

La clave está en identificar la estructura de control dominante y expresar su crecimiento en función del tamaño de la entrada.

### ¿Podemos verificarlo experimentalmente?

¡Claro que sí! De hecho, eso es exactamente lo que haremos hoy. Mediremos tiempos de ejecución reales para distintos valores de `n` y observaremos cómo crecen, comparando lo que la teoría predice con lo que los datos muestran.

### ¿Cómo practicaremos?

*Para quienes aún no realizan el proyecto 1 de la asignatura*, este laboratorio es la oportunidad de aprender a observar los órdenes algorítmicos de forma guiada y sin presión. *Para quienes ya avanzaron con el proyecto*, esta clase permite consolidar aspectos que quizás no se apreciaron con claridad: la presión, la velocidad o la ansiedad muchas veces no permiten entender con calma lo que está ocurriendo.

---

Lo primero que probaremos es precisamente lo anterior: que $2n$, $30n$, $100n$ y $100000n$ son todas $O(n)$. Para ello usaremos tres funciones distintas, todas lineales pero con diferente carga constante.

Si el tiempo alcanza, compararemos también funciones de orden $O(n^2)$ y $O(n \log n)$, para observar qué ocurre cuando el orden cambia.

> [!info] Disponible en [[#Verificación adicional|esta sección]].

---

## La práctica

A continuación se detallan los archivos que utilizaremos y su ubicación dentro del proyecto:

```
.
├── Makefile
├── src/
│   ├── main.c
│   ├── algoritmos.c
│   └── generate_exec_times.c
├── incs/
│   └── base.h
├── plots/
│   └── plot_f1_f3.gp
├── obj/          ← generado al compilar
├── db/           ← generado al ejecutar
└── build/        ← generado al compilar
```

---

## Algoritmos en uso - Orden O(n)

En el archivo `algoritmos.c` guardaremos todos los algoritmos que ejecutemos durante este laboratorio. Comenzaremos con tres funciones de orden $O(n)$ para nuestra primera prueba experimental.

### f1

```c
/* ────────────────────────────────────────────────────────────
 * Función sencilla - O(n) con 1 ciclo.
 * ──────────────────────────────────────────────────────────── */
void f1(int n) {
    int x = 0;
    for (int i = 0; i < n; i++)
        x += i;
}
```

### f2

Con la misma estructura que `f1`, `f2` repite la operación con tres ciclos independientes; Intenta escribirla por tu cuenta antes de continuar.

### f3

`f3` sigue la misma lógica que `f2`, pero con diez ciclos en lugar de tres; Intenta escribirla por tu cuenta antes de continuar.


## Encabezado `base.h`

El encabezado centraliza todo lo que el proyecto necesita: **librerías**, **constantes globales**, la **estructura** de resultados y las **declaraciones** de función.

```c
#ifndef BASE_H  
#define BASE_H  
  
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
  
/* ── Configuración ──────────────────────────────── */  
  
#define NUM_VALUES 101    // [0, N - 1]  
#define DATA_DIR "db/"  
  
/* ── Struct de resultado ────────────────────────── */  
  
typedef struct {  
    int n;  
    double time_f1;  
    double time_f2;  
    double time_f3;  
} ExecResults;  
  
/* ── Declaraciones ──────────────────────────────── */  

void f1(int n);  
void f2(int n);  
void f3(int n);  
void run_experiment(void);  
  
#endif
```

## Correr experimento

El archivo `generate_exec_times.c` concentra la lógica de experimentación. Puedes darle el nombre que prefieras. Dentro de él:

- Declararemos las variables de ejecución
- Crearemos o sobreescribiremos el archivo CSV de resultados
- Ejecutaremos las funciones para distintos valores de `n` y registraremos los tiempos
- Mostraremos los resultados en terminal

### Declaración y creación

La función `run_experiment` inicializa la estructura de resultados, abre el archivo CSV y escribe los encabezados antes de entrar al ciclo principal:

```c
void run_experiment(void) {
    ExecResults resultados[NUM_VALUES];
    int struct_idx = 0;

    FILE *file = fopen(DATA_DIR "comparar_Ns.csv", "w");
    if (file == NULL) {
        printf("Error al crear archivo CSV\n");
        return;
    }

    // Encabezado CSV
    fprintf(file, "n,time_f1,time_f2,time_f3\n");

    // Encabezado Terminal
    printf("n, f1 (s), f2 (s), f3 (s)");
    printf("--------------------------------------\n");

    // A PARTIR DE ACÁ SIGUEN LOS DEMÁS BLOQUES DE CÓDIGO, NO OLVIDES!
}
```

### Ciclo de ejecución

Declaramos `clock_t start` y `end` dentro del ciclo. Aunque podríamos declararlos fuera, hacerlo aquí refuerza que su único propósito es medir cada función puntualmente, sin que sus valores persistan entre iteraciones:

```c
    for (int n = 0; n <= NUM_VALUES; n += 10) {

        clock_t start, end;

        // Tiempos f1
        start = clock();
        f1(n);
        end = clock();
        double time_f1 = (double)(end - start) / CLOCKS_PER_SEC;

        // Tiempos f2
        start = clock();
        f2(n);
        end = clock();
        double time_f2 = (double)(end - start) / CLOCKS_PER_SEC;

        // Tiempos f3
        start = clock();
        f3(n);
        end = clock();
        double time_f3 = (double)(end - start) / CLOCKS_PER_SEC;

        // Dentro del ciclo hay más código que incorporaremos.
    }
```

### Guardar ejecución

Dentro del ciclo, una vez medidos los tiempos, los almacenamos en la estructura antes de escribirlos al CSV y mostrarlos en terminal. Pasar los valores por la estructura puede parecer redundante, pero sirve para tener clara la separación entre la medición y el uso de esos datos; además, si en el futuro queremos procesarlos de otra forma, ya están disponibles en memoria:

```c
    for (int n = 0; n <= NUM_VALUES; n += 10) {
        // ANTES DE ESTO VA LA EJECUCIÓN!

        resultados[struct_idx].n = n;
        resultados[struct_idx].time_f1 = time_f1;
        resultados[struct_idx].time_f2 = time_f2;
        resultados[struct_idx].time_f3 = time_f3;

        // Guardar en CSV
        fprintf(file,
            "%d,%f,%f,%f\n",
            resultados[struct_idx].n,
            resultados[struct_idx].time_f1,
            resultados[struct_idx].time_f2,
            resultados[struct_idx].time_f3
        );

        // Mostrar en terminal
        printf(
            "%d %f %f %f\n",
            resultados[struct_idx].n,
            resultados[struct_idx].time_f1,
            resultados[struct_idx].time_f2,
            resultados[struct_idx].time_f3
        );

        struct_idx++;
    }
    fclose(file);

    printf("\nDatos guardados en comparar_Ns.csv\n");

}
```

## Archivo principal

Siguiendo la buena práctica de mantener `main.c` limpio, la función `main` únicamente delega la ejecución:

```c
#include "base.h"

int main(void) {
   run_experiment();
   return 0;
}
```

> [!important] ¡NO OLVIDES!
> Incluir `#include "base.h"` en todos tus archivos `.c` para que la compilación funcione correctamente.

## Ejecutar con Makefile

El `Makefile` vive en la raíz del proyecto. El flujo completo de trabajo es el mismo que revisamos en la clase anterior:

![[02 Reforzamiento C#Flujo completo de trabajo]]

Al ejecutar `make run`, la salida en terminal debería verse similar a esta:

```bash
n, f1 (s), f2 (s), f3 (s)--------------------------------------
0 0.000001 0.000001 0.000001
10 0.000001 0.000001 0.000001
20 0.000001 0.000001 0.000001
30 0.000001 0.000001 0.000001
40 0.000001 0.000001 0.000001
50 0.000000 0.000000 0.000001
60 0.000001 0.000001 0.000001
70 0.000000 0.000000 0.000001
80 0.000000 0.000000 0.000001
...
```

## Crear gráfico

Usaremos la misma base del gnuplot de la clase anterior. vista en [[02 Reforzamiento C#El script completo|esta sección]].

### Nueva regla

Al final del `Makefile` agregamos la regla para generar el gráfico con un solo comando:

```makefile
plot:
	gnuplot ./plots/plot_f1_f3.gp
```

El nombre del archivo `.gp` puede ser el que prefieras. Recuerda también actualizar el `.PHONY`:

```makefile
.PHONY: clean clean-db folders send run run-db plot
```

Con `make plot` se generará la imagen en la ruta indicada en el archivo `.gp`.

> [!important] No olvides revisar
> Que las rutas en tu archivo `.gp` coincidan con la estructura de directorios de tu proyecto.

## Verificación adicional

Al finalizar todos los pasos, el gráfico debería mostrar que `f1`, `f2` y `f3` crecen de forma lineal. Sin embargo, se hace evidente que `f3` tiene una pendiente más pronunciada que `f2`, y `f2` más que `f1`. Esto es exactamente lo que la teoría predice: mismo orden, distinta constante.

> [!question] ¿Qué ocurrirá?
> Prueba cambiando `NUM_VALUES` por `1001`, `10001` e incluso `100001`.

### Experimentar con otros órdenes

Una vez verificado el comportamiento lineal, el siguiente paso es comprobar otros órdenes de crecimiento.

> [!question] Verifica
> Implementa `f4` como una función con dos ciclos `for` anidados. Según la teoría, debería corresponder a un orden $O(n^2)$.

Finalmente, agrega `f5` de orden $O(n \log n)$:

```c
/* ────────────────────────────────────────────────────────────
 * Función de prueba - O(n log n)
 * ──────────────────────────────────────────────────────────── */
void f5(int n) {
    int x = 0;
    for (int i = 1; i < n; i++) {
        int j = i;
        while (j > 1) {
            x += j;
            j /= 2;   // log₂(i) iteraciones internas
        }
    }
}
```

> [!question] Desde la teoría
> El orden de `f5` puede establecerse analíticamente, pero... ¿coincide con lo que muestran los datos? ¿Su pendiente es mayor o menor que la de `f4`?
