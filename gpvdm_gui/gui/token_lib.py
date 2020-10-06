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

## @package token_lib
#  A library of all tokens used in the model.
#

import re

import i18n
_ = i18n.language.gettext

class my_data():
	token=""
	units=""
	info=""
	def __init__(self,file_name,a,b,info,e,f,widget,defaults=None,units_widget="QLabel",min=None,max=None,hidden=False,hide_on_true_token="none",hide_on_false_token=[],data_type=None,hide_on_token_eq=None):
		self.file_name=file_name
		self.token=a
		self.units=b
		self.info=info
		self.defaults=defaults
		self.number_type=e
		self.number_mul=f
		self.widget=widget
		self.units_widget=units_widget
		self.hidden=hidden
		self.hide_on_true_token=hide_on_true_token
		self.hide_on_false_token=hide_on_false_token
		self.hide_on_token_eq=hide_on_token_eq
		self.data_type=data_type
		self.min=min
		self.max=max


lib=[]

def build_token_lib():
	global lib
	#light.inp
	lib.append(my_data("","#light_wavelength_auto_mesh",_("True/False"),_("Automatically mesh wavelength space"),"e",1.0,"gtkswitch"))
	lib.append(my_data("light.inp","#lpoints","au",_("Mesh points (lambda)"),"e",1.0,"QLineEdit",hide_on_true_token="#light_wavelength_auto_mesh"))
	lib.append(my_data("light.inp","#lstart","m",_("Lambda start"),"e",1.0,"QLineEdit",hide_on_true_token="#light_wavelength_auto_mesh"))
	lib.append(my_data("light.inp","#lstop","m",_("Lambda stop"),"e",1.0,"QLineEdit",hide_on_true_token="#light_wavelength_auto_mesh"))

	lib.append(my_data("light.inp","#electron_eff","0-1",_("Electron generation efficiency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("light.inp","#hole_eff","0-1",_("Hole generation efficiency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("light.inp","#sun",_("filename"),_("Sun's spectra"),"e",1.0,"QLineEdit"))
	lib.append(my_data("light.inp","#light_file_generation","file_name",_("File containing generation rate"),"e",1.0,"gpvdm_select"))

	lib.append(my_data("light.inp","#Dphotoneff","0-1",_("Photon efficiency"),"e",1.0,"QLineEdit",min=0.001,max=1.2))
	lib.append(my_data("light.inp","#light_file_qe_spectra","au",_("QE spectra file"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#light_profile","au",_("Profile of light beam"),"s",1.0,"QComboBoxShape"))

	#filter.inp
	lib.append(my_data("filter.inp","#filter_material","...",_("Optical filter material"),"e",1.0,"gpvdm_select_material" ,units_widget="QPushButton"))
	lib.append(my_data("filter.inp","#filter_db","0-1000dB",_("dB"),"e",1.0,"QLineEdit"))


	#laser?.inp
	lib.append(my_data("","#laserwavelength","m",_("Laser wavelength"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#laser_pulse_width","s",_("Length of pulse"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#spotx","m",_("Spot size x"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#spoty","m",_("Spot size y"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#pulseJ","J",_("Energy in pulse"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#laser_photon_efficiency","0-1",_("Efficiency of photons"),"e",1.0,"QLineEdit"))


	#dos?.inp
	lib.append(my_data("","#dostype","Edit",_("DoS distribution"),"s",1.0,"generic_switch",units_widget="QPushButton",defaults=[[_("Complex"),"complex"],[_("Exponential"),"exponential"]]))
	lib.append(my_data("","#dos_free_carrier_stats","type",_("Free carrier statistics"),"e",1.0,"QComboBoxLang",defaults=[[("mb_equation"),_("Maxwell Boltzmann - analytic")],["mb_look_up_table_analytic",_("Maxwell Boltzmann - numerical+analytic")],["mb_look_up_table",_("Maxwell Boltzmann - full numerical")],["fd_look_up_table",_("Ferm-Dirac - numerical")]]))

	lib.append(my_data("","#Nc","m^{-3}",_("Effective density of free electron states (@300K)"),"e",1.0,"QLineEdit",min=1e10,max=1e27 ))
	lib.append(my_data("","#Nv","m^{-3}",_("Effective density of free hole states (@300K)"),"e",1.0,"QLineEdit",min=1e10,max=1e27 ))

	lib.append(my_data("","#symmetric_mobility_e","m^{2}V^{-1}s^{-1}",_("Electron mobility"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,defaults=[True]))
	lib.append(my_data("","#symmetric_mobility_h","m^{2}V^{-1}s^{-1}",_("Hole mobility"),"e",1.0,"mobility_widget",min=1.0,max=1e-14, defaults=[False] ))

	lib.append(my_data("","#mue_z","m^{2}V^{-1}s^{-1}",_("Electron mobility z"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("","#mue_x","m^{2}V^{-1}s^{-1}",_("Electron mobility x"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("","#mue_y","m^{2}V^{-1}s^{-1}",_("Electron mobility y"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))

	lib.append(my_data("","#muh_z","m^{2}V^{-1}s^{-1}",_("Hole mobility z"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("","#muh_x","m^{2}V^{-1}s^{-1}",_("Hole mobility x"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("","#muh_y","m^{2}V^{-1}s^{-1}",_("Hole mobility y"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))

	lib.append(my_data("","#symmetric_mobility_h","m^{2}V^{-1}s^{-1}",_("Hole mobility"),"e",1.0,"mobility_widget",min=1.0,max=1e-14, defaults=[False] ))

	lib.append(my_data("","#ion_density","m^{-3}",_("Perovskite ion density"),"e",1.0,"QLineEdit",min=1e10,max=1e27,hidden=True))
	#lib.append(my_data("","#ion_mobility","m^{2}V^{-1}s^{-1}",_("Perovskite ion mobility"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#doping_start","m^{-3}",_("Doping density (x=0)"),"e",1.0,"QLineEdit",min=1.0,max=1e27,hidden=True))
	lib.append(my_data("","#doping_stop","m^{-3}",_("Doping density (x=max)"),"e",1.0,"QLineEdit",min=1.0,max=1e27,hidden=True))

	lib.append(my_data("","#Ntrape","m^{-3} eV^{-1}",_("Electron trap density"),"e",1.0,"QLineEdit",min=1e10,max=1e27 ))
	lib.append(my_data("","#Ntraph","m^{-3} eV^{-1}",_("Hole trap density"),"e",1.0,"QLineEdit",min=1e10,max=1e27 ))
	lib.append(my_data("","#Etrape","eV",_("Electron tail slope"),"e",1.0,"QLineEdit",min=20e-3,max=150e-3 ))
	lib.append(my_data("","#Etraph","eV",_("Hole tail slope"),"e",1.0,"QLineEdit",min=20e-3,max=150e-3 ))
	lib.append(my_data("","#epsilonr","au",_("Relative permittivity"),"e",1.0,"QLineEdit",min=1.0,max=10.0 ))
	lib.append(my_data("","#srhsigman_e","m^{-2}",_("Free electron to Trapped electron"),"e",1.0,"QLineEdit",min=1e-27,max=1e-15 ))
	lib.append(my_data("","#srhsigmap_e","m^{-2}",_("Trapped electron to Free hole"),"e",1.0,"QLineEdit",min=1e-27,max=1e-15 ))
	lib.append(my_data("","#srhsigman_h","m^{-2}",_("Trapped hole to Free electron"),"e",1.0,"QLineEdit",min=1e-27,max=1e-15 ))
	lib.append(my_data("","#srhsigmap_h","m^{-2}",_("Free hole to Trapped hole"),"e",1.0,"QLineEdit",min=1e-27,max=1e-15))
	lib.append(my_data("","#free_to_free_recombination","m^{3}s^{-1}",_("n_{free} to p_{free} Recombination rate constant"),"e",1.0,"QLineEdit",min=1e-27,max=1e-15 ))

	#electrical?.inp
	lib.append(my_data("","#electrical_component","type",_("Component"),"e",1.0,"QComboBoxLang",defaults=[[("resistance"),_("Resistance")],["diode",_("Diode")],["link",_("Link")]]))
	lib.append(my_data("","#electrical_shunt","Ohm  m",_("Shunt resistivity"),"e",1.0,"QLineEdit",min=0.1,max=1e20, hide_on_token_eq=[["#electrical_component","resistance"],["#electrical_component","link"]] ))
	#lib.append(my_data("","#electrical_series","Ohm m",_("Series resistivity"),"e",1.0,"QLineEdit",min=0.1,max=1e20, hide_on_token_eq=[["#electrical_component","link"]] ))

	lib.append(my_data("","#electrical_symmetrical_resistance","Ohm m",_("Series resistivity"),"e",1.0,"mobility_widget", defaults=[False] ))

	lib.append(my_data("","#electrical_series_z","Ohm m",_("Series resistivity z"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("","#electrical_series_x","Ohm m",_("Series resistivity x"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("","#electrical_series_y","Ohm m",_("Series resistivity y"),"e",1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))

	lib.append(my_data("","#electrical_n","au",_("Layer ideality factor"),"e",1.0,"QLineEdit",min=0.0,max=1.0, hide_on_token_eq=[["#electrical_component","resistance"],["#electrical_component","link"]] ))
	lib.append(my_data("","#electrical_J0","A m^{-2}",_("Reverse bias current"),"e",1.0,"QLineEdit",min=0.0,max=1e6, hide_on_token_eq=[["#electrical_component","resistance"],["#electrical_component","link"]] ))
	lib.append(my_data("","#electrical_enable_generation",_("True/False"),_("Enable optical charge\ncarrier generation"),"e",1.0,"gtkswitch" ))




	#shape?.inp
	lib.append(my_data("","#shape_type","au",_("Shape type"),"s",1.0,"QComboBoxShape"))
	lib.append(my_data("","#shape_dx","m",_("dx of the object"),"e",1.0,"QLineEdit",data_type="float"))
	lib.append(my_data("","#shape_dy","m",_("dy of the object"),"e",1.0,"QLineEdit",data_type="float"))
	lib.append(my_data("","#shape_dz","m",_("dz of the object"),"e",1.0,"QLineEdit",data_type="float"))

	lib.append(my_data("","#shape_padding_dx","m",_("dx padding"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_padding_dy","m",_("dy padding"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_padding_dz","m",_("dz padding"),"e",1.0,"QLineEdit"))


	lib.append(my_data("","#shape_nx","au",_("Number of objects x"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_ny","au",_("Number of objects y"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_nz","au",_("Number of objects z"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_x0","m",_("x offset"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_y0","m",_("y offset"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_z0","m",_("z offset"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_remove_layer",_("True/False"),_("Remove layer"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#shape_dos",_("Edit"),_("Electrical parameters"),"e",1.0,"shape_dos_switch",units_widget="QPushButton"))
	lib.append(my_data("","#shape_electrical",_("Edit"),_("Electrical parameters"),"e",1.0,"shape_dos_switch",units_widget="QPushButton"))
	lib.append(my_data("","#shape_optical_material",_("Edit"),_("Optical material"),"e",1.0,"gpvdm_select_material" ,units_widget="QPushButton"))
	lib.append(my_data("","#shape_flip_y",_("True/False"),_("Flip y"),"e",1.0,"gtkswitch"))

	#interface?.inp
	lib.append(my_data("","#interface_model","type",_("Interface model"),"e",1.0,"QComboBoxLang",defaults=[[("none"),_("None")],["recombination",_("Recombination")]]))
	lib.append(my_data("","#interface_eh_tau","m^{3}s^{-1}",_("Recombination constant"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#interface_model","none"]]))

	
	1e-15
	#ver
	1.0
	#end

	#stark.inp
	lib.append(my_data("","#stark_startime","s",_("startime"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_ea_factor","au",_("ea_factor"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_Np","1/0",_("Np"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_den","1/0",_("den"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_externalv","V",_("externalv"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_dt_neg_time","s",_("dt_neg_time"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_dt","s",_("dt"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_dt_mull","au",_("dt_mull"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_stop","s",_("stop"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_stark","1/0",_("stark"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_lasereff","1/0",_("lasereff"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_probe_wavelength","nm",_("wavelength"),"e",1e9,"QLineEdit"))
	lib.append(my_data("","#stark_sim_contacts","1/0",_("sim_contacts"),"e",1.0,"QLineEdit"))

	#ref
	lib.append(my_data("","#ref_url","au",_("Website"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_author","au",_("Author"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_jounral","au",_("Journal"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_title","au",_("Title"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_volume","au",_("Volume"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_pages","au",_("Pages"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_year","au",_("Year"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_DOI","au",_("DOI"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_booktitle","au",_("Book title"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_publisher","au",_("Publisher"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_isbn","au",_("ISBN"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ref_unformatted","au",_("Scraped text"),"e",1.0,"QLineEdit"))

	#pulse
	lib.append(my_data("","#Rshort_pulse","Ohms",_("R_{short}"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#pulse_bias","V",_("V_{bias}"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#pulse_light_efficiency","au",_("Efficiency of light"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#pulse_subtract_dc",_("True/False"),_("subtract DC"),"e",1.0,"gtkswitch"))

	#mat.inp
	lib.append(my_data("","#material_type","type",_("Material type"),"e",1.0,"QComboBoxLang",defaults=[[("organic"),_("Organic")],["oxide",_("Oxide")],["inorganic",_("Inorganic")],["metal",_("Metal")],["other",_("Other")]]))
	lib.append(my_data("","#mat_alpha","0-1.0",_("Alpha channel"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#red_green_blue","rgb",_("Color"),"e",1.0,"QColorPicker"))
	lib.append(my_data("","#mat_alpha","0-1",_("Transparency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#status","type",_("Publish material data?"),"e",1.0,"QComboBoxLang",defaults=[[("public"),_("Public")],["private",_("Private")]]))
	lib.append(my_data("","#changelog","au",_("Change log"),"e",1.0,"QChangeLog"))

	#jv.inp
	lib.append(my_data("","#jv_step_mul","0-2.0",_("JV voltage step multiplyer"),"e",1.0,"QLineEdit",hide_on_true_token="#jv_single_point"))
	lib.append(my_data("","#jv_max_j","A m^{-2}",_("Maximum current density"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#jv_light_efficiency","au",_("JV curve photon generation efficiency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#jv_pmax_n","m^{-3}",_("Average carrier density at P_{max}"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#jv_pmax_tau","m^{-1}",_("Recombination time constant"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Vstart","V",_("Start voltage"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Vstop","V",_("Stop voltage"),"e",1.0,"QLineEdit",hide_on_true_token="#jv_single_point"))
	lib.append(my_data("","#Vstep","V",_("Voltage step"),"e",1.0,"QLineEdit",hide_on_true_token="#jv_single_point"))
	lib.append(my_data("","#jv_Rcontact","V",_("Contact resistance"),"e",1.0,"QParasitic"))
	lib.append(my_data("","#jv_Rshunt","V",_("Shunt resistance"),"e",1.0,"QParasitic"))
	lib.append(my_data("","#jv_single_point",_("True/False"),_("Single point"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#jv_use_external_voltage_as_stop",_("True/False"),_("Use external\nvoltage as stop"),"e",1.0,"gtkswitch"))


	#sim_info.dat (jv plugin)
	lib.append(my_data("","#voc","V",_("V_{oc}"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#pce","Percent",_("Power conversion efficiency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#ff","a.u.",_("Fill factor"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Pmax","W m^{-2}",_("Max power"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#v_pmax","V",_("Voltage at max power"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#j_pmax","Am^{-2}",_("Current density at max power"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#voc_nt","m^{-3}",_("Trapped electrons at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_pt","m^{-3}",_("Trapped holes at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_nf","m^{-3}",_("Free electrons at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_pf","m^{-3}",_("Free holes at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_np_tot","m^{-3}",_("Total carriers (n+p)/2 at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_tau","s",_("Recombination time constant at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_R","m^{-3}s^{-1}",_("Recombination rate at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#voc_J","A m^{-2}",_("Current density at Voc"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#jsc","A m^{-2}",_("J_{sc}"),"e",1.0,"QLineEdit"))

	#sim_info.dat (optics plugin)
	lib.append(my_data("","#light_photons_in_active_layer","m^{-2}",_("Photos absorbed in active layer"),"e",1.0,"QLineEdit"))


	#object_stats.dat (optics plugin)
	lib.append(my_data("object_stats.dat","#Rp[0-9]","m",_("Peak height Rp"),"e",1.0,"QLineEdit"))
	lib.append(my_data("object_stats.dat","#Rq[0-9]","m",_("RMS height Rq"),"e",1.0,"QLineEdit"))
	lib.append(my_data("object_stats.dat","#Ra[0-9]","m",_("Average height Ra"),"e",1.0,"QLineEdit"))

	#cv?.inp
	lib.append(my_data("","#cv_start_voltage","Volts",_("Start voltage"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#cv_stop_voltage","Volts",_("Stop voltage"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#cv_dv_step","Volts",_("dV step"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#cv_fx","Hz",_("Frequency"),"e",1.0,"QLineEdit"))


	#sim_info.dat (equlibrium)
	lib.append(my_data("","#left_holes","m^{-3}",_("Left hole density"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#left_electrons","m^{-3}",_("Left electron density"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#right_holes","m^{-3}",_("Right hole density"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#right_electrons","m^{-3}",_("Right electron density"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Vbi","m^{-3}",_("Built in potential"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#electron_affinity_left","eV",_("Electron affinity left"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#electron_affinity_right","eV",_("Electron affinity right"),"e",1.0,"QLineEdit"))

	#tab
	lib.append(my_data("","#english_name","name",_("English name"),"e",1.0,"QLineEdit"))

	#server.inp
	lib.append(my_data("","#gpvdm_core_max_threads","au",_("Number of gpvdm_core threads"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_gpvdm_instances","au",_("Maximum number of gpvdm_core instances"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#server_stall_time","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#server_exit_on_dos_error","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#server_max_run_time","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#server_auto_cpus","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#server_min_cpus","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#server_steel","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#port","au","Cluster port","e",1.0,"QLineEdit"))
	lib.append(my_data("","#path_to_src","au",_("Path to source code"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#path_to_libs","au",_("Path to compiled libs for cluster"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#make_command","au",_("Make command"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#exe_name","au",_("exe name"),"e",1.0,"QLineEdit"))

	#cluster.inp
	lib.append(my_data("","#cluster_user_name","au","User name","e",1.0,"QLineEdit"))
	lib.append(my_data("","#cluster_ip","au","Cluster IP","e",1.0,"QLineEdit"))
	lib.append(my_data("","#cluster_cluster_dir","au",_("Remote cluster directory"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#nodes","au",_("Remote node list"),"e",1.0,"QLineEdit"))


	#triangle mesh editor
	lib.append(my_data("","#mesh_gen_nx","au",_("x-triangles"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#mesh_gen_ny","au",_("y-triangles"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#mesh_gen_opp","au",_("Method"),"e",1.0,"QComboBoxLang",defaults=[["node_reduce",_("Node reduce")],["square_mesh_gen",_("No reduce")]]))


	lib.append(my_data("","#shape_import_blur","width pixels",_("Gaussian blur"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#shape_import_y_norm_percent","percent",_("Percent of histogram to ignore"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#gauss_sigma","pixels",_("Sigma of gaussian"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#gauss_offset_x","pixels",_("Gaussian offset x"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#gauss_offset_y","pixels",_("Gaussian offset y"),"e",1.0,"QLineEdit"))

	#honeycomb
	lib.append(my_data("","#honeycomb_dx","pixels",_("dx of Honeycomb"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#honeycomb_dy","pixels",_("dy of Honeycomb"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#honeycomb_line_width","pixels",_("Line width"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#honeycomb_x_shift","pixels",_("x shift"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#honeycomb_y_shift","pixels",_("y shift"),"e",1.0,"QLineEdit"))

	#boundary
	lib.append(my_data("","#image_boundary_x0","pixels",_("Boundary x0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#image_boundary_x1","pixels",_("Boundary x1"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#image_boundary_y0","pixels",_("Boundary y0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#image_boundary_y1","pixels",_("Boundary y1"),"e",1.0,"QLineEdit"))

	#math.inp
	lib.append(my_data("math.inp","#maxelectricalitt_first","au",_("Max Electrical itterations (first step)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#electricalclamp_first","au",_("Electrical clamp (first step)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#math_electrical_error_first","au",_("Desired electrical solver error (first step)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#math_enable_pos_solver",_("True/False"),_("Enable poisson solver"),"e",1.0,"gtkswitch"))
	lib.append(my_data("math.inp","#maxelectricalitt","au",_("Max electrical itterations"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#electricalclamp","au",_("Electrical clamp"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#posclamp","au",_("Poisson clamping"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#electricalerror","au",_("Minimum electrical error"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#pos_max_ittr","au",_("Poisson solver max itterations"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#newton_clever_exit",_("True/False"),"Newton solver clever exit","e",1.0,"gtkswitch"))
	lib.append(my_data("math.inp","#newton_min_itt","au",_("Newton minimum iterations"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#complex_solver_name",_("dll name"),_("Complex matrix solver to use"),"e",1.0,"QLineEdit"))

	lib.append(my_data("math.inp","#solver_name",_("dll name"),_("Matrix solver"),"e",1.0,"QComboBoxNewtonSelect",defaults=["umfpack","external_solver","superlu","nr_d","nr_ld"]))

	lib.append(my_data("math.inp","#newton_name",_("dll name"),_("Newton solver to use"),"e",1.0,"QComboBoxNewtonSelect",defaults=["none","newton_2d","newton_simple","newton_norm","newton"]))
	lib.append(my_data("math.inp","#math_t0","au",_("Slotboom T0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#math_d0","au",_("Slotboom D0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#math_n0","au",_("Slotboom n0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("math.inp","#math_newton_cache","au",_("Use newton cache (experimental)"),"e",1.0,"gtkswitch"))


	#fit.inp
	lib.append(my_data("fit.inp","#fit_error_mul","au",_("Fit error multiplyer"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fit.inp","#fit_randomize",_("True/False"),_("Randomize fit"),"e",1.0,"gtkswitch"))
	lib.append(my_data("fit.inp","#fit_random_reset_ittr","au",_("Number of iterations between random reset"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fit.inp","#fit_stall_steps","au",_("Stall steps"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fit.inp","#fit_disable_reset_at","au",_("Disable reset at level"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fit.inp","#fit_converge_error","au",_("Fit define convergence"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fit.inp","#fit_enable_simple_reset","au",_("Enable simplex reset"),"e",1.0,"gtkswitch"))
	lib.append(my_data("fit.inp","#fit_enable_simple_reset","au",_("Simplex reset steps"),"e",1.0,"gtkswitch"))
	lib.append(my_data("fit.inp","#fit_method","au",_("Fiting method"),"e",1.0,"QComboBox",defaults=["simplex","newton"]))
	lib.append(my_data("fit.inp","#fit_simplexmul","au",_("Start simplex step multiplication"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fit.inp","#fit_simplex_reset","au",_("Simplex reset steps"),"e",1.0,"QLineEdit"))

	#fit?.inp
	lib.append(my_data("","#fit_subtract_lowest_point",_("True/False"),_("Subtract lowest point"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#fit_set_first_point_to_zero",_("True/False"),_("Set first point to zero="),"e",1.0,"gtkswitch"))

	#eqe.inp
	lib.append(my_data("eqe.inp","#eqe_voltage","au",_("EQE Voltage"),"e",1.0,"QLineEdit"))

	#thermal.inp
	lib.append(my_data("thermal.inp","#thermal_model_type","au",_("Thermal model type"),"s",1.0,"QComboBoxLang",defaults=[["thermal_hydrodynamic",_("Hydrodynamic")],["thermal_lattice",_("Lattice heat")]], hide_on_false_token=["#thermal"]))

	lib.append(my_data("thermal.inp","#Ty0","Kelvin",_("Device temperature at y_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Ty0_boundry", "neumann"]]))
	lib.append(my_data("thermal.inp","#Ty0_boundry","au",_("Boundary condition for y_{min}"),"s",1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#heatsink_y0","W m^{-}K^{-1}",_("Conductivity of heat sink y_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Ty0_boundry", "neumann"],["#Ty0_boundry", "isothermal"]]))
	lib.append(my_data("thermal.inp","#heatsink_length_y0","m",_("Heat sink length y_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Ty0_boundry", "neumann"],["#Ty0_boundry", "isothermal"]]))

	lib.append(my_data("thermal.inp","#Ty1","Kelvin",_("Device temperature at y_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Ty1_boundry", "neumann"]]))
	lib.append(my_data("thermal.inp","#Ty1_boundry","au",_("Boundary condition for y_{max}"),"s",1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#heatsink_y1","W m^{-2}K^{-1}",_("Conductivity of heat sink y_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Ty1_boundry", "neumann"],["#Ty1_boundry", "isothermal"]]))
	lib.append(my_data("thermal.inp","#heatsink_length_y1","m",_("Heat sink length y_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Ty1_boundry", "neumann"],["#Ty1_boundry", "isothermal"]]))

	lib.append(my_data("thermal.inp","#Tx0","Kelvin",_("Device temperature at x_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tx0_boundry", "neumann"]]))
	lib.append(my_data("thermal.inp","#Tx0_boundry","au",_("Boundary condition for x_{min}"),"s",1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#heatsink_x0","W m^{-2}K^{-1}",_("Conductivity of heat sink x_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tx0_boundry", "neumann"],["#Tx0_boundry", "isothermal"]]))
	lib.append(my_data("thermal.inp","#heatsink_length_x0","m",_("Heat sink length x_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tx0_boundry", "neumann"],["#Tx0_boundry", "isothermal"]]))

	lib.append(my_data("thermal.inp","#Tx1","Kelvin",_("Device temperature at x_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tx1_boundry", "neumann"]]))
	lib.append(my_data("thermal.inp","#Tx1_boundry","au",_("Boundary condition for x_{max}"),"s",1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#heatsink_x1","W m^{-2}K^{-1}",_("Conductivity of heat sink x_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tx1_boundry", "neumann"],["#Tx1_boundry", "isothermal"]]))
	lib.append(my_data("thermal.inp","#heatsink_length_x1","m",_("Heat sink length x_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tx1_boundry", "neumann"],["#Tx1_boundry", "isothermal"]]))

	lib.append(my_data("thermal.inp","#Tz0","Kelvin",_("Device temperature at z_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tz0_boundry", "neumann"]]))
	lib.append(my_data("thermal.inp","#Tz0_boundry","au",_("Boundary condition for z_{min}"),"s",1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#heatsink_z0","W m^{-2}K^{-1}",_("Conductivity of heat sink z_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tz0_boundry", "neumann"],["#Tz0_boundry", "isothermal"]]))
	lib.append(my_data("thermal.inp","#heatsink_length_z0","m",_("Heat sink length z_{min}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tz0_boundry", "neumann"],["#Tz0_boundry", "isothermal"]]))

	lib.append(my_data("thermal.inp","#Tz1","Kelvin",_("Device temperature at z_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tz1_boundry", "neumann"]]))
	lib.append(my_data("thermal.inp","#Tz1_boundry","au",_("Boundary condition for z_{max}"),"s",1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#heatsink_z1","W m^{-2}K^{-1}",_("Conductivity of heat sink z_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tz1_boundry", "neumann"],["#Tz1_boundry", "isothermal"]]))
	lib.append(my_data("thermal.inp","#heatsink_length_z1","m",_("Heat sink length z_{max}"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["#Tz1_boundry", "neumann"],["#Tz1_boundry", "isothermal"]]))


	lib.append(my_data("thermal.inp","#thermal_l",_("True/False"),_("Lattice heat model"),"e",1.0,"gtkswitch",hide_on_token_eq=[["#thermal_model_type", "thermal_lattice"]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#thermal_e",_("True/False"),_("Electron heat model"),"e",1.0,"gtkswitch",hide_on_token_eq=[["#thermal_model_type", "thermal_lattice"]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#thermal_h",_("True/False"),_("Hole heat model"),"e",1.0,"gtkswitch",hide_on_token_eq=[["#thermal_model_type", "thermal_lattice"]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#thermal_tau_e","s",_("Electron relaxation time"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#thermal_model_type", "thermal_lattice"]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#thermal_tau_h","s",_("Hole relaxation time"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#thermal_model_type", "thermal_lattice"]], hide_on_false_token=["#thermal"]))

	lib.append(my_data("thermal.inp","#thermal_kl","W m^{-1} C^{-1}",_("Thermal conductivity"),"e",1.0,"QLineEdit", hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#Tliso",_("True/False"),_("Isothermal boundary on left"),"e",1.0,"gtkswitch", hide_on_false_token=["#thermal"]))
	lib.append(my_data("thermal.inp","#Triso",_("True/False"),_("Isothermal boundary on right"),"e",1.0,"gtkswitch", hide_on_false_token=["#thermal"]))


	#dump.inp
	lib.append(my_data("dump.inp","#newton_dump",_("True/False"),_("Dump from newton solver"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#plot",_("True/False"),_("Plot bands etc.. "),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_band_structure","","","e",1.0,"QLineEdit"))
	lib.append(my_data("dump.inp","#dump_slices_by_time",_("True/False"),_("dump slices by time"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_slices",_("True/False"),_("Dump slices"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_dynamic",_("True/False"),_("Dump dynamic"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_zip_files",_("True/False"),_("Dump zip files"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_write_out_band_structure",_("True/False"),_("Write out band structure"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_optics",_("True/False"),_("Dump optical information"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_optics_verbose",_("True/False"),_("Dump optics verbose"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_print_newtonerror",_("True/False"),_("Print newton error"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_print_converge",_("True/False"),_("Print solver convergence"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_write_converge",_("True/False"),_("Write newton solver convergence to disk"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_print_pos_error",_("True/False"),_("Print poisson solver convergence"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_norm_time_to_one",_("True/False"),_("Normalize output x-time to one"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_built_in_voltage",_("True/False"),_("Dump the built in voltage."),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_optical_probe_spectrum",_("True/False"),_("Dump optical probe spectrum"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_optics_summary",_("True/False"),_("Dump optical summary"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_ray_trace_map",_("True/False"),_("Dump raytrace plots"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dumpitdos","","","e",1.0,"QLineEdit"))
	lib.append(my_data("dump.inp","#dump_workbook",_("True/False"),_("Dump an excel workbook for each simulation run congaing the results."),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_file_access_log",_("True/False"),_("Write file access log to disk."),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_use_cache",_("True/False"),_("Use cache for file writes"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_write_headers",_("True/False"),_("Write headers to output files"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_first_guess",_("True/False"),_("Write first guess to equations"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_log_level","au",_("Log verbocity"),"s",1.0,"QComboBoxLang",defaults=[[("none"),_("None")],["screen",_("Screen")],["disk",_("Disk")],["screen_and_disk",_("Screen and disk")]]))
	lib.append(my_data("dump.inp","#dump_log_level","au",_("Log verbocity"),"s",1.0,"QComboBoxLang",defaults=[[("none"),_("None")],["screen",_("Screen")],["disk",_("Disk")],["screen_and_disk",_("Screen and disk")]]))
	lib.append(my_data("dump.inp","#dump_dynamic_pl_energy","au",_("PL dump Energy"),"s",1.0,"QLineEdit"))
	lib.append(my_data("dump.inp","#dump_remove_dos_cache",_("True/False"),_("Clean up DoS cache files"),"e",1.0,"gtkswitch"))
	lib.append(my_data("dump.inp","#dump_verbose_electrical_solver_results",_("True/False"),_("Dump verbose electrical solver results"),"e",1.0,"gtkswitch"))

	#pl_ss?.inp
	lib.append(my_data("","#pl_mode","au",_("Device state"),"s",1.0,"QComboBoxLang",defaults=[[("voc"),_("Voc")],["Jsc",_("Jsc")]]))

	#ray
	lib.append(my_data("ray.inp","#ray_wavelength_points","au",_("Wavelength points"),"e",1.0,"QLineEdit"))
	lib.append(my_data("ray.inp","#ray_auto_run","au",_("Run the ray tracer"),"s",1.0,"QComboBoxLang",defaults=[[("ray_run_never"),_("Never")],["ray_run_once",_("Once per simulation")],["ray_run_step",_("Each simulation step")]]))

	lib.append(my_data("ray.inp","#ray_theta_steps","au",_("Theta steps"),"e",1.0,"QLineEdit"))
	lib.append(my_data("ray.inp","#ray_theta_start","Degrees",_("Theta start"),"e",1.0,"QLineEdit"))
	lib.append(my_data("ray.inp","#ray_theta_stop","Degrees",_("Theta stop"),"e",1.0,"QLineEdit"))

	lib.append(my_data("ray.inp","#ray_phi_steps","au",_("Phi steps"),"e",1.0,"QLineEdit"))
	lib.append(my_data("ray.inp","#ray_phi_start","Degrees",_("Phi start"),"e",1.0,"QLineEdit"))
	lib.append(my_data("ray.inp","#ray_phi_stop","Degrees",_("Phi stop"),"e",1.0,"QLineEdit"))

	lib.append(my_data("ray.inp","#ray_escape_bins","au",_("Escape bins"),"e",1.0,"QLineEdit"))

	lib.append(my_data("ray.inp","#ray_auto_wavelength_range",_("True/False"),_("Automatic wavelength range"),"e",1.0,"gtkswitch"))
	lib.append(my_data("ray.inp","#ray_lambda_start","nm",_("Start wavelength"),"e",1.0,"QLineEdit",hide_on_true_token="#ray_auto_wavelength_range"))
	lib.append(my_data("ray.inp","#ray_lambda_stop","nm",_("Stop wavelength"),"e",1.0,"QLineEdit",hide_on_true_token="#ray_auto_wavelength_range"))
	
	lib.append(my_data("ray.inp","#ray_emission_source","au",_("Emit from"),"s",1.0,"QComboBoxLang",defaults=[[("ray_emission_electrical_mesh"),_("Each electrical mesh point")],["ray_emission_single_point",_("Center of each layer")]]))

	#viewpoint.inp
	lib.append(my_data("view_point.inp","#viewpoint_enabled",_("True/False"),_("Enable viewpoint"),"e",1.0,"gtkswitch"))
	lib.append(my_data("view_point.inp","#viewpoint_size","au",_("View point size"),"e",1.0,"QLineEdit"))
	lib.append(my_data("view_point.inp","#viewpoint_dz","au",_("View point dz"),"e",1.0,"QLineEdit"))
	lib.append(my_data("view_point.inp","#viewpoint_nx","au",_("Mesh points x"),"e",1.0,"QLineEdit"))
	lib.append(my_data("view_point.inp","#viewpoint_nz","au",_("Mesh points z"),"e",1.0,"QLineEdit"))



	#led.inp
	lib.append(my_data("","#led_extract_eff","0.0-1.0",_("LED extraction efficiency"),"e",1.0,"QLineEdit"))

	#device.inp
	#lib.append(my_data("","#invert_applied_bias","au",_("Invert applied bias"),"e",1.0,"gtkswitch"))
	#lib.append(my_data("","#lcharge","m^{-3}",_("Charge on left contact"),"e",1.0,"QLineEdit"))
	#lib.append(my_data("","#rcharge","m^{-3}",_("Charge on right contact"),"e",1.0,"QLineEdit"))

	#parasitic.inp
	lib.append(my_data("parasitic.inp","#Rshunt","Ohms m^{2}",_("Shunt resistance"),"e",1.0,"QLineEdit",min=1e-3,max=1e6))
	lib.append(my_data("parasitic.inp","#Rcontact","Ohms",_("Series resistance"),"e",1.0,"QLineEdit",min=1.0,max=200))
	lib.append(my_data("parasitic.inp","#otherlayers","m",_("Other layers"),"e",1.0,"QLineEdit"))
	lib.append(my_data("parasitic.inp","#test_param","m",_("debug (ignore)"),"e",1.0,"QLineEdit",hidden=True))

	#mesh?.inp
	lib.append(my_data("","#remesh_enable","au",_("Automatic remesh"),"e",1.0,"gtkswitch"))
	#lib.append(my_data("mesh_y.inp","#mesh_layer_points0","s","Mesh points y0","e",1.0,"QLineEdit"))
	#lib.append(my_data("mesh_y.inp","#mesh_layer_points1","s","Mesh points y1","e",1.0,"QLineEdit"))

	#pl?.inp
	lib.append(my_data("","#pl_enabled",_("True/False"),_("Turn on luminescence"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#pl_fe_fh","0.0-1.0",_("n_{free} to p_{free} photon generation efficiency"),"e",1.0,"QLineEdit", hide_on_true_token="#pl_use_experimental_emission_spectra"))
	lib.append(my_data("","#pl_fe_te","0.0-1.0",_("n_{free} to n_{trap} photon generation efficiency"),"e",1.0,"QLineEdit", hide_on_true_token="#pl_use_experimental_emission_spectra"))
	lib.append(my_data("","#pl_te_fh","0.0-1.0",_("n_{trap} to p_{free} photon generation efficiency"),"e",1.0,"QLineEdit", hide_on_true_token="#pl_use_experimental_emission_spectra"))
	lib.append(my_data("","#pl_th_fe","0.0-1.0",_("p_{trap} to n_{free} photon generation efficiency"),"e",1.0,"QLineEdit", hide_on_true_token="#pl_use_experimental_emission_spectra"))
	lib.append(my_data("","#pl_fh_th","0.0-1.0",_("p_{free} to p_{trap} photon generation efficiency"),"e",1.0,"QLineEdit", hide_on_true_token="#pl_use_experimental_emission_spectra"))
	lib.append(my_data("","#pl_input_spectrum",_("Edit"),_("Experimental emission spectra"),"e",1.0,"gpvdm_select_emission" ,units_widget="QPushButton", hide_on_false_token=["#pl_use_experimental_emission_spectra"]))
	lib.append(my_data("","#pl_experimental_emission_efficiency","0.0-1.0",_("Experimental emission efficiency"),"e",1.0,"QLineEdit", hide_on_false_token="#pl_use_experimental_emission_spectra"))
	lib.append(my_data("","#pl_emission_enabled",_("True/False"),_("Emission enabled from this layer"),"e",1.0,"gtkswitch"))

#pl_experimental_emission_efficiency

	lib.append(my_data("","#pl_use_experimental_emission_spectra",_("True/False"),_("Use experimental emission spectra"),"e",1.0,"gtkswitch"))

	#fxdomain?.inp
	lib.append(my_data("","#fxdomain_Rload","Ohms",_("Load resistor"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_points","au",_("fx domain mesh points"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#fxdomain_large_signal","small_signal"]]))
	lib.append(my_data("","#fxdomain_n","au",_("Cycles to simulate"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#fxdomain_large_signal","small_signal"]]))
	lib.append(my_data("","#fxdomain_voltage_modulation_max","V",_("Voltage modulation depth"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#fx_modulation_type","optical"],["#fxdomain_large_signal","small_signal"]]))

	lib.append(my_data("","#fx_modulation_type","au",_("Excite with"),"e",1.0,"QComboBoxLang",defaults=[[("voltage"),_("Voltage")],[("optical"),_("Light")]]))
	lib.append(my_data("","#fxdomain_measure","au",_("Measure"),"e",1.0,"QComboBoxLang",defaults=[[("measure_voltage"),_("Voltage")],[("measure_current"),_("Current")]]))
	lib.append(my_data("","#fxdomain_light_modulation_depth","au",_("Light modulation depth"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#fx_modulation_type","voltage"]]))

	lib.append(my_data("","#fxdomain_do_fit","au",_("Run fit after simulation"),"e",1.0,"gtkswitch",hide_on_token_eq=[["#fxdomain_large_signal","small_signal"],["#fxdomain_large_signal","fourier"]]))
	lib.append(my_data("","#periods_to_fit","au",_("Periods to fit"),"e",1.0,"QLineEdit",hide_on_token_eq=[["#fxdomain_large_signal","small_signal"],["#fxdomain_large_signal","fourier"]]))
	lib.append(my_data("","#fxdomain_r","",_("Re(i)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_i","V",_("Im(i)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_Jr","Am^{-2}",_("Re(J)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_Ji","Am^{-2}",_("Im(J)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_fx","Hz",_("fx"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_delta_i","s",_("di"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_delta_g","s",_("dmodulation"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#fxdomain_delta_phase","rads",_("dphase"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#fxdomain_large_signal","au",_("Simulation type"),"e",1.0,"QComboBoxLang",defaults=[[("large_signal"),_("Large signal")],[("fourier"),_("Fourier")]]))		#,[("small_signal"),_("Small signal")]
	#is?.inp
	lib.append(my_data("","#is_Vexternal","Volts",_("V_{external}"),"e",1.0,"QLineEdit"))

	#node_list.inp
	lib.append(my_data("","#node_list","au",_("Node list"),"e",1.0,"QChangeLog"))

	#crypto.inp
	lib.append(my_data("","#iv","au",_("Initialization vector"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#key","au",_("Cryptographic key"),"e",1.0,"QLineEdit"))

	#lumo?.inp
	lib.append(my_data("","#function_\d+","au","Function","e",1.0,"QLineEdit"))
	lib.append(my_data("","#function_enable_\d+","au","Enabled","e",1.0,"QLineEdit"))
	lib.append(my_data("","#function_a_\d+","au","a","e",1.0,"QLineEdit"))
	lib.append(my_data("","#function_b_\d+","au","b","e",1.0,"QLineEdit"))
	lib.append(my_data("","#function_c_\d+","au","c","e",1.0,"QLineEdit"))

	lib.append(my_data("","#Psun","Sun",_("Intensity of the sun"),"e",1.0,"QLineEdit",hidden=True))

	lib.append(my_data("","#saturation_n0","#saturation_n0",_("#saturation_n0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#saturation_rate","#saturation_rate",_("#saturation_rate"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_saturate","#imps_saturate",_("#imps_saturate"),"e",1.0,"QLineEdit"))


	lib.append(my_data("","#simplephotondensity","m^{-2}s^{-1}",_("Photon density"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#simple_alpha","m^{-1}",_("Absorption of material"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#simmode","au",_("#simmode"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#meshpoints","au",_("Mesh points (x)"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#function","au",_("#function"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Vexternal","V",_("start voltage"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Vmax","V",_("Max voltage"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Eg","eV",_("Eg"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Xi","eV",_("Xi"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#start_stop_time","s",_("Time of pause"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stopstart","1/0",_("Pause between iterations"),"e",1.0,"QComboBox",defaults=["1","0"]))
	lib.append(my_data("","#invert_current",_("True/False"),_("Invert output"),"e",1.0,"QLineEdit"))


	lib.append(my_data("","#use_capacitor","1/0",_("Use capacitor"),"e",1.0,"QComboBox",defaults=["1","0"]))


	#
	lib.append(my_data("","#Rshort_imps","Ohms",_("R_{short}"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_sun","1=1 Sun",_("Backgroud light bias"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_modulation_max","1=1 Sun",_("Modulation depth"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_modulation_fx","Hz",_("Modulation frequency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#high_sun_scale","au",_("High light multiplyer"),"e",1.0,"QLineEdit"))



	lib.append(my_data("","#imps_r","Amps",_("Re(i)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_i","Amps",_("Im(i)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_Jr","Amps $m^{-2}$",_("Re(J)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_Ji","Amps $m^{-2}$",_("Im(J)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_fx","Hz",_("Frequency"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_delta_i","s",_("Phase shift"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_delta_g","s",_("Phase shift"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_delta_phase","s",_("Phase shift"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_points","s",_("points"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_n","s",_("Wavelengths to simulate"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#imps_Vexternal","Volts",_("External voltage"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#Cext","C",_("External C"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#Rext","Ohms",_("External R"),"e",1.0,"QLineEdit"))

	lib.append(my_data("","#Rscope","Ohms",_("Resistance of scope"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_bands","bands",_("Number of traps"),"s",1.0,"QLineEdit"))

	#suns_voc
	lib.append(my_data("","#sun_voc_single_point","True/False",_("Single point"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#sun_voc_Psun_start","Suns",_("Start intensity"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#sun_voc_Psun_stop","Suns",_("Stop intensity"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#sun_voc_Psun_mul","au",_("step multiplier"),"e",1.0,"QLineEdit"))

	#suns_jsc
	lib.append(my_data("suns_voc.inp","#sunstart","Suns",_("Start intensity"), "e",1.0,"QLineEdit"))
	lib.append(my_data("suns_voc.inp","#sunstop","Suns",_("Stop intensity"), "e",1.0,"QLineEdit"))
	lib.append(my_data("suns_voc.inp","#sundp","au",_("Step"), "e",1.0,"QLineEdit"))
	lib.append(my_data("suns_voc.inp","#sundpmul","au",_("step multiplier"), "e",1.0,"QLineEdit"))

	lib.append(my_data("","#simplephotondensity","m^{-2}s^{-1}",_("Photon Flux"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#simple_alpha","m^{-1}",_("Absorption"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#xlen","m",_("device width"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#zlen","m",_("device breadth"),"e",1.0,"QLineEdit"))


	lib.append(my_data("","#ver","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#dostype","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#me","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#mh","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#gendos","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#notused","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#notused","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#Tstart","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#Tstop","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#Tpoints","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#nstart","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#nstop","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#npoints","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#nstart","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#nstop","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#npoints","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srhbands","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_start","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srhvth_e","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srhvth_h","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_cut","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#lumodelstart","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#lumodelstop","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#homodelstart","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#homodelstop","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#gaus_mull","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#Esteps","","","e",1.0,"QLineEdit"))





	lib.append(my_data("","#Rshort","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#Dphoton","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#interfaceleft","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#interfaceright","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#phibleft","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#phibright","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#vl_e","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#vl_h","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#vr_e","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#vr_h","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#light_model","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#NDfilter","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#plottime","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#startstop","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#plotfile","","","e",1.0,"QLineEdit"))


	lib.append(my_data("","#Rshort","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#solve_at_Vbi","","","e",1.0,"QLineEdit"))

	lib.append(my_data("","#remesh","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#newmeshsize","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#epitaxy","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#alignmesh","","","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_start_time","","","e",1.0,"QLineEdit"))


	lib.append(my_data("","#voc_J_to_Jr","au","Ratio of conduction current to recombination current","e",1.0,"QLineEdit"))

	lib.append(my_data("","#voc_i","au",_("Current"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#kl_in_newton","1/0",_("Solve Kirchhoff's current law in Newton solver"),"e",1.0,"QComboBox",defaults=["1","0"]))

	lib.append(my_data("","#simplexmul","au","simplex mull","e",1.0,"QLineEdit"))
	lib.append(my_data("","#simplex_reset","au","Reset steps","e",1.0,"QLineEdit"))

	lib.append(my_data("","#max_nfree_to_ptrap","m^{-3}s^{-1}","nfree_to_ptrap","e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_pfree_to_ntrap","m^{-3}s^{-1}","max_pfree_to_ntrap","e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_nrelax","m^{-3}s^{-1}","max_nrelax","e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_prelax","m^{-3}s^{-1}","max_prelax","e",1.0,"QLineEdit"))

	lib.append(my_data("","#max_nfree","m^{-3}","max_nfree","e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_pfree","m^{-3}","max_pfree","e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_ntrap","m^{-3}","max_ntrap","e",1.0,"QLineEdit"))
	lib.append(my_data("","#max_ptrap","m^{-3}","max_ptrap","e",1.0,"QLineEdit"))
	lib.append(my_data("","#alpha_max_reduction","m^{-1}","alpha_max_reduction","e",1.0,"QLineEdit"))
	lib.append(my_data("","#alpha_max_increase","m^{-1}","alpha_max_increase","e",1.0,"QLineEdit"))

	lib.append(my_data("","#srh_n_r1","m^{-3}s^{-1}","srh electron rate 1","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_n_r2","m^{-3}s^{-1}","srh electron rate 2","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_n_r3","m^{-3}s^{-1}","srh electron rate 3","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_n_r4","m^{-3}s^{-1}","srh electron rate 4","e",1.0,"QLineEdit"))

	lib.append(my_data("","#srh_p_r1","m^{-3}s^{-1}","srh hole rate 1","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_p_r2","m^{-3}s^{-1}","srh hole rate 2","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_p_r3","m^{-3}s^{-1}","srh hole rate 3","e",1.0,"QLineEdit"))
	lib.append(my_data("","#srh_p_r4","m^{-3}s^{-1}","srh hole rate 4","e",1.0,"QLineEdit"))

	lib.append(my_data("","#band_bend_max","percent","band bend max","e",1.0,"QLineEdit"))

	#config.inp
	lib.append(my_data("","#gui_config_3d_enabled",_("True/False"),_("Enable 3d effects"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#gui_use_icon_theme",_("True/False"),_("Use icons from OS"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#matlab_interpreter",_("au"),_("Matlab interpreter"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#gl_render_grid",_("True/False"),_("Render grid"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#gl_bg_color",_("rgb"),_("Color used for 3d background"),"e",1.0,"QColorPicker"))
	lib.append(my_data("","#gl_render_text",_("au"),_("Render text in 3d"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#gl_device_height",_("au"),_("Device Heigh (display only)"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#gl_dy_layer_offset",_("au"),_("Layer offset (display only)"),"e",1.0,"QLineEdit"))





	#fit
	lib.append(my_data("","#time_shift","s","time shift","e",1.0,"QLineEdit"))
	lib.append(my_data("","#start","s","start","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stop","s","stop","e",1.0,"QLineEdit"))
	lib.append(my_data("","#log_x",_("True/False"),_("log x"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#log_y",_("True/False"),_("log y"),"e",1.0,"gtkswitch"))
	lib.append(my_data("","#sim_data",_("filename"),"Fit file name","e",1.0,"QLineEdit"))
	lib.append(my_data("","#fit_invert_simulation_y",_("True/False"),_("Invert simulated data (y)"),"e",1.0,"gtkswitch"))

	#epitaxy.inp
	lib.append(my_data("epitaxy.inp","#layer_width0","nm","start","e",1e9,"QLineEdit"))
	lib.append(my_data("epitaxy.inp","#layer_width1","nm","start","e",1e9,"QLineEdit"))
	lib.append(my_data("epitaxy.inp","#layer_width2","nm","start","e",1e9,"QLineEdit"))


	#
	lib.append(my_data("","#layer0","m",_("Active layer width"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_saturate","au","Stark saturate","e",1.0,"QLineEdit"))

	lib.append(my_data("","#n_mul","au","n mul","e",1.0,"QLineEdit"))
	lib.append(my_data("","#alpha_mul","m^{-1}","Alpha mul","e",1.0,"QLineEdit"))

	lib.append(my_data("","#stark_point0","au","DR/R","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_point1","au","DR/R","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_point2","au","DR/R","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_point3","au","DR/R","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_point4","au","DR/R","e",1.0,"QLineEdit"))
	lib.append(my_data("","#stark_subtracted_value","s","subtracted value","e",1.0,"QLineEdit"))
	lib.append(my_data("","#light_energy","eV","Energy","e",1.0,"QLineEdit"))
	lib.append(my_data("","#sim_id","au","sim id","e",1.0,"QLineEdit"))


	lib.append(my_data("","#Rload","Ohms",_("External load resistor"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#pulse_shift","s","Shift of TPC signal","e",1.0,"QLineEdit"))

	lib.append(my_data("","#light_illuminate_from","au",_("Illuminate from"),"e",1.0,"QComboBoxLang",defaults=[[("top"),_("Top")],[("bottom"),_("Bottom")]]))

	#time_mesh_config*.inp
	lib.append(my_data("","#fs_laser_time","s","Laser start time","e",1.0,"QLineEdit"))


	#fdtd.inp
	lib.append(my_data("fdtd.inp","#use_gpu","au",_("OpenCL GPU acceleration"),"e",1.0,"gtkswitch"))
	lib.append(my_data("fdtd.inp","#fdtd_lambda_start","m",_("Start wavelength"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fdtd.inp","#fdtd_lambda_stop","m",_("Stop wavelength"),"e",1.0,"QLineEdit"))
	lib.append(my_data("fdtd.inp","#fdtd_lambda_points","m",_("Wavelength steps"),"e",1.0,"QLineEdit"))

	#any files
	lib.append(my_data("","#dump_verbosity","au",_("Ouput verbosity to disk"),"e",1.0,"QComboBoxLang",defaults=[["0",_("Key results")],[("1"),_("Write everything to disk")],[("2"),_("Write everything to disk every 2nd step")],[("5"),_("Write everything to disk every 5th step")],[("10"),_("Write everything to disk every 10th step")]]))
	lib.append(my_data("","#dump_screen_verbosity", "au", _("Ouput verbosity to screen"),"e",1.0,"QComboBoxLang",defaults=[[("dump_verbosity_everything"),_("Show lots")],["dump_verbosity_key_results",_("Show key results")]]))

	#circuit diagram
	lib.append(my_data("","#resistance","Ohms",_("Resistor"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#capacitance","F",_("Capacitor"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#inductance","H",_("Inductance"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#J0","Apms m^{-2}",_("J0"),"e",1.0,"QLineEdit"))
	lib.append(my_data("","#nid","(a.u.)",_("Ideality factor"),"e",1.0,"QLineEdit"))

	
class tokens:

	def __init__(self):
		global lib
		if len(lib)==0:
			build_token_lib()

	def find(self,token):
		global lib

		search_token=token.strip()
		if search_token.startswith("#"):
			search_token=search_token[1:]

		for i in range(0, len(lib)):
			if bool(re.match(lib[i].token[1:]+"$",search_token))==True:
				if lib[i].units=="" and lib[i].info=="":
					return False
				else:
					return lib[i]

		#sys.stdout.write("Add -> lib.append(my_data(\""+token+"\",\"\",\"\",[\"text\"]))\n")
		return False

	def dump_lib(self):
		global lib
		for i in range(0, len(lib)):
			print(">",lib[i].token,"<>",lib[i].info,"<")
			
	def get_lib(self):
		global lib
		return lib
