set terminal pngcairo size 900,600
set output 'performance_comparison.png'
set title 'Comparaison du Temps d Execution : Séquentiel vs Multi-threading'
set xlabel 'Filtres Appliqués'
set ylabel 'Temps d Execution (ms)'
set style data histograms
set style fill solid
set boxwidth 0.5
set xtic rotate by -45
plot 'performance_data.txt' using 2:xtic(1) title 'Séquentiel' lt rgb 'red', '' using 3 title 'Multi-threading' lt rgb 'blue'
