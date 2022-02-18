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

## @package shape_bildverarbeitung
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

from PyQt5.QtCore import pyqtSignal
from PIL import Image, ImageFilter,ImageOps 
from PIL.ImageQt import ImageQt

class shape_bildverarbeitung():

	def __init__(self,path,json_obj):
		super().__init__()
		self.path=path
		self.json_obj=json_obj
		self.image_in=os.path.join(self.path,"image.png")
		self.image_out=os.path.join(self.path,"image_out.png")

	def apply_boundary(self,im):
		if self.json_obj.boundary.boundary_enabled==True:
			x0=self.json_obj.boundary.image_boundary_x0
			x0_r=int(float(self.json_obj.boundary.image_boundary_x0_color.split(",")[0])*255)
			x0_g=int(float(self.json_obj.boundary.image_boundary_x0_color.split(",")[1])*255)
			x0_b=int(float(self.json_obj.boundary.image_boundary_x0_color.split(",")[2])*255)

			x1=self.json_obj.boundary.image_boundary_x1
			x1_r=int(float(self.json_obj.boundary.image_boundary_x1_color.split(",")[0])*255)
			x1_g=int(float(self.json_obj.boundary.image_boundary_x1_color.split(",")[1])*255)
			x1_b=int(float(self.json_obj.boundary.image_boundary_x1_color.split(",")[2])*255)

			y0=self.json_obj.boundary.image_boundary_y0
			y0_r=int(float(self.json_obj.boundary.image_boundary_y0_color.split(",")[0])*255)
			y0_g=int(float(self.json_obj.boundary.image_boundary_y0_color.split(",")[1])*255)
			y0_b=int(float(self.json_obj.boundary.image_boundary_y0_color.split(",")[2])*255)

			y1=self.json_obj.boundary.image_boundary_y1
			y1_r=int(float(self.json_obj.boundary.image_boundary_x1_color.split(",")[0])*255)
			y1_g=int(float(self.json_obj.boundary.image_boundary_x1_color.split(",")[1])*255)
			y1_b=int(float(self.json_obj.boundary.image_boundary_x1_color.split(",")[2])*255)

			w, h = im.size
			dr=ImageDraw.Draw(im)
			dr.rectangle([(0, 0), (x0, h)], fill=(x0_r,x0_g,x0_b))
			dr.rectangle([(w-x1, 0), (w, h)], fill=(x1_r,x1_g,x1_b))

			if y0!=0:
				dr.rectangle([(0, 0), (w, y0)], fill=(y0_r,y0_g,y0_b))

			if y1!=0:
				dr.rectangle([(0, h-y1), (w, h)], fill=(y1_r,y1_g,y1_b))

		return im

	def apply_blur(self,im):
		if self.json_obj.blur.shape_import_blur_enabled==True:
			im = im.filter(ImageFilter.GaussianBlur(radius = self.json_obj.blur.shape_import_blur))
		return im

	def apply_rotate(self,im):
		rotate=self.json_obj.import_config.shape_import_rotate
		if rotate!=0:
			im=im.rotate(360-rotate)
		return im

	def norm_y(self,im):
		if self.json_obj.import_config.shape_import_y_norm==True:
			print(self.json_obj.import_config.shape_import_y_norm_percent,im.mode)
			im=ImageOps.autocontrast(im, cutoff=self.json_obj.import_config.shape_import_y_norm_percent, ignore=None)
		return im

	def threshold(self,im):
		if self.json_obj.threshold.threshold_enabled==True:
			fn = lambda x : 255 if x > self.json_obj.threshold.threshold_value else 0
			im = im.convert('L').point(fn, mode='1')
			im = im.convert('RGB')

		return im

	def apply(self,im):
		im.save(self.image_in)
		im=self.norm_y(im)
		im=self.apply_blur(im)
		im=self.apply_rotate(im)
		im=self.threshold(im)
		im=self.apply_boundary(im)
		im.save(self.image_out)



	def this_was_z_norm(self):
		if os.path.isfile(self.image_in)==False:
			self.im=None
			return

		img=Image.open(self.image_in)
		if img.mode!="RGB":
			img=img.convert('RGB')

		self.y_norm=self.json_obj.shape_import_y_norm
		self.z_norm=self.json_obj.shape_import_z_norm
		self.y_norm_percent=self.json_obj.shape_import_y_norm_percent

		if self.z_norm==True:
			img2 = img.resize((1, 1))
			color = img2.getpixel((0, 0))
			avg_pixel=(color[0]+color[1]+color[2])/3
			width, height = img.size
			for z in range(0,height):
				x_avg=0
				for x in range(0,width):
					color=img.getpixel((x, z))
					c=(color[0]+color[1]+color[2])/3
					x_avg=x_avg+c
				x_avg=x_avg/width
				delta=avg_pixel-x_avg
				for x in range(0,width):
					color=img.getpixel((x, z))
					c=(color[0]+color[1]+color[2])/3
					img.putpixel((x,z),(int(c+delta),int(c+delta),int(c+delta)))


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

		if self.show_mesh==True:
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


