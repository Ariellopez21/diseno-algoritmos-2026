# ─────────────────────────────────────────────────────────────────────────────
# plot_naive_solo.gp
# Scheduling Naive — O(2^n · n²)
# Plotea los tiempos medidos y ajusta un modelo exponencial a los datos.
#
# Uso: gnuplot plots/plot_naive_solo.gp  (desde el directorio lab_9/)
# ─────────────────────────────────────────────────────────────────────────────

set terminal pngcairo enhanced font "Arial,12" size 900,620
set output "img/naive_solo.png"

set datafile separator ","

set title  "Scheduling Naive — Fuerza Bruta  O(2^n · n²)" font ",14"
set xlabel "n  (número de intervalos)"
set ylabel "Tiempo promedio (ns)"
set grid lc rgb "#AAAAAA" lt 0
set key top left box opaque

# ── Fit ──────────────────────────────────────────────────────────────────────
# Modelo: f(x) = a · e^(b·x)  captura la componente exponencial dominante.
# Se filtran los valores ≤ 0.09 ns (piso de medición) para que no sesguen
# el ajuste; esos puntos sí se muestran en la gráfica.
f(x) = a * exp(b * x)
a = 0.001
b = 0.5
fit f(x) "db/schedulling_naive.csv" \
    using 1:($2 > 0.09 ? $2 : 1/0) every ::1 via a, b

# Etiqueta con la ecuación ajustada
set label 1 sprintf("Fit: f(n) = %.2e · e^{%.3f·n}", a, b) \
          at graph 0.05, 0.92 font "Arial,11" tc rgb "#2980B9"
set label 2 "Orden teórico: O(2^n · n²)" \
          at graph 0.05, 0.85 font "Arial,10" tc rgb "#7F8C8D"

# ── Plot ─────────────────────────────────────────────────────────────────────
plot "db/schedulling_naive.csv" \
         using 1:2 every ::1 \
         with linespoints lw 2 pt 7 ps 1.4 lc rgb "#E74C3C" \
         title "Naive (medido)", \
     f(x) with lines lw 2.5 lc rgb "#2980B9" dt 2 \
         title "Fit exponencial"
