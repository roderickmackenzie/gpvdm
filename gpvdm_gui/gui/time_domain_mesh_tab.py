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

## @package time_domain_mesh_tab
#  A mesh editor for the time domain mesh.
#


import os
from numpy import *
from gui_util import dlg_get_text
import webbrowser
from util import time_with_units
from cal_path import get_icon_path


#matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
#from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.ticker as ticker

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView
from PyQt5.QtGui import QPainter,QIcon

#windows
from open_save_dlg import save_as_image
from cal_path import get_sim_path
from icon_lib import icon_get
from error_dlg import error_dlg
from gpvdm_tab2 import gpvdm_tab2
from gpvdm_json import gpvdm_data
from json_time_domain import json_time_domain_mesh_segment
import i18n
_ = i18n.language.gettext


class tab_time_mesh(QWidget):

	def save_data(self):
		gpvdm_data().save()



	def callback_laser(self):
		data=gpvdm_data().time_domain.find_object_by_id(self.uid)
		new_time=dlg_get_text( _("Enter the time at which the laser pulse will fire (-1) to turn it off"), str(data.config.fs_laser_time),"laser.png")
		new_time=new_time.ret

		if new_time!=None:
			data.config.fs_laser_time=float(new_time)
			self.build_mesh()
			self.draw_graph()
			self.fig.canvas.draw()
			gpvdm_data().save()

	def update(self):
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()

	def on_cell_edited(self):
		self.save_data()
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()

	def gaussian(self,x, mu, sig):
		return exp(-power(x - mu, 2.) / (2 * power(sig, 2.)))

	def draw_graph(self):
		fs_laser_time=gpvdm_data().time_domain.find_object_by_id(self.uid).config.fs_laser_time
		if (len(self.time)>0):
			mul,unit=time_with_units(float(self.time[len(self.time)-1]-self.time[0]))
		else:
			mul=1.0
			unit="s"

		time=[]
		for i in range(0,len(self.time)):
			time.append(self.time[i]*mul)
		self.fig.clf()
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)


		self.ax1 = self.fig.add_subplot(211)
		self.ax1.ticklabel_format(useOffset=False)
		self.ax1.spines['right'].set_visible(False)
		self.ax1.spines['top'].set_visible(False)
		self.ax1.yaxis.set_ticks_position('left')
		self.ax1.xaxis.set_ticks_position('bottom')
		self.ax1.yaxis.set_major_locator(ticker.MaxNLocator(4))
		self.ax1.set_ylabel(_("Voltage (Volts)"))
		self.ax1.set_xticklabels([])
		self.ax1.grid(True)
		voltage, = self.ax1.plot(time,self.voltage, 'ro-', linewidth=3 ,alpha=1.0)



		self.ax2 = self.fig.add_subplot(212)
		self.ax2.spines['right'].set_visible(False)
		self.ax2.spines['top'].set_visible(False)
		self.ax2.yaxis.set_ticks_position('left')
		self.ax2.xaxis.set_ticks_position('bottom')
		self.ax2.yaxis.set_major_locator(ticker.MaxNLocator(4))
		self.ax2.set_ylabel(_("Suns")+" (Suns)")
		self.ax2.set_xlabel(_("Time")+" ("+unit+')')
		self.ax2.grid(True)
		sun, = self.ax2.plot(time,self.sun, 'go-', linewidth=3 ,alpha=1.0)


		laser, = self.ax2.plot(time,self.laser, 'bo-', linewidth=3 ,alpha=1.0)

		fs_laser_enabled=False
		if fs_laser_time!=-1:
			if len(self.time)>2:
				dt=(self.time[len(time)-1]-self.time[0])/100
				start=fs_laser_time-dt*5
				stop=fs_laser_time+dt*5
				x = linspace(start,stop,100)
				y=self.gaussian(x,fs_laser_time,dt)
				#print y

				fs_laser, = self.ax2.plot(x*mul,y, 'g-', linewidth=3 ,alpha=1.0)
				fs_laser_enabled=True
				self.ax2.ticklabel_format(style='sci', axis='x', scilimits=(0,0))

		self.fig.subplots_adjust(hspace=0)


	def image_save(self):
		response=save_as_image(self)
		if response != None:
			file_name=response

			if os.path.splitext(file_name)[1]:
				self.fig.savefig(file_name)
			else:
				filter=response
				self.fig.savefig(file_name+".png")


	def build_mesh(self):

		self.laser=[]
		self.sun=[]
		self.voltage=[]
		self.time=[]
		self.fs_laser=[]
		data=gpvdm_data().time_domain.find_object_by_id(self.uid)
		mesh=data.mesh
		pos=data.config.start_time
		fired=False

		laser_pulse_width=0.0


		sun_steady_state=float(gpvdm_data().light.Psun)

		voltage_bias=data.config.pulse_bias
		fs_laser_time=data.config.fs_laser_time

		for seg in mesh.segments:
			length=seg.len
			end_time=pos+length

			dt=seg.dt
			if dt<=0.0:
				dt=length/10

			voltage_start=seg.voltage_start
			voltage_stop=seg.voltage_stop

			laser_start=seg.laser_start
			laser_stop=seg.laser_stop

			sun_start=seg.sun_start
			sun_stop=seg.sun_stop

			mul=seg.mul
			if mul<=0:
				mul=1.0

			#print("VOLTAGE=",line[SEG_VOLTAGE],end_time,pos)

			if (length/dt)>100:
				dt=length/100

			voltage=voltage_start
			laser=laser_start
			sun=sun_start
			while(pos<end_time):
				dv=(voltage_stop-voltage_start)*(dt/length)
				dlaser=(laser_stop-laser_start)*(dt/length)
				dsun=(sun_stop-sun_start)*(dt/length)
				self.time.append(pos)
				self.laser.append(laser)
				self.sun.append(sun)
				self.voltage.append(voltage+voltage_bias)
				self.fs_laser.append(0.0)
				pos=pos+dt
				voltage=voltage+dv
				laser=laser+dlaser
				sun=sun+dsun

				if fired==False:
					if pos>fs_laser_time and fs_laser_time!=-1:
						fired=True
						self.fs_laser[len(self.fs_laser)-1]=laser_pulse_width/dt

				dt=dt*mul


	def callback_start_time(self):
		data=gpvdm_data().time_domain.find_object_by_id(self.uid)
		new_time=dlg_get_text( _("Enter the start time of the simulation"), str(data.config.start_time),"start.png")
		new_time=new_time.ret

		if new_time!=None:
			data.config.start_time=float(new_time)
			self.build_mesh()
			self.draw_graph()
			self.fig.canvas.draw()
			gpvdm_data().save()


	def __init__(self,uid):
		self.uid=uid
		QWidget.__init__(self)
		self.main_vbox = QVBoxLayout()
		self.time=[]
		self.voltage=[]
		self.sun=[]
		self.laser=[]

		self.edit_list=[]
		self.line_number=[]
		self.list=[]

		self.fig = Figure(figsize=(5,4), dpi=100)
		self.canvas = FigureCanvas(self.fig)
		self.canvas.figure.patch.set_facecolor('white')

		self.ax1=None
		self.show_key=True

		self.main_vbox.addWidget(self.canvas)

		#toolbar 2

		toolbar2=QToolBar()
		toolbar2.setIconSize(QSize(32, 32))

		self.main_vbox.addWidget(toolbar2)
	
		self.tab = gpvdm_tab2(toolbar=toolbar2)
		self.tab.set_tokens(["len","dt","voltage_start","voltage_stop","mul","sun_start","sun_stop","laser_start","laser_stop"])
		self.tab.set_labels([_("Length"),_("dt"), _("Start Voltage"), _("Stop Voltage"), _("step multiplyer"), _("Sun start"), _("Sun stop"),_("Laser start"),_("Laser stop")])

		self.tab.setMinimumSize(self.width(), 120)

		data=gpvdm_data().time_domain.find_object_by_id(self.uid)
		index=gpvdm_data().time_domain.segments.index(data)
		self.tab.json_search_path="gpvdm_data().time_domain.segments["+str(index)+"].mesh.segments"
		self.tab.base_obj=json_time_domain_mesh_segment()

		self.tab.populate()
		
		self.tab.changed.connect(self.on_cell_edited)

		self.build_mesh()
		self.draw_graph()

		self.main_vbox.addWidget(self.tab)


		self.setLayout(self.main_vbox)




