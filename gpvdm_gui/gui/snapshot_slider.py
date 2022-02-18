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

## @package snapshot_slider
#  A slider to scroll through simulation snapshots.
#
import os
from tab import tab_class

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSlider,QHBoxLayout,QLabel,QComboBox,QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon
from PyQt5.QtCore import pyqtSignal

from help import help_window

from dat_file import dat_file
from PyQt5.QtCore import QTimer
from icon_lib import icon_get
from util import wrap_text

from gpvdm_tab import gpvdm_tab
from inp import inp

class snapshot_slider(QWidget):

	changed = pyqtSignal()

	def timer_toggle(self):
		if self.timer==None:
			self.timer=QTimer()
			self.timer.timeout.connect(self.slider_auto_incroment)
			self.timer.start(1)
			self.tb_play.setIcon(icon_get("media-playback-pause"))
		else:
			self.anim_stop()

	def anim_stop(self):
		if self.timer!=None:
			self.timer.stop()
			self.timer=None
			self.tb_play.setIcon(icon_get("media-playback-start"))

	def slider_auto_incroment(self):
		val=self.slider0.value()
		val=val+1
		if val>self.slider0.maximum():
			val=0
		self.slider0.setValue(val)
		
	def update(self):
		self.dirs=[]
		if os.path.isdir(self.path)==True:
			for name in os.listdir(self.path):
				if name!="." and name!= "..":
					full_path=os.path.join(self.path, name)
					if os.path.isdir(full_path):
						self.dirs.append(name)

		self.dirs.sort(key=int)

		for i in range(0,len(self.dirs)):
			self.dirs[i]=os.path.join(self.path, self.dirs[i])

		self.slider_max=len(self.dirs)-1
		self.slider0.setMaximum(self.slider_max)

		self.tab.clear()
		self.tab.setColumnCount(2)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([ _("File to plot"),_("Plot type")])

		self.tab.setColumnWidth(0, 400)

		self.tab.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		if self.load_state()==False:
			pos=self.tab.insert_row()
			self.insert_row(pos)

	def slider0_change(self):
		value = self.slider0.value()
		self.label0.setText(str(value))
		self.changed.emit()

	def get_file_name(self,pos=-1):
		ret=[]
		plot_types=[]
		if pos==-1:
			val=self.slider0.value()
		else:
			val=pos

		if self.slider_dir_exists()==False:
			return ret

		for i in range(0,self.tab.rowCount()):
			file_path=os.path.join(self.path,self.dirs[val],self.tab.get_value(i,0))
			if os.path.isfile(file_path)==True:
				ret.append(file_path)
				plot_types.append(self.tab.get_value(i,1))

		return ret,plot_types

	def set_path(self,path):
		self.path=path
		self.update()

		
	def __init__(self):
		QWidget.__init__(self)
		self.dirs=[]
		self.path=""
		self.timer=None

		self.tb_play = QAction(icon_get("media-playback-start"), wrap_text(_("Play"),2), self)
		self.tb_play.triggered.connect(self.timer_toggle)

		self.setWindowTitle(_("Snapshot slider")) 
		
		self.main_vbox = QVBoxLayout()

		self.slider_hbox0= QHBoxLayout()
		self.slider_max=30
		
		self.slider0 = QSlider(Qt.Horizontal)
		#self.slider0.setMinimum(0)
		#self.slider0.setMaximum(self.slider_max)

		self.slider0.setTickPosition(QSlider.TicksBelow)
		self.slider0.setTickInterval(5)
		self.slider0.valueChanged.connect(self.slider0_change)
		#self.slider0.setMinimumSize(300, 80)

		self.slider_hbox0.addWidget(self.slider0)

		self.label0 = QLabel()
		self.label0.setText("")

		self.slider0.setValue(1)

		self.slider_hbox0.addWidget(self.label0)

		self.widget0=QWidget()
		self.widget0.setLayout(self.slider_hbox0)

		self.main_vbox.addWidget(self.widget0)

		self.toolbar=QToolBar()
		self.main_vbox.addWidget(self.toolbar)

		self.tab=gpvdm_tab(toolbar=self.toolbar)

		self.main_vbox.addWidget(self.tab)
		self.tab.tb_add.triggered.connect(self.callback_insert_row)
		self.tab.tb_remove.triggered.connect(self.callback_remove_row)


		self.setLayout(self.main_vbox)

	def slider_dir_exists(self):
		if self.slider0.value()==-1:
			return False
		if self.slider0.value()>=len(self.dirs):
			return False
		
		return True

	def update_files_combo(self,combo):
		if self.slider_dir_exists()==False:
			return False
		combo.blockSignals(True)
		combo.clear()
		path=os.path.join(self.path,self.dirs[self.slider0.value()])

		all_files=[]
		if os.path.isdir(path)==True:
			for name in os.listdir(path):
				full_path=os.path.join(path, name)
				if os.path.isfile(full_path):
					if name!="data.json":
						all_files.append(name)

		all_files.sort()
		
		for a in all_files:
			combo.addItem(a)

		all_items  = [combo.itemText(i) for i in range(combo.count())]

		for i in range(0,len(all_items)):
			if all_items[i] == "Jn.dat":
				combo.setCurrentIndex(i)

		combo.blockSignals(False)
		return True

	def save_state(self):
		f=inp()
		for i in range(0,self.tab.rowCount()):
			f.lines.append(self.tab.get_value(i,0))

		f.save_as(os.path.join(self.path,"last.inp"))

	def load_state(self):
		f=inp()
		if f.load(os.path.join(self.path,"last.inp"))==False:
			return False

		if len(f.lines)==0:
			return False
		print("bing")
		for i in range(0,len(f.lines)):
			line=f.lines[i]
			print(line)
			#pos=self.tab.insert_row()
			pos=self.tab.insert_row()
			self.insert_row(pos)
			self.tab.blockSignals(True)
			self.tab.set_value(i,0,line)
			self.tab.blockSignals(False)
			#f.lines.append(self.tab.get_value(i,0))

		return True


	def files_combo_changed(self):
		self.save_state()		
		self.changed.emit()

	def insert_row(self,i):
		self.tab.blockSignals(True)

		self.item = QComboBox()
		self.update_files_combo(self.item)
		self.item.currentIndexChanged.connect(self.files_combo_changed)
		#self.item.setText(v2)
		#self.item.button.clicked.connect(self.callback_show_list)

		self.tab.setCellWidget(i,0,self.item)

		self.item_type = QComboBox()
		self.item_type.addItem("wireframe")
		self.item_type.addItem("heat")
		self.item_type.addItem("contour")
		self.item_type.currentIndexChanged.connect(self.files_combo_changed)

		self.tab.setCellWidget(i,1,self.item_type)

		self.tab.blockSignals(False)

	def callback_insert_row(self):
		pos=self.tab.insert_row()
		self.insert_row(pos)
		self.changed.emit()

	def callback_remove_row(self):
		self.tab.remove()
		self.changed.emit()


