set term png size 768,480
#set style fill solid
set boxwidth 1 relative
set xlabel "weather scenario"
set ylabel "Vehicle Hours Traveled"
set output "vht_bar_chart.png"
set title "VHT of Chicago network for various weather scenarios"
set key right top
plot [:][0:] 'vht_bar_chart.dat' using 2:xticlabels(1) title 'VHT' with histogram
