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

class gl_video_maker(QWidget,tab_base):

	changed = pyqtSignal()

	def __init__(self,gl_widget):
		QWidget.__init__(self)
		self.gl_widget=gl_widget
		self.main_box_widget=QWidget()
		self.vbox=QVBoxLayout()
		self.setWindowTitle("Flyby video maker (https://www.gpvdm.com)")
		self.setWindowIcon(icon_get("fly"))	
		#toolbar
		self.toolbar=QToolBar()
		self.toolbar.setToolButtonStyle( Qt.ToolButtonTextUnderIcon)
		self.toolbar.setIconSize(QSize(42, 42))

		self.tb_start = QAction(icon_get("fly_a"), _("Set start\nposition"), self)
		self.toolbar.addAction(self.tb_start)
		self.tb_start.triggered.connect(self.callback_set_start_pos)

		self.tb_stop = QAction(icon_get("fly_b"), _("Set stop\nposition"), self)
		self.toolbar.addAction(self.tb_stop)
		self.tb_stop.triggered.connect(self.callback_set_stop_pos)

		self.tb_run = QAction(icon_get("forward"), _("Run"), self)
		self.toolbar.addAction(self.tb_run)
		self.tb_run.triggered.connect(self.callback_run)

		self.vbox.addWidget(self.toolbar)

		if len(gpvdm_data().gl.segments)<2:
			gpvdm_data().gl.segments=[]
			gpvdm_data().gl.segments.append(json_gl_view())
			gpvdm_data().gl.segments.append(json_gl_view())

		#tab0
		self.label0=QLabel(_("Start position"))
		self.vbox.addWidget(self.label0)
		self.tab0=json_viewer()
		self.tab0.populate(gpvdm_data().gl.segments[0])
		self.vbox.addWidget(self.tab0)

		#tab1
		self.label1=QLabel(_("Stop position"))
		self.vbox.addWidget(self.label1)
		self.tab1=json_viewer()
		self.tab1.populate(gpvdm_data().gl.segments[1])
		self.vbox.addWidget(self.tab1)

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.vbox.addWidget(spacer)
		self.main_box_widget.setLayout(self.vbox)

		self.icon_widget=QWidget()
		self.icon_widget_vbox=QVBoxLayout()
		self.icon_widget.setLayout(self.icon_widget_vbox)

		
		self.setLayout(self.vbox)

		self.tab0.changed.connect(self.callback_edit)
		self.tab1.changed.connect(self.callback_edit)

	def callback_edit(self):
		gpvdm_data().save()

		self.changed.emit()

	def help(self):
		help_window().get_help(self.file_name)

	def set_edit(self,editable):
		self.tab.editable=editable

	def callback_set_start_pos(self):
		data=gpvdm_data()
		data.gl.segments[0].xRot=self.gl_widget.views[0].xRot
		data.gl.segments[0].yRot=self.gl_widget.views[0].yRot
		data.gl.segments[0].zRot=self.gl_widget.views[0].zRot
		data.gl.segments[0].x_pos=self.gl_widget.views[0].x_pos
		data.gl.segments[0].y_pos=self.gl_widget.views[0].y_pos
		data.gl.segments[0].zoom=self.gl_widget.views[0].zoom
		data.save()
		self.tab0.update_values()

	def callback_set_stop_pos(self):
		data=gpvdm_data()
		data.gl.segments[1].xRot=self.gl_widget.views[0].xRot
		data.gl.segments[1].yRot=self.gl_widget.views[0].yRot
		data.gl.segments[1].zRot=self.gl_widget.views[0].zRot
		data.gl.segments[1].x_pos=self.gl_widget.views[0].x_pos
		data.gl.segments[1].y_pos=self.gl_widget.views[0].y_pos
		data.gl.segments[1].zoom=self.gl_widget.views[0].zoom
		data.save()
		self.tab1.update_values()

	def callback_run(self):
		data=gpvdm_data()
		if (self.gl_widget.width() % 2) != 0:
			return
		self.gl_widget.views[0].xRot=data.gl.segments[0].xRot
		self.gl_widget.views[0].yRot=data.gl.segments[0].yRot
		self.gl_widget.views[0].zRot=data.gl.segments[0].zRot
		self.gl_widget.views[0].x_pos=data.gl.segments[0].x_pos
		self.gl_widget.views[0].y_pos=data.gl.segments[0].y_pos
		self.gl_widget.views[0].zoom=data.gl.segments[0].zoom
		self.gl_widget.views[0].max_angle_shift=1.0

		self.gl_widget.viewtarget.xRot=data.gl.segments[1].xRot
		self.gl_widget.viewtarget.yRot=data.gl.segments[1].yRot
		self.gl_widget.viewtarget.zRot=data.gl.segments[1].zRot
		self.gl_widget.viewtarget.x_pos=data.gl.segments[1].x_pos
		self.gl_widget.viewtarget.y_pos=data.gl.segments[1].y_pos
		self.gl_widget.viewtarget.zoom=data.gl.segments[1].zoom
		self.gl_widget.viewtarget.max_angle_shift=1.0

		self.gl_widget.timer_save_files=True
		self.gl_widget.timer_end_callback=self.callback_timer_end
		self.gl_widget.timer=QTimer()
		self.gl_widget.timer.timeout.connect(self.gl_widget.ftimer_target)
		self.gl_widget.timer.start(250)

	def callback_timer_end(self):
		if self.gl_widget.timer_save_files_number==0:
			return
		out_file_name=os.path.join(os.getcwd(),"flyby","movie.mp4")
		files_list_path=os.path.join(os.getcwd(),"flyby","files.txt")
		f=open(files_list_path,"w")
		for i in range(0,self.gl_widget.timer_save_files_number):
			
			f.write(os.path.join(os.getcwd(),"flyby",str(i)+".jpg"+"\n"))
		f.close()
		
		fps=int(float(self.gl_widget.timer_save_files_number)/20.0)
		encode_line="mencoder mf://@"+files_list_path+" -mf type=jpg:fps="+str(fps)+" -o "+out_file_name+" -ovc x264"

		f=open(os.path.join(os.getcwd(),"flyby","encode.sh"),"w")
		f.write(encode_line)
		f.close()

		os.system(encode_line)
