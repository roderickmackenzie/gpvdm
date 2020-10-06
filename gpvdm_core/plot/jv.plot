unset mouse
#set terminal x11

set multiplot

#set data style lines

set origin 0.5,0.5
set size 0.5,0.25
unset logscale y
plot './solver/Ec.dat' using ($1*1e6):($2) with l lw 2 title 'LUMO (Ec)',\
'./solver/Ev.dat' using ($1*1e6):($2) with l lw 2 title 'HOMO (Ev)',\
'./solver/Fi.dat' using ($1*1e6):($2) with l title 'Fi',\
'./solver/Fn.dat' using ($1*1e6):($2) with lp title 'Fc',\
'./solver/Fp.dat' using ($1*1e6):($2) with lp title 'Fv'

set origin 0.5,0.75
plot './solver/phi.dat' using ($1*1e6):($2) with l lw 2 title 'Phi'
set size 0.5,0.5
set origin 0.0,0.0

plot './solver/Jn.dat' using ($1*1e6):($2) with lp title 'Je',\
'./solver/Jp.dat' using ($1*1e6):($2) with lp title 'Jh',\
'./solver/Jn_plus_Jp.dat' using ($1*1e6):($2) with lp title 'Jtot'

set origin 0.5,0.0
set size 0.5,0.25
set logscale y
plot './solver/nf.dat' using ($1*1e6):($2) with lp title 'n',\
'./solver/pf.dat' using ($1*1e6):($2) with lp title 'p',\
'./solver/nt.dat' using ($1*1e6):($2) with l title 'ntrap',\
'./solver/pt.dat' using ($1*1e6):($2) with l title 'ptrap'

#set origin 0.5,0.25
#unset logscale y
#plot 'Tl.dat' using ($1*1e6):($2) with l title 'Tl',\
#'Te.dat' using ($1*1e6):($2) with l title 'Te',\
#'Th.dat' using ($1*1e6):($2) with l title 'Th'
#$2>0?log10($2):-log10(-$2)
set origin 0.0,0.5
set size 0.5,0.5

#set y2tics
#set logscale y
#plot 'spectra1d.dat' with lp
plot './solver/Rn_srh.dat' using ($1*1e6):(($2*$2)**0.5) axis x1y1 with lp title 'Rn_srh',\
'./solver/Rp_srh.dat' using ($1*1e6):(($2*$2)**0.5) axis x1y1 with l title 'Rp_srh',\
'./solver/R_free.dat' using ($1*1e6):(($2*$2)**0.5) axis x1y1 with l title 'R_free'
unset logscale y


#plot 'Dex.dat' using ($1*1e6):($2) axis x1y1 with l title 'Te',\
#'Dex.dat' using ($1*1e6):($3) axis x1y2 with l title 'Th',\
#'Dex.dat' using ($1*1e6):($4) axis x1y2 with l title 'Tl'

