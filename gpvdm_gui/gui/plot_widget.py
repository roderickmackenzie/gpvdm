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

## @package plot_widget
#  The main plot widget.
#

from __future__ import unicode_literals

import os
import io
from numpy import *


#matplotlib
import matplotlib
from matplotlib import cm
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.pyplot as plt
from matplotlib.pyplot import colorbar
from matplotlib.colors import LogNorm

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QApplication
from PyQt5.QtGui import QPainter,QIcon,QImage

#calpath
from icon_lib import icon_get
from open_save_dlg import save_as_image
from open_save_dlg import save_as_filter

from dat_file import dat_file


from dlg_get_multi_text import dlg_get_multi_text

from mpl_toolkits.mplot3d import Axes3D

from plot_ribbon import plot_ribbon
from lock import get_lock
from dat_files_to_gnuplot import dat_files_to_gnuplot_files
from dat_files_to_gnuplot import dat_files_to_gnuplot

from dat_files_to_csv import dat_files_to_csv
from util_latex import latex

from plot_widget_menu import plot_widget_menu
from plot_widget_matplotlib import plot_widget_matplotlib
from band_graph2 import band_graph2

class plot_widget(plot_widget_menu,plot_widget_matplotlib):


	def keyPressEvent(self, event):
		keyname=event.key()
		if (keyname>64 and keyname<91 ) or (keyname>96 and keyname<123):
			modifiers = event.modifiers()
			keyname=chr(keyname)
			if keyname.isalpha()==True:
				if Qt.ShiftModifier == modifiers:
					keyname=keyname.upper()
				else:
					keyname=keyname.lower()
		else:
			return


		if keyname=="a":
			self.do_plot()
		elif keyname=="q":
			self.destroy()
		elif  modifiers == Qt.ControlModifier and keyname=='c':
			if get_lock().is_trial()==False:
				self.callback_do_clip()

		if len(self.data)>0:
			if keyname=='g':
				if self.data[0].grid==False:
					for i in range(0,len(self.ax)):
						self.ax[i].grid(True)
					self.data[0].grid=True
				else:
					for i in range(0,len(self.ax)):
						self.ax[i].grid(False)
					self.data[0].grid=False
			elif keyname=="r":
				if self.lx==None:
					for i in range(0,len(self.ax)):
						self.lx = self.ax[i].axhline(color='k')
						self.ly = self.ax[i].axvline(color='k')
				self.lx.set_ydata(self.ydata)
				self.ly.set_xdata(self.xdata)

			elif keyname=="l":
				if self.data[0].logdata==True:
					self.data[0].logdata=False
					for i in range(0,len(self.ax)):
						self.ax[i].set_yscale("linear")
				else:
					self.data[0].logdata=True
					for i in range(0,len(self.ax)):
						self.ax[i].set_yscale("log")

			elif keyname=="L":
				if self.data[0].logy==True:
					self.data[0].logy=False
					for i in range(0,len(self.ax)):
						self.ax[i].set_xscale("linear")
				else:
					self.data[0].logy=True
					for i in range(0,len(self.ax)):
						self.ax[i].set_xscale("log")


			self.fig.canvas.draw()

	def contextMenuEvent(self,event):
		if self.open_gl_enabled==True:
			try:
				event.reject()
			except:
				pass
			#self.gl_plot.main_menu.exec_(event.globalPos())
		else:
			self.main_menu.exec_(event.globalPos())

	def callback_do_clip(self):
		buf = io.BytesIO()
		self.fig.savefig(buf)
		QApplication.clipboard().setImage(QImage.fromData(buf.getvalue()))
		buf.close()


	def mouse_move(self,event):
		self.xdata=event.xdata
		self.ydata=event.ydata

	def log_3d_workaround(self):
		if self.done_log==False:
			my_max,my_min=self.data[0].max_min()
			for i in range(0,len(self.data)):
				if self.data[i].logdata==True:
					my_max,my_min=self.data[i].max_min(cur_min=my_min,cur_max=my_max)

			zticks=[]
			zticks_txt=[]
			pos=pow(10,floor(log10(abs(my_min))))

			if my_min>0:
				while (pos<my_max):
					pos=pos*10
					zticks.append(pos)
					zticks_txt.append("{:.0e}".format(pos))

			if (len(zticks)>5):
				delta=int(len(zticks)/5)
				pos=0
				rebuild=[]
				rebuild_txt=[]
				while(pos<len(zticks)):
					rebuild.append(zticks[pos])
					rebuild_txt.append(zticks_txt[pos])
					pos=pos+delta
				zticks=rebuild
				zticks_txt=rebuild_txt


			changed=False
			for i in range(0,len(self.data)):
				if self.data[i].y_len>1 and self.data[i].x_len>1 and self.data[i].logdata==True:
						changed=True
						for x in range(0,self.data[i].x_len):
							for y in range(0,self.data[i].y_len):
								for z in range(0,self.data[i].z_len):
									if self.data[i].data[z][x][y]!=0:
										self.data[i].data[z][x][y]=log10(abs(self.data[i].data[z][x][y]))
			
			if changed==True and self.fix_scales==False:
				self.ax[0].set_zticks(log10(zticks))
				self.ax[0].set_zticklabels(zticks_txt)
		self.done_log=True



	def do_plot(self):
		if self.open_gl_enabled==True:
			self.gl_plot.graph_data=self.data
			self.gl_plot.force_redraw()
		else:
			if self.widget_mode=="band_graph" or self.widget_mode=="gpvdm_graph":
				self.canvas.set_data_file(self.input_files[0])
				self.canvas.draw_graph()
			else:
				self.matplotlib_do_plot()
			

	def save_image(self,file_name):
		if self.open_gl_enabled==False:
			self.fig.savefig(file_name)
		else:
			self.gl_plot.grabFrameBuffer().save(file_name)

	def callback_save_image(self):
		file_name=save_as_image(self)
		if file_name != None:
			self.save_image(file_name)

	def callback_save_csv(self):
		file_name=save_as_filter(self,"*.csv")
		if file_name != None:
			dat_files_to_csv(file_name,self.data)

	def callback_save_xlsx(self):
		from dat_files_to_excel import dat_files_to_excel
		file_name=save_as_filter(self,"*.xlsx")
		if file_name != None:
			dat_files_to_excel(file_name,self.data)

	def callback_save_txt(self):
		file_name=save_as_filter(self,"*.txt")
		if file_name != None:
			self.data[0].save_as_txt(file_name)

	def callback_save_gnuplot(self):
		file_name=save_as_filter(self,"gnuplot (*.)")
		if file_name != None:
			dat_files_to_gnuplot(file_name,self.data)
			#dat_files_to_gnuplot_files(file_name,self.data)

	def set_labels(self,labels):
		if len(self.data)!=len(labels):
			#print("oops",len(self.data),len(labels))
			return

		for i in range(0,len(self.data)):
			self.data[i].key_text=labels[i]

	def set_plot_types(self,plot_types):
		if len(self.data)!=len(plot_types):
			#print("oops",len(self.data),len(plot_types))
			return

		for i in range(0,len(self.data)):
			#print("setting",plot_types[i])
			self.data[i].plot_type=plot_types[i]

	def reload(self):
		self.data=[]
		self.done_log=False

		
		for i in range(0,len(self.input_files)):
			dat=dat_file()
			ret=dat.load(self.input_files[i])
			if self.base_file_names!=None:
				sub=dat_file()
				sub.load(self.base_file_names[i])
				dat=dat-sub

			if ret!=False:
				#dat.chop_y(0,-3)
				self.data.append(dat)
				self.y1_y2.append("y1")

		if self.open_gl_enabled==False:
			self.matplotlib_norm_data()

	def load_data(self,input_files):
		self.lx=None
		self.ly=None
		self.input_files=input_files

		if len(input_files)==0:
			print(_("No files were given to load"))
			return

		self.reload()



	def callback_black(self):
		if len(self.data)>0:
			self.do_plot()

	def callback_rainbow(self):
		if len(self.data)>0:
			self.do_plot()


	def callback_key(self):
		if len(self.data)>0:
			self.data[0].legend_pos=widget.get_label()
			self.do_plot()

	def callback_units(self):
		if len(self.data)>0:
			units=dlg_get_text( "Units:", self.data[0].key_units)
			if units!=None:
				self.data[0].key_units=units
			self.do_plot()


	def callback_autoscale_y(self):
		self.fix_scales=not self.fix_scales
		if self.fix_scales==True:
			my_max,my_min=self.data[0].max_min()
			for i in range(0,len(self.data)):
				my_max,my_min=self.data[i].max_min(cur_min=my_min,cur_max=my_max)
			self.force_data_max=my_max
			self.force_data_min=my_min
			self.do_plot()
		else:
			self.force_data_max=False
			self.force_data_min=False

	def callback_math_opp(self,opp):
		for i in range(0,len(self.data)):
			if opp=="abs":
				self.data[i].abs()

		self.do_plot()

	def callback_norm_to_peak_of_all_data(self):
		if len(self.data)>0:
			self.data[0].norm_to_peak_of_all_data=not self.data[0].norm_to_peak_of_all_data
			self.matplotlib_norm_data()
			self.do_plot()

	def callback_toggle_log_scale_y(self):
		for i in range(0,len(self.data)):
			self.data[i].logdata=not self.data[i].logdata
		self.do_plot()

	def callback_toggle_log_scale_x(self):
		if len(self.data)>0:
			self.data[0].logy=not self.data[0].logy
			self.do_plot()

	def callback_toggle_label_data(self):
		for i in range(0,len(self.data)):
			self.data[i].label_data=not self.data[i].label_data
		self.do_plot()

	def callback_set_heat_map(self):
		if len(self.data)>0:
			self.data[0].type="heat"
			self.do_plot()

	def callback_heat_map_edit(self):
		ret = dlg_get_multi_text([["x start",str(self.data[0].x_start)],["x stop",str(self.data[0].x_stop)],["x points",str(self.data[0].x_points)],["y start",str(self.data[0].y_start)],["y stop",str(self.data[0].y_stop)],["y points",str(self.data[0].y_points)]],title="2D plot editor")
		ret.run()
		ret=ret.get_values()
		if ret!=False:
			[a,b,c,d,e,f] = ret

			self.data[0].x_start=float(a)
			self.data[0].x_stop=float(b)
			self.data[0].x_points=float(c)

			self.data[0].y_start=float(d)
			self.data[0].y_stop=float(e)
			self.data[0].y_points=float(f)

			self.do_plot()


	def callback_toggle_invert_y(self):
		if len(self.data)>0:
			self.data[0].invert_y=not self.data[0].invert_y
			self.matplotlib_norm_data()
			self.do_plot()

	def callback_toggle_subtract_first_point(self):
		if len(self.data)>0:
			self.data[0].subtract_first_point=not self.data[0].subtract_first_point
			self.matplotlib_norm_data()
			self.do_plot()

	def callback_toggle_add_min(self):
		if len(self.data)>0:
			self.data[0].add_min=not self.data[0].add_min
			self.matplotlib_norm_data()
			self.do_plot()

	def update(self):
		self.load_data(self.input_files)
		self.do_plot()

	def build_toolbar(self,enable_3d):
		ribbon=plot_ribbon()
		ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)

		#self.ribbon.tb_export_as_jpg.triggered.connect(self.save_image)

		if enable_3d==True:
			self.tb_3d_mode = QAction(icon_get("vector"), _("3D\nMode"), self)
			self.tb_3d_mode.setCheckable(True)
			self.tb_3d_mode.triggered.connect(self.callback_3d_mode)
			ribbon.plot_toolbar.addAction(self.tb_3d_mode)

		if self.widget_mode=="matplotlib":
			self.matplotlib_nav_bar=NavigationToolbar(self.canvas,self)
			actions = self.matplotlib_nav_bar.findChildren(QAction)
			for a in actions:
				if a.text() == 'Save':
					self.matplotlib_nav_bar.removeAction(a)
					break

			self.matplotlib_nav_bar.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
			self.matplotlib_nav_bar.setIconSize(QSize(42, 42))
			ribbon.plot_toolbar.addWidget(self.matplotlib_nav_bar)



			self.fig.canvas.mpl_connect('motion_notify_event', self.mouse_move)

			ribbon.tb_color_black.triggered.connect(self.callback_black)
			ribbon.tb_color_rainbow.triggered.connect(self.callback_rainbow)

			ribbon.tb_scale_autoscale.triggered.connect(self.callback_autoscale_y)
			ribbon.tb_scale_log_y.triggered.connect(self.callback_toggle_log_scale_y)
			ribbon.tb_scale_log_x.triggered.connect(self.callback_toggle_log_scale_x)

			#new way to do math
			for o in ribbon.math_opps:
				o[0].triggered.connect(lambda: self.callback_math_opp(o[1]))

			#old way to do math
			ribbon.math_norm_to_peak_of_all_data.triggered.connect(self.callback_norm_to_peak_of_all_data)
			ribbon.math_heat_map.triggered.connect(self.callback_set_heat_map)
			ribbon.math_heat_map_edit.triggered.connect(self.callback_heat_map_edit)
			ribbon.math_subtract_first_point.triggered.connect(self.callback_toggle_subtract_first_point)
			ribbon.math_add_min_point.triggered.connect(self.callback_toggle_add_min)
			ribbon.math_invert_y_axis.triggered.connect(self.callback_toggle_invert_y)

		return ribbon
	#modes="matplotlib","gpvdm_graph", "band_graph"

	def __init__(self,enable_toolbar=True,enable_3d=True,widget_mode="matplotlib",force_2d3d=False):
		QWidget.__init__(self)
		self.setFocusPolicy(Qt.StrongFocus)
		self.data=[]
		self.input_files=[]
		self.show_title=True
		self.show_key=True
		self.force_data_max=False
		self.force_data_min=False
		self.fix_scales=False
		self.y1_y2=[]
		self.setWindowIcon(icon_get("plot"))
		self.force_2d3d=force_2d3d
		self.base_file_names=None
		self.ax=[]
		self.last_plot=[]

		self.main_vbox = QVBoxLayout()
		self.fig = Figure(figsize=(2.5,2), dpi=100)

		self.widget_mode=widget_mode
		if self.widget_mode=="band_graph":
			self.canvas = band_graph2()
		elif self.widget_mode=="gpvdm_graph":
			from gpvdm_graph import gpvdm_graph 
			self.canvas=gpvdm_graph()
		else:
			self.canvas = FigureCanvas(self.fig)
			self.canvas.figure.patch.set_facecolor("white")

		self.open_gl_enabled=False
		self.gl_plot=None

		self.zero_frame_enable=False
		self.zero_frame_list=[]

		self.cb=None

		if enable_toolbar==True:
			self.plot_ribbon=self.build_toolbar(enable_3d)
			self.main_vbox.addWidget(self.plot_ribbon)

		if force_2d3d=="3d":
			self.open_gl_enabled=True
			self.callback_3d_mode()

		#self.canvas.setMinimumSize(800, 350)
		self.canvas.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.main_vbox.addWidget(self.canvas)

		self.setLayout(self.main_vbox)
		self.menu_build()


	def callback_3d_mode(self):
		self.open_gl_enabled=False

		if self.tb_3d_mode.isChecked()==True:
			self.open_gl_enabled=True

		if self.force_2d3d=="3d":
			self.open_gl_enabled=True

		if self.open_gl_enabled==True:
			self.canvas.setVisible(False)
			self.plot_ribbon.setTabEnabled(0,False) 
			self.plot_ribbon.setTabEnabled(1,False) 
			self.plot_ribbon.setTabEnabled(2,False)
			self.plot_ribbon.setTabEnabled(3,False)
			self.plot_ribbon.setTabEnabled(4,True)
			self.plot_ribbon.setCurrentWidget(self.plot_ribbon.tb_video)
			if self.gl_plot==None:
				from gl import glWidget
				self.gl_plot=glWidget(self)
				self.gl_plot.draw_electrical_mesh=False
				self.gl_plot.view_options.draw_device=True
				self.gl_plot.enable_draw_ray_mesh=True
				self.gl_plot.view_options.enable_draw_light_source=False
				self.gl_plot.view_options.draw_rays=False
				self.gl_plot.view_options.render_fdtd_grid=False
				self.gl_plot.scene_built=True
				self.gl_plot.plot_graph=True
				self.main_vbox.insertWidget(1,self.gl_plot)
				self.plot_ribbon.tb_video.addWidget(self.gl_plot.toolbar1)
				#self.plot_ribbon.plot_toolbar.removeAction(self.matplotlib_nav_bar)
				#self.self.tb_3d_mode.setVisible(False)

			self.gl_plot.setVisible(True)
			self.gl_plot.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
			#self.gl_plot.setMinimumSize(800, 350)
			self.gl_plot.setMinimumSize(948, 572)

		else:
			self.gl_plot.setVisible(False)
			self.canvas.setVisible(True)
			#self.matplotlib_nav_bar.setVisible(True)
