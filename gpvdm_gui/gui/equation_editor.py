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

## @package equation_editor
#  An equation editor
#

import os
import sys
from gui_util import dlg_get_text
from util import fx_with_units
from icon_lib import icon_get

import i18n
_ = i18n.language.gettext

#inp
from inp import inp_load_file
from inp import inp_read_next_item

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QDialog,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView

#windows
from open_save_dlg import save_as_filter

from error_dlg import error_dlg

#window

from help import help_window
from QWidgetSavePos import QWidgetSavePos
from gui_util import yes_no_dlg

from PyQt5.QtCore import pyqtSignal
from dat_file import dat_file

from gpvdm_tab import gpvdm_tab
mesh_articles = []

class equation_editor(QWidgetSavePos):

	data_written = pyqtSignal()
	def save_data(self):

		out_text=[]
		out_text.append("#points")
		out_text.append(str(self.data.y_len))
		out_text.append("#equations")
		out_text.append(str(self.tab.rowCount()))

		for i in range(0,self.tab.rowCount()):
			out_text.append("#start"+str(i))
			out_text.append(str(self.tab.get_value(i, 0)))

			out_text.append("#stop"+str(i))
			out_text.append(str(self.tab.get_value(i, 1)))

			out_text.append("#equation"+str(i))
			out_text.append(str(self.tab.get_value(i, 2)))

		out_text.append("#ver")
		out_text.append("1.0")
		out_text.append("#end")
		
		dump=""
		for item in out_text:
			dump=dump+item+"\n"

		dump=dump.rstrip("\n")

		f=open(os.path.join(self.path,self.equation_file), mode='wb')
		lines = f.write(str.encode(dump))
		f.close()


	def callback_add_section(self):

		self.tab.add(["100e-9","1000e-9",self.default_value])

		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_remove_item(self):
		self.tab.remove()

		self.build_mesh()

		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_move_down(self):
		self.tab.move_down()

		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_move_up(self):

		self.tab.move_up()

		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def update(self):
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()

	def draw_graph(self):
		self.fig.clf()
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)
		self.ax1 = self.fig.add_subplot(111)
		self.ax1.ticklabel_format(useOffset=False)

		#self.ax1.set_ylabel(self.ylabel)
		if self.data.y_scale!=None:
			y_nm= [y * self.data.y_mul for y in self.data.y_scale]
			frequency, = self.ax1.plot(y_nm,self.data.data[0][0], 'ro-', linewidth=3 ,alpha=1.0)

		self.ax1.set_xlabel(_("Wavelength")+" (nm)")

	def load_data(self):
		self.tab.clear()
		self.tab.setColumnCount(3)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("start")+" (m)", _("stop")+" (m)", _("Python Equation")])
		self.tab.setColumnWidth(2, 400)
		lines=[]
		pos=0
		lines=inp_load_file(os.path.join(self.path,self.equation_file))
		if lines!=False:
			token,self.data.y_len,pos=inp_read_next_item(lines,pos)
			token,equations,pos=inp_read_next_item(lines,pos)
			equations=int(equations)
			self.data.y_len=int(self.data.y_len)

			self.data.init_mem()
			for i in range(0, equations):
				token,start,pos=inp_read_next_item(lines,pos)
				token,stop,pos=inp_read_next_item(lines,pos)
				token,equation,pos=inp_read_next_item(lines,pos)
				self.tab.add([str(start),str(stop),str(equation)])

	def build_mesh(self):

		data_min=100.0
		if self.tab.rowCount()!=0:
			for i in range(0,self.tab.rowCount()):
				val=float(self.tab.get_value(i, 0))
				if val<data_min:
					data_min=val

			#find max
			data_max=0.0
			for i in range(0,self.tab.rowCount()):
				val=float(self.tab.get_value(i, 1))
				if val>data_max:
					data_max=val

			w=data_min
			dx=(data_max-data_min)/(float(self.data.y_len))

			for i in range(0,self.data.y_len):
				val=0.0
				for ii in range(0,self.tab.rowCount()):
					range_min=float(self.tab.get_value(ii, 0))
					range_max=float(self.tab.get_value(ii, 1))
					command=self.tab.get_value(ii, 2)
					try:
						equ=eval(command)
					except:
						print(sys.exc_info())
						error_dlg(self,_("You've made a mistake in the equation, use w for wavelength. " + command))
						equ=-1
						return
					
					if w>=range_min and w <=range_max:
						val=val+equ
				if val<0.0:
					val=1.0

				self.data.y_scale[i]=w
				self.data.data[0][0][i]=val

				w=w+dx

	def on_cell_edited(self, x,y):
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def set_default_value(self,value):
		self.default_value=value

	def set_ylabel(self,value):
		self.ylabel=value

	def callback_import(self):
		response=yes_no_dlg(self,_("Are you sure you wisht to import this, it will overwrite "))
		if response==True:
			out_file=os.path.join(self.path,self.out_file)

			self.data.save(out_file)
			self.data_written.emit()
			self.close()

	
	
	def callback_play(self):
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()

	def load(self):
		self.data.x_len=1
		self.data.y_len=4000
		self.data.z_len=1

		self.data.y_mul=1e9

		self.data.init_mem()

		self.load_data()


		self.build_mesh()
		self.draw_graph()

	def __init__(self,path,equation_file,out_file):
		QWidgetSavePos.__init__(self,"equation_editor")

		self.data=dat_file()

		self.default_value="3.0"
		self.setMinimumSize(600,300)
		self.setWindowIcon(icon_get("vars"))
		self.setWindowTitle(_("Equation editor")+" (https://www.gpvdm.com)") 

		self.path=path
		self.equation_file=equation_file
		self.out_file=out_file

		self.fig = Figure(figsize=(7,3), dpi=100)
		self.canvas = FigureCanvas(self.fig)
		self.canvas.figure.patch.set_facecolor('white')

		self.main_vbox = QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)

		self.tb_ref= QAction(icon_get("import"), _("Import\ninto model"), self)
		self.tb_ref.triggered.connect(self.callback_import)
		toolbar.addAction(self.tb_ref)

		self.tb_play = QAction(icon_get("media-playback-start"), _("Calculate"), self)
		self.tb_play.triggered.connect(self.callback_play)
		toolbar.addAction(self.tb_play)

		self.main_vbox.addWidget(toolbar)


		self.main_vbox.addWidget(self.canvas)

		#toolbar 2

		toolbar2=QToolBar()
		toolbar2.setIconSize(QSize(32, 32))

		self.tb_add = QAction(icon_get("list-add"), _("Add section"), self)
		self.tb_add.triggered.connect(self.callback_add_section)
		toolbar2.addAction(self.tb_add)

		self.tb_remove = QAction(icon_get("list-remove"), _("Delete section"), self)
		self.tb_remove.triggered.connect(self.callback_remove_item)
		toolbar2.addAction(self.tb_remove)

		self.tb_move = QAction(icon_get("go-down"), _("Move down"), self)
		self.tb_move.triggered.connect(self.callback_move_down)
		toolbar2.addAction(self.tb_move)

		self.tb_move_up = QAction(icon_get("go-up"), _("Move up"), self)
		self.tb_move_up.triggered.connect(self.callback_move_up)
		toolbar2.addAction(self.tb_move_up)

		self.main_vbox.addWidget(toolbar2)

		self.tab = gpvdm_tab()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

		self.main_vbox.addWidget(self.tab)

		self.setLayout(self.main_vbox)


		self.tab.cellChanged.connect(self.on_cell_edited)
