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


## @package cmp_class
#  Compare simulation results window as a function of time.
#

import os
from inp import inp_update_token_value
from inp import inp_save
from inp import inp_load_file
from inp import inp_search_token_value
from plot_widget import plot_widget
from dat_file import dat_file
from plot_io import plot_load_info
from cal_path import get_exe_command
from icon_lib import icon_get
import webbrowser

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget, QStatusBar, QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QHBoxLayout,QLabel,QComboBox
from PyQt5.QtGui import QPainter,QIcon

from snapshot_slider import snapshot_slider

from open_save_dlg import save_as_filter

from PyQt5.QtWidgets import QApplication

from help import help_window
from QWidgetSavePos import QWidgetSavePos
from cal_path import get_sim_path

from dat_file_math import dat_file_max_min
from PIL import Image
from PyQt5.QtGui import QImage
import io
from util_text import str_to_latex

class cmp_class(QWidgetSavePos):

	def update(self):
		files=[]
		types=[]
		key=[]
		file_names,graph_types=self.slider.get_file_name()
		for i in range(0,len(file_names)):
			f=file_names[i]
			if f!=None:
				files.append(f)
				key.append(str_to_latex(os.path.basename(f)))
				types.append(graph_types[i])

		if len(files)!=0:
			self.plot.load_data(files)
			self.plot.set_labels(key)
			self.plot.set_plot_types(types)
			self.plot.do_plot()

	def save_image(self,out_file_name):
		dir_name, ext = os.path.splitext(out_file_name)

		if ext==".avi":

			if os.path.isdir(dir_name)==False:
				os.mkdir(dir_name)

			jpgs=""
			fmax=self.slider.slider_max
			for i in range(0,fmax):
				self.slider.slider0.setValue(i)
				#QApplication.processEvents()
				self.update()
				#self.plot.do_plot()
				image_name=os.path.join(dir_name,"image_"+str(i)+".jpg")
				print(image_name)
				self.plot.fig.savefig(image_name)
				jpgs=jpgs+image_name+"\n"

			files_list_path=os.path.join(dir_name,"files.txt")
			f=open(files_list_path,"w")
			f.write(jpgs)
			f.close()
			
			fps=int(fmax/60)
			encode_line="mencoder mf://@"+files_list_path+" -mf type=jpg:fps="+str(fps)+" -o "+out_file_name+" -ovc x264"

			os.system(encode_line)
			
			#worked in fedora
			#os.system("mencoder "+jpgs+" -mf type=jpg:fps=1.0 -o "+file_name+" -ovc lavc -lavcopts vcodec=mpeg1video:vbitrate=800")
			#msmpeg4v2
		else:
			print("Unknown file extension")

	def callback_save(self, widget, data=None):
		file_name=save_as_filter(self,"avi (*.avi)")
		if file_name!=None:
			self.save_image(file_name)

	def callback_storyboard(self):
		#file_name=save_as_filter(self,"jpg (*.jpg)")
		#if file_name!=None:

			#dir_name, ext = os.path.splitext(file_name)

			#if os.path.isdir(dir_name)==False:
			#	os.mkdir(dir_name)

			#jpgs=[]
		number_of_images=4
		fmax=self.slider.slider_max
		step=fmax/number_of_images
		pos=0
		buf=[]
		for i in range(0,number_of_images):
			self.slider.slider0.setValue(pos)
			QApplication.processEvents()
			self.update()
			#image_name=os.path.join(dir_name,"image_"+str(i)+".jpg")
			#print(image_name)
			#self.plot.fig.savefig(image_name)
			buf.append(io.BytesIO())
			self.plot.fig.savefig(buf[-1])
			#jpgs.append(image_name)
			pos=pos+step

		rows=4
		cols=(int)(number_of_images/rows)

		image = Image.open(buf[0])
		w,h= image.size
		new_im = Image.new('RGB', (w*cols, h*rows))
		x=0
		y=0
		for i in range(0,len(buf)):
			image = Image.open(buf[i])
			w,h= image.size
			new_im.paste(image, (x,y))
			x=x+w
			print(x,w*cols)
			if x>=w*cols:
				x=0
				y=y+h

		#new_im.save(file_name)
		buf = io.BytesIO()
		new_im.save(buf,format="JPEG")
		#self.fig.savefig(buf)
		QApplication.clipboard().setImage(QImage.fromData(buf.getvalue()))
		buf.close()

	def callback_help(self, widget, data=None):
		webbrowser.open('http://www.gpvdm.com/man/gpvdm.html')


	def callback_toggle_subtract(self, widget, data):
		self.plot.zero_frame_enable=data.get_active()
		self.update(self.adj1.value)
		print("CONVERTh!!!!!!!!!!!",type(self.plot.plot_token.key_units))
		self.plot.do_plot()

	def callback_multi_plot(self, data, widget):
		if widget.get_active()==True:
			self.multi_plot=True
		else:
			self.multi_plot=False
		self.update(self.adj1.value)
		self.plot.do_plot()

	def find_snapshots(self):

		matches = []
		for root, dirnames, filenames in os.walk(get_sim_path()):
			for filename in filenames:
				my_file=os.path.join(root,filename)
				if my_file.endswith("snapshots.inp")==True:
					matches.append( os.path.dirname(my_file))

		return matches

	def callback_snapshots_combobox(self):
		self.slider.set_path(self.snapshots_combobox.currentText())


	def __init__(self,path=None):
		QWidgetSavePos.__init__(self,"cmpclass")
		self.snapshots_widget=None
		self.setWindowTitle(_("Examine simulation results in time domain")) 
		self.setWindowIcon(icon_get("cover_flow"))

		self.snapshot_dirs=[]

		if path==None:
			self.snapshots_hbox = QHBoxLayout()
			self.snapshots_label= QLabel("Snapshots")
			self.snapshots_hbox.addWidget(self.snapshots_label)
			self.snapshots_combobox=QComboBox()
			self.snapshots_hbox.addWidget(self.snapshots_combobox)
			self.snapshots_widget=QWidget()
			self.snapshot_dirs=self.find_snapshots()
			for i in range(0,len(self.snapshot_dirs)):
				self.snapshots_combobox.addItem(self.snapshot_dirs[i])
			self.snapshots_combobox.currentIndexChanged.connect(self.callback_snapshots_combobox)
			self.snapshots_widget.setLayout(self.snapshots_hbox)

		else:
			self.snapshot_dirs.append(path)

		self.main_vbox = QVBoxLayout()

		self.slider=snapshot_slider()


		if len(self.snapshot_dirs)!=0:
			self.slider.set_path(os.path.join(get_sim_path(),self.snapshot_dirs[0]))


		self.slider.changed.connect(self.update)
		self.plot=plot_widget()

		self.tb_video = QAction(icon_get("video"), _("Save\nvideo"), self)
		self.tb_video.triggered.connect(self.callback_save)
		self.plot.plot_ribbon.file_toolbar.addAction(self.tb_video)

		self.tb_storyboard = QAction(icon_get("storyboard"), _("Storyboard\nto clipboard"), self)
		self.tb_storyboard.triggered.connect(self.callback_storyboard)
		self.plot.plot_ribbon.file_toolbar.addAction(self.tb_storyboard)


		self.plot.plot_ribbon.plot_toolbar.addAction(self.slider.tb_play)




		self.main_vbox.addWidget(self.plot)


		if self.snapshots_widget!=None:
			self.main_vbox.addWidget(self.snapshots_widget)
		
		self.main_vbox.addWidget(self.slider)

		self.status_bar=QStatusBar()
		self.status_bar.showMessage(self.snapshot_dirs[0])
		self.main_vbox.addWidget(self.status_bar)

		self.setLayout(self.main_vbox)

		#if os.path.isfile(os.path.join(get_sim_path(),"snapshots","0","Ec.dat"))==False:
		#	help_window().help_append(["warning.png",_("No electrical slice data has been stored in the snapshots directory.  To turn this on set Simulation->Configure->Dump->Dump 1D Slices to on.  This will dump a lot of data and slow down your simulations.")])
		
		#self.light.currentIndexChanged.connect(self.call_back_light_changed)

		self.update()

	def closeEvent(self, event):
		self.slider.anim_stop()
		event.accept()

