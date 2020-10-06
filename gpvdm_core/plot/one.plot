#set term postscript eps enhanced color "Helvetica" 8
#set ylabel 'Delta T/T_f - delta T/T'
#set xlabel 'Time (s)'
#unset logscale
set multiplot
set size 0.5,0.5
set origin 0.0,0.0
#set logscale y
#plot \
#'./sim/jv0/jvexternal.dat' using ($1):($2) with lp lw 4 lt 2 title 'sim1',\
#'./exp/jv0/jv.dat' using ($1):($2) with lp lw 4 lt 3 title 'exp1'
#unset logscale y

set xrange [:1.0]
set origin 0.5,0.0
plot \
'./sim/jv0/jvexternal.dat' using ($1):($2) with lp lw 4 lt 2 title 'sim1',\
'./exp/jv0/jv.dat' using ($1):($2) with lp lw 4 lt 3 title 'exp1'

set xrange [*:*]
set origin 0.5,0.5
plot \
'./error_pulse_voc_sim0.dat' using ($1):($2) with lp lw 4 lt 2 title 'sim1',\
'./error_pulse_voc_exp0.dat' using ($1):($2) with lp lw 4 lt 3 title 'exp1'

