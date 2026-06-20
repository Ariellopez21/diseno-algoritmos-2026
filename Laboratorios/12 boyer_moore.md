# Algoritmo de Boyer Moore

Este es un algoritmo de búsqueda exacta en texto que funciona diferente a los algoritmos vistos con anterioridad:
- Fuerza Bruta
- KMP (Como tarea investigativa)

Sí bien KMP utiliza una tabla de preprocesamiento al igual que lo veremos con BM, este último tiene un funcionamiento que lo cambia todo, pues, recorre el patrón de derecha a izquierda, mientras que utiliza al texto a su favor para avanzar o retroceder, alineando el patrón con el texto de tal forma que se hace más práctico para hallar patrones de lenguaje natural.

Conceptos que se deben entender:
- Qué es sufijo y prefijo: (insertar)
- Ejemplo de sufijo fácil (insertar)
- Cómo se ve dentro de BM (insertar un ejemplo fácil de ver)

## Qué es bad character

Cuando hay mismatch entre `T[i+j]` y `P[j]`, miras el **carácter del texto que causó el problema** (el 'bad character') y buscas si ese carácter aparece en algún lugar a la izquierda de `j` dentro del patrón. Si aparece, alineas esa ocurrencia con el bad character. Si no aparece, saltas el patrón completo.

Bad char, es una tabla que se rellena como preprocesamiento antes de entrar a analizar el texto.

La tabla se ve tal que así,

P = ABCABD

bad_char[256]= {
	'A':3,
	'B':4,
	'C':2,
	'D':5,
	'`*`':-1,
}

`*` representa cualquier carácter no presente en P.
Ejemplo, 'J' no aparece en ABCABD, por lo tanto bad_char['J'] = -1.

---

Boyer Moore:
- Recorre el patrón de derecha a izquierda.
- Recorre el texto de izquierda a derecha.

Bad Char:
Cuando hay mismatch entre `T[i + j]` y `P[j]`, miras el carácter del texto que causó el problema (el 'bad character') y buscas si ese carácter aparece en algún lugar a la izquierda de `j` dentro del patrón.

- Si aparece en la tabla, alineas esa ocurrencia con el bad character.
- Si no aparece en la tabla, saltas el patrón completo.

`salto_1 = j - bad_char[char_texto_no_coincidente]`

específicamente,

`salto = MAX(salto_1, 1)`

> salto_1 nos puede dar <= 0 en algunas ocasiones.

> `i = i + salto`

Good Suffix:
Cuando hay mismatch en posición `j`, los caracteres `P[j+1..m-1]` ya coincidieron con el texto. Ese tramo coincidente se llama **good suffix** (sufijo bueno).

Receta
**Caso base**: cuando sufijo es vacío, `gs[j] = 1`.

**Caso 2**: sí existe un subpatrón del sufijo en el prefijo, se realiza la resta:
`gs[j] = primer_elemento_sufijo - primer_elemento_prefijo`.

**Caso 3**: sí no existe subpatrón del sufijo en el prefijo, saltamos el patrón entero:
`gs[j] = m`.

> Se calcula la **última coincidencia**, leyendo de izquierda a derecha

Boyer Moore:

`salto = MAX(salto_bc, salto_gs)`.

Ejercicios:

Primer ejercicio 1:

Patrones:
- ABABC
- AJHA
- CACA

Texto:
- ABAAABCABACCABABC

Segundo ejercicio 2:

Patrones:
- tristes
- acuesta

Texto:
- Tres **tistes** tigres, tragaban trigo en un trigal, en tres tristes trastos, tragaban trigo tres tristes tigres. A Cuesta le cuesta subir la cuesta, y en medio de la cuesta, va y se acuesta.