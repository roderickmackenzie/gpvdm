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

## @package plot_widget
#  The main plot widget.
#

from __future__ import unicode_literals

import os
import io
from numpy import *
from plot_io import plot_load_info


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

from util import numbers_to_latex
from util import pygtk_to_latex_subscript
from util import fx_with_units
from util import time_with_units

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QApplication
from PyQt5.QtGui import QPainter,QIcon,QImage

#calpath
from icon_lib import icon_get
from open_save_dlg import save_as_image
from open_save_dlg import save_as_filter

from dat_file_math import dat_file_max_min
from dat_file_math import dat_file_sub
from dat_file_math import dat_file_mul
from dat_file_math import dat_file_sub_float
from dat_file_math import dat_file_abs
from dat_file import dat_file
from dat_file import read_data_2d


from dlg_get_multi_text import dlg_get_multi_text

from mpl_toolkits.mplot3d import Axes3D

from colors import get_color
from colors import get_color_black
from colors import get_marker

from dat_file import dat_file_print
from plot_ribbon import plot_ribbon
from lock import get_lock
from dat_files_to_gnuplot import dat_files_to_gnuplot_files

from dat_files_to_csv import dat_files_to_csv

class plot_widget(QWidget):

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
			my_max,my_min=dat_file_max_min(self.data[0])
			for i in range(0,len(self.data)):
				if self.data[i].logdata==True:
					my_max,my_min=dat_file_max_min(self.data[i],cur_min=my_min,cur_max=my_max)

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

	def setup_axis(self):
		this_plot=[]
		for d in self.data:
			this_plot.append(os.path.basename(d.file_name))

		if (this_plot==self.last_plot)==False:
			#print("redoooo!!!!!!!!!!!!!")
			self.fig.clf()
			self.fig.subplots_adjust(bottom=0.2)
			self.fig.subplots_adjust(bottom=0.2)
			self.fig.subplots_adjust(left=0.1)
			self.fig.subplots_adjust(hspace = .001)

			self.ax=[]

			if self.plot_type=="linegraph":
				for i in range(0,len(self.data)):
					self.ax.append(self.fig.add_subplot(111,facecolor='white'))

			elif self.plot_type=="rgb":
				for i in range(0,len(self.data)):
					self.ax.append(self.fig.add_subplot(111,facecolor='white'))
			elif self.plot_type=="wireframe":
				self.ax=[self.fig.add_subplot(111,facecolor='white' ,projection='3d')]
			elif self.plot_type=="heat":
				for i in range(0,len(self.data)):
					self.ax.append(self.fig.add_subplot(111,facecolor='white'))
			elif self.plot_type=="3d":
				for i in range(0,len(self.data)):
					self.ax.append(self.fig.add_subplot(111,facecolor='white' ,projection='3d'))
			elif self.plot_type=="quiver":
				for i in range(0,len(self.data)):
					self.ax.append(self.fig.add_subplot(111,facecolor='white' ,projection='3d'))
		else:
			if self.plot_type=="3d":
				for a in self.ax:
					for c in a.collections:
						c.remove()
			elif self.plot_type=="linegraph":
				for a in self.ax:
					a.clear()
			elif self.plot_type=="heat":
				for a in self.ax:
					a.clear()
				if self.cb!=None:
					self.cb.remove()
					self.cb=None
			else:
				for a in self.ax:
					a.clear()

		self.last_plot=[]
		for d in self.data:
			self.last_plot.append(os.path.basename(d.file_name))

	def do_plot(self):
		if len(self.data)==0:
			return
		
		if self.data[0].valid_data==False:
			return

		key_text=[]

		self.plot_type=""

		#print(self.data[0].x_len,self.data[0].z_len,self.data[0].data)
		if self.data[0].type=="rgb":
			self.plot_type="rgb"
		elif self.data[0].type=="quiver":
			self.plot_type="quiver"
		else:
			if self.data[0].x_len==1 and self.data[0].z_len==1:
				self.plot_type="linegraph"
			elif self.data[0].x_len>1 and self.data[0].y_len>1 and self.data[0].z_len==1:
				if self.data[0].type=="3d":
					self.plot_type="wireframe"
				if self.data[0].type=="heat":
					self.plot_type="heat"
			elif self.data[0].x_len>1 and self.data[0].y_len>1 and self.data[0].z_len>1:
				print("ohhh full 3D")
				self.plot_type="3d"
			else:
				print(_("I don't know how to process this type of file!"),self.data[0].x_len, self.data[0].y_len,self.data[0].z_len)
				return

		self.setup_axis()


		all_plots=[]
		files=[]
		my_max=1.0

		if self.plot_type=="linegraph":		#This is for the 1D graph case
			self.ax[0].set_xlabel(self.data[0].y_label+" ("+str(self.data[0].y_units)+")")
			self.ax[0].set_ylabel(self.data[0].data_label+" ("+self.data[0].data_units+")")
	
			for i in range(0,len(self.data)):
				if self.data[0].logy==True:
					self.ax[i].set_xscale("log")

				if self.data[0].logdata==True:
					self.ax[i].set_yscale("log")

				if self.data[i].data_min!=None:
					self.ax[i].set_ylim([self.data[i].data_min,self.data[i].data_max])

				if self.data[i].rgb()!=None:
					col="#"+self.data[i].rgb()
				else:
					col=get_color(i)
				cur_plot, = self.ax[i].plot(self.data[i].y_scale,self.data[i].data[0][0], linewidth=3 ,alpha=1.0,color=col,marker=get_marker(i))
				#print(self.data[i].y_scale,self.data[i].data[0][0])
				if self.data[i].key_text!="":
					key_text.append("$"+numbers_to_latex(str(self.data[i].key_text))+ " "+pygtk_to_latex_subscript(self.data[0].key_units) +"$")

				all_plots.append(cur_plot)
				
				if len(self.data[i].labels)!=0:
					#we only want this many data labels or it gets crowded
					max_points=12
					n_points=range(0,len(self.data[i].y_scale))
					if len(n_points)>max_points:
						step=int(len(n_points)/max_points)
						n_points=[]
						pos=0
						while(len(n_points)<max_points):
							n_points.append(pos)
							pos=pos+step

					for ii in n_points:
						label_text=self.data[i].labels[ii]
						self.ax[i].annotate(label_text,xy = (self.data[i].y_scale[ii], self.data[i].data[0][0][ii]), xytext = (-20, 20),textcoords = 'offset points', ha = 'right', va = 'bottom',bbox = dict(boxstyle = 'round,pad=0.5', fc = 'yellow', alpha = 0.5),arrowprops = dict(arrowstyle = '->', connectionstyle = 'arc3,rad=0'))


				#print(self.data[i].labels)
		elif self.plot_type=="wireframe":

			self.ax[0].set_xlabel('\n'+self.data[0].x_label+'\n ('+self.data[0].x_units+")")
			self.ax[0].set_ylabel('\n'+self.data[0].y_label+'\n ('+self.data[0].y_units+")")
			self.ax[0].set_zlabel('\n'+self.data[0].data_label+'\n ('+self.data[0].data_units+")")

			self.log_3d_workaround()

			if self.force_data_max==False:
				my_max,my_min=dat_file_max_min(self.data[0])
				for i in range(0,len(self.data)):
					my_max,my_min=dat_file_max_min(self.data[i],cur_min=my_min,cur_max=my_max)
			else:
				my_max=self.force_data_max
				my_min=self.force_data_min

			self.ax[0].set_zlim(my_min, my_max)

			for i in range(0,len(self.data)):

				if self.data[i].logx==True:
					self.ax[i].set_xscale("log")

				if self.data[i].logy==True:
					self.ax[i].set_yscale("log")

				#if self.data[i].key_text!="":
				key="$"+numbers_to_latex(str(self.data[i].key_text))+ " "+pygtk_to_latex_subscript(self.data[0].key_units) +"$"

				X, Y = meshgrid( self.data[i].y_scale,self.data[i].x_scale)
				Z = self.data[i].data[0]

				# Plot the surface
				col=get_color(i)
				#print(self.data[i].plot_type,"here")
				if self.data[i].plot_type=="wireframe" or self.data[i].plot_type=="":
					im=self.ax[0].plot_wireframe( Y,X, array(Z),color=col, label=key, clip_on=True)
				elif self.data[i].plot_type=="contour":
					im=self.ax[0].contourf( Y,X, array(Z),color=col)
				elif self.data[i].plot_type=="heat":
					my_max,my_min=dat_file_max_min(self.data[0])
					im=self.ax[0].plot_surface(Y,X, array(Z), linewidth=0, vmin=my_min, vmax=my_max,cmap="hot", antialiased=False)

				self.ax[0].legend()
				#im=self.ax[0].contourf( Y,X, Z,color=col)

#cset = ax.contourf(X, Y, Z, zdir='y', offset=40, cmap=cm.coolwarm)
		elif self.plot_type=="heat":
			self.ax[0].set_xlabel(self.data[0].y_label+" ("+self.data[0].y_units+")")
			self.ax[0].set_ylabel(self.data[0].x_label+" ("+self.data[0].x_units+")")
			my_max,my_min=dat_file_max_min(self.data[0])
			for i in range(0,len(self.data)):
				if self.data[i].logdata==True:
					if my_min==0:
						my_min=1.0
					im=self.ax[0].pcolor(self.data[i].y_scale,self.data[i].x_scale,self.data[i].data[0], norm=LogNorm(vmin=my_min, vmax=my_max), vmin=my_min, vmax=my_max,cmap="gnuplot")
				else:
					im=self.ax[0].pcolor(self.data[i].y_scale,self.data[i].x_scale,self.data[i].data[0], vmin=my_min, vmax=my_max,cmap="gnuplot")

				self.cb=self.fig.colorbar(im)

		elif self.plot_type=="3d":
			self.ax[0].set_xlabel(self.data[0].x_label+" ("+self.data[0].x_units+")")
			self.ax[0].set_ylabel(self.data[0].y_label+" ("+self.data[0].y_units+")")
			i=0
			y_scale=self.data[i].y_scale
			x_scale=self.data[i].x_scale
			X, Y = meshgrid( y_scale,x_scale)		#self.data[i].y_scale,self.data[i].x_scale
			Z = self.data[i].data[0]
			col=get_color(i)
			my_max,my_min=dat_file_max_min(self.data[0])
		elif self.plot_type=="rgb":
			self.ax[0].set_xlabel(self.data[0].y_label+" ("+str(self.data[0].y_units)+")")
			self.ax[0].set_ylabel(self.data[0].data_label+" ("+self.data[0].data_units+")")
			self.ax[0].imshow(self.data[0].data[0])		#
			#,extent=[self.data[0].y_scale[0],self.data[0].y_scale[-1],0,20]
		elif self.plot_type=="quiver":
			self.ax[0].set_xlabel(self.data[0].x_label+" ("+self.data[0].x_units+")")
			self.ax[0].set_ylabel(self.data[0].y_label+" ("+self.data[0].y_units+")")
			self.ax[0].set_zlabel(self.data[0].z_label+" ("+self.data[0].z_units+")")
			X=[]
			Y=[]
			Z=[]
			U=[]
			V=[]
			W=[]
			mag=[]
			for d in self.data[0].data:
				X.append(d.x)
				Y.append(d.y)
				Z.append(d.z)
				U.append(d.dx)
				V.append(d.dy)
				W.append(d.dz)
				mag.append(d.mag)

			c=plt.cm.hsv(mag)

			mag=[]
			for d in self.data[0].data:

				mag.append(2.0)

			self.ax[0].quiver(X, Y, Z, U, V, W,colors=c,linewidths=mag)
			self.ax[0].set_xlim([0, self.data[0].xmax])
			self.ax[0].set_ylim([0, self.data[0].ymax])
			self.ax[0].set_zlim([0, self.data[0].zmax])


		#setup the key
		if self.data[0].legend_pos=="No key":
			self.ax[i].legend_ = None
		else:
			if len(files)<40:
				self.fig.legend(all_plots, key_text, self.data[0].legend_pos)

		if get_lock().is_trial()==True:
			x=0.25
			y=0.25
			#while(x<1.0):
			#	y=0
			#	while(y<1.0):
			self.fig.text(x, y, 'gpvdm trial', fontsize=20, color='gray', ha='right', va='bottom', alpha=self.watermark_alpha)

			#		y=y+0.1
			#	x=x+0.25

		if self.hide_title==False:
			title=self.data[0].title
			if self.data[0].time!=-1.0 and self.data[0].Vexternal!=-1.0:
				mul,unit=time_with_units(self.data[0].time)
				title=title+" V="+str(self.data[0].Vexternal)+" "+_("time")+"="+str(self.data[0].time*mul)+" "+unit

			self.fig.suptitle(title)

			self.setWindowTitle(title+" - www.gpvdm.com")

		self.fig.canvas.draw()


	def save_image(self):
		self.callback_save_image()

	def callback_save_image(self):
		file_name=save_as_image(self)
		if file_name != None:
			self.fig.savefig(file_name)

	def callback_save_csv(self):
		file_name=save_as_filter(self,"*.csv")
		if file_name != None:
			dat_files_to_csv(file_name,self.data)

	def callback_save_txt(self):
		file_name=save_as_filter(self,"*.txt")
		if file_name != None:
			self.data[0].save_as_txt(file_name)

	def callback_save_xls(self):
		return

	def callback_save_gnuplot(self):
		file_name=save_as_filter(self,"gnuplot (*.)")
		if file_name != None:
			#dat_files_to_gnuplot(file_name,self.data)
			dat_files_to_gnuplot_files(file_name,self.data)

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
			if ret!=False:
				dat.chop_y(0,-3)
				self.data.append(dat)


		self.norm_data()

	def load_data(self,input_files):
		self.lx=None
		self.ly=None
		self.input_files=input_files

		if len(input_files)==0:
			print(_("No files were given to load"))
			return

		self.reload()



	def norm_data(self):
		if len(self.data)>0:
			if self.data[0].type=="rgb" or self.data[0].type=="quiver":
				return

			if self.zero_frame_enable==True:
				if len(self.data)>1:
					for i in range(1,len(self.data)):
						dat_file_sub(self.data[i],self.data[0])
					
					dat_file_sub(self.data[0],self.data[0])
			for i in range(0,len(self.data)):
				for x in range(0,self.data[i].x_len):
					self.data[i].x_scale[x]=self.data[i].x_scale[x]*self.data[i].x_mul

				for y in range(0,self.data[i].y_len):
					self.data[i].y_scale[y]=self.data[i].y_scale[y]*self.data[i].y_mul

				for z in range(0,self.data[i].z_len):
					self.data[i].z_scale[z]=self.data[i].z_scale[z]*self.data[i].z_mul

				for x in range(0,self.data[i].x_len):
					for y in range(0,self.data[i].y_len):
						for z in range(0,self.data[i].z_len):
							self.data[i].data[z][x][y]=self.data[i].data[z][x][y]*self.data[i].data_mul

			if self.data[0].invert_y==True:
				for i in range(0,len(self.data)):
					dat_file_mul(self.data[i],-1)

			if self.data[0].subtract_first_point==True:
				val=self.data[0].data[0][0][0]
				for i in range(0,len(self.data)):
					dat_file_sub_float(self.data[i],val)


			if self.data[0].add_min==True:
				my_max,my_min=dat_file_max_min(self.data[0])
				for i in range(0,len(self.data)):
					dat_file_sub_float(self.data[i],my_min)



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
			my_max,my_min=dat_file_max_min(self.data[0])
			for i in range(0,len(self.data)):
				my_max,my_min=dat_file_max_min(self.data[i],cur_min=my_min,cur_max=my_max)
			self.force_data_max=my_max
			self.force_data_min=my_min
			self.do_plot()
		else:
			self.force_data_max=False
			self.force_data_min=False

	def callback_math_opp(self,opp):
		for i in range(0,len(self.data)):
			if opp=="abs":
				dat_file_abs(self.data[i])

		self.do_plot()

	def callback_norm_to_peak_of_all_data(self):
		if len(self.data)>0:
			self.data[0].norm_to_peak_of_all_data=not self.data[0].norm_to_peak_of_all_data
			self.norm_data()
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
			self.norm_data()
			self.do_plot()

	def callback_toggle_subtract_first_point(self):
		if len(self.data)>0:
			self.data[0].subtract_first_point=not self.data[0].subtract_first_point
			self.norm_data()
			self.do_plot()

	def callback_toggle_add_min(self):
		if len(self.data)>0:
			self.data[0].add_min=not self.data[0].add_min
			self.norm_data()
			self.do_plot()

	def update(self):
		self.load_data(self.input_files)
		self.do_plot()

	def callback_refresh(self):
		self.update()

	def __init__(self,enable_toolbar=True):
		QWidget.__init__(self)

		self.watermark_alpha=0.05
		self.data=[]
		self.input_files=[]
		self.hide_title=False
		self.force_data_max=False
		self.force_data_min=False
		self.fix_scales=False

		self.setWindowIcon(icon_get("plot"))

		self.ax=[]
		self.last_plot=[]

		self.main_vbox = QVBoxLayout()
		self.fig = Figure(figsize=(2.5,2), dpi=100)
		self.canvas = FigureCanvas(self.fig)

		self.zero_frame_enable=False
		self.zero_frame_list=[]

		self.cb=None

		if enable_toolbar==True:
			self.plot_ribbon=plot_ribbon()
			self.plot_ribbon.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Minimum)

			self.plot_ribbon.tb_export_as_jpg.clicked.connect(self.callback_save_image)
			self.plot_ribbon.tb_export_as_csv.clicked.connect(self.callback_save_csv)
			self.plot_ribbon.tb_export_as_txt.clicked.connect(self.callback_save_txt)
			self.plot_ribbon.tb_export_as_gnuplot.clicked.connect(self.callback_save_gnuplot)

			self.plot_ribbon.tb_copy.clicked.connect(self.callback_do_clip)

			#self.plot_ribbon.tb_export_as_jpg.triggered.connect(self.save_image)


			self.tb_refresh = QAction(icon_get("view-refresh"), _("Refresh graph"), self)
			self.tb_refresh .triggered.connect(self.callback_refresh)
			self.plot_ribbon.plot_toolbar.addAction(self.tb_refresh )



			nav_bar=NavigationToolbar(self.canvas,self)
			actions = nav_bar.findChildren(QAction)
			for a in actions:
				if a.text() == 'Save':
					nav_bar.removeAction(a)
					break

			nav_bar.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
			nav_bar.setIconSize(QSize(42, 42))
			self.plot_ribbon.plot_toolbar.addWidget(nav_bar)

			self.fig.canvas.mpl_connect('motion_notify_event', self.mouse_move)

			self.plot_ribbon.tb_color_black.triggered.connect(self.callback_black)
			self.plot_ribbon.tb_color_rainbow.triggered.connect(self.callback_rainbow)

			self.plot_ribbon.tb_scale_autoscale.triggered.connect(self.callback_autoscale_y)
			self.plot_ribbon.tb_scale_log_y.triggered.connect(self.callback_toggle_log_scale_y)
			self.plot_ribbon.tb_scale_log_x.triggered.connect(self.callback_toggle_log_scale_x)

			#new way to do math
			for o in self.plot_ribbon.math_opps:
				o[0].triggered.connect(lambda: self.callback_math_opp(o[1]))

			#old way to do math
			self.plot_ribbon.math_norm_to_peak_of_all_data.triggered.connect(self.callback_norm_to_peak_of_all_data)
			self.plot_ribbon.math_heat_map.triggered.connect(self.callback_set_heat_map)
			self.plot_ribbon.math_heat_map_edit.triggered.connect(self.callback_heat_map_edit)
			self.plot_ribbon.math_subtract_first_point.triggered.connect(self.callback_toggle_subtract_first_point)
			self.plot_ribbon.math_add_min_point.triggered.connect(self.callback_toggle_add_min)
			self.plot_ribbon.math_invert_y_axis.triggered.connect(self.callback_toggle_invert_y)


			self.main_vbox.addWidget(self.plot_ribbon)


		self.canvas.figure.patch.set_facecolor("white")
		self.canvas.setMinimumSize(800, 350)
		self.canvas.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.main_vbox.addWidget(self.canvas)

		self.setLayout(self.main_vbox)



