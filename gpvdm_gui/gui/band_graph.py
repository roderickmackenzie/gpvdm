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

## @package band_graph
#  Band diagram graph
#


import os
import io
import sys
from numpy import *

#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

import zipfile
from util import lines_to_xyz
from dat_file import dat_file


from matplotlib.figure import Figure
from plot_io import get_plot_file_info

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu,QApplication
from PyQt5.QtGui import QIcon,QPixmap,QImage

#inp
from inp import inp_load_file
from inp import inp_search_token_value
from inp import inp_get_token_value
#calpath
from icon_lib import icon_get

from open_save_dlg import save_as_filter
from cal_path import get_sim_path
from cal_path import get_materials_path
from cal_path import get_default_material_path

from lock import get_lock

from epitaxy import get_epi
from PyQt5.QtCore import pyqtSignal

class band_widget(FigureCanvas):
	changed = pyqtSignal()

	def __init__(self,figure):
		FigureCanvas.__init__(self,figure)
		self.show_labels=True
		self.show_energies=True
		self.build_menu()

	def build_menu(self):

		self.menu = QMenu(self)

		self.menu_show_labels=self.menu.addAction(_("Show labels"))
		self.menu_show_labels.triggered.connect(self.menu_toggle)
		self.menu_show_labels.setCheckable(True)
		self.menu_show_labels.setChecked(True)

		self.menu_show_energies=self.menu.addAction(_("Show energies"))
		self.menu_show_energies.triggered.connect(self.menu_toggle)
		self.menu_show_energies.setCheckable(True)
		self.menu_show_energies.setChecked(True)

		self.menu_copy=self.menu.addAction(_("Copy to clipboard"))

	def menu_toggle(self):
		action = self.sender()
		text=action.text()
		self.show_labels=self.menu_show_labels.isChecked()
		self.show_energies=self.menu_show_energies.isChecked()
		self.changed.emit()

	def mouseReleaseEvent(self,event):
		self.menu.exec_(event.globalPos())



class band_graph(QWidget):
	def __init__(self):
		QWidget.__init__(self)
		self.main_vbox = QVBoxLayout()

		self.my_figure=Figure(figsize=(5,4), dpi=100)
		self.canvas = band_widget(self.my_figure)
		self.canvas.setFocusPolicy( Qt.ClickFocus )
		self.canvas.setFocus()
		self.canvas.figure.patch.set_facecolor('white')
		#self.canvas.set_size_request(600, 400)
		self.canvas.changed.connect(self.draw_graph)
		self.canvas.menu_copy.triggered.connect(self.do_clip)

		self.canvas.show()
		
		self.main_vbox.addWidget(self.canvas)

		#self.canvas.connect('key_press_event', self.on_key_press_event)

		self.setLayout(self.main_vbox)

		self.epi=get_epi()


	def save_image(self):
		response=save_as_filter(self,"png (*.png);;jpg (*.jpg);;svg (*.svg)")
		if response != None:
			#print(response)
			self.my_figure.savefig(response)

	def do_clip(self):
		buf = io.BytesIO()
		self.my_figure.savefig(buf)
		QApplication.clipboard().setImage(QImage.fromData(buf.getvalue()))
		buf.close()

	def set_data_file(self,file):
		self.data_file=file

	def draw_graph(self):

		self.my_figure.clf()
		self.layer_end=[]
		self.layer_name=[]
		self.optical_mode_file=os.path.join(get_sim_path(),"optical_output",self.data_file)
		#print(self.optical_mode_file)
		self.my_figure.clf()
		ax1 = self.my_figure.add_subplot(111)
		ax2 = ax1.twinx()
		x_pos=0.0
		nlayer=0
		color =['r','g','b','y','o','r','g','b','y','o','r','g','b','y','o','r','g','b','y','o']

		self.epi=get_epi()


		x_pos=0.0
		for layer in self.epi.layers:
			layer_ticknes=layer.dy
			layer_material=layer.optical_material
			lumo=0.0
			homo=0.0
			
			delta=float(layer_ticknes)*1e9

			lines=[]
			#we could have zipped the file
			mat_file=os.path.join(get_materials_path(),layer_material,'mat.inp')
			archive=os.path.basename(os.path.dirname(mat_file))+".zip"

			material_type=inp_get_token_value(mat_file, "#material_type",archive=archive)
			unknown_lumo_eg=False

			if layer.dos_file.startswith("dos")==False:
				dos_file=os.path.join(get_materials_path(),layer_material,'dos.inp')
				if os.path.isfile(dos_file)==False:
					unknown_lumo_eg=True
					dos_file=os.path.join(get_default_material_path(),"dos.inp")

				lines=inp_load_file(dos_file,archive=archive)
				#print("rod",lines,dos_file,material_type,os.path.join(get_materials_path(),layer_material,'mat.inp'))
				if lines!=False:
					lumo=-float(inp_search_token_value(lines, "#Xi"))
					Eg=float(inp_search_token_value(lines, "#Eg"))
			else:
				lines=inp_load_file(os.path.join(get_sim_path(),layer.dos_file+".inp"))
				#print(lines)
				if lines!=False:
					lumo=-float(inp_search_token_value(lines, "#Xi"))
					Eg=float(inp_search_token_value(lines, "#Eg"))
					#print("b")

			x = [x_pos,x_pos+delta,x_pos+delta,x_pos]

			#print("lumo=",lumo)
			lumo_delta=lumo-0.1

			homo=lumo-Eg
			homo_delta=homo-0.1

			draw_homo=True
			y_name_pos=lumo-Eg+Eg*0.1

			if Eg==0.0 or material_type=="metal":
				lumo_delta=-7.0
				draw_homo=False
				y_name_pos=-6.5#lumo-1.0

			x_pos=x_pos+delta
			self.layer_end.append(x_pos)
			self.layer_name.append(layer_material)

			rot=0
			if len(self.epi.layers)>5:
				rot=90

			if self.canvas.show_labels==True:
				item=ax2.text(x_pos-delta/1.5, y_name_pos, layer.name, rotation=rot)
				item.set_fontsize(15)

			lumo_shape = [lumo,lumo,lumo_delta,lumo_delta]
			ax2.fill(x,lumo_shape, color[nlayer],alpha=0.4)
			if unknown_lumo_eg==False:
				if self.canvas.show_energies==True:
					item=ax2.text(x_pos-delta/1.5, lumo+0.1, "%.2f eV" % lumo)
					item.set_fontsize(15)

			if draw_homo==True:
				homo_shape = [homo,homo,homo_delta,homo_delta]
				ax2.fill(x,homo_shape, color[nlayer],alpha=0.4)
				if unknown_lumo_eg==False:
					if self.canvas.show_energies==True:
						item=ax2.text(x_pos-delta/1.5, lumo-Eg-0.4, "%.2f eV" % homo)
						item.set_fontsize(15)

			nlayer=nlayer+1

		state=dat_file()
		if state.load(self.optical_mode_file)==True:
			#print(state.data)
			ax1.set_ylabel(state.data_label+" ("+state.data_units+")")
			ax1.set_xlabel(_("Position")+" (nm)")
			ax2.set_ylabel(_("Energy")+" (eV)")
			ax2.set_xlim([0.0, x_pos])
			#ax2.axis(max=)#autoscale(enable=True, axis='x', tight=None)

			for i in range(0,len(state.y_scale)):
				state.y_scale[i]=state.y_scale[i]*1e9
			
			ax1.plot(state.y_scale,state.data[0][0], 'black', linewidth=3 ,alpha=0.5)



		self.my_figure.tight_layout()

		if get_lock().is_trial()==True:
			self.my_figure.text(0.90, 0.25, 'Upgrade to gpvdm proessional.', fontsize=15, color='gray', ha='right', va='bottom', alpha=0.1)
			self.my_figure.text(0.40, 0.80, 'Upgrade to gpvdm proessional.', fontsize=15, color='gray', ha='right', va='bottom', alpha=0.1)
			self.my_figure.text(0.40, 0.20, 'Upgrade to gpvdm proessional.', fontsize=15, color='gray', ha='right', va='bottom', alpha=0.1)

		self.canvas.draw()




