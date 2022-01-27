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
import os

from PyQt5.QtCore import QDir, Qt, QUrl
from PyQt5.QtWidgets import QHBoxLayout,QSpinBox, QLineEdit, QLabel, QPushButton, QSizePolicy, QSlider, QStyle, QVBoxLayout, QWidget,QDesktopWidget
from PyQt5.QtWidgets import QWidget, QPushButton, QAction
from PyQt5.QtGui import QIcon
import sys

from cal_path import get_cache_path
from cal_path import get_user_settings_dir

from inp import inp_save_lines_to_file
from inp import inp_load_file
from inp import inp_search_token_value

from progress_class import progress_class
from PyQt5.QtGui import QFont
from gpvdm_progress import gpvdm_progress

from icon_lib import icon_get
from inp import inp_update_token_value
from safe_delete import gpvdm_delete_file
from gtkswitch import gtkswitch
from str2bool import str2bool


class server_cache_config(QWidget):

	def __init__(self,only_open_if_full=False):
		QWidget.__init__(self)
		self.setWindowIcon(icon_get("cache"))
		self.setMinimumSize(800,300)
		self.cur_size=round(self.get_size())
		self.load_config()
		if only_open_if_full==True:
			if (self.cur_size/self.max_size)<0.9:
				return
		self.setWindowTitle("Gpvdm cache editor (https://www.gpvdm.com)")
		centerPoint = QDesktopWidget().availableGeometry().center()
		qtRectangle = self.frameGeometry()
		qtRectangle.moveCenter(centerPoint)
		self.move(qtRectangle.topLeft())

		vbox=QVBoxLayout()

		self.title_text=QLabel("Gpvdm cache editor:  Gpvdm uses a disk based cached to accelerate simulation, use this window to either increase the size of the cache or to clear the cache.  If you want to save disk space clear the cache, if you want fast simulations increase the size of the cache.")
		self.title_text.setFont(QFont('SansSerif', 15))
		#self.title_text.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.title_text.setWordWrap(True)
		vbox.addWidget(self.title_text)

		self.progress=gpvdm_progress()
		self.progress.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.progress.setMinimumSize(-1,50)
		vbox.addWidget(self.progress)

		self.data_text=QLabel()
		self.data_text.setFont(QFont('SansSerif', 15))

		self.data_text.setWordWrap(True)
		vbox.addWidget(self.data_text)

		button_box=QHBoxLayout()


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		button_box.addWidget(spacer)

		self.ignore_button=QPushButton("Close", self)
		self.ignore_button.clicked.connect(self.callback_close)
		button_box.addWidget(self.ignore_button)

		self.clear=QPushButton("Clear cache", self)
		self.clear.clicked.connect(self.callback_clear_cache)
		button_box.addWidget(self.clear)

		#spin
		spin_widget=QWidget()
		spin_widget_layout=QHBoxLayout()
		spin_widget.setLayout(spin_widget_layout)

		spin_label=QLabel(("Maximum size in Mb:"))
		spin_label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
		spin_widget_layout.addWidget(spin_label)

		self.spin=QSpinBox()
		self.spin.setMaximum(1e6)
		self.spin.setValue(self.max_size)

		spin_widget_layout.addWidget(self.spin)

		vbox.addWidget(spin_widget)
		self.spin.valueChanged.connect(self.callback_spin_edited)

		#turn off cache
		widget=QWidget()
		widget.setMinimumSize(-1,50)
		widget_layout=QHBoxLayout()
		widget.setLayout(widget_layout)

		label=QLabel(("Cache enabled:"))
		label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
		widget_layout.addWidget(label)

		self.enable_switch=gtkswitch()
		self.enable_switch.set_value(self.enabled)
		self.enable_switch.changed.connect(self.callback_enabled)
		widget_layout.addWidget(self.enable_switch)

		vbox.addWidget(widget)

		##

		button_box_widget=QWidget()
		button_box_widget.setLayout(button_box)
		vbox.addWidget(button_box_widget)

		self.setLayout(vbox)
		self.update_progress()
		self.show()

	def callback_close(self):
		self.close()

	def callback_enabled(self):
		self.enabled= not self.enabled
		inp_update_token_value(self.cache_config_file,"#cache_enabled",str(self.enabled))

	def callback_clear_cache(self):
		path=os.path.join(get_user_settings_dir(),"cache")
		gpvdm_delete_file(path,allow_dir_removal=True)
		self.close()

	def update_progress(self):
		self.progress.setValue(self.cur_size/self.max_size)
		self.data_text.setText(str(self.cur_size)+"Mb used of "+str(self.max_size)+"Mb used.")

	def callback_spin_edited(self):
		self.max_size=self.spin.value()
		inp_update_token_value(self.cache_config_file,"#cache_size",str(self.max_size))
		self.update_progress()

	def load_config(self):
		self.cache_config_file=os.path.join(get_user_settings_dir(),"cache.inp")
		self.max_size=1000
		self.enabled=True
		if os.path.isfile(self.cache_config_file)==False:

			lines=[]
			lines.append("#cache_size")
			lines.append(str(self.max_size))
			lines.append("#cache_disk_speed")
			lines.append(str(speed))
			lines.append("#cache_enabled")
			lines.append(str(self.enabled))
			lines.append("#ver")
			lines.append("1.0")
			lines.append("#end")

			inp_save_lines_to_file(self.cache_config_file,lines)

		else:
			lines=inp_load_file(os.path.join(get_user_settings_dir(),"cache.inp"))
			self.max_size=int(inp_search_token_value(lines,"#cache_size"))
			self.enabled=str2bool(inp_search_token_value(lines,"#cache_enabled"))
			#print(lines)

	def get_size(self):
		tot = 0
		for dirpath, dirnames, filenames in os.walk(get_cache_path()):
			for f in filenames:
			    fp = os.path.join(dirpath, f)
			    if not os.path.islink(fp):
			        tot += os.path.getsize(fp)

		return tot/1024/1024

