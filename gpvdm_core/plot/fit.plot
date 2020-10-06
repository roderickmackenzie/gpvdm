plot \
'./error_sun_voc0_sim.dat' using ($1):($2) with lp lw 4 lt 2 title 'sim0',\
'./error_sun_voc0_exp.dat' using ($1):($2) with lp lw 4 lt 3 title 'exp0',\
'./old.dat' using ($1):($2) with lp lw 4 lt 4 title 'old'
