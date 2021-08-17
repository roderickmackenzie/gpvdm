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

## @package shape_editor
#  The shape editor
#


import os
from tab import tab_class
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget, QDialog
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

from image_discretizer import image_discretizer
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

class shape_editor(QWidgetSavePos):

	def changed_click(self):

		if self.notebook.tabText(self.notebook.currentIndex()).strip()==_("Shape"):
			b=bibtex()
			if b.load(os.path.join(self.path,"shape.bib"))!=False:
				text=b.get_text()
				help_window().help_set_help(["shape.png",_("<big><b>Shape file</b></big><br>"+text)])


	def reload(self):
		self.discretizer.force_update()
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
			self.three_d_shape.gl_objects_remove_regex("bing")
			a=gl_base_object()
			a.id=["bing"]
			a.type="solid_and_mesh"
			a.r=data.r
			a.g=data.g
			a.b=data.b
			xyz=vec()
			xyz.x=-plot_size/2.0
			xyz.y=-plot_size
			xyz.z=-plot_size/2.0
			a.xyz.append(xyz)
			a.dxyz.x=plot_size
			a.dxyz.y=plot_size
			a.dxyz.z=plot_size
			a.rotate_x=180
			a.triangles.extend(data.data)
			self.three_d_shape.gl_objects_add(a)
			last_obj=self.three_d_shape.objects[-1]
			self.three_d_shape.objects[-1].compile("triangles_solid",[1.0,0.0,0.0,0.5],[last_obj.r_false,last_obj.g_false,last_obj.b_false])
			self.three_d_shape.objects[-1].compile("triangles_open",[0.9,0.0,0.0,1.0],[last_obj.r_false,last_obj.g_false,last_obj.b_false],line_width=5)
			self.three_d_shape.scene_built=True
			self.three_d_shape.gl_objects_add_grid()

	def callback_help(self):
		webbrowser.open("https://www.gpvdm.com/docs.html")

	def update(self):
		self.alpha.update()

	def callback_norm_y(self):
		self.io.import_config.shape_import_y_norm=self.ribbon.tb_norm_y.isChecked()
		self.io.save()
		self.discretizer.force_update()

	def callback_tb_gaus(self):
		self.io.draw_gauss()
		self.discretizer.force_update()

	def callback_tb_honeycomb(self):
		self.io.draw_honeycomb()
		self.discretizer.force_update()

	def callback_norm_z(self):
		self.io.import_config.shape_import_z_norm=self.ribbon.tb_norm_z.isChecked()
		self.io.save()
		self.discretizer.force_update()

	def callback_rotate(self):
		rotate=self.io.import_config.shape_import_rotate
		rotate=rotate+90
		if rotate>360:
			rotate=0
		self.io.import_config.shape_import_rotate=rotate
		self.io.save()
		self.discretizer.force_update()


	def callback_blur_enable(self):
		self.io.blur.shape_import_blur_enabled=self.ribbon.tb_blur.isChecked()
		self.io.save()
		self.discretizer.force_update()

	def callback_menu_blur(self):
		self.config_window=class_config_window([self.io.blur],[_("Blur menu")],data=self.io)
		self.config_window.show()
		self.discretizer.force_update()

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
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		shape_import_y_norm_percent=f.get_token("#shape_import_y_norm_percent")

		self.a=json_dialog(title=_("Normalization editor"),icon="shape")
		ret=self.a.run(["#shape_import_y_norm_percent",shape_import_y_norm_percent,"#end"])
		if ret==QDialog.Accepted:
			f.replace("#shape_import_y_norm_percent",self.a.tab.f.get_token("#shape_import_y_norm_percent"))
			f.save()
			self.discretizer.force_update()


	def callback_open_image(self):
		file_name=open_as_filter(self,"png (*.png);;jpg (*.jpg)",path=self.path)
		if file_name!=None:
			im = Image.open(file_name)
			im.save(os.path.join(self.path,"image.png"))
			self.discretizer.load_image()
			self.discretizer.build_mesh()

	def callback_script(self):
		self.scripts.show()

	def __init__(self,path):
		QWidgetSavePos.__init__(self,"shape_import")
		self.path=path
		self.io=shape_editor_io()
		self.io.load(os.path.join(self.path,"data.json"))
		self.io.loaded=True
		self.io.save()
		self.discretizer=image_discretizer(self.path)


		self.setMinimumSize(900, 900)
		self.setWindowIcon(icon_get("shape"))

		#self.setWindowTitle(_("Import microscope image")+" (https://www.gpvdm.com)") 
		self.setWindowTitle(os.path.basename(self.path)+" "+_("Shape editor")) 

		self.scripts=scripts(path=self.path,api_callback=self.discretizer.force_update)

		self.scripts.ribbon.help.setVisible(False)
		self.scripts.ribbon.plot.setVisible(False)
		self.scripts.ribbon.hashtag.setVisible(False)

		self.main_vbox = QVBoxLayout()

		self.ribbon=ribbon_shape_import()

		self.ribbon.tb_norm_y.setChecked(self.io.import_config.shape_import_y_norm)
		self.ribbon.tb_norm_z.setChecked(self.io.import_config.shape_import_z_norm)
		self.ribbon.tb_blur.setChecked(self.io.blur.shape_import_blur_enabled)

		self.ribbon.mesh_edit.triggered.connect(self.callback_mesh_editor)
		self.ribbon.mesh_build.clicked.connect(self.callback_mesh_build)
		server_get().sim_finished.connect(self.ribbon.mesh_build.stop)
		server_get().sim_started.connect(self.ribbon.mesh_build.start)

		self.ribbon.edit_norm_y.triggered.connect(self.callback_edit_norm_y)
		self.ribbon.menu_blur.triggered.connect(self.callback_menu_blur)
		self.ribbon.tb_honeycomb_menu_edit.triggered.connect(self.callback_honeycomb_menu_edit)
		#self.ribbon.tb_boundary_menu_edit.triggered.connect(self.callback_boundary_menu_edit)


		self.ribbon.tb_gaus_menu_edit.triggered.connect(self.callback_gaus_menu_edit)
		self.ribbon.tb_gaus.triggered.connect(self.callback_tb_gaus)
		self.ribbon.tb_honeycomb.triggered.connect(self.callback_tb_honeycomb)
		self.ribbon.tb_boundary.triggered.connect(self.callback_boundary_menu_edit)
		self.ribbon.tb_configure.triggered.connect(self.callback_configure)

		self.ribbon.tb_norm_y.triggered.connect(self.callback_norm_y)
		self.ribbon.tb_norm_z.triggered.connect(self.callback_norm_z)
		self.ribbon.tb_blur.triggered.connect(self.callback_blur_enable)
		self.ribbon.tb_rotate.triggered.connect(self.callback_rotate)

		self.ribbon.import_image.clicked.connect(self.callback_open_image)
		self.ribbon.save_data.clicked.connect(self.callback_import)
		self.ribbon.show_mesh.clicked.connect(self.callback_show_mesh)

		self.ribbon.tb_script.clicked.connect(self.callback_script)
		#self.ribbon.help.triggered.connect(self.callback_help)

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

		self.notebook.addTab(self.three_d_shape,_("Shape"))

		self.notebook.addTab(self.discretizer,_("Image"))

		self.setLayout(self.main_vbox)
		
		self.load_data()
		self.notebook.currentChanged.connect(self.changed_click)

	def callback_honeycomb_menu_edit(self):
		self.config_window=class_config_window([self.io.honeycomb],[_("Configure honeycomb")],data=self.io)
		self.config_window.show()


	def callback_gaus_menu_edit(self):
		self.config_window=class_config_window([self.io.gauss],[_("Configure gaussian")],data=self.io)
		self.config_window.show()

	def callback_boundary_menu_edit(self):
		self.config_window=class_config_window([self.io.boundary],[_("Configure boundary")],data=self.io)
		self.config_window.show()

	def callback_configure(self):
		self.config_window=class_config_window([self.io],[_("Configure")],data=self.io)
		self.config_window.show()

	def callback_show_mesh(self):
		self.discretizer.show_mesh=self.ribbon.show_mesh.isChecked()
		self.discretizer.repaint()

	def callback_import(self):
		self.discretizer.save_mesh()



