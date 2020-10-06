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

## @package cluster_config_window
#  A window used to configure the cluster done via ssh.
#

import os

#qt
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QMenu,QToolBar,QSizePolicy, QVBoxLayout, QTabWidget, QAbstractItemView, QListWidgetItem,QPushButton, QListView,QWidget,QListWidget,QAction
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal

#cal_path
from icon_lib import icon_get
from cal_path import get_ui_path

from tab import tab_class

from help import help_window

from error_dlg import error_dlg

from inp import inp_get_token_value
from inp import inp_load_file
from inp import inp_get_token_value_from_list
from inp import inp_update_token_value
from cal_path import get_sim_path

from QWidgetSavePos import QWidgetSavePos

from css import css_apply

from cluster_config_ribbon import cluster_config_ribbon
from cal_path import get_cluster_path

import i18n
_ = i18n.language.gettext

import random

class cluster_config_window(QWidgetSavePos):

	def generate_keys(self):
		tab = self.notebook.currentWidget()
		file_name=tab.file_name

		iv = random.getrandbits(128)
		iv="%032x" % iv
		inp_update_token_value(os.path.join(get_sim_path(),"cluster","crypto.inp"),"#iv",iv)
		inp_update_token_value(os.path.join(get_sim_path(),file_name),"#iv",iv)

		key = random.getrandbits(128)
		key="%032x" % key
		inp_update_token_value(os.path.join(get_sim_path(),"cluster","crypto.inp"),"#key",key)
		inp_update_token_value(os.path.join(get_sim_path(),file_name),"#key",key)

		tab.update()

	def __init__(self,parent):
		QWidgetSavePos.__init__(self,"cluster_window")
		self.ribbon=cluster_config_ribbon()
		self.ribbon.install.triggered.connect(self.install_to_cluster)
		self.ribbon.boot.triggered.connect(self.boot_cluster)
		self.ribbon.generate_keys.triggered.connect(self.generate_keys)

		self.ribbon.stop.triggered.connect(self.kill_cluster)
		self.ribbon.remove.triggered.connect(self.remove_from_cluster)

		self.setFixedSize(900, 600)
		self.setWindowIcon(icon_get("preferences-system"))

		self.setWindowTitle(_("Configure")+" (https://www.gpvdm.com)") 
		

		self.main_vbox = QVBoxLayout()


		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)


		self.main_vbox.addWidget(self.ribbon)


		self.notebook = QTabWidget()
		css_apply(self.notebook,"tab_default.css")
		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		self.ribbon.order_widget.added.connect(self.callback_add_page)

		self.ribbon.order_widget.notebook_pointer=self.notebook
		self.ribbon.order_widget.show_enable()
		self.ribbon.order_widget.load_tabs()

		self.setLayout(self.main_vbox)


	def get_config(self):
		tab = self.notebook.currentWidget()
		file_name=tab.file_name

		self.user_name=inp_get_token_value(os.path.join(get_sim_path(),file_name), "#cluster_user_name")
		self.ip=inp_get_token_value(os.path.join(get_sim_path(),file_name), "#cluster_ip")
		self.cluster_dir=inp_get_token_value(os.path.join(get_sim_path(),file_name), "#cluster_cluster_dir")

	def write_cluster_config(self):
		tab = self.notebook.currentWidget()
		file_name=tab.file_name

		cluster_ip=inp_get_token_value(os.path.join(get_sim_path(),file_name), "#cluster_ip")
		inp_update_token_value(os.path.join(get_cluster_path(),"node.inp"),"#master_ip",cluster_ip)

		cluster_ip=inp_get_token_value(os.path.join(get_sim_path(),file_name), "#nodes")
		print(cluster_ip)
		inp_update_token_value(os.path.join(get_cluster_path(),"node_list.inp"),"#node_list",cluster_ip)

	def install_to_cluster(self):
		self.get_config()
		self.write_cluster_config()

		if len(self.cluster_dir)<2:
			return

		cpy_src="rsync -avh --delete -e ssh "+get_cluster_path()+"/ "+self.user_name+"@"+self.ip+":"+self.cluster_dir+"/"

		copy_to_nodes="ssh -n -f "+self.user_name+"@"+self.ip+" \"sh -c \'cd "+self.cluster_dir+"/; ./install.sh\'\""
		command=cpy_src+";"+copy_to_nodes
		print(command)

		os.system(command)

	def boot_cluster(self):
		self.get_config()
		command="ssh -n -f "+self.user_name+"@"+self.ip+" \"sh -c \'cd "+self.cluster_dir+"; ./boot.sh\'\""
		print(command)

		os.system(command)

	def kill_cluster(self):
		self.get_config()
		command="ssh -n -f "+self.user_name+"@"+self.ip+" \"sh -c \'cd "+self.cluster_dir+"; ./kill.sh\'\""
		print(command)

		os.system(command)

	def remove_from_cluster(self):
		self.get_config()
		command="ssh -n -f "+self.user_name+"@"+self.ip+" \"sh -c \'cd "+self.cluster_dir+"; ./remove.sh\'\""
		os.system(command)

	def callback_help(self,widget):
		webbrowser.open('http://www.gpvdm.com/man/index.html')

	def callback_add_page(self,new_filename):
		self.add_page(new_filename)

	def add_page(self,file_name):
		name=inp_get_token_value(file_name, "#tab_name")
		tab=tab_class(file_name)
		self.notebook.addTab(tab,name)


