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

## @package window_ml
#  ML editor
#

import os
from gui_util import dlg_get_text
import webbrowser

from global_objects import global_object_get
from icon_lib import icon_get
from global_objects import global_object_register

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from QWidgetSavePos import QWidgetSavePos
from progress_class import progress_class
from process_events import process_events
from global_objects import global_object_run
from experiment import experiment
from play import play
from server import server_get
from cal_path import gpvdm_paths
from QAction_lock import QAction_lock
from gpvdm_json import gpvdm_data
from tab import tab_class
from config_window import class_config_window
from threading import Thread
import codecs
from clone import clone_sim_dir
from clean_sim import clean_sim_dir
from search import find_sims
from clean_sim import ask_to_delete
from inp import inp
from scan_human_labels import set_json_from_human_path
import json
from gpvdm_json import all_gpvdm_data
from scan_tree import random_log
import random
from scan_io import scan_archive
from ml_vectors import ml_vectors

class window_ml(experiment):


	def __init__(self):
		experiment.__init__(self,window_save_name="window_ml", window_title=_("Machine learning"),name_of_tab_class="tab_ml",json_search_path="gpvdm_data().ml")

		self.tb_configure= QAction_lock("cog", _("Configure"), self,"ribbon_configure")
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_configure)
		self.tb_configure.clicked.connect(self.callback_configure)

		self.tb_build_vectors= QAction_lock("matrix", _("Build\nvectors"), self,"ribbon_configure")
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_build_vectors)
		self.tb_build_vectors.clicked.connect(self.callback_build_vectors)

		self.run = play(self,"ml_ribbon_run",run_text=_("Run\nGenerator"),connect_to_server=False)
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.run)
		self.run.start_sim.connect(self.callback_run)


		self.notebook.currentChanged.connect(self.switch_page)
		self.my_server=server_get()
		self.my_server.sim_finished.connect(self.callback_sim_finished)
		self.switch_page()

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)


	def callback_run(self):
		self.n=0
		tab = self.notebook.currentWidget()
		self.obj=gpvdm_data().ml.find_object_by_id(tab.uid)

		self.ml_dir=os.path.join(gpvdm_paths.get_sim_path(),self.obj.english_name)
		dirs_to_del=[]
		for dir_name in os.listdir(self.ml_dir):
			full_path=os.path.join(self.ml_dir,dir_name)
			if os.path.isdir(full_path):
				dirs_to_del.append(full_path)


		ask_to_delete(self,dirs_to_del,interactive=True)
		self.run.start()

		self.running=False
		self.timer=QTimer()
		self.timer.timeout.connect(self.callback_timer)
		self.timer.start(10)

	def callback_timer(self):
		if self.running==False:
			self.running=True
			self.n=self.n+1
			for n in range(0,self.obj.ml_config.ml_sims_per_archive):
				random_file_name=codecs.encode(os.urandom(int(16 / 2)), 'hex').decode()
				cur_dir=os.path.join(self.ml_dir,random_file_name)

				for sim in self.obj.ml_sims.segments:
					sub_sim_dir=os.path.join(cur_dir,sim.sim_name)
					clone_sim_dir(sub_sim_dir,gpvdm_paths.get_sim_path())
					clean_sim_dir(sub_sim_dir)

					data=all_gpvdm_data()
					data.load(os.path.join(sub_sim_dir,"sim.json"))

					for patch_item in self.obj.ml_patch.segments:
						if patch_item.ml_patch_enabled:
							data.set_val(patch_item.json_var,patch_item.ml_patch_val)

					for patch_item in sim.ml_patch.segments:
						if patch_item.ml_patch_enabled:
							data.set_val(patch_item.json_var,patch_item.ml_patch_val)

					for random_item in self.obj.ml_random.segments:
						if random_item.random_var_enabled:
							if random_item.random_distribution=="log":
								val=float(random_log(random_item.min,random_item.max))
							else:
								val=random.uniform(random_item.min, random_item.max)
							data.set_val(random_item.json_var,val)
							
					data.save()
					self.my_server.add_job(sub_sim_dir,"")

			self.my_server.start()

	def callback_sim_finished(self):
		self.my_server.remove_debug_info()
		self.my_server.clear_jobs()
		scan_archive(self.ml_dir)
		if self.n>self.obj.ml_config.ml_number_of_archives:
			self.timer.stop()
			self.run.stop()
		self.running=False

	def callback_configure(self):
		tab = self.notebook.currentWidget()
		obj=gpvdm_data().ml.find_object_by_id(tab.uid)

		self.config_window=class_config_window([obj.ml_config],[_("Config")])
		self.config_window.show()

	def callback_build_vectors(self):
		tab = self.notebook.currentWidget()
		obj=gpvdm_data().ml.find_object_by_id(tab.uid)

		self.ml_dir=os.path.join(gpvdm_paths.get_sim_path(),obj.english_name)

		scan=ml_vectors()
		scan.build_vector(self.ml_dir)

