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

## @package import_data_json
#  Import data window.
#


import os
from icon_lib import icon_get

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QHBoxLayout,QSpinBox,QToolBar,QSizePolicy,QAction,QTabWidget,QTextEdit,QComboBox,QLabel,QLineEdit,QDialog,QCheckBox
from PyQt5.QtGui import QPainter,QIcon

#windows
from PyQt5.QtCore import pyqtSignal

from open_save_dlg import open_as_filter

from dat_file import dat_file

from PyQt5.QtCore import pyqtSignal

from QWidgetSavePos import QWidgetSavePos

from util import wrap_text
from cal_path import get_sim_path

from ribbon_import import ribbon_import

from str2bool import str2bool
from error_dlg import error_dlg
from gpvdm_json import gpvdm_data
from QWidgetSavePos import resize_window_to_be_sane

class decoder:
	def __init__(self):
		self.input_description=""
		self.input_units=""
		self.output_description=""
		self.output_units=""
		self.equation_to_si=""
		self.mul_to_display=""
		self.need_area=""
		self.pre_eval=""
		self.index=-1

class import_data_json(QDialog):

	changed = pyqtSignal()

	def combo_to_item(self,combo):
		search=combo.currentText()
		for item in self.items:
			item_decode=item.input_description+" ("+item.input_units+")"
			if item_decode==search:
				return item
		return None

	def index_to_text(self,index):
		for item in self.items:
			if index==item.index:
				return item.input_description+" ("+item.input_units+")"

		return None

	def populate_boxes(self):
		x_combo=self.combo_to_item(self.x_combo)
		self.x_label=x_combo.output_description
		self.x_units=x_combo.output_units
		self.x_mul_to_si=x_combo.equation_to_si
		self.x_disp_mul=x_combo.mul_to_display

		data_combo=self.combo_to_item(self.data_combo)
		self.data_label=data_combo.output_description
		self.data_units=data_combo.output_units
		self.data_disp_mul=data_combo.mul_to_display

		print(x_combo.output_description,data_combo.output_description)
		self.set_xlabel(x_combo.output_description)
		self.set_data_label(data_combo.output_description)
		self.set_title(x_combo.output_description+" - "+data_combo.output_description)

		if data_combo.need_area==True or x_combo.need_area==True:
			self.area_widget.show()
		else:
			self.area_widget.hide()

	def add_units(self,box):
		for i in range(0,len(self.items)):
			box.addItem(self.items[i].input_description+" ("+self.items[i].input_units+")")

	def callback_edited(self):
		if self.disable_callbacks==True:
			return

		self.populate_boxes()
		self.save_config()
		self.update()

	def get_area(self):
		try:
			val=float(self.area_entry.text())/100/100

			if val==0:
				val=1.0
		except:
			val=1.0
		return val

	def save_config(self):
		data=gpvdm_data()
		x_combo=self.combo_to_item(self.x_combo)
		data_combo=self.combo_to_item(self.data_combo)

		self.data.import_x_combo_pos=x_combo.index
		self.data.import_data_combo_pos=data_combo.index
		self.data.import_x_spin=self.x_spin.value()
		self.data.import_data_spin=self.data_spin.value()
		self.data.import_title=self.title_entry.text()
		self.data.import_xlabel=self.xlabel_entry.text()
		self.data.import_data_label=self.data_label_entry.text()
		self.data.import_area=float(self.area_entry.text())
		self.data.import_data_invert=str2bool(self.data_invert.isChecked())
		self.data.import_x_invert=str2bool(self.x_invert.isChecked())
		data.save()

	def load_config(self):
		if os.path.isfile(self.data.import_file_path)==True:
			self.path=os.path.dirname(self.data.import_file_path)
		else:
			self.path=self.export_path

		self.disable_callbacks=True

		text=self.index_to_text(int(self.data.import_x_combo_pos))
		self.x_combo.setCurrentText(text)

		text=self.index_to_text(int(self.data.import_data_combo_pos))
		self.data_combo.setCurrentText(text)

		self.title_entry.setText(self.data.import_title)
		self.xlabel_entry.setText(self.data.import_xlabel)
		self.data_label_entry.setText(self.data.import_data_label)
		self.area_entry.setText(str(self.data.import_area))

		self.x_spin.setValue(self.data.import_x_spin)
		self.data_spin.setValue(int(self.data.import_data_spin))

		self.x_invert.setChecked(str2bool(self.data.import_x_invert))
		self.data_invert.setChecked(str2bool(self.data.import_data_invert))
		self.disable_callbacks=False
		self.update()

		
	def set_xlabel(self,text):
		self.xlabel_entry.blockSignals(True)
		self.xlabel_entry.setText(text)
		self.xlabel_entry.blockSignals(False)

	def set_data_label(self,text):
		self.data_label_entry.blockSignals(True)
		self.data_label_entry.setText(text)
		self.data_label_entry.blockSignals(False)

	def set_title(self,text):
		self.title_entry.blockSignals(True)
		self.title_entry.setText(text)
		self.title_entry.blockSignals(False)

	def enable_units(self,val):
		self.x_combo.setEnabled(val)
		self.data_combo.setEnabled(val)

	def get_title(self):
		return self.title_entry.text()

	def get_xlabel(self):
		return self.xlabel_entry.text()

	def get_data_label(self):
		return self.data_label_entry.text()
	
	def callback_tab_changed(self):
		self.update()

	def __init__(self,data,export_path=get_sim_path()):
		QDialog.__init__(self)
		self.data=data
		self.export_path=export_path
		self.disable_callbacks=False
		resize_window_to_be_sane(self,0.6,0.7)

		self.setWindowIcon(icon_get("import"))

		self.setWindowTitle(_("Import data")+" (https://www.gpvdm.com)") 

		self.main_vbox = QVBoxLayout()
	
		self.ribbon=ribbon_import()
		
		self.ribbon.open_data.triggered.connect(self.callback_open)

		self.ribbon.import_data.triggered.connect(self.callback_import)

			
		self.main_vbox.addWidget(self.ribbon)
		
		self.input_output_hbox=QHBoxLayout()

		self.raw_data_widget=QWidget()
		self.raw_data_hbox=QVBoxLayout()
		self.raw_data_widget.setLayout(self.raw_data_hbox)
		self.raw_data = QTextEdit(self)
		self.raw_data.setReadOnly(True)
		self.raw_data.setLineWrapMode(QTextEdit.NoWrap)
		font = self.raw_data.font()
		font.setFamily("Courier")
		font.setPointSize(10)
		self.raw_data_label=QLabel(_("The file to import:"))
		self.raw_data_hbox.addWidget(self.raw_data_label)
		self.raw_data_hbox.addWidget(self.raw_data)
		self.raw_data_path=QLabel()
		self.raw_data_hbox.addWidget(self.raw_data_path)

		self.out_data_widget=QWidget()
		self.out_data_hbox=QVBoxLayout()
		self.out_data_widget.setLayout(self.out_data_hbox)
		self.out_data = QTextEdit(self)
		self.out_data.setReadOnly(True)
		self.out_data.setLineWrapMode(QTextEdit.NoWrap)
		font = self.out_data.font()
		font.setFamily("Courier")
		font.setPointSize(10)
		self.out_data_label=QLabel(_("The imported file, the numbers should now be in SI units"))
		self.out_data_hbox.addWidget(self.out_data_label)

		self.out_data_hbox.addWidget(self.out_data)

		self.out_data_path=QLabel()
		self.out_data_hbox.addWidget(self.out_data_path)
		
		self.input_output_hbox.addWidget(self.raw_data_widget)
		self.input_output_hbox.addWidget(self.out_data_widget)
		self.input_output_widget=QWidget()
		self.input_output_widget.setLayout(self.input_output_hbox)

		###################
		self.build_top_widget()
		###################

		self.main_vbox.addWidget(self.top_widget)
		self.main_vbox.addWidget(self.input_output_widget)		
		self.input_output_widget.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.setLayout(self.main_vbox)

		self.out_data_path.setText(os.path.join(self.export_path,self.data.data_file))
		if self.load_config()==False:
			self.open_file()
		self.load_file(self.data.import_file_path)
		self.update()

	def build_top_widget(self):
		self.build_decoder()

		#self.items.append([_("Reflectance"),"au",_("Reflectance"),"au","val/(max(data.data[0][0]))",1.0,True])

		i=0
		self.x_label=self.items[i].output_description
		self.x_units=self.items[i].output_units
		self.x_mul_to_si=self.items[i].equation_to_si
		self.x_disp_mul=self.items[i].mul_to_display

		self.data_label=self.items[i].output_description
		self.data_units=self.items[i].output_units
		self.data_disp_mul=self.items[i].mul_to_display

		self.top_widget=QWidget()
		self.top_vbox=QVBoxLayout()
		self.x_combo=QComboBox()
		self.add_units(self.x_combo)
		self.data_combo=QComboBox()
		self.add_units(self.data_combo)
		
		self.units_x_label=QLabel(_("x units:"))
		self.units_data_label=QLabel(_("y units:"))
		

		self.x_combo.currentIndexChanged.connect(self.callback_edited)
		self.data_combo.currentIndexChanged.connect(self.callback_edited)

		self.title_widget=QWidget()
		self.title_hbox=QHBoxLayout()
		self.title_label=QLabel(_("Title:"))
		self.title_entry=QLineEdit()
		self.title_hbox.addWidget(self.title_label)
		self.title_hbox.addWidget(self.title_entry)
		self.title_widget.setLayout(self.title_hbox)
		self.top_vbox.addWidget(self.title_widget)



		self.xlabel_widget=QWidget()
		self.xlabel_hbox=QHBoxLayout()
		self.xlabel_label=QLabel(_("x-label:"))
		self.xlabel_entry=QLineEdit()
		self.x_column_label=QLabel(_("x-column:"))
		self.x_spin=QSpinBox()
		self.x_spin.setValue(0)
		self.x_spin.valueChanged.connect(self.callback_edited)

		self.x_invert=QCheckBox("invert")
		
		self.xlabel_hbox.addWidget(self.xlabel_label)
		self.xlabel_hbox.addWidget(self.xlabel_entry)
		self.xlabel_hbox.addWidget(self.x_column_label)
		self.xlabel_hbox.addWidget(self.x_spin)
		self.xlabel_hbox.addWidget(self.units_x_label)
		self.xlabel_hbox.addWidget(self.x_combo)
		self.xlabel_hbox.addWidget(self.x_invert)
		self.xlabel_widget.setLayout(self.xlabel_hbox)
		self.top_vbox.addWidget(self.xlabel_widget)

		self.data_label_widget=QWidget()
		self.data_label_hbox=QHBoxLayout()
		self.data_label_label=QLabel(_("y-label:"))
		self.data_label_entry=QLineEdit()
		self.data_column_label=QLabel(_("y-column:"))
		self.data_spin=QSpinBox()
		self.data_spin.setValue(1)
		self.data_spin.valueChanged.connect(self.callback_edited)

		self.data_invert=QCheckBox("invert")

		self.data_label_hbox.addWidget(self.data_label_label)
		self.data_label_hbox.addWidget(self.data_label_entry)
		self.data_label_hbox.addWidget(self.data_column_label)
		self.data_label_hbox.addWidget(self.data_spin)
		self.data_label_hbox.addWidget(self.units_data_label)
		self.data_label_hbox.addWidget(self.data_combo)
		self.data_label_hbox.addWidget(self.data_invert)
		self.data_label_widget.setLayout(self.data_label_hbox)
		self.top_vbox.addWidget(self.data_label_widget)

		self.area_widget=QWidget()
		self.area_hbox=QHBoxLayout()
		self.area_label=QLabel(_("device area:"))
		self.area_hbox.addWidget(self.area_label)
		self.area_entry=QLineEdit()
		self.area_entry.setText(str(round(gpvdm_data().mesh.mesh_x.get_len()*gpvdm_data().mesh.mesh_z.get_len()*100*100, 3)))
		self.area_hbox.addWidget(self.area_entry)
		self.area_units=QLabel("cm2")
		self.area_hbox.addWidget(self.area_units)
		self.area_widget.setLayout(self.area_hbox)
		self.top_vbox.addWidget(self.area_widget)

		self.area_widget.hide()
		
		self.xlabel_entry.textEdited.connect(self.callback_edited)
		self.data_label_entry.textEdited.connect(self.callback_edited)
		self.title_entry.textEdited.connect(self.callback_edited)
		self.area_entry.textEdited.connect(self.callback_edited)
		self.x_invert.stateChanged.connect(self.callback_edited)
		self.data_invert.stateChanged.connect(self.callback_edited)
		self.top_widget.setLayout(self.top_vbox)

	def update(self):
		if os.path.isfile(self.data.import_file_path)==True:
			data=self.transform(self.data.import_file_path)
			if data==False:
				error_dlg(self,_("Can not load file "+self.data.import_file_path))
				return

			text="\n".join(data.gen_output_data())
			self.out_data.setText(text)

	def transform(self,file_name):

		data=dat_file()
		#print("here")
		ret=data.import_data(file_name,x_col=self.x_spin.value(),y_col=self.data_spin.value())
		#print("end")
		if ret==True:
			self.populate_boxes()
		else:
			return False

		data.title=self.get_title()

		data.y_label=self.get_xlabel()
		data.data_label=self.get_data_label()

		data.y_units=self.x_units
		data.data_units=self.data_units
		
		data.y_mul=self.x_disp_mul
		data.data_mul=self.data_disp_mul

		data_combo=self.combo_to_item(self.data_combo)

		if data_combo.pre_eval!="":
			exec(data_combo.pre_eval)

		#rescale the data
		for i in range(0,data.y_len):
			x_scale=0
			val=data.y_scale[i]
			x_scale=eval(self.x_mul_to_si)

			val=data.data[0][0][i]
			x_val=x_scale
			dat=eval(data_combo.equation_to_si)

			if self.x_invert.isChecked() == True:
				x_scale=x_scale*-1

			if self.data_invert.isChecked() == True:
				dat=dat*-1

			data.data[0][0][i]=dat
			data.y_scale[i]=x_scale
		if len(data.y_scale)==len(data.data[0][0]):
			if len(data.y_scale)>0:
				data.y_scale, data.data[0][0] = zip(*sorted(zip(data.y_scale, data.data[0][0])))

		return data
		
	def callback_import(self):
		data=self.transform(self.data.import_file_path)
		data.save(os.path.join(self.export_path,self.data.data_file))
		self.accept()

	def open_file(self):
		file_name=open_as_filter(self,"dat (*.dat);;csv (*.csv);;txt (*.txt);;tdf (*.tdf)",path=self.path)

		if file_name!=None:
			if self.load_file(file_name)==False:
				return

			self.update()

	def load_file(self,file_name):
		if os.path.isfile(file_name)==True:
			try:
				f = open(file_name, "r")
				lines = f.readlines()
				f.close()
				text=""
				for l in range(0, len(lines)):
					text=text+lines[l].rstrip()+"\n"
				self.raw_data_path.setText(file_name)
				self.raw_data.setText(text)

				self.data.import_file_path=file_name		#do this last
			except:
				error_dlg(self,_("I can't read the file: ")+file_name+_("I can only read plain text files such as csv formats. I can not read binary files such as excel documents."))
				return False

			self.ribbon.import_data.setEnabled(True)
			return True
		return False


	def callback_open(self):
		self.open_file()

	def run(self):
		self.exec_()

	def build_decoder(self):
		self.items=[]

		a=decoder()
		a.input_description=_("Wavelength")
		a.input_units="nm"
		a.output_description=_("Wavelength")
		a.output_units="nm"
		a.equation_to_si="val*1e-9"
		a.mul_to_display=1e9
		a.need_area=False
		a.index=0
		self.items.append(a)

		a=decoder()
		a.input_description=_("Wavelength")
		a.input_units="um"
		a.output_description=_("Wavelength")
		a.output_units="nm"
		a.equation_to_si="val*1e-6"
		a.mul_to_display=1e9
		a.need_area=False
		a.index=1
		self.items.append(a)

		a=decoder()
		a.input_description=_("Wavelength")
		a.input_units="cm"
		a.output_description=_("Wavelength")
		a.output_units="nm"
		a.equation_to_si="val*1e-3"
		a.mul_to_display=1e9
		a.need_area=False
		a.index=2
		self.items.append(a)

		a=decoder()
		a.input_description=_("Wavelength")
		a.input_units="m"
		a.output_description=_("Wavelength")
		a.output_units="nm"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1e9
		a.need_area=False
		a.index=3
		self.items.append(a)

		a=decoder()
		a.input_description=_("Photonenergy")
		a.input_units="eV"
		a.output_description=_("Wavelength")
		a.output_units="nm"
		a.equation_to_si="3e8*(6.62607004e-34/1.6e-19)/val"
		a.mul_to_display=1e9
		a.need_area=False
		a.index=4
		self.items.append(a)

		a=decoder()
		a.input_description=_("J")
		a.input_units="mA/cm2"
		a.output_description=_("J")
		a.output_units="A/m2"
		a.equation_to_si="val*10000.0/1000.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=5
		self.items.append(a)

		a=decoder()
		a.input_description=_("J")
		a.input_units="A/cm2"
		a.output_description=_("J")
		a.output_units="A/m2"
		a.equation_to_si="val*10000.0/1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=6
		self.items.append(a)

		a=decoder()
		a.input_description=_("J")
		a.input_units="A/m2"
		a.output_description=_("J")
		a.output_units="A/m2"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=7
		self.items.append(a)

		a=decoder()
		a.input_description=_("IMPS Re(Z)")
		a.input_units="Am^{2}/W"
		a.output_description=_("IMPS Re(Z)")
		a.output_units="Am^{2}/W"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=8
		self.items.append(a)

		a=decoder()
		a.input_description=_("IMPS Im(Z)")
		a.input_units="Am^{2}/W"
		a.output_description=_("IMPS Im(Z)")
		a.output_units="Am^{2}/W"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=9
		self.items.append(a)

		a=decoder()
		a.input_description=_("IMVS Re(Z)")
		a.input_units="Vm^{2}/W"
		a.output_description=_("IMVS Re(Z)")
		a.output_units="Vm^{2}/W"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=10
		self.items.append(a)

		a=decoder()
		a.input_description=_("IMVS Im(Z)")
		a.input_units="Vm^{2}/W"
		a.output_description=_("IMVS Im(Z)")
		a.output_units="Vm^{2}/W"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=11
		self.items.append(a)

		a=decoder()
		a.input_description=_("Amps")
		a.input_units="A"
		a.output_description=_("J")
		a.output_units="A/m2"
		a.equation_to_si="val*1.0/(self.get_area())"
		a.mul_to_display=1.0
		a.need_area=True
		a.index=12
		self.items.append(a)

		a=decoder()
		a.input_description=_("Amps - no convert")
		a.input_units="A"
		a.output_description=_("I")
		a.output_units="A"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=13
		self.items.append(a)

		a=decoder()
		a.input_description=_("Voltage")
		a.input_units="V"
		a.output_description=_("Voltage")
		a.output_units="V"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=14
		self.items.append(a)

		a=decoder()
		a.input_description=_("-Voltage")
		a.input_units="V"
		a.output_description=_("Voltage")
		a.output_units="V"
		a.equation_to_si="val*-1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=15
		self.items.append(a)

		a=decoder()
		a.input_description=_("Voltage")
		a.input_units="mV"
		a.output_description=_("Voltage")
		a.output_units="V"
		a.equation_to_si="val*1e-3"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=16
		self.items.append(a)

		a=decoder()
		a.input_description=_("Frequency")
		a.input_units="Hz"
		a.output_description=_("Frequency")
		a.output_units="Hz"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=17
		self.items.append(a)

		a=decoder()
		a.input_description=_("Angular frequency")
		a.input_units="Rads"
		a.output_description=_("Frequency")
		a.output_units="Hz"
		a.equation_to_si="val/(2.0*3.1415926)"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=18
		self.items.append(a)

		a=decoder()
		a.input_description=_("Resistance")
		a.input_units="Ohms"
		a.output_description=_("Resistance")
		a.output_units="Ohms"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=19
		self.items.append(a)

		a=decoder()
		a.input_description=_("Refractive index")
		a.input_units="au"
		a.output_description=_("Refractive index")
		a.output_units="au"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=20
		self.items.append(a)

		a=decoder()
		a.input_description=_("Absorption")
		a.input_units="m^{-1}"
		a.output_description=_("Absorption")
		a.output_units="m^{-1}"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=21
		self.items.append(a)

		a=decoder()
		a.input_description=_("Absorption")
		a.input_units="cm^{-1}"
		a.output_description=_("Absorption")
		a.output_units="m^{-1}"
		a.equation_to_si="val*1e2"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=22
		self.items.append(a)

		a=decoder()
		a.input_description=_("Attenuation coefficient")
		a.input_units="au"
		a.output_description=_("Absorption")
		a.output_units="m^{-1}"
		a.equation_to_si="val*4*3.14159/x_val"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=23
		self.items.append(a)

		a=decoder()
		a.input_description=_("Time")
		a.input_units="s"
		a.output_description=_("Time")
		a.output_units="s"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=24
		self.items.append(a)

		a=decoder()
		a.input_description=_("Suns")
		a.input_units="Suns"
		a.output_description=_("Suns")
		a.output_units="Suns"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=25
		self.items.append(a)

		a=decoder()
		a.input_description=_("Intensity")
		a.input_units="um^{-1}.Wm^{-2}"
		a.output_description=_("Intensity")
		a.output_units="m^{-1}.Wm^{-2}"
		a.equation_to_si="val*1e6"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=26
		self.items.append(a)

		a=decoder()
		a.input_description=_("Charge density")
		a.input_units="m^{-3}"
		a.output_description=_("Charge density")
		a.output_units="m^{-3}"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=27
		self.items.append(a)

		a=decoder()
		a.input_description=_("Capacitance")
		a.input_units="F cm^{-2}"
		a.output_description=_("Capacitance")
		a.output_units="F"
		a.equation_to_si="val*100.0*100.0*(self.get_area())"
		a.mul_to_display=1.0
		a.need_area=True
		a.index=28
		self.items.append(a)

		a=decoder()
		a.input_description=_("Suns")
		a.input_units="percent"
		a.output_description=_("Suns")
		a.output_units="Suns"
		a.equation_to_si="val/100.0"
		a.mul_to_display=1.0
		a.need_area=True
		a.index=29
		self.items.append(a)

		a=decoder()
		a.input_description=_("Charge")
		a.input_units="C"
		a.output_description=_("Charge")
		a.output_units="C"
		a.equation_to_si="val*1.0"
		a.mul_to_display=1.0
		a.need_area=True
		a.index=30
		self.items.append(a)

		a=decoder()
		a.input_description=_("mA")
		a.input_units="mA"
		a.output_description=_("J")
		a.output_units="A/m2"
		a.equation_to_si="val*1e-3/(self.get_area())"
		a.mul_to_display=1.0
		a.need_area=True
		a.index=31
		self.items.append(a)

		a=decoder()
		a.input_description=_("Reflectance")
		a.input_units="au"
		a.output_description=_("Reflectance")
		a.output_units="au"
		a.equation_to_si="val/data_max"
		a.mul_to_display=1.0
		a.need_area=False
		a.index=32
		a.pre_eval="data_max=max(data.data[0][0])"
		self.items.append(a)
