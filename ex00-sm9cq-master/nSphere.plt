set term png
set output "nSphere.png"

set logscale y
set title "Volume of Hypersphere"
set xlabel "dimension"
set ylabel "V(n) [m^n]"
set label 1 at  28, 1e-6 "r=1.0"
set label 2 at  43, 5000 "r=2.0"
plot [-2:53] for [i=2:21] "nSphere.dat" using 1:i w lines title ""

print("Generated: nSphere.png")