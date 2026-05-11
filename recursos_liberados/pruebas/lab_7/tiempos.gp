# graficar_tiempos.gp
set terminal pngcairo size 900,600 enhanced font "Arial,12"
set output "tiempos_fibonacci.png"

set title "Tiempo promedio por llamada (ns) — escala log"
set xlabel "n"
set ylabel "Tiempo (nanosegundos)"
set key top left
set grid
set datafile separator ","
set style data linespoints

plot "tiempos.csv" using 1:2 title "fib\_iter"  lw 2 lc rgb "#2196F3", \
     "tiempos.csv" using 1:4 title "fib\_memo"  lw 2 lc rgb "#4CAF50", \
     "tiempos.csv" using 1:5 title "fib\_tab"   lw 2 lc rgb "#FF9800"