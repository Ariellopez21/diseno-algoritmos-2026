# ─────────────────────────────────────────────────────────────────────────────
# plot_greedy_comparison.gp
# Comparación de los dos algoritmos Greedy de scheduling:
#   · Greedy sin peso   — O(n log n)   (scheduling_greedy)
#   · Greedy ponderado  — O(n²)        (scheduling_weighted_greedy)
#
# Uso: gnuplot plots/plot_greedy_comparison.gp  (desde el directorio lab_9/)
# ─────────────────────────────────────────────────────────────────────────────

set terminal pngcairo enhanced font "Arial,12" size 900,620
set output "img/greedy_comparison.png"

set datafile separator ","

set title  "Scheduling Greedy: sin peso  O(n log n)  vs  ponderado O(n²)" \
           font ",14"
set xlabel "n  (número de intervalos)"
set ylabel "Tiempo promedio (ns)"
set grid lc rgb "#AAAAAA" lt 0
set key top left box opaque

plot \
    "db/schedulling_greedy.csv" \
        using 1:2 every ::1 \
        with linespoints lw 2 pt 7 ps 1.3 lc rgb "#2ECC71" \
        title "Greedy sin peso — O(n log n)", \
    \
    "db/schedulling_weighted_greedy.csv" \
        using 1:2 every ::1 \
        with linespoints lw 2 pt 9 ps 1.3 lc rgb "#E67E22" \
        title "Greedy ponderado — O(n²)"
