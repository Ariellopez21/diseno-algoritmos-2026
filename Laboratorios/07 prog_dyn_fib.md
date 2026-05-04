Este solo es un recordatorio de ejecución y prácticas para el correcto análisis:

Cualquiera de las primeras herramientas se pueden ejecutar simplemente con el compilador de gcc:

```bash
$ gcc tool1.c -o tool1
$ gcc tool2.c -o tool2
$ gcc tool3.c -o tool3
```

Pero la herramienta de ejecución `tool4.c` debe ejecutarse así:

```bash
$ gcc -O0 tool4.c -o tool4
```

Pues `O0` le dice al compilador que no aplique ninguna optimización.

Por defecto, gcc compila con la flag `O0` pero puede ocurrir que no, o bien, puedes ejecutar con un makefile predeterminado sin notarlo, así que es importante considerar este flag para la correcta ejecución de la toma de tiempos.

Por eso también existe la variable `dummy` dentro de los algoritmos, pues, **acumular el resultado fuerza al compilador a ejecutar la llamada**. 

Sin embargo, con la flag en `-O2` o `-O3` incluso la variable `dummy` puede ser optimizado si el compilador determina que nunca se lee con efecto visible (precisamente, lo que estamos haciendo en los algoritmos que lo usamos). La flag `-O0` elimina ese riesgo completamente.

> [!info] Algo que notar....
> Con `-O3`, GCC puede detectar patrones recursivos y aplicar su propia memoización automática, haciendo que `fib_rec` se comporte diferente a lo que el código dice.