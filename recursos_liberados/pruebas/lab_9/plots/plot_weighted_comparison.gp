# ─────────────────────────────────────────────────────────────────────────────
# plot_weighted_comparison.gp
# Scheduling con pesos: DP O(n log n) vs Greedy O(n²)
#
# Uso: gnuplot plots/plot_weighted_comparison.gp  (desde el directorio lab_9/)
# ─────────────────────────────────────────────────────────────────────────────

set terminal pngcairo enhanced font "Arial,12" size 900,620
set output "img/weighted_comparison.png"

set datafile separator ","

set title  "Scheduling con Pesos: DP  O(n log n)  vs  Greedy O(n²)" font ",14"
set xlabel "n  (número de intervalos)"
set ylabel "Tiempo promedio (ns)"
set grid lc rgb "#AAAAAA" lt 0
set key top left box opaque

plot \
    "db/schedulling_weighted_dp.csv" \
        using 1:2 every ::1 \
        with linespoints lw 2 pt 7 ps 1.3 lc rgb "#27AE60" \
        title "Weighted DP — O(n log n)", \
    \
    "db/schedulling_weighted_greedy.csv" \
        using 1:2 every ::1 \
        with linespoints lw 2 pt 9 ps 1.3 lc rgb "#E67E22" \
        title "Weighted Greedy — O(n²)"
