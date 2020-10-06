set term postscript eps enhanced color "Helvetica" 8
plot \
'./error_jv0a_exp.dat' using ($1):($2) with lp lw 4 lt 2 title 'sim0',\
'./error_jv0a_sim.dat' using ($1):($2) with lp lw 4 lt 3 title 'exp0'
