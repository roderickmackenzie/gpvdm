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

## @package gpvdm_viewer
#  A directory/file browser for gpvdm.
#


import os
from safe_delete import gpvdm_delete_file
from dat_file import dat_file

#qt
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QSize, Qt, QTimer
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QMenu,QAbstractItemView,QListWidgetItem,QPushButton,QListView,QWidget,QListWidget,QAction,QDialog
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal

#cal_path
from icon_lib import icon_get
from cal_path import get_ui_path

from help import help_window

from error_dlg import error_dlg

from bibtex import bibtex
from gui_util import dlg_get_text
from gui_util import yes_no_dlg


from inp import inp_get_token_value
from inp import inp_get_token_value_from_list
from inp import inp

from util import isfiletype
from win_lin import desktop_open

from str2bool import str2bool

from plot_gen import plot_gen

from util_zip import zip_lsdir
from util_zip import read_lines_from_archive

import webbrowser
from window_json_ro_viewer import window_json_ro_viewer

import psutil

from icon_lib import icon_get

from cal_path import get_home_path
from cal_path import get_desktop_path
from cal_path import get_videos_path
from cal_path import get_downloads_path
from cal_path import get_sim_path

from win_lin import running_on_linux
from util_zip import archive_compress
from util_zip import archive_decompress
from dir_decode import get_dir_type

from progress_class import progress_class
from process_events import process_events
import shutil

COL_PATH = 0
COL_PIXBUF = 1
COL_IS_DIRECTORY = 2

import i18n
_ = i18n.language.gettext

#util
from util import latex_to_html
from util import peek_data
from multiplot import multiplot
from scans_io import scans_io
import operator
from scan_io import scan_io
from gpvdm_json import gpvdm_data
from search import find_shapshots
from gpvdm_viewer_new import gpvdm_viewer_new
from file_store import file_store

class gpvdm_viewer(QListWidget,gpvdm_viewer_new):

	accept = pyqtSignal()
	reject = pyqtSignal()
	path_changed = pyqtSignal()
	selection_changed = pyqtSignal()

	def keyPressEvent(self, ev):
		if ev.key() in (Qt.Key_Enter, Qt.Key_Return):
			self.on_item_activated(self.currentItem())
			ev.accept()
		else:
			return QListWidget.keyPressEvent(self, ev)

	def dragEnterEvent(self, event):
		#self.setText("<drop content>")
	#	print("c")
		#self.setBackgroundRole(QtGui.QPalette.Highlight)
		event.acceptProposedAction()
		#self.changed.emit(event.mimeData())

	def dragMoveEvent(self, event):
		#print("b")
		event.acceptProposedAction()

	def dropEvent(self, event):
		mimeData = event.mimeData()

		if mimeData.hasUrls():
			a=[url.path() for url in mimeData.urls()]
			print("d",a)

#		self.setBackgroundRole(QtGui.QPalette.Dark)
		event.acceptProposedAction()


	def dropMimeData(self, data, action, row, column, parent):
		print()
#		print(data)

	def __init__(self,path,show_inp_files=True,open_own_files=True):
		QWidget.__init__(self)
		gpvdm_viewer_new.__init__(self)
		self.setAcceptDrops(True)
		self.setDragEnabled(True)
		self.setDragDropMode(QAbstractItemView.DragDrop)
		self.open_own_files=open_own_files
		self.file_list=[]
		self.menu_new_material_enabled=False
		self.menu_new_spectra_enabled=False
		self.show_inp_files=show_inp_files
		self.show_directories=True
		self.file_path=""
		self.show_back_arrow=False

		self.setStyleSheet("margin: 0; padding: 0; ")

		self.show_hidden=False
		self.enable_menu=True
		self.path=""
		self.allow_navigation=False

		self.set_path(path)
		self.root_dir= self.path
		self.windows=[]
	
		self.show_only=[]

		self.setIconSize(QSize(64,64))

		
		self.fill_store()

		self.itemDoubleClicked.connect(self.on_item_activated)
		self.setContextMenuPolicy(Qt.CustomContextMenu)
		self.itemSelectionChanged.connect(self.on_selection_changed)
		self.customContextMenuRequested.connect(self.callback_menu)
		self.resizeEvent=self.resizeEvent
		self.selected=[]
		self.snapshot_window=[]
		#self.show()

	def set_back_arrow(self,data):
		self.show_back_arrow=data

	def set_show_hidden(self,data):
		self.show_hidden=data

	def set_multi_select(self):
		self.setSelectionMode(QAbstractItemView.ExtendedSelection)

	def set_enable_menu(self,data):
		self.enable_menu=data

	def set_grid_view(self,height=90):
		self.setIconSize(QSize(64,64))
		self.setViewMode(QListView.IconMode)
		self.setSpacing(8)
		self.setWordWrap(True)
		self.setTextElideMode ( Qt.ElideNone)
		gridsize=self.size()
		gridsize.setWidth(100)
		gridsize.setHeight(height)
		self.setGridSize(gridsize)

	def set_list_view(self):
		self.setViewMode(QListView.ListMode)
		self.setIconSize(QSize(32,32))
		gridsize=self.size()
		gridsize.setWidth(100)
		gridsize.setHeight(40)
		self.setGridSize(gridsize)

	def set_directory_view(self,data):
		if data==True:
			self.set_grid_view()


	def callback_menu(self,event):
		if self.enable_menu==False:
			return
		menu = QMenu(self)
		unpack_action=False
		pack_action=False

		menu_new = menu.addMenu(_("New"))

		newdirAction = menu_new.addAction(icon_get("folder"),_("New directory"))
		menu_new.addSeparator()
		menu_new_material = menu_new.addAction(icon_get("organic_material"),_("New material"))
		menu_new_material.triggered.connect(self.new_material)

		menu_new_sepctra = menu_new.addAction(icon_get("spectra_file"),_("New spectra"))
		menu_new_sepctra.triggered.connect(self.new_spectra)

		menu_new_emission = menu_new.addAction(icon_get("emission"),_("PL/EL spectra"))
		menu_new_emission.triggered.connect(self.new_emission)

		menu_new_shape = menu_new.addAction(icon_get("shape"),_("New shape"))
		menu_new_shape.triggered.connect(self.new_shape)

		menu_new_filter = menu_new.addAction(icon_get("filter_wheel"),_("New filter"))
		menu_new_filter.triggered.connect(self.new_filter)

		deleteAction = menu.addAction(_("Delete file"))
		renameAction = menu.addAction(_("Rename"))
		cloneAction = menu.addAction(_("Copy"))
		cleanAction = menu.addAction(_("Clean"))
		renameAction.setEnabled(False)
		deleteAction.setEnabled(False)
		cloneAction.setEnabled(False)
		if len(self.selectedItems())==1:
			renameAction.setEnabled(True)
			cloneAction.setEnabled(True)

		items=self.selectedItems()
		if len(items)>0:
			deleteAction.setEnabled(True)
			
			full_file_name=os.path.join(self.path,self.decode_name(items[0].text()).file_name)
			if full_file_name.endswith(".gpvdm"):
				menu.addSeparator()

				pack_action = menu.addAction(_("Pack archive"))
				unpack_action = menu.addAction(_("Unpack archive"))

		action = menu.exec_(self.mapToGlobal(event))

		if action == pack_action:
			archive_compress(full_file_name)
		elif action == unpack_action:
			archive_decompress(full_file_name,remove_gpvdm_file=False)
			
		elif action == newdirAction:
			new_sim_name=dlg_get_text( _("New directory name:"), _("New directory"),"document-new")
			new_sim_name=new_sim_name.ret

			if new_sim_name!=None:
				name=os.path.join(self.path,new_sim_name)
				os.mkdir(name)
		elif action == deleteAction:
			self.delete()
		elif action == cloneAction:
			self.clone()
		elif action == renameAction:
			self.rename()
		elif action == cleanAction:
			self.clean()

		self.fill_store()

	def clone(self):
		old_name=self.currentItem().text()
		decode=self.decode_name(old_name)
		if decode.type=="scan_dir":
			new_sim_name=dlg_get_text( _("Clone the file to be called:"), old_name+"_new","clone.png")
			new_sim_name=new_sim_name.ret

			if new_sim_name!=None:
				scans=scans_io(self.path)
				if scans.clone(new_sim_name,old_name)==False:
					error_dlg(self,_("The file name already exists"))
		else:
			new_sim_name=dlg_get_text( _("Clone the file to be called:"), old_name+"_new","clone.png")
			new_sim_name=new_sim_name.ret

			if new_sim_name!=None:
				if os.path.isdir(os.path.join(self.path,new_sim_name))==False:
					shutil.copytree(os.path.join(self.path,old_name), os.path.join(self.path,new_sim_name))

		self.fill_store()

	def rename(self):
		old_name=self.currentItem().text()
		decode=self.decode_name(old_name)
		if decode.type=="scan_dir":
			new_sim_name=dlg_get_text( _("Rename the simulation to be called:"), old_name,"rename.png")
			new_sim_name=new_sim_name.ret

			if new_sim_name!=None:
				scan=scan_io()
				print(self.path,old_name)
				scan.load(os.path.join(self.path,decode.file_name))
				scan.rename(new_sim_name)
		else:
			new_sim_name=dlg_get_text( _("Rename:"), self.currentItem().text(),"rename")
			new_sim_name=new_sim_name.ret

			if new_sim_name!=None:
				new_name=os.path.join(self.path,new_sim_name)
				old_name=os.path.join(self.path,old_name)
				#print(old_name, new_name)
				os.rename(old_name, new_name)

	def clean(self):
		progress_window=progress_class()
		progress_window.show()
		progress_window.start()

		process_events()

		print("searching")
		files=find_shapshots(self.path)
		if len(files)<20:
			disp="\n".join(files)
		else:
			disp="\n".join(files[:20])+"..."

		ret=yes_no_dlg(self,_("Are you sure you want to delete the files ?")+"\n\n"+disp)
		if ret==True:
			i=0
			for f in files:
				progress_window.set_fraction(float(i)/float(len(files)))
				progress_window.set_text("Deleting: "+f)
				process_events()

				gpvdm_delete_file(f,allow_dir_removal=True)

				i=i+1

		progress_window.stop()

	def delete(self):
		files=""
		for i in self.selectedItems():
			files=files+os.path.join(self.path,i.text())+"\n"
		ret=yes_no_dlg(self,_("Are you sure you want to delete the files ?")+"\n\n"+files)
		if ret==True:
			for i in self.selectedItems():
				decode=self.decode_name(i.text())
				if decode.type=="scan_dir":
					scans=scans_io(self.path)
					scans.delete(decode.display_name)
				else:
					file_to_remove=os.path.join(self.path,decode.file_name)
					gpvdm_delete_file(file_to_remove,allow_dir_removal=True)

	def resizeEvent(self,resizeEvent):
		self.fill_store()

	def get_icon(self, name):
		return icon_get(name+"_file")

	def get_filename(self):
		return self.file_path


	def set_path(self,path):
		self.path=path		
		self.path_changed.emit()

	def add_back_arrow(self):
		if self.show_back_arrow==True:
			if self.path==self.root_dir and self.allow_navigation==False:
				return

			if self.path!="/gpvdmroot":
				itm = QListWidgetItem( ".." )
				itm.setIcon(icon_get('go-previous'))
				self.addItem(itm)

	#builds virtule list of files visible to user
	def listdir(self):
		ret=[]

		if self.path=="/gpvdmroot":
			itm=file_store()
			itm.file_name="simulation_dir"
			itm.icon="si"
			itm.display_name=_("Simulation")
			ret.append(itm)

			itm=file_store()
			itm.file_name="home_dir"
			itm.icon="user-home"
			itm.display_name=_("Home")
			ret.append(itm)

			if get_desktop_path()!=False:
				itm=file_store()
				itm.file_name="desktop_dir"
				itm.icon="desktop"
				itm.display_name=_("Desktop")
				ret.append(itm)

			if get_downloads_path()!=False:
				itm=file_store()
				itm.file_name="downloads_dir"
				itm.icon="folder-download"
				itm.display_name=_("Downloads")
				ret.append(itm)

			itm=file_store()
			itm.file_name="gpvdm_configure"
			itm.icon="cog"
			itm.display_name=_("Configure")
			ret.append(itm)

			for p in psutil.disk_partitions():
				name=p.mountpoint
				if running_on_linux()==True:
					name=os.path.basename(name)

				if name=="":
					name="/"
				itm=file_store()
				itm.file_name="mount_point::::"+p.mountpoint
				itm.icon="drive-harddisk"
				itm.display_name=name
				ret.append(itm)

		elif self.path=="/gpvdmroot/icons/":
			from icon_lib import icon_get_db
			ilib=icon_get_db()
			for item in ilib.db:
				itm=file_store()
				itm.file_name=item.name[0]
				itm.icon=item.name[0]
				itm.display_name=item.name[0]
				ret.append(itm)

		elif self.path=="/gpvdmroot/gpvdm_configure":
			pass


		else:
			files=os.listdir(self.path)
			for f in files:
				itm=get_dir_type(os.path.join(self.path,f))
				if itm!=None:
					add=True
					if get_sim_path()==self.path:
						if f=="sim.gpvdm":
							add=False
					if itm.type=="scan_dir":
						add=False

					if add==True:
						ret.append(itm)

			if get_sim_path()==self.path:
				scan=scans_io(get_sim_path())
				scans=scan.get_scans()
				for s in scans:
					for i in range(0,len(ret)):
						if ret[i].file_name==s.human_name:
							ret.pop(i)
							break

					itm=file_store()
					itm.type="scan_dir"
					itm.isdir=True
					itm.file_name=os.path.basename(s.config_file)
					itm.display_name=s.human_name
					ret.append(itm)

		ret=sorted(ret, key=operator.attrgetter('display_name'))
		#files = sorted(files, key=operator.attrgetter('file_name'))
		return ret

	def fill_store(self):

		self.file_list=[]

		path=self.path

		all_files=self.listdir()
		#all_files.sort()
		if path.endswith("dynamic")==True:
			self.set_list_view()
		elif path.endswith("device_lib")==True:
			self.set_grid_view(height=120)
		else:
			self.set_grid_view()

		for itm in all_files:
			#if it is a directory
			file_name=os.path.join(path, itm.file_name)

			if itm.isdir==True:
				if itm.type=="spectra":
					itm.icon="spectra"
				elif itm.type=="shape":
					itm.icon="shape"
				elif itm.type=="light":
					itm.icon="optics2"
				elif itm.type=="material":
					itm.icon="organic_material"
				elif itm.type=="emission":
					itm.icon="emission"
				elif itm.type=="filter":
					itm.icon="filter_wheel"
				elif itm.type=="backup_main":
					itm.icon="backup"
				elif itm.type=="multi_plot_dir":
					itm.icon="multi_plot_dir"
				elif itm.type=="backup":
					itm.icon="backup"
				elif itm.type=="scan_dir":
					itm.icon="scan"
				elif itm.type=="parameter_dir":
					itm.icon="star"
				elif itm.type=="cache":
					itm.hidden=True

			else:
				#append=False

				ext=os.path.splitext(file_name)
				if len(ext)>1:
					ext=ext[1].lower()
				else:
					ext=""

				if ext==".dat" or ext==".csv":
					text=peek_data(file_name)
					if itm.file_name in ["theta_small_x.dat","theta_small_z.dat","theta_Y.dat", "theta_small_y.dat","theta_X.dat", "theta_Z.dat", "theta_RGB.dat"]:

						itm.icon="color"
					elif text.startswith(b"#multiplot"):
						itm.icon="multiplot"
					elif str(text).count("#type poly")>0:
						itm.icon="vector"
					elif str(text).count("#type zxy-d")>0:
						itm.icon="data_3d"
					elif str(text).count("#gobj")>0:
						itm.icon="vector"
					elif text.startswith(b"#"):
						itm.icon="dat_file"
					elif text.startswith(b"{"):
						itm.icon="json_file"
					else:
						itm.hidden=True

				elif (ext==".chk"):
					itm.hidden=True

				elif (ext==".inp"):
					if self.show_inp_files==True:
						itm.icon="text-x-generic"
					else:
						itm.hidden=True

				elif (ext==".gmat"):
						itm.icon="gmat"

				elif (ext==".m"):
						itm.icon="gpvdm_matlab"

				elif os.path.basename(file_name)=="sim_info.dat":
					itm.icon="info"

				elif file_name.endswith("default.gpvdm")==False and file_name.endswith(".gpvdm"):
					text=peek_data(file_name)
					if text.startswith(b"gpvdmenc"):
						itm.hidden=True
						itm.icon="sim_lock"
					else:
						f=inp()
						if f.load_json("sim.json",archive=file_name)!=False:
							itm.display_name=f.json['english_name']
							icon_name=f.json['icon']
							itm.icon=icon_name
							itm.hidden=False

							a=zip_lsdir(file_name,sub_dir="fs/") #,zf=None,sub_dir=None
							if len(a)!=0:
								for fname in a:
									lines=ret=read_lines_from_archive(file_name,"fs/"+fname)
									if lines!=False:
										web_link=inp_get_token_value_from_list(lines, "#web_link")
										name=inp_get_token_value_from_list(lines, "#name")
										sub_itm=file_store()
										sub_itm.icon="internet-web-browser"
										sub_itm.display_name=name
										sub_itm.file_name=web_link
										sub_itm.hidden=False
										self.file_list.append(sub_itm)

				if itm.icon=="":
					if icon_get(ext)!=False:	
						itm.icon=ext
					else:
						itm.icon="misc"

			if itm.display_name=="":
				itm.display_name=itm.file_name

			if os.path.basename(file_name)=="data.json":
				itm.hidden=True

			if len(self.show_only)!=0:
				if itm.type not in self.show_only:
					itm.hidden=True
			
			self.file_list.append(itm)

		for i in range(0,len(self.file_list)):
			if self.file_list[i].file_name=="p3htpcbm.gpvdm":
				self.file_list.insert(0, self.file_list.pop(i))

			if self.file_list[i].file_name=="perovskite.gpvdm":
				self.file_list.insert(1, self.file_list.pop(i))

			if self.file_list[i].file_name=="ofet.gpvdm":
				self.file_list.insert(2, self.file_list.pop(i))

			if self.file_list[i].file_name=="oled.gpvdm":
				self.file_list.insert(3, self.file_list.pop(i))

		#print(">>>>>>")
		#for r in self.file_list:
		#	print(r)

		self.paint()

	def paint(self):
		self.clear()

		self.add_back_arrow()

		for i in range(0,len(self.file_list)):
			draw=True
			if self.file_list[i].file_name=="":
				draw=False
			
			if self.file_list[i].hidden==True and self.show_hidden==False:
				draw=False

			if draw==True:
				itm = QListWidgetItem( self.file_list[i].display_name )
				a=icon_get(self.file_list[i].icon)
				if a==False:
					print("icon not found",self.file_list[i].icon)
					a=icon_get("dat_file")
				itm.setIcon(a)

				#if self.file_list[i].display_name=="data.xlsx":
				#	print(itm.icon,a,self.file_list[i].icon)
				self.addItem(itm)

	def decode_name(self,display_name):
		fname=""
		for i in range(0,len(self.file_list)):
			if self.file_list[i].display_name==display_name:
				return self.file_list[i]
		return None

	def on_item_activated(self,item):
		data=gpvdm_data()
		text=item.text()
		if text=="..":
			if self.path==self.root_dir:
				self.set_path("/gpvdmroot")
			else:
				old_path=self.path
				self.set_path(os.path.dirname(self.path))
				#print(self.path,old_path,os.path.dirname(self.path))
				if old_path==self.path:
					self.set_path("/gpvdmroot")
			self.fill_store()
			return

		decode=self.decode_name(text)
		if decode==None:
			return

		full_path=os.path.join(self.path,decode.file_name)

		if decode.file_name.startswith("http"):
			webbrowser.open(decode)
			return
		elif decode.file_name=="home_dir":
			self.set_path(get_home_path())
			self.fill_store()
			return
		elif decode.file_name=="desktop_dir":
			self.set_path(get_desktop_path())
			self.fill_store()
			return
		elif decode.file_name=="gpvdm_configure":
			self.set_path("/gpvdmroot/gpvdm_configure")
			self.fill_store()
			return

		elif decode.file_name=="music_dir":
			self.set_path(get_music_path())
			self.fill_store()
			return
		elif decode.file_name=="downloads_dir":
			self.set_path(get_downloads_path())
			self.fill_store()
			return
		elif decode.file_name=="simulation_dir":
			self.set_path(get_sim_path())
			self.fill_store()
			return			
		elif decode.file_name.startswith("mount_point")==True:
			point=decode.file_name.split("::::")
			self.set_path(point[1])
			self.fill_store()
			return
		elif decode.type=="scan_dir":
			from scan_tab import scan_vbox
			#print(full_path) 
			self.scan_window=scan_vbox(full_path)
			self.scan_window.show()
			return
		elif decode.type=="parameter_dir":
			self.set_path(full_path)
			self.fill_store()
			return
		
		dir_info=get_dir_type(full_path)
		if dir_info!=None:
			if self.open_own_files==True:
				self.file_path=full_path

				if dir_info.type=="spectra":
					from spectra_main import spectra_main
					self.mat_window=spectra_main(full_path)
					self.mat_window.show()
					return
				if dir_info.type=="shape":
					from shape_editor import shape_editor
					self.windows.append(shape_editor(full_path))
					self.windows[-1].show()
					return
				if dir_info.type=="light":
					from optics import class_optical 
					self.optics_window=class_optical()
					self.optics_window.show()

				if dir_info.type=="material":
					from materials_main import materials_main
					self.mat_window=materials_main(full_path)
					self.mat_window.show()
					return

				if dir_info.type=="emission":
					from emission_main import emission_main
					self.emission_window=emission_main(full_path)
					self.emission_window.show()
					return

				if dir_info.type=="filter":
					from filter_main import filter_main
					self.filter_window=filter_main(full_path)
					self.filter_window.show()
					return

				if dir_info.type=="snapshots":
					from cmp_class import cmp_class

					help_window().help_set_help(["plot_time.png",_("<big><b>Examine the results in time domain</b></big><br> After you have run a simulation in time domain, if is often nice to be able to step through the simulation and look at the results.  This is what this window does.  Use the slider bar to move through the simulation.  When you are simulating a JV curve, the slider sill step through voltage points rather than time points.")])
					widget_mode="matplotlib"
					if text=="optical_snapshots":
						widget_mode="band_graph"
					self.snapshot_window.append(cmp_class(full_path,widget_mode=widget_mode))
					self.snapshot_window[-1].show()
					#print("snapshots!!")
					return

				if dir_info.type=="backup":
					ret=yes_no_dlg(self,_("Are you sure you want restore this file from the backup, it will overwrite all files in the simulation directory?")+"\n\n"+full_path)
					if ret==True:
						from backup import backup_restore
						backup_restore(get_sim_path(),full_path)


				if dir_info.type=="file":
					self.file_path=full_path
					if os.path.basename(full_path)=="sim_info.dat":
						self.sim_info_window=window_json_ro_viewer(full_path)
						self.sim_info_window.show()
						return

					if isfiletype(full_path,"dat")==True or isfiletype(full_path,"csv")==True:
						text=peek_data(full_path)
						if text.startswith(b"#multiplot"):
							my_multiplot=multiplot()
							my_multiplot.plot(full_path)
						else:
							plot_gen([full_path],[],"auto")
						return

					if  isfiletype(full_path,"gpvdm")==True:
						return

					desktop_open(full_path)
					return

			if dir_info.type=="dir" or dir_info.type=="backup_main" or dir_info.type=="multi_plot_dir" :
				self.file_path=full_path
				self.set_path(full_path)
				self.fill_store()
			else:
				self.accept.emit()


	def on_selection_changed(self):
		self.selected=[]

		if len(self.selectedItems())>0:
			item=self.selectedItems()[0]
			if type(item)!=None:
				obj=self.decode_name(item.text())
				if obj==None:
					return

				file_name=obj.file_name

				self.file_path=os.path.join(self.path, file_name)
	
			for item in self.selectedItems():
				obj_decode=self.decode_name(item.text())
				if obj_decode!=None:
					self.selected.append(obj_decode)

			full_path=self.file_path

			if file_name.endswith(".dat")==True:
				state=dat_file()
				state.load_only_info(full_path)
				summary="<big><b>"+file_name+"</b></big><br><br>"+_("title")+": "+state.title+"<br>"+_("x axis")+": "+state.y_label+" ("+latex_to_html(state.y_units)+")<br>"+_("y axis")+": "+state.data_label+" ("+latex_to_html(state.data_units)+")<br><br><big><b>"+_("Double click to open")+"</b></big>"
				help_window().help_set_help(["dat_file.png",summary])
			elif file_name.endswith(".csv")==True:
				state=dat_file()
				state.load_only_info(full_path)
				summary="<big><b>"+file_name+"</b></big><br><br>"+_("title")+": "+state.title+"<br>"+_("x axis")+": "+state.y_label+" ("+latex_to_html(state.y_units)+")<br>"+_("y axis")+": "+state.data_label+" ("+latex_to_html(state.data_units)+")<br><br><big><b>"+_("Double click to open")+"</b></big>"
				help_window().help_set_help(["csv.png",summary])
			if file_name.endswith("equilibrium"):
				state=dat_file()
				state.load_only_info(full_path)
				summary="<big><b>"+_("equilibrium")+"</b></big><br><br>"+_("This contains the simulation output at 0V in the dark.")
				help_window().help_set_help(["folder.png",summary])

			#if os.path.isdir(full_path)==True:
			dir_info=get_dir_type(full_path)
			if dir_info!=None:
				if dir_info.type=="material":

					summary="<b><big>"+file_name+"</b></big><br>"
					ref_path=os.path.join(full_path,"mat.bib")
					b=bibtex()
					
					if b.load(ref_path)!=False:
						summary=summary+b.get_text()
					help_window().help_set_help(["organic_material",summary])

		self.selection_changed.emit()
