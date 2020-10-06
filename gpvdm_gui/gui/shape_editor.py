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

from import_data import import_data

from open_save_dlg import open_as_filter

from shutil import copyfile
from dat_file import dat_file

from triangle import triangle
from triangle_io import triangles_get_min
from triangle_io import triangles_get_max

from PyQt5.QtCore import pyqtSignal
from PIL import Image, ImageFilter,ImageOps, ImageDraw
from PIL.ImageQt import ImageQt
from inp import inp
from inp_dialog import inp_dialog
from str2bool import str2bool

from image_discretizer import image_discretizer
from scripts import scripts

from math import sin
from math import exp
from config_window import class_config_window
from cal_path import get_exe_command
from server import server_get
from cal_path import get_sim_path

from math import sin
from math import cos
from math import exp
from math import pow

from gl import glWidget
from gl_base_object import gl_base_object
from triangle_io import triangles_scale_for_gl
from bibtex import bibtex

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
			self.three_d_shape.gl_objects_remove_regex("bing")
			a=gl_base_object()
			a.id=["bing"]
			a.type="solid_and_mesh_color"
			a.r=data.r
			a.g=data.g
			a.b=data.b

			a.triangles=triangles_scale_for_gl(data.data)
			if a.triangles!=False:
				self.three_d_shape.gl_objects_add(a)
				self.three_d_shape.scene_built=True


	def callback_help(self):
		webbrowser.open("https://www.gpvdm.com/docs.html")

	def update(self):
		self.alpha.update()

	def callback_norm_y(self):
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		f.replace("#shape_import_y_norm",str(self.ribbon.tb_norm_y.isChecked()))
		f.save()
		self.discretizer.force_update()

	def callback_tb_gaus(self):
		f=inp()
		f.load(os.path.join(self.path,"gauss.inp"))
		sigma=int(f.get_token("#gauss_sigma"))
		gauss_offset_x=int(f.get_token("#gauss_offset_x"))
		gauss_offset_y=int(f.get_token("#gauss_offset_y"))

		ylen=400
		xlen=400
		im= Image.new("RGB", (xlen, ylen), "#FF0000")

		for y in range(0,ylen):
			for x in range(0,xlen):
				mag=int(255*exp(-((x-gauss_offset_x-xlen/2)/sigma)**2 -((y-gauss_offset_y-ylen/2)/sigma)**2))

				im.putpixel((x,y),(mag, mag, mag))
		im.save(os.path.join(self.path,"image.png"))
		
		self.discretizer.force_update()

	def gen_poly(self,x0,y0,dx0,dy0):
		ret=[]
		start=0.0
		stop=360
		steps=6
		dphi_deg=(stop-start)/steps
		phi_deg=start
		while (phi_deg<stop):
			phi=3.1415926*2*phi_deg/360
			dx=sin(phi)*dx0
			dy=cos(phi)*dy0
			ret.append((x0+dx, y0+dy))
			phi_deg=phi_deg+dphi_deg
			#print(phi_deg)
		ret.append(ret[0])
		x_min=1e6
		x_max=0
		y_min=1e6
		y_max=0

		for r in ret:
			if x_min>r[0]:
				x_min=r[0]

			if x_max<r[0]:
				x_max=r[0]

			if y_min>r[1]:
				y_min=r[1]

			if y_max<r[1]:
				y_max=r[1]

		w=x_max-x_min
		#h=y_max-y_min
		h=abs(ret[0][1]-ret[1][1])+abs(ret[1][1]-ret[2][1])
		#seg_len=pow(pow((ret[0][0]-ret[1][0]),2.0)+pow((ret[0][1]-ret[1][1]),2.0),0.5)
		return w,h,ret

	def apply_boundary(self,im):
		f=inp()
		f.load(os.path.join(self.path,"boundary.inp"))
		x0=int(f.get_token("#image_boundary_x0"))
		x1=int(f.get_token("#image_boundary_x1"))
		y0=int(f.get_token("#image_boundary_y0"))
		y1=int(f.get_token("#image_boundary_y1"))
		h, w = im.size
		dr=ImageDraw.Draw(im)
		#print(x0)
		dr.rectangle([(0, 0), (x0, h)], fill="white")

	#def rotate(self,im):
	#	f=inp()
	#	f.load(os.path.join(self.path,"shape_import.inp"))
	#	rotate=int(f.get_token("#shape_import_rotate"))

	#	return im.rotate(360-rotate)

	def callback_tb_honeycomb(self):
		f=inp()
		f.load(os.path.join(self.path,"honeycomb.inp"))
		dx=int(f.get_token("#honeycomb_dx"))
		dy=int(f.get_token("#honeycomb_dy"))
		shift_x=int(f.get_token("#honeycomb_x_shift"))
		shift_y=int(f.get_token("#honeycomb_y_shift"))
		line_width=int(f.get_token("#honeycomb_line_width"))

		ylen=200
		xlen=200
		im= Image.new("RGB", (xlen, ylen), "#000000")

		x_start=-20
		x_stop=xlen
		n_x=(x_stop-x_start)/dx

		y_start=-20
		y_stop=ylen
		n_y=(y_stop-y_start)/dy

		x_pos=0
		y_pos=0
		delta=0.0
		while(y_pos<y_stop):
			x_pos=delta
			while(x_pos<x_stop):
				#ImageDraw.Draw(im).polygon(, width=7)
				w,h,points=self.gen_poly(shift_x+x_pos,shift_y+y_pos,dx,dy)
				dr=ImageDraw.Draw(im)
				dr.line(points, fill="white", width=line_width)

				x_pos=x_pos+w

			#v_seg_len=2*dy
			y_pos=y_pos+h

			if delta==0.0:
				delta=w/2
			else:
				delta=0

		self.apply_boundary(im)
		#im=self.rotate(im)
		im.save(os.path.join(self.path,"image.png"))
		self.discretizer.force_update()

	def callback_norm_z(self):
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		f.replace("#shape_import_z_norm",str(self.ribbon.tb_norm_z.isChecked()))
		f.save()
		self.discretizer.force_update()

	def callback_rotate(self):
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		rotate=int(f.get_token("#shape_import_rotate"))
		rotate=rotate+90
		if rotate>360:
			rotate=0
		f.replace("#shape_import_rotate",str(rotate))
		f.save()
		self.discretizer.force_update()


	def callback_blur_enable(self):
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		f.replace("#shape_import_blur_enabled",str(self.ribbon.tb_blur.isChecked()))
		f.save()
		self.discretizer.force_update()

	def callback_menu_blur(self):
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		blur=f.get_token("#shape_import_blur")

		self.a=inp_dialog(title=_("Gaussian blur editor"),icon="blur")
		ret=self.a.run(["#shape_import_blur",blur,"#end"])
		if ret==QDialog.Accepted:
			f.replace("#shape_import_blur",self.a.tab.f.get_token("#shape_import_blur"))

			f.save()
			self.discretizer.force_update()

	def callback_mesh_editor(self):
		self.config_window=class_config_window()
		self.config_window.files=[os.path.join(self.path,"mesh_gen.inp")]
		self.config_window.description=[_("Configure mesh")]
		self.config_window.init()
		self.config_window.show()

	def callback_mesh_build(self):
		self.my_server=server_get()
		self.my_server.add_job(get_sim_path(),"--simmode mesh_gen@mesh_gen --path "+self.path)
		self.my_server.sim_finished.connect(self.reload)
		self.my_server.start()

	def callback_edit_norm_y(self):
		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		shape_import_y_norm_percent=f.get_token("#shape_import_y_norm_percent")

		self.a=inp_dialog(title=_("Normalization editor"),icon="shape")
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

		f=inp()
		f.load(os.path.join(self.path,"shape_import.inp"))
		self.ribbon.tb_norm_y.setChecked(str2bool(f.get_token("#shape_import_y_norm")))
		self.ribbon.tb_norm_z.setChecked(str2bool(f.get_token("#shape_import_z_norm")))
		self.ribbon.tb_blur.setChecked(str2bool(f.get_token("#shape_import_blur_enabled")))

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
		self.three_d_shape.view.draw_device=False
		self.three_d_shape.enable_draw_ray_mesh=True
		self.three_d_shape.enable_draw_light_source=False
		self.three_d_shape.enable_draw_rays=False
		self.three_d_shape.view.render_photons=False

		self.notebook.addTab(self.three_d_shape,_("Shape"))

		self.notebook.addTab(self.discretizer,_("Image"))

		self.setLayout(self.main_vbox)
		
		self.load_data()
		self.notebook.currentChanged.connect(self.changed_click)

	def callback_honeycomb_menu_edit(self):
		self.config_window=class_config_window()
		self.config_window.files=[os.path.join(self.path,"honeycomb.inp")]
		self.config_window.description=[_("Configure honeycomb")]
		self.config_window.init()
		self.config_window.show()


	def callback_gaus_menu_edit(self):
		self.config_window=class_config_window()
		self.config_window.files=[os.path.join(self.path,"gauss.inp")]
		self.config_window.description=[_("Configure gaussian")]
		self.config_window.init()
		self.config_window.show()

	def callback_boundary_menu_edit(self):
		self.config_window=class_config_window()
		self.config_window.files=[os.path.join(self.path,"boundary.inp")]
		self.config_window.description=[_("Configure boundary")]
		self.config_window.init()
		self.config_window.show()

	def callback_show_mesh(self):
		self.discretizer.show_mesh=self.ribbon.show_mesh.isChecked()
		self.discretizer.repaint()

	def callback_import(self):
		self.discretizer.save_mesh()



