unset mouse
#set terminal x11

set multiplot

#set data style lines

set origin 0.5,0.5
set size 0.5,0.25
unset logscale y
plot 'Ec.dat' using ($1*1e6):($2) with l lw 2 title 'LUMO (Ec)',\
'Ev.dat' using ($1*1e6):($2) with l lw 2 title 'HOMO (Ev)',\
'Fi.dat' using ($1*1e6):($2) with l title 'Fi',\
'Fn.dat' using ($1*1e6):($2) with lp title 'Fc',\
'Fp.dat' using ($1*1e6):($2) with lp title 'Fv'

set origin 0.5,0.75
plot 'phi.dat' using ($1*1e6):($2) with l lw 2 title 'Phi'
set size 0.5,0.5
set origin 0.0,0.0

plot 'j.dat' using ($1*1e6):($2) with lp title 'Je',\
'j.dat' using ($1*1e6):($3) with lp title 'Jh',\
'j.dat' using ($1*1e6):($2+$3) with lp title 'Jtot'

set origin 0.5,0.0
set size 0.5,0.25
set logscale y
plot 'nf.dat' using ($1*1e6):($2) with lp title 'n',\
'pf.dat' using ($1*1e6):($2) with lp title 'p',\
'nt.dat' using ($1*1e6):($2) with l title 'ntrap',\
'pt.dat' using ($1*1e6):($2) with l title 'ptrap'

set origin 0.5,0.25
unset logscale y
plot 'dcharge.dat' using ($1*1e6):($2) with l title 'chrage'
#$2>0?log10($2):-log10(-$2)
set origin 0.0,0.5
set size 0.5,0.5

#set y2tics
set logscale y
#plot 'spectra1d.dat' with lp
plot 'R.dat' using ($1*1e6):(($2*$2)**0.5) axis x1y1 with lp title 'Rn',\
'R.dat' using ($1*1e6):(($3*$3)**0.5) axis x1y1 with l title 'Rp'
unset logscale y


#plot 'Dex.dat' using ($1*1e6):($2) axis x1y1 with l title 'Te',\
#'Dex.dat' using ($1*1e6):($3) axis x1y2 with l title 'Th',\
#'Dex.dat' using ($1*1e6):($4) axis x1y2 with l title 'Tl'

