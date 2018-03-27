reset
set ylabel 'time(cycles)'
#set style fill solid
set title 'cycle freqency distribution'
set term png enhanced font 'Verdana,10'
set output 'cycleFreq.png'

plot [:20000][:]'result/countCycle.txt' using 4:5 with points pointtype 1 lc rgb "blue" title "ref",\
'' using 4:6 with points pointtype 1 lc rgb "yellow" title "cpy"