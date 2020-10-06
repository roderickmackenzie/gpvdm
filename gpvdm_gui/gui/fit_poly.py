# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#
#   https://www.gpvdm.com
#   Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
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
# 

## @package fit_poly
#  Fit a polynominal to some data.
#

import os
import sys
from gui_util import dlg_get_text
import webbrowser
from util import fx_with_units
from icon_lib import icon_get

import i18n
_ = i18n.language.gettext

#inp
from inp import inp_search_token_value
from inp import inp_read_next_item

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
import numpy as np
import matplotlib.pyplot as plt

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QDialog,QVBoxLayout,QPushButton,QLabel,QSpinBox,QAction,QTabWidget,QTableWidget,QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon


class fit_poly(QDialog):

	def __init__(self,x_min,x_max,data):
		QWidget.__init__(self)
		self.setWindowTitle(_("Poly fit")+"(https://www.gpvdm.com)") 
		self.data=data
		self.ret_math=None
		self.ret=None
		self.interal_data=[]
		for i in range (0,self.data.y_len):		#x_nm= [x * 1e9 for x in self.data.y_scale]
			x=self.data.y_scale[i]
			#print(x_min,x,x_max)
			if x>=x_min and x<=x_max:
				self.interal_data.append((self.data.y_scale[i],self.data.data[0][0][i]))
				#print(self.data.y_scale[i],self.data.data[0][0][i])
			#frequency, = self.ax1.plot(x_nm,self.data.data[0][0][i], 'bo-', linewidth=3 ,alpha=1.0)
		#print(self.interal_data)
		self.main_vbox=QVBoxLayout()
		self.label = QLabel(_("Polynomial coefficients"))
		self.main_vbox.addWidget(self.label)
		self.sp = QSpinBox()
		self.main_vbox.addWidget(self.sp)
		self.button = QPushButton(_("Ok"), self)
		self.button.clicked.connect(self.callback_click_ok)
		self.main_vbox.addWidget(self.button)
		self.setLayout(self.main_vbox)
		ret=False

	def callback_click_ok(self,data):
		points = np.array(self.interal_data)	#[(1, 1), (2, 2), (3, 3), (7, 3), (9, 3)]
		# get x and y vectors
		x = points[:,0]
		y = points[:,1]

		# calculate polynomial
		terms=int(self.sp.value())
		self.ret = np.polyfit(x, y, terms)
		f = np.poly1d(self.ret)

		tot=""
		val=0
		for i in range(0,len(self.ret)):
			p=len(self.ret)-1-i
			tot=tot+str(self.ret[i])+"*pow(w,"+str(p)+")"+"+"
		tot=tot[:-1]
		self.ret_math=tot

		self.close()

	def run(self):
		self.exec_()

