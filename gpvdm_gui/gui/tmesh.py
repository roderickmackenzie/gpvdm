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

## @package tmesh
#  A mesh editor for the time domain mesh.
#


import os
from numpy import *
from gui_util import dlg_get_text
import webbrowser
from util import time_with_units
from cal_path import get_icon_path
from code_ctrl import enable_betafeatures
from scan_human_labels import get_scan_human_labels

#inp
from inp import inp_load_file
from inp import inp_read_next_item
from inp import inp_search_token_value
from inp import inp_get_token_value
from inp import inp_save

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
from gpvdm_tab import gpvdm_tab

import i18n
_ = i18n.language.gettext


class tab_time_mesh(QWidget):

	def save_data(self):
		out_text=[]
		out_text.append("#start_time")
		out_text.append(str(float(self.start_time)))
		out_text.append("#fs_laser_time")
		out_text.append(str(float(self.fs_laser_time)))
		out_text.append("#time_segments")
		out_text.append(str(self.tab.rowCount()))

		for i in range(0,self.tab.rowCount()):
			out_text.append("#time_segment"+str(i)+"_len")
			out_text.append(str(self.tab.get_value(i, 0)))

			out_text.append("#time_segment"+str(i)+"_dt")
			out_text.append(str(self.tab.get_value(i, 1)))

			out_text.append("#time_segment"+str(i)+"_voltage_start")
			out_text.append(str(self.tab.get_value(i, 2)))

			out_text.append("#time_segment"+str(i)+"_voltage_stop")
			out_text.append(str(self.tab.get_value(i, 3)))

			out_text.append("#time_segment"+str(i)+"_mul")
			out_text.append(str(self.tab.get_value(i, 4)))

			out_text.append("#time_segment"+str(i)+"_sun")
			out_text.append(str(self.tab.get_value(i, 5)))

			out_text.append("#time_segment"+str(i)+"_laser")
			out_text.append(str(self.tab.get_value(i, 6)))

		out_text.append("#ver")
		out_text.append("1.1")
		out_text.append("#end")

		inp_save(os.path.join(get_sim_path(),self.file_name),out_text)
		self.update_scan_tokens()

	def update_scan_tokens(self):
		self.scan_human_labels.remove_file(self.file_name)

		for i in range(0,len(self.list)):
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_len",_("Part ")+str(i)+_(" period"))
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_dt",_("Part ")+str(i)+_(" dt"))
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_voltage_start",_("Part ")+str(i)+_(" start voltage"))
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_voltage_stop",_("Part ")+str(i)+_(" stop voltage"))
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_mul",_("Part ")+str(i)+_(" mul"))
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_sun",_("Part ")+str(i)+_(" Sun"))
			self.scan_human_labels.add_item(self.file_name,"#time_segment"+str(i)+"_laser",_("Part ")+str(i)+_(" CW laser"))


	def callback_add_section(self):
		self.tab.add(["10e-6","0.1e-6","0.0","0.0","1.0","0.0","0.0"])

		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_remove_item(self):
		self.tab.remove()

		self.build_mesh()

		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_move_down(self):
		self.tab.move_down()
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_move_up(self):
		self.tab.move_up()
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()
		self.save_data()

	def callback_laser(self):
		new_time=dlg_get_text( _("Enter the time at which the laser pulse will fire (-1) to turn it off"), str(self.fs_laser_time),"laser.png")
		new_time=new_time.ret

		if new_time!=None:
			self.fs_laser_time=float(new_time)
			self.build_mesh()
			self.draw_graph()
			self.fig.canvas.draw()
			self.save_data()

	def update(self):
		self.build_mesh()
		self.draw_graph()
		self.fig.canvas.draw()

	def on_cell_edited(self, x,y):
		if self.check_mesh()==False:
			error_dlg(self,_("You have entered a non numeric value."))
		else:
			self.save_data()
			self.build_mesh()
			self.draw_graph()
			self.fig.canvas.draw()

	def gaussian(self,x, mu, sig):
		return exp(-power(x - mu, 2.) / (2 * power(sig, 2.)))

	def draw_graph(self):
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


		if enable_betafeatures()==True:
			laser, = self.ax2.plot(time,self.laser, 'bo-', linewidth=3 ,alpha=1.0)

		fs_laser_enabled=False
		if self.fs_laser_time!=-1:
			if len(self.time)>2:
				dt=(self.time[len(time)-1]-self.time[0])/100
				start=self.fs_laser_time-dt*5
				stop=self.fs_laser_time+dt*5
				x = linspace(start,stop,100)
				y=self.gaussian(x,self.fs_laser_time,dt)
				#print y

				fs_laser, = self.ax2.plot(x*mul,y, 'g-', linewidth=3 ,alpha=1.0)
				fs_laser_enabled=True
				self.ax2.ticklabel_format(style='sci', axis='x', scilimits=(0,0))

		self.fig.subplots_adjust(hspace=0)
		#if enable_betafeatures()==True:
		#	if fs_laser_enabled==True:
		#		self.fig.legend((voltage, sun, laser,fs_laser), (_("Voltage"), _("Sun"), _("CW laser"), _("fs laser")), 'upper right')
		#	else:
		#		self.fig.legend((voltage, sun, laser), (_("Voltage"), _("Sun"), _("CW laser")), 'upper right')
		#else:
		#	if fs_laser_enabled==True:
		#		self.fig.legend((voltage, sun, fs_laser), (_("Voltage"), _("Sun"), _("fs laser")), 'upper right')
		#	else:
		#		self.fig.legend((voltage, sun), (_("Voltage"), _("Sun")), 'upper right')

	def image_save(self):
		response=save_as_image(self)
		if response != None:
			file_name=response

			if os.path.splitext(file_name)[1]:
				self.fig.savefig(file_name)
			else:
				filter=response
				self.fig.savefig(file_name+".png")



	def load_data(self):
		self.tab.setColumnCount(7)
		self.tab.clear()
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)

		lines=[]
		self.start_time=0.0
		self.fs_laser_time=0.0
		self.list=[]

		self.tab.setHorizontalHeaderLabels([_("Length"),_("dt"), _("Start Voltage"), _("Stop Voltage"), _("step multiplyer"), _("Suns"),_("Laser")])

		print("loading",self.file_name)
		lines=inp_load_file(self.file_name)
		if lines!=False:
			if inp_search_token_value(lines, "#ver")=="1.1":
				pos=0
				token,value,pos=inp_read_next_item(lines,pos)
				self.start_time=float(value)

				token,value,pos=inp_read_next_item(lines,pos)
				self.fs_laser_time=float(value)

				token,value,pos=inp_read_next_item(lines,pos)
				segments=int(value)

				for i in range(0, segments):
					token,length,pos=inp_read_next_item(lines,pos)
					token,dt,pos=inp_read_next_item(lines,pos)
					token,voltage_start,pos=inp_read_next_item(lines,pos)
					token,voltage_stop,pos=inp_read_next_item(lines,pos)
					token,mul,pos=inp_read_next_item(lines,pos)
					token,sun,pos=inp_read_next_item(lines,pos)
					token,laser,pos=inp_read_next_item(lines,pos)

					self.tab.add([str(length),str(dt),str(voltage_start),str(voltage_stop),str(mul),str(sun),str(laser)])


				return True
			else:
				print("file "+self.file_name+"wrong version")
				exit("")
				return False
		else:
			print("file "+self.file_name+" not found")
			return False

		return False

	def check_mesh(self):
		try:
			for i in range(0,self.tab.rowCount()):
				length=float(self.tab.get_value(i, 0))
				dt=float(self.tab.get_value(i, 1))
				voltage_start=float(self.tab.get_value(i, 2))
				voltage_stop=float(self.tab.get_value(i, 3))
				mul=float(self.tab.get_value(i, 4))
				sun=float(self.tab.get_value(i, 5))
				laser=float(self.tab.get_value(i, 6))
			return True
		except:
			return False

	def build_mesh(self):
		self.laser=[]
		self.sun=[]
		self.voltage=[]
		self.time=[]
		self.fs_laser=[]
		pos=self.start_time
		fired=False

		laser_pulse_width=0.0


		sun_steady_state=float(inp_get_token_value(os.path.join(get_sim_path(),"light.inp"), "#Psun"))

		voltage_bias=float(inp_get_token_value(os.path.join(get_sim_path(),"pulse"+str(self.index)+".inp"), "#pulse_bias"))


		seg=0
		for i in range(0,self.tab.rowCount()):
			length=float(self.tab.get_value(i, 0))
			end_time=pos+length
			dt=float(self.tab.get_value(i, 1))
			voltage_start=float(self.tab.get_value(i, 2))
			voltage_stop=float(self.tab.get_value(i, 3))
			mul=float(self.tab.get_value(i, 4))
			sun=float(self.tab.get_value(i, 5))
			laser=float(self.tab.get_value(i, 6))
			#print("VOLTAGE=",line[SEG_VOLTAGE],end_time,pos)

			if (length/dt)>100:
				dt=length/100

			if dt!=0.0 and mul!=0.0:
				voltage=voltage_start
				while(pos<end_time):
					dv=(voltage_stop-voltage_start)*(dt/length)
					self.time.append(pos)
					self.laser.append(laser)
					self.sun.append(sun+sun_steady_state)
					self.voltage.append(voltage+voltage_bias)
					#print(seg,voltage)
					self.fs_laser.append(0.0)
					pos=pos+dt
					voltage=voltage+dv

					if fired==False:
						if pos>self.fs_laser_time and self.fs_laser_time!=-1:
							fired=True
							self.fs_laser[len(self.fs_laser)-1]=laser_pulse_width/dt

					dt=dt*mul

			seg=seg+1

		#print(self.voltage)

		#self.statusbar.push(0, str(len(self.time))+_(" mesh points"))

	def callback_start_time(self):
		new_time=dlg_get_text( _("Enter the start time of the simulation"), str(self.start_time),"start.png")
		new_time=new_time.ret

		if new_time!=None:
			self.start_time=float(new_time)
			self.build_mesh()
			self.draw_graph()
			self.fig.canvas.draw()
			self.save_data()


	def __init__(self,index):
		self.index=index
		self.file_name=os.path.join(get_sim_path(),"time_mesh_config"+str(self.index)+".inp")
		self.scan_human_labels=get_scan_human_labels()
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

		#toolbar=QToolBar()
		#toolbar.setIconSize(QSize(48, 48))

		#nav_bar=NavigationToolbar(self.canvas,self)
		#toolbar.addWidget(nav_bar)


		#self.main_vbox.addWidget(toolbar)



		#canvas.set_size_request(500, 150)

		self.ax1=None
		self.show_key=True

		self.main_vbox.addWidget(self.canvas)

		#self.canvas.show()

		#toolbar 2

		toolbar2=QToolBar()
		toolbar2.setIconSize(QSize(32, 32))

		self.tb_add = QAction(icon_get("list-add"), _("Add section"), self)
		self.tb_add.triggered.connect(self.callback_add_section)
		toolbar2.addAction(self.tb_add)

		self.tb_remove = QAction(icon_get("list-remove"), _("Delete section"), self)
		self.tb_remove.triggered.connect(self.callback_remove_item)
		toolbar2.addAction(self.tb_remove)

		self.tb_move = QAction(icon_get("go-down"), _("Move down"), self)
		self.tb_move.triggered.connect(self.callback_move_down)
		toolbar2.addAction(self.tb_move)

		self.tb_move_up = QAction(icon_get("go-up"), _("Move up"), self)
		self.tb_move_up.triggered.connect(self.callback_move_up)
		toolbar2.addAction(self.tb_move_up)

		self.main_vbox.addWidget(toolbar2)



		self.tab = gpvdm_tab()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)


		self.load_data()
		self.update_scan_tokens()

		self.build_mesh()
		self.draw_graph()

		self.tab.cellChanged.connect(self.on_cell_edited)

		self.main_vbox.addWidget(self.tab)

		w=self.width()

		#self.canvas.setMinimumSize(w, 400)

		self.tab.setMinimumSize(w, 120)


		self.setLayout(self.main_vbox)






