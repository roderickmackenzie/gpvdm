# -*- coding: utf-8 -*-
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

## @package plot_window
#  A plot window which uses the plot widget.
#


import os
#import shutil
#from token_lib import tokens
from plot_widget import plot_widget
from PyQt5.QtWidgets import QWidget, QVBoxLayout, QSlider, QStatusBar, QSizePolicy
from PyQt5.QtCore import Qt
from dat_file import dat_file
from icon_lib import icon_get

from gl_base_object import gl_base_object

from gl import glWidget
from triangle import vec

class dissection(QWidget):
	def __init__(self):
		QWidget.__init__(self)
		self.frac_max=1.0
		self.frac_min=0.0

		self.setWindowTitle("Dissection Window")
		main_layout = QVBoxLayout()

		self.slider_y_min = QSlider(Qt.Horizontal)
		self.slider_y_min.setRange(0, 100)

		self.slider_y_max = QSlider(Qt.Horizontal)
		self.slider_y_max.setRange(0, 100)


		main_layout.addWidget(self.slider_y_min)
		main_layout.addWidget(self.slider_y_max)

		self.setLayout(main_layout)


class plot_window(QWidget):
	def __init__(self):
		QWidget.__init__(self)
		self.main_vbox=QVBoxLayout()
		self.setMinimumSize(200,200)
		self.shown=False
		self.data=dat_file()
		self.status_bar=QStatusBar()

	def update_status_bar(self,text):
		self.status_bar.showMessage(text)

	def destroy(self):
		self.shown=False
		self.window.destroy()

	def callback_destroy(self,widget):
		self.destroy()

	def update_dissection_y_min(self, position):
		self.plot.frac_min=position/100.0
		self.plot.do_draw()

	def update_dissection_y_max(self, position):
		self.frac_max=position/100.0
		self.plot.plot.do_draw()


	def init(self,input_files,plot_labels,force_mode="2d"):
		three_d=False
		data_type="xy"

		if len(input_files)==1:

			self.data.load(input_files[0])

			data_type=self.data.type

		three_d=False
		if data_type=="gobj":
			three_d=True

		if data_type=="zxy-d":
			three_d=True

		if force_mode=="3d":
			three_d=True

		if three_d==True:
			self.setWindowTitle(_("3D object viewer")+" https://www.gpvdm.com")
			self.setWindowIcon(icon_get("shape"))
			self.dissection=dissection()

			self.plot=glWidget(self)
			self.plot.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
			self.main_vbox.addWidget(self.plot)
			self.main_vbox.addWidget(self.status_bar)
			self.plot.text_output.connect(self.update_status_bar)

			self.setLayout(self.main_vbox)

			#self.plot.triangle_file=input_files[0]

			self.plot.draw_electrical_mesh=False
			self.plot.view_options.draw_device=False
			self.plot.enable_draw_ray_mesh=False
			self.plot.view_options.enable_draw_light_source=False
			self.plot.view_options.draw_rays=False
			self.plot.view_options.render_photons=False
			self.plot.view_options.optical_mode=False

			if data_type=="zxy-d":
				self.plot.plot_graph=True
				self.plot.graph_data.load(input_files[0])
				if self.plot.graph_data.title!="":
					self.setWindowTitle(self.plot.graph_data.title)

				self.dissection.slider_y_min.sliderMoved.connect(self.update_dissection_y_min)
				self.dissection.slider_y_max.sliderMoved.connect(self.update_dissection_y_max)

				self.show()
				self.dissection.show()

			if data_type=="gobj":
				self.plot.pre_built_scene=self.plot.scale.project_base_objects_from_m_2_screen(self.data.data)
				self.show()
				self.plot.force_redraw()

				
		elif data_type=="circuit" or data_type=="poly":
			self.setWindowTitle(_("3D object viewer")+" https://www.gpvdm.com")
			self.setWindowIcon(icon_get("shape"))
			self.plot=glWidget(self)
			self.plot.scale.set_m2screen()
			self.plot.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
			self.main_vbox.addWidget(self.plot)
			self.setLayout(self.main_vbox)

			if data_type=="circuit":

				self.plot.draw_electrical_mesh=False
				self.plot.view_options.draw_device=False
				self.plot.enable_draw_ray_mesh=False
				self.plot.view_options.enable_draw_light_source=False
				self.plot.view_options.draw_rays=False
				self.plot.plot_graph=False
				self.plot.plot_circuit=True
				self.plot.view_options.render_photons=False
				self.plot.graph_data.load(input_files[0])
				self.show()
			elif data_type=="poly":

				self.plot.draw_electrical_mesh=False
				self.plot.view_options.draw_device=False
				self.plot.enable_draw_ray_mesh=True
				self.plot.view_options.enable_draw_light_source=False
				self.plot.view_options.draw_rays=False
				self.plot.scene_built=True
				self.plot.view_options.render_photons=False

				if self.data.load(input_files[0])==True:
					a=gl_base_object()
					a.id=["bing"]
					a.type="open_triangles"
					a.r=self.data.r
					a.g=self.data.g
					a.b=self.data.b
					xyz=vec()
					xyz.x=0.0
					xyz.y=0.0
					xyz.z=0.0
					a.xyz.append(xyz)
					a.triangles=self.plot.scale.project_trianges_m2screen(self.data.data)
					self.plot.gl_objects_add(a)

				self.main_vbox.addWidget(self.status_bar)
				self.plot.text_output.connect(self.update_status_bar)

				self.show()
		else:
			self.shown=True

			self.plot=plot_widget()

			if len(plot_labels)==0:
				for i in range(0,len(input_files)):
					plot_labels.append(os.path.basename(input_files[i]).replace("_","\_"))

			#print plot_labels
			for i in range(0,len(plot_labels)):
				if len(plot_labels[i])>0:
					if plot_labels[i][0]=="\\":
						plot_labels[i]=plot_labels[i][1:]
				plot_labels[i].replace("\\","/")

			self.plot.load_data(input_files)
			self.plot.set_labels(plot_labels)

			self.plot.do_plot()
			self.plot.show()	
		



