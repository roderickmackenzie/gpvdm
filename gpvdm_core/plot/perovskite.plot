unset mouse
#set terminal x11


plot 'ion.dat' using ($1*1e9):($2) with l lw 2 title 'Ion'
