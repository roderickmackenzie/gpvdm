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

## @package hpc
#  Widget to display the status of each node on the hpc
#


import os
from icon_lib import icon_get
from status_icon import status_icon_stop

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication
from PyQt5.QtGui import QPainter,QFont,QColor,QPen,QPainterPath,QBrush
from PyQt5.QtCore import QRectF,QPoint
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QHBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QVBoxLayout,QTabWidget,QLabel,QSlider,QWidgetItem
from about import about_dlg
from error_dlg import error_dlg

from progress_class import progress_class
from server import server_get

class node_indicator(QWidget):
	def __init__(self):
		QWidget.__init__(self)
		self.name=""
		self.ip=""
		self.cpus=-1
		self.load=-1
		self.max_cpus=-1
		self.last_seen=-1

		self.setMinimumSize(300, 80)


		self.hbox=QHBoxLayout()
		
		self.bar=progress_class()
		self.bar.spinner.stop()
		self.bar.spinner.hide()
		self.label=QLabel()
		
		self.slider = QSlider(Qt.Horizontal)


		self.slider.setTickPosition(QSlider.TicksBelow)
		self.slider.setTickInterval(1)
		#self.slider.valueChanged.connect(self.slider0_change)
		self.slider.setMinimumSize(300, 80)
		self.slider.valueChanged.connect(self.slider_changed)
		self.slider.setTickPosition(QSlider.TicksBelow)
		self.hbox.addWidget(self.label)
		self.bar.hide_time()
		self.hbox.addWidget(self.bar)
		self.hbox.addWidget(self.slider)

		#self.setLayout(self.hbox)

	def block_signals(self,val):
		self.slider.blockSignals(val)
		
	def slider_changed(self):
		self.max_cpus=self.slider.value()
		self.update()
		
	def set_cpus(self,cpus):
		self.cpus=cpus
		self.slider.setMinimum(0)
		self.slider.setMaximum(cpus)

	def set_text(self,text):
		self.label.setText(text)

	def set_name(self,name):
		self.name=name		

	def set_ip(self,ip):
		self.ip=ip		

	def set_load(self,load):
		self.load=load

	def set_max_cpus(self,max_cpus):
		self.slider.setValue(max_cpus)
		self.max_cpus=max_cpus

	def set_last_seen(self,last_seen):
		self.last_seen=last_seen
		
	def update(self):
		self.repaint()


	def paintEvent(self, e):
		qp = QPainter()
		qp.begin(self)
		qp.setRenderHint(QPainter.Antialiasing)
		self.drawWidget(qp)
		qp.end()


	def drawWidget(self, qp):
		self.bar_w=400
		bar_h=20
		self.bar_start=100
		self.bar_end=self.bar_start+self.bar_w
		
		if float(self.cpus)==0:
			bar_l=0
		else:
			ratio=float(self.load)/float(self.cpus)
			if ratio>1.0:
				ratio=1.0

			bar_l=(self.bar_w-3)*ratio

		
		qp = QPainter()
		qp.begin(self)

		color = QColor(0, 0, 0)
		color.setNamedColor('#d4d4d4')
		qp.setBrush(color)
		
		path=QPainterPath()
		path.addRoundedRect(QRectF(self.bar_start, 0, self.bar_w, bar_h), 0, 0)
		qp.fillPath(path,QColor(206 , 206, 206));


		path=QPainterPath()
		path.addRoundedRect(QRectF(self.bar_start, 3, bar_l, (bar_h-6)/2), 5, 5)
		qp.fillPath(path,QColor(71 , 142, 216));

		qp.drawText(1, 60, "IP:"+self.ip)

		qp.drawText(120, 60, "Load: "+str(self.load))

		qp.drawText(200, 60, "Last seen: "+str(self.last_seen))

		qp.drawText(1,15,self.name)

		#path=QPainterPath()
		#path.addRoundedRect(QRectF(self.bar_start, (bar_h-8)/2+3, bar_cpus, (bar_h-6)/2), 5, 5)
		#qp.fillPath(path,QColor(200 , 0, 0))

		qp.end()





	def mouseReleaseEvent(self, QMouseEvent):
		x=QMouseEvent.x()
		if x>self.bar_start and x<self.bar_end:
			x=x-self.bar_start
			print(int(self.cpus*x/self.bar_w))

#		print(QMouseEvent.x(),QMouseEvent.y())

class hpc_class(QWidget):

	name=""
	cpus=[]


	def slider_changed(self, widget):
		ip=[]
		loads=[]

		for i in range(0, self.node_view_vbox.count()):
			item=self.node_view_vbox.itemAt(i).widget()
			ip.append(item.ip)
			loads.append(item.max_cpus)
		self.myserver.set_cluster_loads(ip,loads)
	
	def callback_cluster_get_info(self):
		if self.updating==False:
			self.updating=True
			#print("a",len(self.myserver.nodes),self.node_view_vbox.count())
			if len(self.myserver.nodes)>self.node_view_vbox.count():
				needed=len(self.myserver.nodes)-self.node_view_vbox.count()
				for i in range(0,needed):
					self.node_widget=node_indicator()
					self.node_widget.show()
					self.node_widget.slider.valueChanged.connect(self.slider_changed)
					self.node_view_vbox.addWidget(self.node_widget)
					print("Add widget",i)

			#print("b",len(self.myserver.nodes),self.node_view_vbox.count())
			if self.node_view_vbox.count()>len(self.myserver.nodes):
				for i in range(len(self.myserver.nodes), self.node_view_vbox.count()):
					item=self.node_view_vbox.itemAt(i).widget()
					item.deleteLater()
					print("delete",i)

			for i in range(0, self.node_view_vbox.count()):
				item=self.node_view_vbox.itemAt(i).widget()
				item.block_signals(True)
				item.set_name(self.myserver.nodes[i].name)
				item.set_ip(self.myserver.nodes[i].ip)
				item.set_cpus(int(self.myserver.nodes[i].cpus))
				item.set_load(self.myserver.nodes[i].load)
				item.set_max_cpus(int(self.myserver.nodes[i].max_cpus))
				item.set_last_seen(self.myserver.nodes[i].last_seen)

				item.update()
				item.block_signals(False)
			self.updating=False
		else:
			print("not updating")


	def __init__(self):
		QWidget.__init__(self)
		self.updating=False
		#self.setMinimumSize(900, 600)
		self.setWindowIcon(icon_get("connected"))
		self.setWindowTitle(_("Cluster status (www.gpvdm.com)")) 

		self.myserver=server_get()

		self.node_view=QWidget()
		self.node_view_vbox=QVBoxLayout()
		self.node_view.setLayout(self.node_view_vbox)

		
		self.main_vbox = QVBoxLayout()
		
		self.main_vbox.addWidget(self.node_view)

		self.node_view.show()


		self.setLayout(self.main_vbox)

		self.myserver.load_update.connect(self.callback_cluster_get_info)





