# ─────────────────────────────────────────────────────────────────────────────
# plot_all_comparison.gp
# Comparación de los 4 algoritmos de scheduling.
#
# El algoritmo Naive (O(2^n·n²)) sólo llega a n=20 con valores de nanosegundos
# minúsculos (~0-20 ns), mientras que los demás llegan a n=500 con decenas de
# miles de ns.  Se usan DOS ejes Y para que ambas escalas sean legibles:
#   · Eje Y izquierdo  (y1) → Naive        [escala reducida]
#   · Eje Y derecho    (y2) → los otros 3   [escala amplia]
#
# Uso: gnuplot plots/plot_all_comparison.gp  (desde el directorio lab_9/)
# ─────────────────────────────────────────────────────────────────────────────

set terminal pngcairo enhanced font "Arial,12" size 1000,660
set output "img/all_comparison.png"

set datafile separator ","

set title  "Comparación de los 4 Algoritmos de Scheduling" font ",14"
set xlabel "n  (número de intervalos)"
set grid lc rgb "#AAAAAA" lt 0
set key top left box opaque spacing 1.3

# ── Ejes duales ───────────────────────────────────────────────────────────────
set ytics  nomirror
set y2tics nomirror

set ylabel  "Naive — tiempo (ns)"        tc rgb "#E74C3C"
set y2label "Greedy / DP — tiempo (ns)"  tc rgb "#333333"

# Rango izquierdo: ajustado a los valores del naive (0 – ~25 ns)
set yrange  [0:28]
# Rango derecho: ajustado a los demás algoritmos (0 – ~26 000 ns)
set y2range [0:28000]

plot \
    "db/schedulling_naive.csv" \
        using 1:2 every ::1 axes x1y1 \
        with linespoints lw 2.5 pt 7 ps 1.4 lc rgb "#E74C3C" \
        title "Naive  O(2^n·n²)  [eje izq.]", \
    \
    "db/schedulling_greedy.csv" \
        using 1:2 every ::1 axes x1y2 \
        with linespoints lw 2 pt 9 ps 1.2 lc rgb "#2ECC71" \
        title "Greedy  O(n log n)  [eje der.]", \
    \
    "db/schedulling_weighted_dp.csv" \
        using 1:2 every ::1 axes x1y2 \
        with linespoints lw 2 pt 5 ps 1.2 lc rgb "#3498DB" \
        title "Weighted DP  O(n log n)  [eje der.]", \
    \
    "db/schedulling_weighted_greedy.csv" \
        using 1:2 every ::1 axes x1y2 \
        with linespoints lw 2 pt 11 ps 1.2 lc rgb "#E67E22" \
        title "Weighted Greedy  O(n²)  [eje der.]"
