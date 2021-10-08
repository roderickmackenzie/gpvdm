# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2008-2022 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#   
#   https://www.gpvdm.com
#   
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License v2.0, as published by
#   the Free Software Foundation.
#   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   

## @package gtkswitch
#  Package to provide an equivlent to the gnome switch
#


import math
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter,QFont,QColor,QPen

from PyQt5.QtCore import pyqtSignal
from generic_switch import generic_switch 
class gtkswitch(generic_switch):

	def __init__(self):      
		generic_switch.__init__(self,state0="On",state1="Off",state0_value=True,state1_value=False)



