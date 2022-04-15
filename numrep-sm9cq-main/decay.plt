# simple plot example for radio decay data
set title "Radioisotope decay plot"
set xlabel "time [seconds]"
set ylabel "fraction of atoms remaining"

plot "decay01.dat" pt 4,0.99**x lw 3 lc 'green' title "Binomial Model",\
exp(-0.01*x) ti "Poisson Model" lc 'red'

set term png
set output "decay.png"
replot

reset
set term pop
