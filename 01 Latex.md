---

---
**`Ariel López S.`**
---

# Práctica de LaTeX

---

En esta práctica veremos los fundamentos de LaTeX orientados a la redacción de informes académicos de ingeniería. Cubriremos la **estructura básica** de un documento, **formato** de texto, **inserción** de imágenes y tablas, modo matemático, código fuente con `listings`, **pseudocódigo** con `algorithm`, tabla de contenidos, bibliografía en formato IEEE y referencias cruzadas. Al final de la práctica contarás con todas las herramientas necesarias para redactar un informe técnico completo y profesional.

---

## Qué es

LaTeX es un sistema de composición tipográfica de alta calidad, ampliamente usado en el ámbito académico y científico. A diferencia de editores _WYSIWYG_ (lo que ves es lo que obtienes) como Microsoft Word, en LaTeX escribes **código** que luego se **compila** para producir un PDF con formato consistente y profesional.

Sus principales ventajas son:

- Manejo automático de referencias, índices y bibliografía.
- Renderizado de ecuaciones matemáticas sin igual.
- Formato consistente a lo largo de todo el documento.
- Estándar en publicaciones científicas e informes de ingeniería.

## Instalación

**Alternativas de instalación:**

- `texlive-full` en Linux — instalación completa vía gestor de paquetes: `sudo apt install texlive-full`.

> [!warning] `texlive-full` pesa aproximadamente 4.5GB.

- IDEs para Linux y Windows: **TeXstudio** (recomendado en escritorio), **VS Code** con extensión _LaTeX Workshop_.
- **Overleaf** — mi favorito personal: editor en línea, sin instalación, con colaboración en tiempo real, plantillas y vista visual. Ideal para aprender.

**Alternativas modernas** que también uso para documentos e informes:

- **Typst** — sistema tipográfico más moderno, sintaxis más simple que LaTeX, compilación instantánea.
- **Obsidian** — editor Markdown con soporte de LaTeX para ecuaciones; útil para notas, no tanto para informes formales.

> [!warning] No se aceptarán informes con estas alternativas modernas.

## Compilación

Si usas LaTeX de manera local, el proceso de compilación **no es un solo clic**. El flujo completo para generar `main.pdf` con bibliografía e índices es el siguiente:

```bash
pdflatex main.tex      # 1ra compilación: genera estructura base
bibtex main            # procesa la bibliografía del .bib
pdflatex main.tex      # 2da compilación: integra las citas
pdflatex main.tex      # 3ra compilación: resuelve referencias cruzadas
```

> [!warning] Dificultades comunes
> 
> - **Bibliografía:** si usas `biber` en lugar de `bibtex`, el segundo paso cambia a `biber main`. Overleaf detecta esto automáticamente.
> - **Índices (`\tableofcontents`, etc.):** se generan al **final** del proceso. En la primera compilación es normal que no aparezcan en el PDF; compila al menos dos veces seguidas para que se reflejen correctamente.
> - En Overleaf, todo esto ocurre de forma transparente al presionar _Recompile_.

---

## Contenido

A continuación veremos todo lo importante de LaTeX para creación de informes.

> [!important] ¡Importante! Todo lo que verán puede consultársele a su **IA de preferencia** para que les ayude a redactar un informe con todo lo que aprenderemos, pero es importante que se **aprendan los nombres** para saber utilizar correctamente el prompt.

### Secciones

Las secciones son la columna vertebral de cualquier documento. Los comandos más comunes, en orden jerárquico, son:

```latex
\section{Introducción}
\subsection{Contexto del problema}
\subsubsection{Detalle específico}
\paragraph{Nota puntual}
```

**Diferencia entre `\section` y `\section*`:**

|Comando|Numeración|Aparece en índice|
|---|---|---|
|`\section{Título}`|Sí, automática|Sí|
|`\section*{Título}`|No|No|

Usa `\section*` cuando quieras un título de sección sin numeración y que no aparezca en la tabla de contenidos, por ejemplo en una sección de _Agradecimientos_ o _Resumen_.

### Decoradores

Los decoradores permiten dar énfasis y estilo al texto:

|Comando|Resultado|Uso típico|
|---|---|---|
|`\textbf{texto}`|**texto**|Negrita, términos clave|
|`\textit{texto}`|_texto_|Cursiva, términos técnicos|
|`\underline{texto}`|<u>texto</u>|Subrayado|
|`\texttt{texto}`|`texto`|Monoespaciado, nombres de variables, comandos|
|`\emph{texto}`|_texto_|Énfasis contextual (se adapta al entorno)|
|`\textsc{texto}`|Versalitas|Acrónimos, nombres propios|

Ejemplo de uso combinado:

```latex
El algoritmo \textbf{Bubble Sort} tiene complejidad \texttt{O(n²)},
mientras que \textit{Merge Sort} alcanza \texttt{O(n log n)}.
```

> [!info] Resultado
> El algoritmo **Bubble Sort** tiene complejidad `O(n²)`, mientras que *Merge Sort* alcanza `O(n log n)`.

### Recursos

Existen recursos de uso común que serán utilizados a diario si se dedican a hacer documentos en LaTeX:

##### **Imágenes**

Para insertar imágenes se requiere el paquete `graphicx` y se recomienda `float` para el uso del especificador `[H]`.

```latex
\usepackage{graphicx}
\usepackage{float}
```

Ejemplo de inserción de imagen:

```latex
\begin{figure}[H]
    \centering
    \includegraphics
	    [width=0.8\textwidth]
	    {img/analisis_complejidad.jpg}
    \caption
	    {Análisis de complejidad}
    \label
	    {fig:analisis_complejidad}
\end{figure}
```

> [!info] Resultado
> ![[Pasted image 20260322213606.png|center]]

**Especificadores de posición más usados:**

|Especificador|Significado|
|---|---|
|`[H]`|Fuerza la imagen exactamente aquí (requiere `float`)|
|`[h]`|Preferentemente aquí, pero LaTeX puede moverla|
|`[t]`|Al inicio (_top_) de la página|
|`[b]`|Al final (_bottom_) de la página|

> [!tip] Buenas prácticas
> 
> - Usa siempre `\caption{}` para describir la figura.
> - Usa siempre `\label{fig:nombre}` para poder referenciarla en el texto con `\ref{fig:nombre}`.
> - El prefijo `fig:` en el label es convención, no obligación.

##### **Tablas**

Para insertar tablas se usa el entorno `tabular` dentro de `table`. Ejemplo de una tabla 3×3:

```latex
\begin{table}[H]
    \centering
    \begin{tabular}{|c|c|c|}
        \hline
        \textbf{Columna 1} & \textbf{Columna 2} & \textbf{Columna 3} \\
        \hline
        Dato A & Dato B & Dato C \\
        Dato D & Dato E & Dato F \\
        Dato G & Dato H & Dato I \\
        \hline
    \end{tabular}
    \caption{Tabla de resultados de ejemplo.}
    \label{tab:resultados}
\end{table}
```

> [!info] Resultado
> ![[Pasted image 20260322214858.png|center|800]]

**Sobre la sintaxis de `tabular`:**

- `{|c|c|c|}` define 3 columnas centradas (`c`) separadas por líneas verticales (`|`).
- Otras alineaciones: `l` (izquierda), `r` (derecha).
- `\hline` dibuja una línea horizontal.
- Las columnas se separan con `&` y las filas con `\\`.

> [!tip] Para tablas complejas, usa el generador online **TablesGenerator** (tablesgenerator.com) y pega el código directamente en tu `.tex`.

##### **Modo matemático**

LaTeX ofrece tres formas principales de escribir matemáticas:

**1. Modo en línea con `$...$`** — para expresiones dentro del párrafo:

```latex
La complejidad del algoritmo es $O(n \log n)$.
```

> [!info] Resultado
> La complejidad del algoritmo es $O(n \log n)$.

**2. Modo en línea con `\(...\)`** — equivalente al anterior, sintaxis preferida en LaTeX moderno:

```latex
La fórmula cuadrática es \(x = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a}\).
```

> [!info] Resultado
> La fórmula cuadrática es $x = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a}$.

**3. Modo display con `\[...\]`** — para ecuaciones centradas en su propia línea:

```latex
A continuación, una sumatoria
\[
    \sum_{i=1}^{n} i = \frac{n(n+1)}{2}
\]
```

> [!info] Resultado
> A continuación, una sumatoria
> $$\sum_{i=1}^{n} i = \frac{n(n+1)}{2}$$

Para ecuaciones numeradas y referenciables, usa el entorno `equation`:

```latex
\begin{equation}
    E = mc^2
    \label{eq:einstein}
\end{equation}
```

> [!important] El modo matemático es uno de los bloques más extensos de LaTeX; es prácticamente un lenguaje dentro del lenguaje. Existe documentación específica para símbolos (`\alpha`, `\beta`, `\infty`, `\int`, etc.), matrices, sistemas de ecuaciones, y más. Consúltale a tu IA de preferencia con nombres precisos: _"entorno align en LaTeX"_, _"matriz con bmatrix"_, etc.

##### **Listings**

El paquete `listings` permite insertar bloques de código fuente con resaltado de sintaxis. Primero se declara en el preámbulo:

```latex
\usepackage{listings}
\usepackage{xcolor}

\lstset{
    language=C,
    basicstyle=\ttfamily\small,
    keywordstyle=\color{blue},
    commentstyle=\color{gray},
    stringstyle=\color{orange},
    numbers=left,
    numberstyle=\tiny,
    breaklines=true,
    frame=single
}
```

Luego, en el cuerpo del documento:

```latex
\begin{lstlisting}[language=C, caption={Hola Mundo en C}, label={lst:hola}]
#include <stdio.h>

int main() {
    printf("Hello world!\n");
    return 0;
}
\end{lstlisting}
```

> [!info] Resultado
> ```c
> #include <stdio.h>
> int main() {
>     printf("Hello world!\n");
>     return 0;
> }
> ```

> [!tip] Con `caption` y `label` en el `lstlisting` puedes referenciar el bloque con `\ref{lst:hola}` y agregarlo al índice de listings con `\lstlistoflistings`.

##### **Algorithm**

El entorno `algorithm` junto con `algpseudocode` permite escribir pseudocódigo formal y numerado, ideal para describir algoritmos en informes. Se declaran en el preámbulo:

```latex
\usepackage{algorithm}
\usepackage{algpseudocode}
```

Ejemplo completo — Búsqueda Binaria:

```latex
\begin{algorithm}[H]
\caption{Búsqueda Binaria}
\label{alg:binarysearch}
\begin{algorithmic}[1]
    \Require Array ordenado $A[0 \ldots n-1]$, valor buscado $x$
    \Ensure Índice de $x$ en $A$, o $-1$ si no existe

    \State $izq \gets 0$
    \State $der \gets n - 1$

    \While{$izq \leq der$}
        \State $mid \gets \lfloor (izq + der) / 2 \rfloor$
        \If{$A[mid] = x$}
            \Return $mid$
        \ElsIf{$A[mid] < x$}
            \State $izq \gets mid + 1$
        \Else
            \State $der \gets mid - 1$
        \EndIf
    \EndWhile

    \Return $-1$
\end{algorithmic}
\end{algorithm}
```

> [!info] Resultado
> ![[Pasted image 20260322215516.png|center|650]]

**Comandos clave de `algpseudocode`:**

|Comando|Uso|
|---|---|
|`\State`|Instrucción simple|
|`\If{cond}` ... `\EndIf`|Condicional|
|`\ElsIf{cond}` / `\Else`|Ramas alternativas|
|`\While{cond}` ... `\EndWhile`|Bucle While|
|`\For{cond}` ... `\EndFor`|Bucle For|
|`\Return`|Valor de retorno|
|`\gets`|Asignación ($\gets$)|
|`\Require` / `\Ensure`|Precondición / Postcondición|

### Tabla de contenidos

Con una sola línea LaTeX genera automáticamente la tabla de contenidos a partir de las secciones del documento:

```latex
\tableofcontents        % Índice general
\listoffigures          % Índice de figuras
\listoftables           % Índice de tablas
\lstlistoflistings      % Índice de listings (requiere paquete listings)
\listofalgorithms       % Índice de algoritmos (requiere paquete algorithm)
```

Coloca estos comandos justo después de `\begin{document}`, antes del contenido principal. 

> [!info] ¡Recuerda! 
> Requieren **al menos dos compilaciones** para aparecer correctamente.

### Bibliografía

La bibliografía en LaTeX se gestiona con un archivo `.bib` separado que contiene todas las referencias, y se integra al documento mediante comandos específicos.

**Paso 1 — Crear el archivo `referencias.bib`:**

```bibtex
@book{knuth1997,
    author    = {Donald E. Knuth},
    title     = {The Art of Computer Programming, Volume 1},
    publisher = {Addison-Wesley},
    year      = {1997},
    edition   = {3}
}
```

**Tipos de entrada comunes en `.bib`:**

|Tipo|Uso|
|---|---|
|`@book`|Libros|
|`@article`|Artículos de revista|
|`@inproceedings`|Artículos de conferencia|
|`@misc`|Páginas web, recursos varios|
|`@techreport`|Informes técnicos|

**Paso 2 — Integrar en `main.tex` con estilo IEEE:**

##### **BibTEX**

```latex
% En el preámbulo:
\usepackage{cite}

% Al final del documento, antes de \end{document}:
\bibliographystyle{IEEEtran}
\bibliography{referencias}
```

##### **Biber**

```latex
% En el preámbulo:
\usepackage[style=ieee, backend=biber]{biblatex}
\addbibresource{referencias.bib}

% Al final del documento, antes de \end{document}:
\printbibliography
```

### Citas

Todo elemento que tenga asignado un `\label{}` puede ser referenciado en cualquier parte del texto con el comando correspondiente:

|Elemento|Se etiqueta con|Se referencia con|
|---|---|---|
|Sección|`\label{sec:intro}`|`\ref{sec:intro}`|
|Figura|`\label{fig:diagrama}`|`\ref{fig:diagrama}`|
|Tabla|`\label{tab:resultados}`|`\ref{tab:resultados}`|
|Ecuación|`\label{eq:formula}`|`\ref{eq:formula}`|
|Listing|`\label{lst:codigo}`|`\ref{lst:codigo}`|
|Algoritmo|`\label{alg:busqueda}`|`\ref{alg:busqueda}`|
|Bibliografía|clave en `.bib`|`\cite{knuth1997}`|

Ejemplo de uso en texto:

```latex
Como se muestra en la Figura~\ref{fig:diagrama}, el flujo del proceso...
El algoritmo implementado se detalla en el Algoritmo~\ref{alg:binarysearch}.
Este enfoque está basado en \cite{knuth1997}.
```

> [!tip] Usa `~` (espacio no separable) antes de `\ref{}` para evitar que el número quede solo en una nueva línea.

---

## Otros contenidos

### Comandos personalizados

LaTeX permite definir tus propios comandos con `\newcommand`, lo que es útil para abreviar expresiones repetitivas:

```latex
% En el preámbulo:
\newcommand{\R}{\mathbb{R}}          % \R → ℝ
\newcommand{\norm}[1]{\|#1\|}        % \norm{x} → ‖x‖
\newcommand{\BigO}[1]{O\!\left(#1\right)}  % \BigO{n^2} → O(n²)
```

Esto mejora la legibilidad del código LaTeX y facilita cambios globales: si decides cambiar cómo se ve un elemento, lo modificas en un solo lugar.

### TikZ

**TikZ** es el paquete de LaTeX para crear gráficos vectoriales directamente desde código. Sirve para diagramas de flujo, grafos, árboles, figuras geométricas, circuitos, cronogramas, entre otros.

```latex
\usepackage{tikz}

\begin{tikzpicture}
    \draw[->] (0,0) -- (2,0) node[right] {$x$};
    \draw[->] (0,0) -- (0,2) node[above] {$y$};
    \draw[blue, thick] (0,0) circle (1cm);
\end{tikzpicture}
```

No es necesario dominarlo ahora, pero es importante saber que existe. En el futuro puede ser útil para graficar estructuras de datos, topologías o diagramas de algoritmos directamente en el informe, sin depender de imágenes externas.

---

## Tarea de Laboratorio 1

Redactar un informe corto en LaTeX que contenga los siguientes elementos:

> [!info] La entrega debe ser en formato PDF con el archivo que contenga todo lo solicitado. (Recuerden incluir su nombre).

- Al menos 1 `\section` y 2 `\subsection`.
- Al menos 1 tabla **o** 1 imagen (con `\caption` y `\label`).
- Al menos 1 bloque `listings` con código de no más de 5 líneas en el lenguaje de su preferencia.
- Al menos 1 ecuación en modo display `\[\]` con un elemento matemático simple (por ejemplo, una sumatoria).
- Un archivo `.bib` con al menos 2 referencias.
- Al menos 3 referencias cruzadas en el texto usando `\ref{}` o `\cite{}`, donde **al menos 1** corresponda a una entrada de la bibliografía.
- Incluir las tablas de contenido correspondientes a los elementos referenciables presentes en el informe (`\listoffigures`, `\listoftables`, `\lstlistoflistings`, según aplique).

> [!tip] ¡Tip!
> Pueden usar el paquete `lipsum` para generar texto de relleno sin tener que redactar contenido real:
> ```latex
> \usepackage{lipsum}   % en el preámbulo
> \lipsum[1]            % genera un párrafo de Lorem Ipsum
> \lipsum[1-3]          % genera del párrafo 1 al 3
> ```

> [!info] Duración estimada: 15 minutos.

