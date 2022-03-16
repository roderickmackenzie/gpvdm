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
from inp import inp_get_token_value_from_list
from inp import inp_update_token_value
from cal_path import get_sim_path

from QWidgetSavePos import QWidgetSavePos

from css import css_apply

from cal_path import get_cluster_path

import i18n
_ = i18n.language.gettext

import random

from experiment import experiment
from gpvdm_local import gpvdm_local
from gpvdm_json import gpvdm_data

class cluster_config_window(experiment):


	def __init__(self,data=None):
		data=gpvdm_local()
		
		experiment.__init__(self,window_save_name="cluster_window", window_title=_("Configure")+" (https://www.gpvdm.com)",name_of_tab_class="cluster_tab",json_search_path="gpvdm_data().cluster",icon="preferences-system")

		self.ribbon.addTab(self.cluser_ribbon_ssh(),_("SSH"))

		self.tb_install.triggered.connect(self.install_to_cluster)

		self.tb_boot.triggered.connect(self.boot_cluster)
		self.tb_generate_keys.triggered.connect(self.generate_keys)

		self.tb_stop.triggered.connect(self.kill_cluster)
		self.tb_remove.triggered.connect(self.remove_from_cluster)

		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()


	def cluser_ribbon_ssh(self):
		toolbar = QToolBar()
		toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		toolbar.setIconSize(QSize(42, 42))


		self.tb_generate_keys = QAction(icon_get("gnome-dialog-password"), _("Generate keys"), self)
		self.tb_generate_keys.setStatusTip(_("Generate keys"))
		toolbar.addAction(self.tb_generate_keys)

		self.tb_install = QAction(icon_get("install-to-cluster"), _("Install to cluster"), self)
		self.tb_install.setStatusTip(_("Install to cluster"))
		toolbar.addAction(self.tb_install)

		self.tb_boot = QAction(icon_get("run-cluster"), _("Boot cluster"), self)
		self.tb_boot.setStatusTip(_("Boot cluster"))
		toolbar.addAction(self.tb_boot)

		self.tb_stop = QAction(icon_get("stop-cluster"), _("Stop cluster"), self)
		self.tb_stop.setStatusTip(_("Stop cluster"))
		toolbar.addAction(self.tb_stop)

		self.tb_remove = QAction(icon_get("remove-from-cluster"), _("Remove from cluster"), self)
		self.tb_remove.setStatusTip(_("Remove from cluster"))
		toolbar.addAction(self.tb_remove)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		toolbar.addWidget(spacer)

		self.tb_home_help = QAction(icon_get("internet-web-browser"), _("Help"), self)
		toolbar.addAction(self.tb_home_help)

		return toolbar

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)

	def generate_keys(self):
		data=gpvdm_local()

		tab = self.notebook.currentWidget()

		iv = random.getrandbits(128)
		iv="%032x" % iv
		tab.data.config.cluster_iv=iv

		key = random.getrandbits(128)
		key="%032x" % key
		tab.data.config.cluster_key=key
		tab.tab.tab.update_values()
		data.save()

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




