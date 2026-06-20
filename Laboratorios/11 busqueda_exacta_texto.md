# Semana 1 — Búsqueda exacta en texto

**Diseño de Algoritmos · Clase práctica | Lenguaje C**

---

## ¿Qué es la búsqueda en texto?

La búsqueda en texto consiste en encontrar una cadena de caracteres llamada **patrón** dentro de otra cadena más larga llamada **texto**. El objetivo es determinar si el patrón aparece, y en qué posición(es) del texto.

---

## Algoritmos de búsqueda exacta en texto

- Fuerza bruta
- Boyer-Moore
- KMP (Knuth-Morris-Pratt)
- Rabin-Karp
- Aho-Corasick

---

## Actividad parte 1 — Pizarra

### Búsqueda exacta por fuerza bruta

- Intentando todo sin inteligencia.
- No recuerda nada de lo que ya se comparó.
- Cada vez que hay un `mismatch`, retrocede y lo intenta con el siguiente carácter.
- La cadena `texto` tiene longitud `n`.
- La cadena `patron` tiene longitud `m`.

#### análisis de casos

Desarrollaremos juntos los siguientes ejemplos en la pizarra, identificando los casos clave del algoritmo. Para cada ejemplo: traza manualmente el recorrido, indica los índices `i` y `j`, y anota cuándo se produce un *match* o un *mismatch*.

| # | Texto (T)   | Patrón (P) | ¿Qué caso ilustra?                        |
|---|-------------|------------|-------------------------------------------|
| 1 | `ABCABD`    | `ABD`      | Coincidencia parcial → descarte y reinicio |
| 2 | `AAAAAAB`   | `AAAB`     | Peor caso: muchos reinicios               |
| 3 | `ABCDEFG`   | `XYZ`      | Patrón ausente — recorrido completo       |
| 4 | `AABABC`    | `AB`       | Primera ocurrencia (ver nota abajo)       |

> [!note] Nota importante — Ejemplo 4
> El algoritmo debe reportar **únicamente la primera ocurrencia** del patrón en el texto. Una vez encontrada, el recorrido se detiene. No se buscan ocurrencias adicionales. Esto aplica tanto a la implementación en C como a la receta/pseudocódigo.

---

## Actividad parte 2 — Implementación

### Archivo: `busqueda_texto_bf.c`

Con base en los ejemplos trabajados en pizarra, implementar el algoritmo de fuerza bruta en lenguaje C.

1. Implementar la función de búsqueda que reciba el texto y el patrón como argumentos.
2. El programa debe imprimir la **primera posición** donde se encontró el patrón, o un mensaje indicando que no fue encontrado.
3. Verificar el funcionamiento con al menos los 4 ejemplos de la pizarra.

---

## Tarea — Para la próxima clase

### Informe KMP en LaTeX

Investigar el algoritmo **Knuth-Morris-Pratt (KMP)** y redactar un informe en LaTeX de **1 a 3 planas** que incluya:

- Breve reseña histórica: ¿quiénes lo crearon y cuándo?
- Explicación de cómo funciona (sin código; usar explicación textual y/o pseudocódigo/receta).
- 2 ejemplos de aplicación del algoritmo, trazados paso a paso.

> [!info] Formato
> - Sin portada, sin índice, sin bibliografía formal. Debe incluir el nombre del estudiante. Entregar como `.pdf` compilado desde LaTeX.

---

*Diseño de Algoritmos — Proyecto 4: Búsqueda exacta en texto · Semana 1 de 3*
