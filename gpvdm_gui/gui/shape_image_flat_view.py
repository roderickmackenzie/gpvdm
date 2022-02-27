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
from PIL import Image, ImageFilter,ImageOps, ImageDraw
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget, QDialog, QMenu
from PyQt5.QtGui import QPainter,QIcon,QPixmap,QPen,QColor

#python modules
import webbrowser

from help import help_window

from plot_widget import plot_widget
from win_lin import desktop_open

from QWidgetSavePos import QWidgetSavePos

from ribbon_shape_import import ribbon_shape_import

from open_save_dlg import open_as_filter

from shutil import copyfile
from dat_file import dat_file

from triangle import triangle

from PyQt5.QtCore import pyqtSignal
from PIL import Image, ImageFilter,ImageOps 
from PIL.ImageQt import ImageQt
from inp import inp
from str2bool import str2bool

from PyQt5.QtWidgets import QApplication
import time
from triangle_io import triangles_get_min
from triangle_io import triangles_sub_vec
from triangle_io import triangles_get_max
from triangle_io import triangles_mul_vec
from triangle_io import triangles_div_vec

class shape_image_flat_view(QWidget):
	changed = pyqtSignal()

	def __init__(self,path,config):
		super().__init__()
		self.path=path
		self.config=config
		self.image_in=os.path.join(self.path,"image.png")
		self.image_out=os.path.join(self.path,"image_out.png")
		self.setGeometry(30, 30, 500, 300)
		self.len_x=800e-9
		self.len_y=800e-9
		self.len_z=800e-9
		self.im=None

		self.dat_file=dat_file()
		self.triangles=[]
		self.load_image()

	def build_mesh(self):
		if self.config.mesh.mesh_show==True:
			width, height = self.im.size

			if self.dat_file.load(os.path.join(self.path,"shape.inp"))==True:
				if len(self.dat_file.data)!=0:
					width, height = self.im.size
					min=triangles_get_min(self.dat_file.data)
					self.dat_file.data=triangles_sub_vec(self.dat_file.data,min)
					max=triangles_get_max(self.dat_file.data)
					
					self.dat_file.data=triangles_div_vec(self.dat_file.data,max)


	def force_update(self):
		self.load_image()
		self.build_mesh()
		self.repaint()


	def load_image(self):
		if os.path.isfile(self.image_out)==False:
			self.im=None
			return

		img=Image.open(self.image_out)
		if img.mode!="RGB":
			img=img.convert('RGB')

		self.im = img.convert('RGB')

		self.build_mesh()

	def mouseMoveEvent(self, event):
		width, height = self.im.size
		if event.buttons()==Qt.LeftButton:
			x=int(width*event.x()/self.width())
			y=int(height*event.y()/self.height())
			drawer=ImageDraw.Draw(self.im)
			drawer.rectangle([(x, y), (x+5, y+5)], fill="black")
			self.update()

	def mouseReleaseEvent(self, event):
		self.im.save(self.image_in)

	def paintEvent(self, event):
		painter = QPainter(self)

		if self.im==None:
			return
		width, height = self.im.size
		#print(type(self.im))
		qim = ImageQt(self.im)
		pixmap = QPixmap.fromImage(qim)
		#self.im=pixmap.toImage()
		x_mul=self.height()
		z_mul=self.width()

		painter.drawPixmap(self.rect(), pixmap)
		pen = QPen(Qt.red, 3)
		painter.setPen(pen)

		if self.config.mesh.mesh_show==True:
			if self.dat_file.data==None:
				self.build_mesh()

			if self.dat_file.data!=None:
				for t in self.dat_file.data:
					#print(t.xyz0.x,t.xyz0.x*x_mul,self.width())
					painter.drawLine(t.xyz0.z*z_mul, t.xyz0.x*x_mul, t.xyz1.z*z_mul, t.xyz1.x*x_mul)
					painter.drawLine(t.xyz1.z*z_mul, t.xyz1.x*x_mul, t.xyz2.z*z_mul, t.xyz2.x*x_mul)
					painter.drawLine(t.xyz2.z*z_mul, t.xyz2.x*x_mul, t.xyz0.z*z_mul, t.xyz0.x*x_mul)


	def callback_copy(self,event):
		self.menu.close()
		time.sleep(0.1)
		QApplication.processEvents()
		screen = QApplication.primaryScreen()
		QApplication.clipboard().setImage(screen.grabWindow(self.winId()).toImage())



	def contextMenuEvent(self, event):
		self.menu = QMenu(self)

		export=self.menu.addMenu(_("Export"))

		action=export.addAction(_("Copy"))
		action.triggered.connect(self.callback_copy)
		self.menu.exec_(event.globalPos())


