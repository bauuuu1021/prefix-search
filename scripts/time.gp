reset
set ylabel 'time(sec)'
set style fill solid
set title 'tst build time'
set term png enhanced font 'Verdana,10'
set output 'buildtime.png'

plot [:][0:0.4]'result/buildTime.txt' \
using 2:xtic(1) with histogram lt 3 title '', \
'' using ($0-0.04):($2+0.01):2 with labels title ' ' textcolor lt 3, \
'' using 4:xtic(3) with histogram lc rgb "chartreuse" title 'cpy'