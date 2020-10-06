set ylabel 'Current (A)'
set y2label 'Voltage (V)'
set xlabel 'Time (us)'
set y2tics
plot \
'./celiv_i.dat' using ($1*1e6):($2) axis x1y1 with lp lw 4 title 'I',\
'./celiv_v.dat' using ($1*1e6):($2) axis x1y2 with lp lw 4 title 'V'

