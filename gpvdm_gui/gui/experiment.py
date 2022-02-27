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

## @package experiment
#  The main experiment window, used for configuring time domain experiments.
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
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QMenuBar,QStatusBar
from PyQt5.QtGui import QPainter,QIcon

#window
from time_domain_experiment_tab import time_domain_experiment_tab
from detectors_tab import detectors_tab
from tab_light_src import tab_light_src
from cluster_tab import cluster_tab

from QHTabBar import QHTabBar
from gui_util import yes_no_dlg
from PyQt5.QtCore import pyqtSignal
from util import wrap_text
from QWidgetSavePos import QWidgetSavePos
from css import css_apply

from progress_class import progress_class
from process_events import process_events

from ribbon_experiment import ribbon_experiment
from gpvdm_json import gpvdm_data
import copy
from global_objects import global_object_run
from cal_path import gpvdm_paths

class experiment(QWidgetSavePos):

	changed = pyqtSignal()

	def update(self):
		for i in range(0,self.notebook.count()):
			w=self.notebook.widget(i)
			w.update()


	def callback_save(self):
		tab = self.notebook.currentWidget()
		tab.image_save()


	def remove_invalid(self,input_name):
		return input_name.replace (" ", "_")


	def load_tabs(self):

		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		process_events()
		i=0

		exec("from "+self.name_of_tab_class+" import "+self.name_of_tab_class)

		data=self.get_json_obj()
		for sim in data.segments:
			tab=eval(self.name_of_tab_class+"(sim)")
			#tab=time_domain_experiment_tab(sim)
			tab.uid=sim.id
			self.notebook.addTab(tab,sim.english_name)

			progress_window.set_fraction(float(i)/float(len(data.segments)))
			progress_window.set_text(_("Loading")+" "+sim.english_name)
			process_events()
			i=i+1

		progress_window.stop()
		self.update_interface()

	def clear_pages(self):
		self.notebook.clear()

	def __init__(self,name_of_tab_class,window_save_name="time_domain_experiment", window_title=_("Time domain experiment window"),json_search_path=None,icon="icon"):
		QWidgetSavePos.__init__(self,window_save_name)
		self.main_vbox = QVBoxLayout()
		self.json_search_path=json_search_path
		self.name_of_tab_class=name_of_tab_class
		self.setMinimumSize(800, 700)
		self.setWindowTitle(window_title+" (https://www.gpvdm.com)")
		self.setWindowIcon(icon_get(icon))

		self.ribbon=ribbon_experiment()
		#self.ribbon.tb_save.triggered.connect(self.callback_save)

		#self.ribbon.tb_laser_start_time.triggered.connect(self.callback_laser_start_time)

		#self.ribbon.tb_start.triggered.connect(self.callback_start_time)


		self.main_vbox.addWidget(self.ribbon)


		self.notebook = QTabWidget()
		css_apply(self.notebook ,"style_h.css")
		self.tab_bar=QHTabBar()
		self.notebook.setTabBar(self.tab_bar)
		self.notebook.setTabPosition(QTabWidget.West)
		self.notebook.setMovable(True)

		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)
		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)

		self.main_vbox.addWidget(self.notebook)

		self.status_bar=QStatusBar()
		self.main_vbox.addWidget(self.status_bar)

		self.setLayout(self.main_vbox)

		self.load_tabs()

		self.ribbon.tb_clone.triggered.connect(self.callback_clone_page)
		self.ribbon.tb_rename.triggered.connect(self.callback_rename_page)
		self.tab_bar.rename.connect(self.callback_rename_page)
		self.ribbon.tb_delete.triggered.connect(self.callback_delete_page)
		self.tab_bar.delete.connect(self.callback_delete_page)
		self.ribbon.tb_new.triggered.connect(self.callback_add_page)

		self.notebook.currentChanged.connect(self.update_interface)

		self.tab_bar.paste.connect(self.do_paste)
		self.tab_bar.tabMoved.connect(self.callback_tab_moved)

	def callback_rename_page(self):
		tab = self.notebook.currentWidget()
		data=gpvdm_data()
		new_sim_name=dlg_get_text( _("Rename:"), tab.get_json_obj().english_name,"rename.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			
			tab.get_json_obj().english_name=new_sim_name
			self.notebook.setTabText(self.notebook.currentIndex(), new_sim_name)
			data.save()
			global_object_run("ribbon_sim_mode_update")

	def callback_clone_page(self):
		tab = self.notebook.currentWidget()
		data=gpvdm_data()
		new_sim_name=dlg_get_text( _("Clone:"), tab.get_json_obj().english_name+"_new","clone.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:
			
			a=copy.deepcopy(tab.get_json_obj())
			a.english_name=new_sim_name
			data=self.get_json_obj()
			data.segments.append(a)
			tab=eval(self.name_of_tab_class+"(tab.get_json_obj().segments[-1])")
			#tab=time_domain_experiment_tab(data.segments[-1])
			self.notebook.addTab(tab,new_sim_name)
			gpvdm_data().save()
			global_object_run("ribbon_sim_mode_update")

	def callback_delete_page(self):
		data=self.get_json_obj()
		tab = self.notebook.currentWidget()
		obj=data.find_object_by_id(tab.uid)
		response=yes_no_dlg(self,_("Are you sure you want to delete : ")+tab.get_json_obj().english_name)
		if response == True:
			index=self.notebook.currentIndex()
			data.segments.remove(obj)
			self.notebook.removeTab(index)
			gpvdm_data().save()
			global_object_run("ribbon_sim_mode_update")

	def callback_add_page(self):
		data=gpvdm_data()
		tab = self.notebook.currentWidget()
		if tab!=None:
			new_name=tab.get_json_obj().english_name+"_new"
		else:
			new_name="new"

		new_sim_name=dlg_get_text( _("Make a new:"), new_name,"document-new.png")

		new_sim_name=new_sim_name.ret

		if new_sim_name!=None:

			exec("from "+self.name_of_tab_class+" import "+self.name_of_tab_class)

			data=self.get_json_obj()
			if data.segment_examples[0]==None:
				dfasdasd

			a=copy.deepcopy(data.segment_examples[0])
			a.english_name=new_sim_name
			a.update_random_ids()
			data.segments.append(a)
			tab=eval(self.name_of_tab_class+"(data.segments[-1])")
			tab.uid=a.id
			self.notebook.addTab(tab,new_sim_name)
			gpvdm_data().save()
			global_object_run("ribbon_sim_mode_update")

	def update_interface(self):
		data=self.get_json_obj()
		tab = self.notebook.currentWidget()
		if tab!=None:
			self.status_bar.showMessage(gpvdm_paths.get_sim_path()+","+tab.get_json_obj().english_name+", segment"+str(data.segments.index(tab.get_json_obj())))
			self.tab_bar.obj_search_path=self.json_search_path
			self.tab_bar.obj_id=tab.uid
			self.ribbon.tb_clone.setEnabled(True)
			self.ribbon.tb_rename.setEnabled(True)
			self.ribbon.tb_delete.setEnabled(True)
		else:
			self.ribbon.tb_clone.setEnabled(False)
			self.ribbon.tb_rename.setEnabled(False)
			self.ribbon.tb_delete.setEnabled(False)

	def get_json_obj(self):
		return eval(self.json_search_path)

	def do_paste(self):
		exec("from "+self.name_of_tab_class+" import "+self.name_of_tab_class)
		data=gpvdm_data()
		data=self.get_json_obj()
		a=copy.deepcopy(data.segment_examples[0])
		a.load_from_json(self.tab_bar.paste_data)
		a.update_random_ids()
		data.segments.append(a)
		tab=eval(self.name_of_tab_class+"(data.segments[-1])")
		tab.uid=a.id
		self.notebook.addTab(tab,data.segments[-1].english_name)

	def callback_tab_moved(self,from_pos,to_pos):
		data=gpvdm_data()
		segments=self.get_json_obj().segments
		segments.insert(to_pos, segments.pop(from_pos))
		data.save()

