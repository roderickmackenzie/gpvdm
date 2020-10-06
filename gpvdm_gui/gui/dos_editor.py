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


## @package tab_homo
#  A tab to draw the analytical HOMO/LUMO.
#

import os
from inp import inp_load_file
from inp import inp_save

from numpy import *
from open_save_dlg import save_as_image

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QGroupBox,QHBoxLayout, QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from PyQt5.QtCore import pyqtSignal

from icon_lib import icon_get

from ribbon_complex_dos import ribbon_complex_dos 
from gpvdm_tab import gpvdm_tab
from inp import inp_get_token_value
from cal_path import get_sim_path
from dat_file import dat_file

from plot_widget import plot_widget

from file_watch import get_watch

from QComboBoxLang import QComboBoxLang

from gui_util import dlg_get_text
from error_dlg import error_dlg
from inp import inp

class equation_editor(QGroupBox):

	changed = pyqtSignal()

	def load(self):
		self.tab.blockSignals(True)

		lines=[]
		self.tab.clear()
		self.tab.setHorizontalHeaderLabels([_("Function"), _("Enabled"), _("a"), _("b"), _("c")])

		lines=inp_load_file(self.file_name)
		pos=0

		row=0
		while True:
			if lines[pos]=="#end":
				break
			if lines[pos]=="#ver":
				break

			tag=lines[pos]
			pos=pos+1	#skip hash tag

			function=lines[pos]	#read label
			pos=pos+1

			tag=lines[pos]
			pos=pos+1	#skip hash tag

			enabled=lines[pos] 	#read value
			pos=pos+1

			tag=lines[pos]
			pos=pos+1	#skip hash tag

			a=lines[pos] 	#read value
			pos=pos+1

			tag=lines[pos]
			pos=pos+1	#skip hash tag

			b=lines[pos] 	#read value
			pos=pos+1

			tag=lines[pos]
			pos=pos+1	#skip hash tag
			c=lines[pos] 	#read value
			pos=pos+1

			if (row+1)>self.tab.rowCount():
				self.tab.insert_row()

			self.add_row(row,function,enabled,a,b,c)

			row=row+1

		self.tab.blockSignals(False)

	def __init__(self,file_name,name):
		QGroupBox.__init__(self)
		self.file_name=file_name
		self.name=name
		self.setTitle(name)
		self.setStyleSheet("QGroupBox {  border: 1px solid gray;}")
		vbox=QVBoxLayout()
		self.setLayout(vbox)

		toolbar=QToolBar()
		self.tab = gpvdm_tab(toolbar=toolbar)

		self.tab.tb_add.triggered.connect(self.add_item_clicked)

		self.tab.tb_remove.triggered.connect(self.on_remove_click)

		vbox.addWidget(toolbar)


		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

		self.tab.clear()
		self.tab.setColumnCount(5)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)

		self.load()

		self.tab.cellChanged.connect(self.tab_changed)

		vbox.addWidget(self.tab)

	def tab_changed(self):
		self.save()
		self.changed.emit()

	def add_row(self,i,function,enabled,a,b,c):

		self.tab.blockSignals(True)

		function_combo = QComboBoxLang()

		function_combo.addItemLang("exp",_("Exponential"))
		function_combo.addItemLang("gaus",_("Gaussian"))
		self.tab.setCellWidget(i,0, function_combo)
		function_combo.setValue_using_english(str(function).lower())
		function_combo.currentIndexChanged.connect(self.tab_changed)

		enabled_combo = QComboBoxLang()

		enabled_combo.addItemLang("true",_("True"))
		enabled_combo.addItemLang("false",_("False"))

		self.tab.setCellWidget(i,1, enabled_combo)
		enabled_combo.setValue_using_english(str(enabled).lower())
		enabled_combo.currentIndexChanged.connect(self.tab_changed)

		item1 = QTableWidgetItem(str(a))
		self.tab.setItem(i,2,item1)

		item2 = QTableWidgetItem(str(b))
		self.tab.setItem(i,3,item2)

		item3= QTableWidgetItem(str(c))
		self.tab.setItem(i,4,item3)

		self.tab.blockSignals(False)
		
	def add_item_clicked(self):
		self.tab.add([ "exp", "true", "a", "b", "c"])
		self.save()
		self.changed.emit()

	def on_remove_click(self):
		self.tab.remove()
		self.save()
		self.changed.emit()
		
	def save(self):
		lines=[]
		for i in range(0,self.tab.rowCount()):
			lines.append("#function_"+str(i))
			lines.append(self.tab.get_value(i, 0))
			lines.append("#function_enable_"+str(i))
			lines.append(self.tab.get_value(i, 1))
			lines.append("#function_a_"+str(i))
			lines.append(self.tab.get_value(i, 2))
			lines.append("#function_b_"+str(i))
			lines.append(self.tab.get_value(i, 3))
			lines.append("#function_c_"+str(i))
			lines.append(self.tab.get_value(i, 4))
		lines.append("#ver")
		lines.append("#1.0")
		lines.append("#end")
		inp_save(self.file_name,lines)

		
############
class dos_editor(QWidget):

	def update_graph(self):
		self.gen_mesh()
		self.plot.do_plot()

	def callback_save(self):
		file_name=save_as_image(self)
		if file_name!=False:
			self.canvas_lumo.figure.savefig(file_name)

	def callback_trap_depth(self):
		lines=inp()
		lines.load(os.path.join(get_sim_path(),self.dos_file))

		ret=dlg_get_text( _("Maximum trap depth (eV):"), lines.get_token("#srh_start"),"electrical.png")
		if ret.ret!=None:
			depth=0.0
			try:
				depth=-abs(float(ret.ret))
			except:
				error_dlg(self,_("That's not a number.."))
		
			lines.replace("#srh_start",str(depth))
			lines.save()
			self.update_graph()

	def gen_mesh(self):
		self.mesh=[]
		try:
			Xi=-float(inp_get_token_value(os.path.join(get_sim_path(),self.dos_file), "#Xi"))
			Eg=float(inp_get_token_value(os.path.join(get_sim_path(),self.dos_file), "#Eg"))

			srh_stop=float(inp_get_token_value(os.path.join(get_sim_path(),self.dos_file), "#srh_start"))
			bands=int(inp_get_token_value(os.path.join(get_sim_path(),self.dos_file), "#srh_bands"))
		except:
			return

		start=Xi
		stop=Xi-Eg
		pos=start
		dx=(stop-start)/100
		while(pos>stop):
			pos=pos+dx
			self.mesh.append(pos)

		self.data_lumo=dat_file()
		self.data_lumo.data_min=1e10
		self.data_lumo.data_max=1e27
		self.data_lumo.title="LUMO Density of states"

		self.data_lumo.y_label="Energy"
		self.data_lumo.data_label="States"

		self.data_lumo.y_units="Ev"
		self.data_lumo.data_units="m^{-3} eV"
		
		self.data_lumo.y_mul=1.0
		self.data_lumo.data_mul=1.0

		self.data_lumo.logdata=True

		self.data_lumo.x_len=1
		self.data_lumo.y_len=len(self.mesh)
		self.data_lumo.z_len=1

		self.data_lumo.init_mem()

		self.data_numerical_lumo=dat_file()
		self.data_numerical_lumo.title="LUMO Numberical DoS"

		self.data_numerical_lumo.data_min=1e10
		self.data_numerical_lumo.data_max=1e27

		self.data_numerical_lumo.y_label="Energy"
		self.data_numerical_lumo.data_label="States"

		self.data_numerical_lumo.y_units="Ev"
		self.data_numerical_lumo.data_units="m^{-3} eV"
		
		self.data_numerical_lumo.y_mul=1.0
		self.data_numerical_lumo.data_mul=1.0

		self.data_numerical_lumo.logdata=True

		self.data_numerical_lumo.x_len=1
		self.data_numerical_lumo.y_len=len(self.mesh)
		self.data_numerical_lumo.z_len=1

		self.data_numerical_lumo.init_mem()

		self.data_homo=dat_file()

		self.data_homo.title="HOMO Density of states"

		self.data_homo.data_min=1e10
		self.data_homo.data_max=1e27

		self.data_homo.y_label="Energy"
		self.data_homo.data_label="States"

		self.data_homo.y_units="Ev"
		self.data_homo.data_units="m^{-3} eV"
		
		self.data_homo.y_mul=1.0
		self.data_homo.data_mul=1.0

		self.data_homo.logdata=True

		self.data_homo.x_len=1
		self.data_homo.y_len=len(self.mesh)
		self.data_homo.z_len=1

		self.data_homo.init_mem()

		self.data_numerical_homo=dat_file()
		self.data_numerical_homo.title="HOMO Numerical DoS"

		self.data_numerical_homo.data_min=1e10
		self.data_numerical_homo.data_min=1e27

		self.data_numerical_homo.y_label="Energy"
		self.data_numerical_homo.data_label="States"

		self.data_numerical_homo.y_units="Ev"
		self.data_numerical_homo.data_units="m^{-3} eV"
		
		self.data_numerical_homo.y_mul=1.0
		self.data_numerical_homo.data_mul=1.0

		self.data_numerical_homo.logdata=True

		self.data_numerical_homo.x_len=1
		self.data_numerical_homo.y_len=len(self.mesh)
		self.data_numerical_homo.z_len=1

		self.data_numerical_homo.init_mem()

		for iy in range(0,len(self.mesh)):
			x=self.mesh[iy]
			y=0
			homo_y=0

			for i in range(0,self.lumo.tab.rowCount()):
				if self.lumo.tab.get_value(i,1)=="true":
					try:
						a=float(self.lumo.tab.get_value(i,2))
						b=float(self.lumo.tab.get_value(i,3))
						c=float(self.lumo.tab.get_value(i,4))
					except:
						a=0.0
						b=0.0
						c=0.0

					if self.lumo.tab.get_value(i,0)=="exp":
						if b!=0:
							y = y+ a*exp((x-Xi)/b)

					if self.lumo.tab.get_value(i,0)=="gaus":
						if c!=0:
							y = y+ a*exp(-pow(((b+(x-Xi))/(sqrt(2.0)*c*1.0)),2.0))

			for i in range(0,self.homo.tab.rowCount()):
				if self.homo.tab.get_value(i,1)=="true":
		
					try:
						a=float(self.homo.tab.get_value(i,2))
						b=float(self.homo.tab.get_value(i,3))
						c=float(self.homo.tab.get_value(i,4))
					except:
						a=0.0
						b=0.0
						c=0.0

					if self.homo.tab.get_value(i,0)=="exp":
						if b!=0:
							homo_y = homo_y+ a*exp((Xi-Eg-x)/b)

					if self.homo.tab.get_value(i,0)=="gaus":
						if c!=0:
							homo_y = homo_y+ a*exp(-pow(((Xi-Eg-x+b)/(sqrt(2.0)*c*1.0)),2.0))

			self.data_numerical_lumo.y_scale[iy]=x
			self.data_numerical_homo.y_scale[iy]=x

			self.data_lumo.y_scale[iy]=x
			self.data_lumo.data[0][0][iy]=y

			self.data_homo.y_scale[iy]=x
			#print(x,homo_y)
			self.data_homo.data[0][0][iy]=homo_y

		if bands!=0:
			dE_band=srh_stop/bands

			srh_lumo_pos=Xi
			srh_homo_pos=Xi-Eg

			srh_lumo_stop_points=[]
			srh_lumo_avg=[]
			srh_lumo_count=[]

			srh_homo_stop_points=[]
			srh_homo_avg=[]
			srh_homo_count=[]

			for i in range(0,bands+1):

				srh_lumo_stop_points.append(srh_lumo_pos)
				srh_homo_stop_points.append(srh_homo_pos)

				srh_lumo_avg.append(0.0)
				srh_homo_avg.append(0.0)

				srh_lumo_count.append(0.0)
				srh_homo_count.append(0.0)

				srh_lumo_pos=srh_lumo_pos+dE_band
				srh_homo_pos=srh_homo_pos-dE_band

			#build numeical LUMO
			for iy in range(0,len(self.mesh)):
				x=self.mesh[iy]
				for i in range(0,len(srh_lumo_stop_points)-1):
					if srh_lumo_stop_points[i+1]<x:
						srh_lumo_avg[i]=srh_lumo_avg[i]+self.data_lumo.data[0][0][iy]
						srh_lumo_count[i]=srh_lumo_count[i]+1
						break

			for iy in range(0,len(self.mesh)):
				x=self.mesh[iy]
				for i in range(0,len(srh_lumo_stop_points)-1):
					if srh_lumo_stop_points[i+1]<x:
						self.data_numerical_lumo.data[0][0][iy]=srh_lumo_avg[i]/srh_lumo_count[i]
						break

			#build numeical HOMO
			for iy in range(0,len(self.mesh)):
				x=self.mesh[iy]
				for i in range(0,len(srh_homo_stop_points)-1):
					if srh_homo_stop_points[i+1]>x:
						srh_homo_avg[i]=srh_homo_avg[i]+self.data_homo.data[0][0][iy]
						srh_homo_count[i]=srh_homo_count[i]+1
						break

			for iy in range(0,len(self.mesh)):
				x=self.mesh[iy]
				for i in range(0,len(srh_homo_stop_points)-1):
					if srh_homo_stop_points[i+1]>x:
						if srh_homo_count[i]!=0:
							self.data_numerical_homo.data[0][0][iy]=srh_homo_avg[i]/srh_homo_count[i]
						break

		self.data_numerical_lumo.file_name=os.path.join(self.dos_dir,"lumo_numberical.dat")
		self.data_numerical_homo.file_name=os.path.join(self.dos_dir,"homo_numberical.dat")

		self.data_lumo.file_name=os.path.join(self.dos_dir,"lumo.dat")
		self.data_homo.file_name=os.path.join(self.dos_dir,"homo.dat")

		self.plot.data=[self.data_numerical_lumo,self.data_numerical_homo,self.data_lumo,self.data_homo]	

	def __init__(self,file_name):
		QWidget.__init__(self)
		self.dos_file=file_name

		self.dos_dir=os.path.join(get_sim_path(),file_name[:-4])
		if os.path.isdir(self.dos_dir)==False:
			os.mkdir(self.dos_dir)

		ext=file_name[3:]

		self.lumo_file="lumo"+ext
		self.homo_file="homo"+ext

		self.setWindowTitle(_("Complex Density of states editor - gpvdm"))
		self.setWindowIcon(icon_get("electrical"))
		self.setMinimumSize(1400,500)

		edit_boxes=QWidget()
		vbox=QVBoxLayout()

		self.lumo=equation_editor("lumo"+ext,"LUMO")
		vbox.addWidget(self.lumo)
		
		self.homo=equation_editor("homo"+ext,"HOMO")
		vbox.addWidget(self.homo)
		
		self.plot=plot_widget(enable_toolbar=False)
		self.plot.set_labels([_("LUMO"),_("HOMO"),_("LUMO numerical"),_("HOMO numerical")])


		self.gen_mesh()

		edit_boxes.setLayout(vbox)

		hbox=QHBoxLayout()

		self.plot.do_plot()

		#self.plot.do_plot()

		hbox.addWidget(self.plot)


		hbox.addWidget(edit_boxes)
		
		self.ribbon=ribbon_complex_dos()
		self.ribbon.tb_save.triggered.connect(self.callback_save)
		self.ribbon.tb_trap_depth.triggered.connect(self.callback_trap_depth)
		self.main_layout_widget=QWidget()
		self.main_layout_widget.setLayout(hbox)

		self.big_vbox=QVBoxLayout()

		self.big_vbox.addWidget(self.ribbon)
		self.big_vbox.addWidget(self.main_layout_widget)

		self.setLayout(self.big_vbox)



		self.lumo.changed.connect(self.update_graph)
		self.homo.changed.connect(self.update_graph)

		get_watch().add_call_back(self.dos_file,self.update_graph)
		get_watch().add_call_back(self.lumo_file,self.lumo.load)
		get_watch().add_call_back(self.homo_file,self.homo.load)


		
	

