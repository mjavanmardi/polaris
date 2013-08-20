set term png size 768,480
set xlabel "weather scenario"
set ylabel "Vehicle Miles Traveled"
set output "vmt_bar_chart.png"
set title "VMT of Chicago network for various weather scenarios"
set key right top
plot [:][0:] 'vmt_bar_chart.dat' using 2:xticlabels(1) title 'VMT' with histogram
