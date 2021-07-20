set term postscript eps enhanced color "Helvetica" 8
set multiplot layout 2, 1 
plot './fit_error_sim0.dat' using ($1):($2) with l lt 1 title 'sim0',\
'./fit_error_exp0.dat' using ($1):($2) with lp lt 1 title 'exp0' 
plot './fit_error_sim2.dat' using ($1):($2) with l lt 3 title 'sim2',\
'./fit_error_exp2.dat' using ($1):($2) with lp lt 3 title 'exp2' 

