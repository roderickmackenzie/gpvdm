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

## @package shape_editor
#  The shape editor
#


import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget, QDialog, QHBoxLayout
from PyQt5.QtGui import QPainter,QIcon,QPixmap,QPen,QColor

#python modules
import webbrowser

from help import help_window

from plot_widget import plot_widget
from win_lin import desktop_open

from QWidgetSavePos import QWidgetSavePos

from ribbon_shape_import import ribbon_shape_import

from open_save_dlg import open_as_filter

from dat_file import dat_file

from triangle import triangle

from PyQt5.QtCore import pyqtSignal
from PIL import Image, ImageFilter,ImageOps, ImageDraw
from PIL.ImageQt import ImageQt
from json_dialog import json_dialog

from shape_image_flat_view import shape_image_flat_view
from scripts import scripts

from config_window import class_config_window
from cal_path import get_exe_command
from server import server_get
from cal_path import get_sim_path

from gl import glWidget
from gl_base_object import gl_base_object
from triangle_io import triangles_scale_for_gl
from bibtex import bibtex
from json_shape_db_item import shape_db_item
from shape_editor_io import shape_editor_io

from triangle_io import triangles_get_min
from triangle_io import triangles_get_max
from triangle_io import triangles_sub_vec
from triangle_io import triangles_div_vec
from triangle_io import triangles_mul_vec
from triangle_io import triangles_rotate_y
from triangle import vec
from json_base import json_base
import shutil
from shape_bildverarbeitung import shape_bildverarbeitung

class shape_editor(QWidgetSavePos):

	def changed_click(self):

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Shape"):
			b=bibtex()
			if b.load(os.path.join(self.path,"shape.bib"))!=False:
				text=b.get_text()
				help_window().help_set_help(["shape.png",_("<big><b>Shape file</b></big><br>"+text)])


	def reload(self):
		self.shape_image_flat_view.force_update()
		self.load_data()
		self.three_d_shape.do_draw()

	def load_data(self):
		data=dat_file()

		if data.load(os.path.join(self.path,"shape.inp"))==True:
			plot_size=5
			min_vec=triangles_get_min(data.data)
			data.data=triangles_sub_vec(data.data,min_vec)
			max_vec=triangles_get_max(data.data)
			data.data=triangles_div_vec(data.data,max_vec)

			min_vec=triangles_get_min(data.data)
			max_vec=triangles_get_max(data.data)
			max_vec.x=max_vec.x*7
			max_vec.y=max_vec.y*7
			max_vec.z=max_vec.z*7

			self.three_d_shape.scale.world_min=min_vec
			self.three_d_shape.scale.world_max=max_vec
			self.three_d_shape.scale.refresh_world_size=False
			self.three_d_shape.scale.set_m2screen()

			data.data=self.three_d_shape.scale.scale_trianges_m2screen(data.data)

			self.three_d_shape.gl_objects_remove_regex("bing")
			a=gl_base_object()
			a.id=["bing"]
			a.type="solid_and_mesh"
			a.r=data.r
			a.g=data.g
			a.b=data.b
			xyz=vec()
			xyz.x=self.three_d_shape.scale.project_m2screen_y(-2.5)
			xyz.y=self.three_d_shape.scale.project_m2screen_y(max_vec.y-2.0)
			xyz.z=self.three_d_shape.scale.project_m2screen_y(-1.0)
			a.xyz.append(xyz)
			a.dxyz.x=5.0*self.three_d_shape.scale.x_mul
			a.dxyz.y=2.0*self.three_d_shape.scale.y_mul
			a.dxyz.z=5.0*self.three_d_shape.scale.z_mul
			a.rotate_x=180
			a.triangles.extend(data.data)




			self.three_d_shape.gl_objects_add(a)
			last_obj=self.three_d_shape.objects[-1]
			self.three_d_shape.objects[-1].compile("triangles_solid",[1.0,0.0,0.0,0.5])
			self.three_d_shape.objects[-1].compile("triangles_open",[0.9,0.0,0.0,1.0],line_width=5)
			self.three_d_shape.scene_built=True
			self.three_d_shape.gl_objects_add_grid(-18,20,self.three_d_shape.scale.project_m2screen_y(self.three_d_shape.scale.world_max.y),None,-18,20)

	def update(self):
		self.alpha.update()

	def callback_norm_y(self):
		self.io.import_config.shape_import_y_norm=self.ribbon.tb_norm_y.isChecked()
		self.io.save()
		im=self.io.load_image()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)
		self.shape_image_flat_view.force_update()

	def callback_tb_gaus(self):
		im=self.io.draw_gauss()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)	
		self.shape_image_flat_view.force_update()

	def callback_tb_honeycomb(self):
		im=self.io.draw_honeycomb()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)	
		self.shape_image_flat_view.force_update()

	def callback_tb_xtal(self):
		im=self.io.draw_xtal()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)	
		self.shape_image_flat_view.force_update()

	def callback_tb_lens(self):
		im=self.io.draw_lens()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)	
		self.shape_image_flat_view.force_update()

	def callback_rotate(self):
		rotate=self.io.import_config.shape_import_rotate
		rotate=rotate+90
		if rotate>360:
			rotate=0
		self.io.import_config.shape_import_rotate=rotate
		self.io.save()
		im=self.io.load_image()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)
		self.shape_image_flat_view.force_update()


	def callback_filters_update(self):
		self.io.blur.shape_import_blur_enabled=self.ribbon.tb_blur.isChecked()
		self.io.threshold.threshold_enabled=self.ribbon.tb_threshold.isChecked()
		self.io.import_config.shape_import_z_norm=self.ribbon.tb_norm_z.isChecked()
		self.io.boundary.boundary_enabled=self.ribbon.tb_boundary.isChecked()
		self.io.save()
		im=self.io.load_image()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)
		self.shape_image_flat_view.force_update()

	def callback_menu_blur(self):
		self.config_window=class_config_window([self.io.blur],[_("Blur menu")],data=self.io)
		self.config_window.show()

	def callback_mesh_editor(self):
		self.config_window=class_config_window([self.io.mesh],[_("Configure mesh")],data=self.io)
		self.config_window.show()

	def callback_mesh_build(self):
		self.my_server=server_get()
		self.io.add_job_to_server(get_sim_path(),self.my_server)
		#self.my_server.add_job(get_sim_path(),"--simmode data@mesh_gen --path "+self.path)
		self.my_server.sim_finished.connect(self.reload)
		self.my_server.start()

	def callback_edit_norm_y(self):
		self.a=json_dialog(title=_("Normalization editor"),icon="shape")
		data=json_base("dlg")
		data.var_list.append(["shape_import_y_norm_percent",self.io.import_config.shape_import_y_norm_percent])
		data.var_list_build()
		ret=self.a.run(data)

		if ret==QDialog.Accepted:
			self.io.import_config.shape_import_y_norm_percent=data.shape_import_y_norm_percent
			self.io.save()
			im=self.io.load_image()
			f=shape_bildverarbeitung(self.path,self.io)
			f.apply(im)
			self.shape_image_flat_view.force_update()


	def callback_open_image(self):
		file_name=open_as_filter(self,"png (*.png);;jpg (*.jpg)",path=self.path)
		if file_name!=None:
			im = Image.open(file_name)
			im.save(os.path.join(self.path,"image.png"))				#This is the edited one
			im.save(os.path.join(self.path,"image_original.png"))		#This is the original image
			self.shape_image_flat_view.load_image()
			self.shape_image_flat_view.build_mesh()
			self.callback_filters_update()

	def callback_script(self):
		self.scripts.show()

	def __init__(self,path):
		QWidgetSavePos.__init__(self,"shape_import")
		self.path=path

		#backward compatibility
		self.orig_image_file=os.path.join(path,"image_original.png")
		self.image_file=os.path.join(path,"image.png")
		if os.path.isfile(self.orig_image_file)==False:
			if os.path.isfile(self.image_file)==True:
				shutil.copyfile(self.image_file, self.orig_image_file)



		self.io=shape_editor_io()
		self.io.load(os.path.join(self.path,"data.json"))
		self.io.loaded=True
		self.io.save()
		self.shape_image_flat_view=shape_image_flat_view(self.path,self.io)


		self.setMinimumSize(900, 900)
		self.setWindowIcon(icon_get("shape"))

		#self.setWindowTitle(_("Import microscope image")+" (https://www.gpvdm.com)") 
		self.setWindowTitle(os.path.basename(self.path)+" "+_("Shape editor")) 

		self.scripts=scripts(path=self.path,api_callback=self.shape_image_flat_view.force_update)

		self.scripts.ribbon.help.setVisible(False)
		self.scripts.ribbon.plot.setVisible(False)
		self.scripts.ribbon.hashtag.setVisible(False)

		self.main_vbox = QVBoxLayout()

		self.ribbon=ribbon_shape_import()

		self.ribbon.tb_norm_y.setChecked(self.io.import_config.shape_import_y_norm)



		self.ribbon.menu_threshold.triggered.connect(self.callback_threshold_menu_edit)

		self.ribbon.mesh_edit.triggered.connect(self.callback_mesh_editor)
		self.ribbon.mesh_build.clicked.connect(self.callback_mesh_build)
		server_get().sim_finished.connect(self.ribbon.mesh_build.stop)
		server_get().sim_started.connect(self.ribbon.mesh_build.start)

		self.ribbon.edit_norm_y.triggered.connect(self.callback_edit_norm_y)
		self.ribbon.menu_blur.triggered.connect(self.callback_menu_blur)
		self.ribbon.tb_honeycomb_menu_edit.triggered.connect(self.callback_honeycomb_menu_edit)
		self.ribbon.tb_xtal_menu_edit.triggered.connect(self.callback_xtal_menu_edit)
		self.ribbon.tb_lens_menu_edit.triggered.connect(self.callback_lens_menu_edit)


		self.ribbon.tb_gaus_menu_edit.triggered.connect(self.callback_gaus_menu_edit)
		self.ribbon.menu_boundary.triggered.connect(self.callback_boundary_menu_edit)
		self.ribbon.tb_gaus.triggered.connect(self.callback_tb_gaus)
		self.ribbon.tb_honeycomb.triggered.connect(self.callback_tb_honeycomb)
		self.ribbon.tb_xtal.triggered.connect(self.callback_tb_xtal)
		self.ribbon.tb_lens.triggered.connect(self.callback_tb_lens)
		self.ribbon.tb_configure.triggered.connect(self.callback_configure)

		#On button depress filters
		self.ribbon.tb_norm_z.triggered.connect(self.callback_filters_update)
		self.ribbon.tb_norm_z.setChecked(self.io.import_config.shape_import_z_norm)

		self.ribbon.tb_blur.triggered.connect(self.callback_filters_update)
		self.ribbon.tb_blur.setChecked(self.io.blur.shape_import_blur_enabled)

		self.ribbon.tb_threshold.triggered.connect(self.callback_filters_update)
		self.ribbon.tb_threshold.setChecked(self.io.threshold.threshold_enabled)

		self.ribbon.tb_boundary.triggered.connect(self.callback_filters_update)
		self.ribbon.tb_boundary.setChecked(self.io.boundary.boundary_enabled)

		self.ribbon.tb_norm_y.triggered.connect(self.callback_norm_y)
		self.ribbon.tb_rotate.triggered.connect(self.callback_rotate)
		self.ribbon.tb_apply.triggered.connect(self.callback_filters_update)

		self.ribbon.import_image.clicked.connect(self.callback_open_image)
		self.ribbon.save_data.clicked.connect(self.callback_import)
		self.ribbon.show_mesh.clicked.connect(self.callback_show_mesh)
		self.ribbon.show_mesh.setChecked(self.io.mesh.mesh_show)

		self.ribbon.tb_script.clicked.connect(self.callback_script)

		self.main_vbox.addWidget(self.ribbon)

		self.notebook = QTabWidget()

		self.notebook.setMovable(True)

		self.main_vbox.addWidget(self.notebook)

		self.notebook.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)


		#3d widget
		self.three_d_shape=glWidget(self)
		self.three_d_shape.triangle_file=""

		self.three_d_shape.draw_electrical_mesh=False
		self.three_d_shape.view_options.draw_device=False
		self.three_d_shape.enable_draw_ray_mesh=True
		self.three_d_shape.enable_draw_light_source=False
		self.three_d_shape.view_options.draw_rays=False
		self.three_d_shape.view_options.render_photons=False

		display=QWidget()
		layout = QHBoxLayout()
		display.setLayout(layout)
		layout.addWidget(self.three_d_shape)
		layout.addWidget(self.shape_image_flat_view)

		self.notebook.addTab(display,_("Shape"))

		#self.notebook.addTab(self.shape_image_flat_view,_("Image"))

		self.setLayout(self.main_vbox)
		
		self.load_data()
		self.notebook.currentChanged.connect(self.changed_click)

	def callback_honeycomb_menu_edit(self):
		self.config_window=class_config_window([self.io.honeycomb],[_("Configure honeycomb")],data=self.io)
		self.config_window.show()

	def callback_xtal_menu_edit(self):
		self.config_window=class_config_window([self.io.xtal],[_("Configure photonic xtal")],data=self.io)
		self.config_window.show()

	def callback_lens_menu_edit(self):
		self.config_window=class_config_window([self.io.lens],[_("Configure photonic lens")],data=self.io)
		self.config_window.show()

	def callback_gaus_menu_edit(self):
		self.config_window=class_config_window([self.io.gauss],[_("Configure gaussian")],data=self.io)
		self.config_window.show()

	def callback_boundary_menu_edit(self):
		self.config_window=class_config_window([self.io.boundary],[_("Configure boundary")],data=self.io)
		self.config_window.show()

	def callback_threshold_menu_edit(self):
		self.config_window=class_config_window([self.io.threshold],[_("Configure threshold")],data=self.io)
		self.config_window.show()

	def callback_configure(self):
		self.config_window=class_config_window([self.io],[_("Configure")],data=self.io)
		self.config_window.show()

	def callback_show_mesh(self):
		self.io.mesh.mesh_show=self.ribbon.show_mesh.isChecked()
		self.shape_image_flat_view.repaint()
		self.io.save()

	def callback_import(self):
		shutil.copyfile( self.orig_image_file,self.image_file)
		im=self.io.load_image()
		f=shape_bildverarbeitung(self.path,self.io)
		f.apply(im)
		self.shape_image_flat_view.force_update()

