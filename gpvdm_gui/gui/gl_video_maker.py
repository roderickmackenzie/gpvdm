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

## @package gl_video_maker
#  The main tab class, used to display material properties.
#

import os

from token_lib import tokens
from undo import undo_list_class
from tab_base import tab_base
from str2bool import str2bool
from util import latex_to_html
from help import help_window

from PyQt5.QtCore import pyqtSignal

from PyQt5.QtWidgets import QTextEdit,QWidget, QScrollArea,QVBoxLayout,QLabel,QHBoxLayout,QPushButton, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QGridLayout,QLineEdit, QToolBar,QAction
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QPixmap, QIcon

from icon_lib import icon_get

from PyQt5.QtCore import QTimer

import i18n
_ = i18n.language.gettext

import functools
from error_dlg import error_dlg

from json_viewer import json_viewer
from gpvdm_json import gpvdm_data
from json_gl import json_gl_view
from experiment import experiment
from tab_gl_video_maker import tab_gl_video_maker

class gl_video_maker(experiment):

	changed = pyqtSignal()

	def __init__(self,gl_widget):
		experiment.__init__(self,"tab_gl_video_maker",window_save_name="tab_gl_video_maker", window_title=_("Flyby video maker"),json_search_path="gpvdm_data().gl",icon="fly")

		self.tb_start = QAction(icon_get("fly"), _("Set\nposition"), self)
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_start)
		self.tb_start.triggered.connect(self.callback_set_pos)

		self.tb_goto = QAction(icon_get("fly"), _("Goto"), self)
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_goto)
		self.tb_goto.triggered.connect(self.callback_goto)

		self.tb_run = QAction(icon_get("forward"), _("Run"), self)
		self.ribbon.file.insertAction(self.ribbon.tb_rename,self.tb_run)
		self.tb_run.triggered.connect(self.callback_run)

		self.notebook.currentChanged.connect(self.switch_page)
		self.switch_page()
		self.gl_widget=gl_widget

	def callback_goto(self):
		tab = self.notebook.currentWidget()
		data=tab.get_json_obj()
		self.gl_widget.viewtarget.xRot=data.xRot
		self.gl_widget.viewtarget.yRot=data.yRot
		self.gl_widget.viewtarget.zRot=data.zRot
		self.gl_widget.viewtarget.x_pos=data.x_pos
		self.gl_widget.viewtarget.y_pos=data.y_pos
		self.gl_widget.viewtarget.zoom=data.zoom

		self.gl_widget.timer=QTimer()
		self.gl_widget.timer.timeout.connect(self.gl_widget.ftimer_target)
		self.gl_widget.timer.start(25)

	def switch_page(self):
		tab = self.notebook.currentWidget()
		#self.tb_lasers.update(tab.data)


	def set_edit(self,editable):
		self.tab.editable=editable

	def callback_set_pos(self):
		tab = self.notebook.currentWidget()
		data=tab.get_json_obj()
		data.xRot=self.gl_widget.views[0].xRot
		data.yRot=self.gl_widget.views[0].yRot
		data.zRot=self.gl_widget.views[0].zRot
		data.x_pos=self.gl_widget.views[0].x_pos
		data.y_pos=self.gl_widget.views[0].y_pos
		data.zoom=self.gl_widget.views[0].zoom
		gpvdm_data().save()
		tab.tab.tab.update_values()


	def callback_run(self):
		data=gpvdm_data()
		if (self.gl_widget.width() % 2) != 0:
			print("window width not divisible by two")
			return
		if (self.gl_widget.height() % 2) != 0:
			print("window height not divisible by two")
			return

		self.gl_widget.views[0].xRot=data.gl.segments[0].xRot
		self.gl_widget.views[0].yRot=data.gl.segments[0].yRot
		self.gl_widget.views[0].zRot=data.gl.segments[0].zRot
		self.gl_widget.views[0].x_pos=data.gl.segments[0].x_pos
		self.gl_widget.views[0].y_pos=data.gl.segments[0].y_pos
		self.gl_widget.views[0].zoom=data.gl.segments[0].zoom
		self.gl_widget.views[0].max_angle_shift=1.0

		self.next=1
		self.set_next_target()

	def set_next_target(self):
		data=gpvdm_data()
		if self.next<len(data.gl.segments):

			self.gl_widget.viewtarget.xRot=data.gl.segments[self.next].xRot
			self.gl_widget.viewtarget.yRot=data.gl.segments[self.next].yRot
			self.gl_widget.viewtarget.zRot=data.gl.segments[self.next].zRot
			self.gl_widget.viewtarget.x_pos=data.gl.segments[self.next].x_pos
			self.gl_widget.viewtarget.y_pos=data.gl.segments[self.next].y_pos
			self.gl_widget.viewtarget.zoom=data.gl.segments[self.next].zoom
			self.gl_widget.viewtarget.max_angle_shift=1.0
			self.next=self.next+1

			self.gl_widget.timer_save_files=True
			self.gl_widget.timer_end_callback=self.callback_timer_end
			self.gl_widget.timer=QTimer()
			self.gl_widget.timer.timeout.connect(self.gl_widget.ftimer_target)
			self.gl_widget.timer.start(250)
			return True
		return False

	def callback_timer_end(self):
		print(self.gl_widget.timer_save_files_number)
		if self.gl_widget.timer_save_files_number==0:
			return

		if self.set_next_target()==True:
			print("oh")
			return

		out_file_name=os.path.join(os.getcwd(),"flyby","movie.mp4")
		files_list_path=os.path.join(os.getcwd(),"flyby","files.txt")
		f=open(files_list_path,"w")
		for i in range(0,self.gl_widget.timer_save_files_number):
			
			f.write(os.path.join(os.getcwd(),"flyby",str(i)+".jpg"+"\n"))
		f.close()
		
		fps=int(float(self.gl_widget.timer_save_files_number)/5.0)
		encode_line="mencoder mf://@"+files_list_path+" -mf type=jpg:fps="+str(fps)+" -o "+out_file_name+" -ovc x264"
		encode_line2="\nffmpeg -i movie.mp4 -r 15 -vf scale=-1:-1 -ss 00:00:1 movie.gif"

		f=open(os.path.join(os.getcwd(),"flyby","encode.sh"),"w")
		f.write(encode_line+encode_line2)
		f.close()

		os.system(encode_line)

		self.gl_widget.timer_save_files=False
		self.gl_widget.timer_save_files_number=0
