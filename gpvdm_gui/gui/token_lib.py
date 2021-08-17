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
	def __init__(self,a,b,info,f,widget,defaults=None,units_widget="QLabel",min=None,max=None,hidden=False,hide_on_true_token="none",hide_on_false_token=[],hide_on_token_eq=None):
		self.token=a
		self.units=b
		self.info=info
		self.defaults=defaults
		self.number_mul=f
		self.widget=widget
		self.units_widget=units_widget
		self.hidden=hidden
		self.hide_on_true_token=hide_on_true_token
		self.hide_on_false_token=hide_on_false_token
		self.hide_on_token_eq=hide_on_token_eq
		self.min=min
		self.max=max


lib=[]

def build_token_lib():
	global lib
	#light.inp
	lib.append(my_data("light_wavelength_auto_mesh",_("True/False"),_("Automatically mesh wavelength space"),1.0,"gtkswitch"))
	lib.append(my_data("lpoints","au",_("Mesh points (lambda)"),1.0,"QLineEdit",hide_on_true_token="light_wavelength_auto_mesh"))
	lib.append(my_data("lstart","m",_("Lambda start"),1.0,"QLineEdit",hide_on_true_token="light_wavelength_auto_mesh"))
	lib.append(my_data("lstop","m",_("Lambda stop"),1.0,"QLineEdit",hide_on_true_token="light_wavelength_auto_mesh"))

	lib.append(my_data("electron_eff","0-1",_("Electron generation efficiency"),1.0,"QLineEdit"))
	lib.append(my_data("hole_eff","0-1",_("Hole generation efficiency"),1.0,"QLineEdit"))
	lib.append(my_data("light_file_generation","file_name",_("File containing generation rate"),1.0,"gpvdm_select"))

	lib.append(my_data("Dphotoneff","0-1",_("Photon efficiency"),1.0,"QLineEdit",min=0.001,max=1.2))
	lib.append(my_data("light_file_qe_spectra","au",_("QE spectra file"),1.0,"QLineEdit"))
	lib.append(my_data("meshpoints","au",_("Mesh points (x)"),1.0,"QLineEdit"))

	lib.append(my_data("light_spectrum","au",_("Light spectrum"),1.0,"tb_spectrum"))
	lib.append(my_data("light_multiplyer","au",_("Light multiplyer"),1.0,"QLineEdit"))
	lib.append(my_data("light_iluminate_from","au",_("Iluminate from"),1.0,"QComboBoxLang",defaults=[[("y0"),_("Left")],[("y1"),_("Right")]]))

	#sim_info.dat from light
	lib.append(my_data("J_photo","Am^{-2}",_("Photo current density"),1.0,"QLineEdit"))
	lib.append(my_data("I_photo","A",_("Photo current"),1.0,"QLineEdit"))

	#filter.inp
	lib.append(my_data("filter_material","...",_("Optical filter material"),1.0,"gpvdm_select_material" ,units_widget="QPushButton"))
	lib.append(my_data("filter_db","0-1000dB",_("dB"),1.0,"QLineEdit"))

	#light sources
	lib.append(my_data("light_external_n","Refractive index",_("n"),1.0,"QLineEdit"))
	lib.append(my_data("light_illuminate_from","au",_("Illuminate from"),1.0,"QComboBoxLang",defaults=[[("y0"),_("Top")],[("y1"),_("Bottom")],[("x0"),_("Left")],[("x1"),_("Right")],[("xyz"),_("xyz")]]))
	#generic
	lib.append(my_data("log_fit",_("True/False"),_("Log fit"),1.0,"gtkswitch"))
	lib.append(my_data("human_var",_("Select"),_("Select"),1.0,"gpvdm_select"))
	lib.append(my_data("fit_var_enabled",_("True/False"),_("Enable fit variable"),1.0,"gtkswitch"))
	lib.append(my_data("duplicate_var_enabled",_("True/False"),_("Enable fit variable"),1.0,"gtkswitch"))

	#fit duplicate
	#lib.append(my_data("enabled",_("True/False"),_("Enabled"),1.0,"gtkswitch"))
	lib.append(my_data("human_src",_("Select"),_("Select"),1.0,"gpvdm_select"))
	lib.append(my_data("human_dest",_("Select"),_("Select"),1.0,"gpvdm_select"))
	lib.append(my_data("multiplier",_("multiplier"),_("Multiplier"),1.0,"QLineEdit"))
	lib.append(my_data("json_src",_("json_src"),_("json_src"),1.0,"QLineEdit"))
	lib.append(my_data("json_dest",_("json_dest"),_("json_dest"),1.0,"QLineEdit"))

	#fit rules
	lib.append(my_data("human_x",_("Select"),_("Select"),1.0,"gpvdm_select"))
	lib.append(my_data("human_y",_("Select"),_("Select"),1.0,"gpvdm_select"))
	lib.append(my_data("fit_rule_enabled",_("Select"),_("Select"),1.0,"gtkswitch"))
	#laser?.inp
	lib.append(my_data("laserwavelength","m",_("Laser wavelength"),1.0,"QLineEdit"))
	lib.append(my_data("laser_pulse_width","s",_("Length of pulse"),1.0,"QLineEdit"))
	lib.append(my_data("spotx","m",_("Spot size x"),1.0,"QLineEdit"))
	lib.append(my_data("spoty","m",_("Spot size y"),1.0,"QLineEdit"))
	lib.append(my_data("pulseJ","J",_("Energy in pulse"),1.0,"QLineEdit"))
	lib.append(my_data("laser_photon_efficiency","0-1",_("Efficiency of photons"),1.0,"QLineEdit"))

	#layer widget
	lib.append(my_data("solve_optical_problem","type",_("Solve optical problem"),1.0,"QComboBoxLang",defaults=[[("true"),_("Yes")],["false",_("No")]]))
	lib.append(my_data("solve_thermal_problem","type",_("Solve optical problem"),1.0,"QComboBoxLang",defaults=[[("true"),_("Yes")],["false",_("No")]]))
	lib.append(my_data("layer_type","type",_("Layer type"),1.0,"QComboBoxLang",defaults=[[("contact"),_("contact")],["active",_("active layer")],["other",_("other")]]))

	#dos?.inp
	lib.append(my_data("dostype","Edit",_("DoS distribution"),1.0,"generic_switch",units_widget="QPushButton",defaults=[[_("Complex"),"complex"],[_("Exponential"),"exponential"]],hide_on_token_eq=[["srh_bands",0]]))
	lib.append(my_data("dos_free_carrier_stats","type",_("Free carrier statistics"),1.0,"QComboBoxLang",defaults=[[("mb_equation"),_("Maxwell Boltzmann - analytic")],["mb_look_up_table_analytic",_("Maxwell Boltzmann - numerical+analytic")],["mb_look_up_table",_("Maxwell Boltzmann - full numerical")],["fd_look_up_table",_("Ferm-Dirac - numerical")]]))

	lib.append(my_data("Nc","m^{-3}",_("Effective density of free electron states (@300K)"),1.0,"QLineEdit",min=1e10,max=1e27, ))
	lib.append(my_data("Nv","m^{-3}",_("Effective density of free hole states (@300K)"),1.0,"QLineEdit",min=1e10,max=1e27 ))

	lib.append(my_data("symmetric_mobility_e","m^{2}V^{-1}s^{-1}",_("Electron mobility"),1.0,"mobility_widget",min=1.0,max=1e-1,defaults=[True]))
	lib.append(my_data("symmetric_mobility_h","m^{2}V^{-1}s^{-1}",_("Hole mobility"),1.0,"mobility_widget",min=1.0,max=1e-14, defaults=[False] ))

	lib.append(my_data("mue_z","m^{2}V^{-1}s^{-1}",_("Electron mobility z"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("mue_x","m^{2}V^{-1}s^{-1}",_("Electron mobility x"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("mue_y","m^{2}V^{-1}s^{-1}",_("Electron mobility y"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))

	lib.append(my_data("muh_z","m^{2}V^{-1}s^{-1}",_("Hole mobility z"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("muh_x","m^{2}V^{-1}s^{-1}",_("Hole mobility x"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("muh_y","m^{2}V^{-1}s^{-1}",_("Hole mobility y"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))

	lib.append(my_data("T_start","K",_("Start temperature"),1.0,"QLineEdit"))
	lib.append(my_data("T_stop","K",_("Stop temperature"),1.0,"QLineEdit"))
	lib.append(my_data("T_steps","au",_("Temperature steps"),1.0,"QLineEdit"))


	lib.append(my_data("symmetric_mobility_h","m^{2}V^{-1}s^{-1}",_("Hole mobility"),1.0,"mobility_widget",min=1.0,max=1e-14, defaults=[False] ))

	lib.append(my_data("ion_density","m^{-3}",_("Perovskite ion density"),1.0,"QLineEdit",min=1e10,max=1e27,hidden=True))
	#lib.append(my_data("#ion_mobility","m^{2}V^{-1}s^{-1}",_("Perovskite ion mobility"),1.0,"QLineEdit"))

	lib.append(my_data("doping_start","m^{-3}",_("Doping density (x=0)"),1.0,"QLineEdit",min=1.0,max=1e27,hidden=True))
	lib.append(my_data("doping_stop","m^{-3}",_("Doping density (x=max)"),1.0,"QLineEdit",min=1.0,max=1e27,hidden=True))

	lib.append(my_data("Ntrape","m^{-3} eV^{-1}",_("Electron trap density"),1.0,"QLineEdit",min=1e10,max=1e27,hide_on_token_eq=[["dostype","complex"],["srh_bands",0]] ))
	lib.append(my_data("Ntraph","m^{-3} eV^{-1}",_("Hole trap density"),1.0,"QLineEdit",min=1e10,max=1e27,hide_on_token_eq=[["dostype","complex"],["srh_bands",0]] ))
	lib.append(my_data("Etrape","eV",_("Electron tail slope"),1.0,"QLineEdit",min=20e-3,max=150e-3,hide_on_token_eq=[["dostype","complex"],["srh_bands",0]] ))
	lib.append(my_data("Etraph","eV",_("Hole tail slope"),1.0,"QLineEdit",min=20e-3,max=150e-3,hide_on_token_eq=[["dostype","complex"],["srh_bands",0]] ))
	lib.append(my_data("epsilonr","au",_("Relative permittivity"),1.0,"QLineEdit",min=1.0,max=10.0 ))
	lib.append(my_data("srhsigman_e","m^{-2}",_("Free electron to Trapped electron"),1.0,"QLineEdit",min=1e-27,max=1e-15,hide_on_token_eq=[["srh_bands",0]] ))
	lib.append(my_data("srhsigmap_e","m^{-2}",_("Trapped electron to Free hole"),1.0,"QLineEdit",min=1e-27,max=1e-15,hide_on_token_eq=[["srh_bands",0]] ))
	lib.append(my_data("srhsigman_h","m^{-2}",_("Trapped hole to Free electron"),1.0,"QLineEdit",min=1e-27,max=1e-15,hide_on_token_eq=[["srh_bands",0]] ))
	lib.append(my_data("srhsigmap_h","m^{-2}",_("Free hole to Trapped hole"),1.0,"QLineEdit",min=1e-27,max=1e-15,hide_on_token_eq=[["srh_bands",0]]))
	lib.append(my_data("free_to_free_recombination","m^{3}s^{-1}",_("n_{free} to p_{free} Recombination rate constant"),1.0,"QLineEdit",min=1e-27,max=1e-15 ))
	lib.append(my_data("Eg","eV",_("Eg"),1.0,"QLineEdit"))
	lib.append(my_data("Xi","eV",_("Xi"),1.0,"QLineEdit"))

	#materials database
	#electrical constants
	lib.append(my_data("material_blend",_("True/False"),_("Blended material"),1.0,"gtkswitch" ))
	lib.append(my_data("Eg0","eV",_("Eg_{0}"),1.0,"QLineEdit"))
	lib.append(my_data("Xi0","eV",_("Xi_{0}"),1.0,"QLineEdit"))
	lib.append(my_data("Eg1","eV",_("Eg_{1}"),1.0,"QLineEdit",hide_on_false_token=["material_blend"]))
	lib.append(my_data("Xi1","eV",_("Xi_{1}"),1.0,"QLineEdit",hide_on_false_token=["material_blend"]))
	lib.append(my_data("srh_bands","bands",_("Number of traps"),1.0,"QLineEdit"))
	#thermal constants
	lib.append(my_data("thermal_kl","W m^{-1} C^{-1}",_("Thermal conductivity"),1.0,"QLineEdit"))
	lib.append(my_data("thermal_tau_e","s",_("Electron relaxation time"),1.0,"QLineEdit"))
	lib.append(my_data("thermal_tau_h","s",_("Hole relaxation time"),1.0,"QLineEdit"))


	#electrical?.inp
	lib.append(my_data("electrical_component","type",_("Component"),1.0,"QComboBoxLang",defaults=[[("resistance"),_("Resistance")],["diode",_("Diode")],["link",_("Link")]]))
	lib.append(my_data("electrical_shunt","Ohm  m",_("Shunt resistivity"),1.0,"QLineEdit",min=0.1,max=1e20, hide_on_token_eq=[["electrical_component","resistance"],["electrical_component","link"]] ))

	lib.append(my_data("electrical_symmetrical_resistance","Ohm m",_("Series resistivity"),1.0,"mobility_widget", defaults=[False] ))

	lib.append(my_data("electrical_series_z","Ohm m",_("Series resistivity z"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("electrical_series_x","Ohm m",_("Series resistivity x"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))
	lib.append(my_data("electrical_series_y","Ohm m",_("Series resistivity y"),1.0,"mobility_widget",min=1.0,max=1e-1,hidden=True))

	lib.append(my_data("electrical_n","au",_("Layer ideality factor"),1.0,"QLineEdit",min=0.0,max=1.0, hide_on_token_eq=[["electrical_component","resistance"],["electrical_component","link"]] ))
	lib.append(my_data("electrical_J0","A m^{-2}",_("Reverse bias current"),1.0,"QLineEdit",min=0.0,max=1e6, hide_on_token_eq=[["electrical_component","resistance"],["electrical_component","link"]] ))
	lib.append(my_data("electrical_enable_generation",_("True/False"),_("Enable optical charge\ncarrier generation"),1.0,"gtkswitch", hide_on_token_eq=[["electrical_component","resistance"],["electrical_component","link"]] ))


	#spm?.inp
	lib.append(my_data("spm_voltage","V",_("Applied voltage"),1.0,"QLineEdit"))
	lib.append(my_data("spm_scan_section","su",_("Scan section"),1.0,"QComboBoxLang",defaults=[[("spm_whole_device"),_("Whole device")],["spm_box",_("Box section")],["spm_x_cut",_("x-Scan")]]))
	lib.append(my_data("spm_x0","m",_("x-start"),1.0,"QLineEdit",hide_on_token_eq=[["spm_scan_section","spm_whole_device"],["spm_scan_section","spm_x_cut"]]))
	lib.append(my_data("spm_z0","m",_("z-start"),1.0,"QLineEdit",hide_on_token_eq=[["spm_scan_section","spm_whole_device"],["spm_scan_section","spm_x_cut"]]))
	lib.append(my_data("spm_x1","m",_("x-stop"),1.0,"QLineEdit",hide_on_token_eq=[["spm_scan_section","spm_whole_device"],["spm_scan_section","spm_x_cut"]]))
	lib.append(my_data("spm_z1","m",_("z-stop"),1.0,"QLineEdit",hide_on_token_eq=[["spm_scan_section","spm_whole_device"],["spm_scan_section","spm_x_cut"]]))

	#shape?.inp
	lib.append(my_data("shape_type",_("Edit"),_("Shape type"),1.0,"gpvdm_select_shape",units_widget="QPushButton"))
	lib.append(my_data("rotate_y","degrees",_("Rotate around y-axis"),1.0,"QLineEdit"))
	lib.append(my_data("rotate_x","degrees",_("Rotate around x-axis"),1.0,"QLineEdit"))

	lib.append(my_data("dx","m",_("dx of the object"),1.0,"QLineEdit"))
	lib.append(my_data("dy","m",_("dy of the object"),1.0,"QLineEdit"))
	lib.append(my_data("dz","m",_("dz of the object"),1.0,"QLineEdit"))

	lib.append(my_data("dx_padding","m",_("dx padding"),1.0,"QLineEdit"))
	lib.append(my_data("dy_padding","m",_("dy padding"),1.0,"QLineEdit"))
	lib.append(my_data("dz_padding","m",_("dz padding"),1.0,"QLineEdit"))


	lib.append(my_data("shape_nx","au",_("Number of objects x"),1.0,"QLineEdit"))
	lib.append(my_data("shape_ny","au",_("Number of objects y"),1.0,"QLineEdit"))
	lib.append(my_data("shape_nz","au",_("Number of objects z"),1.0,"QLineEdit"))
	lib.append(my_data("x0","m",_("x offset"),1.0,"QLineEdit"))
	lib.append(my_data("y0","m",_("y offset"),1.0,"QLineEdit"))
	lib.append(my_data("z0","m",_("z offset"),1.0,"QLineEdit"))
	lib.append(my_data("shape_remove_layer",_("True/False"),_("Remove layer"),1.0,"gtkswitch"))
	lib.append(my_data("shape_dos",_("Edit"),_("Drift diffusion"),1.0,"shape_dos_switch",units_widget="QPushButton"))
	lib.append(my_data("shape_electrical",_("Edit"),_("Circuit model\nparameters"),1.0,"shape_electrical_switch",units_widget="QPushButton"))
	lib.append(my_data("optical_material",_("Edit"),_("Optical material"),1.0,"gpvdm_select_material" ,units_widget="QPushButton"))
	lib.append(my_data("color_r","rgb",_("Color"),1.0,"QColorPicker"))

	#interface?.inp
	lib.append(my_data("interface_model","type",_("Interface model"),1.0,"QComboBoxLang",defaults=[[("none"),_("None")],["recombination",_("Free-to-Free")],["recombination_srh",_("Free-to-trap (SRH)")]]))
	lib.append(my_data("interface_eh_tau","m^{3}s^{-1}",_("Recombination constant"),1.0,"QLineEdit",hide_on_token_eq=[["interface_model","none"]]))

	lib.append(my_data("interface_left_doping_enabled",_("True/False"),_("Interface doping LHS"),1.0,"gtkswitch"))
	lib.append(my_data("interface_left_doping","m^{-3}",_("Doping LHS"),1.0,"QLineEdit", hide_on_false_token=["interface_left_doping_enabled"]))

	lib.append(my_data("interface_right_doping_enabled",_("True/False"),_("Interface doping RHS"),1.0,"gtkswitch"))
	lib.append(my_data("interface_right_doping","m^{-3}",_("Doping RHS"),1.0,"QLineEdit", hide_on_false_token=["interface_right_doping_enabled"]))


	#stark.inp
	lib.append(my_data("#stark_startime","s",_("startime"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_ea_factor","au",_("ea_factor"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_Np","1/0",_("Np"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_den","1/0",_("den"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_externalv","V",_("externalv"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_dt_neg_time","s",_("dt_neg_time"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_dt","s",_("dt"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_dt_mull","au",_("dt_mull"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_stop","s",_("stop"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_stark","1/0",_("stark"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_lasereff","1/0",_("lasereff"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_probe_wavelength","nm",_("wavelength"),1e9,"QLineEdit"))
	lib.append(my_data("#stark_sim_contacts","1/0",_("sim_contacts"),1.0,"QLineEdit"))

	#ref / bib
	lib.append(my_data("url","au",_("Website"),1.0,"QLineEdit"))
	lib.append(my_data("author","au",_("Author"),1.0,"QLineEdit"))
	lib.append(my_data("journal","au",_("Journal"),1.0,"QLineEdit"))
	lib.append(my_data("title","au",_("Title"),1.0,"QLineEdit"))
	lib.append(my_data("volume","au",_("Volume"),1.0,"QLineEdit"))
	lib.append(my_data("pages","au",_("Pages"),1.0,"QLineEdit"))
	lib.append(my_data("year","au",_("Year"),1.0,"QLineEdit"))
	lib.append(my_data("DOI","au",_("DOI"),1.0,"QLineEdit"))
	lib.append(my_data("booktitle","au",_("Book title"),1.0,"QLineEdit"))
	lib.append(my_data("publisher","au",_("Publisher"),1.0,"QLineEdit"))
	lib.append(my_data("isbn","au",_("ISBN"),1.0,"QLineEdit"))
	lib.append(my_data("unformatted","au",_("Scraped text"),1.0,"QLineEdit"))

	#pulse?.inp
	lib.append(my_data("pulse_shift","s","Shift of TPC signal",1.0,"QLineEdit"))
	lib.append(my_data("Rshort_pulse","Ohms",_("R_{short}"),1.0,"QLineEdit"))
	lib.append(my_data("pulse_bias","V",_("V_{bias}"),1.0,"QLineEdit"))
	lib.append(my_data("pulse_light_efficiency","au",_("Efficiency of light"),1.0,"QLineEdit"))
	lib.append(my_data("pulse_subtract_dc",_("True/False"),_("subtract DC"),1.0,"gtkswitch"))
	lib.append(my_data("Rload","Ohms",_("External load resistor"),1.0,"QLineEdit"))

	#mat.inp
	lib.append(my_data("material_type","type",_("Material type"),1.0,"QComboBoxLang",defaults=[[("organic"),_("Organic")],["oxide",_("Oxide")],["inorganic",_("Inorganic")],["metal",_("Metal")],["other",_("Other")]]))
	lib.append(my_data("mat_alpha","0-1",_("Transparency"),1.0,"QLineEdit"))
	lib.append(my_data("status","type",_("Privicy options"),1.0,"QComboBoxLang",defaults=[[("public"),_("Public - everyone")],[("public_internal"),_("Public - within org")],["private",_("Private - never publish")]]))
	lib.append(my_data("changelog","au",_("Change log"),1.0,"QChangeLog"))

	#jv.inp
	lib.append(my_data("jv_step_mul","0-2.0",_("JV voltage step multiplyer"),1.0,"QLineEdit",hide_on_true_token="#jv_single_point"))
	lib.append(my_data("jv_max_j","A m^{-2}",_("Maximum current density"),1.0,"QLineEdit"))
	lib.append(my_data("jv_light_efficiency","au",_("JV curve photon generation efficiency"),1.0,"QLineEdit"))
	lib.append(my_data("jv_pmax_n","m^{-3}",_("Average carrier density at P_{max}"),1.0,"QLineEdit"))
	lib.append(my_data("Vstart","V",_("Start voltage"),1.0,"QLineEdit"))
	lib.append(my_data("Vstop","V",_("Stop voltage"),1.0,"QLineEdit",hide_on_true_token="#jv_single_point"))
	lib.append(my_data("Vstep","V",_("Voltage step"),1.0,"QLineEdit",hide_on_true_token="#jv_single_point"))
	lib.append(my_data("jv_Rcontact","V",_("JV curve contact resistance"),1.0,"QParasitic"))
	lib.append(my_data("jv_Rshunt","V",_("JV curve shunt resistance"),1.0,"QParasitic"))
	lib.append(my_data("jv_single_point",_("True/False"),_("Single point"),1.0,"gtkswitch"))
	lib.append(my_data("jv_use_external_voltage_as_stop",_("True/False"),_("Use external\nvoltage as stop"),1.0,"gtkswitch"))


	#sim_info.dat (jv plugin)
	lib.append(my_data("voc","V",_("V_{oc}"),1.0,"QLineEdit"))
	lib.append(my_data("pce","Percent",_("Power conversion efficiency"),1.0,"QLineEdit"))
	lib.append(my_data("ff","a.u.",_("Fill factor"),1.0,"QLineEdit"))
	lib.append(my_data("Pmax","W m^{-2}",_("Max power"),1.0,"QLineEdit"))
	lib.append(my_data("v_pmax","V",_("Voltage at max power"),1.0,"QLineEdit"))
	lib.append(my_data("j_pmax","Am^{-2}",_("Current density at max power"),1.0,"QLineEdit"))

	lib.append(my_data("voc_nt","m^{-3}",_("Trapped electrons at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("voc_pt","m^{-3}",_("Trapped holes at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("voc_nf","m^{-3}",_("Free electrons at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("voc_pf","m^{-3}",_("Free holes at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("voc_np_tot","m^{-3}",_("Total carriers (n+p)/2 at Voc"),1.0,"QLineEdit"))

	lib.append(my_data("voc_R","m^{-3}s^{-1}",_("Recombination rate at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("voc_J","A m^{-2}",_("Current density at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("jsc","A m^{-2}",_("J_{sc}"),1.0,"QLineEdit"))

	#mu
	lib.append(my_data("mue_pmax","m^{2}V^{-1}s^{-1}",_("Electron mobility at P_{max}"),1.0,"QLineEdit"))
	lib.append(my_data("muh_pmax","m^{2}V^{-1}s^{-1}",_("Hole mobility at P_{max}"),1.0,"QLineEdit"))
	lib.append(my_data("mu_jsc","m^{2}V^{-1}s^{-1}",_("Average mobility as J_{sc}"),1.0,"QLineEdit"))
	lib.append(my_data("mu_pmax","m^{2}V^{-1}s^{-1}",_("Average mobility at P_{max}"),1.0,"QLineEdit"))
	lib.append(my_data("mu_voc","m^{2}V^{-1}s^{-1}",_("Average mobility at V_{oc}"),1.0,"QLineEdit"))

	#theta

	lib.append(my_data("theta_srh_free","au",_("Theta_{SRH} - free P_{max}"),1.0,"QLineEdit"))
	lib.append(my_data("theta_srh_free_trap","au",_("Theta_{SRH} - free-trap P_{max}"),1.0,"QLineEdit"))
	#tau
	lib.append(my_data("tau_pmax","m^{-1}",_("Recombination time constant"),1.0,"QLineEdit"))
	lib.append(my_data("tau_voc","s",_("Recombination time constant at Voc"),1.0,"QLineEdit"))
	lib.append(my_data("device_C","F",_("Device capacitance"),1.0,"QLineEdit"))



	#sim_info.dat (optics plugin)
	lib.append(my_data("#light_photons_in_active_layer","m^{-2}",_("Photos absorbed in active layer"),1.0,"QLineEdit"))


	#object_stats.dat (optics plugin)
	lib.append(my_data("object_stats.dat","#Rp[0-9]","m",_("Peak height Rp"),1.0,"QLineEdit"))
	lib.append(my_data("object_stats.dat","#Rq[0-9]","m",_("RMS height Rq"),1.0,"QLineEdit"))
	lib.append(my_data("object_stats.dat","#Ra[0-9]","m",_("Average height Ra"),1.0,"QLineEdit"))

	#cv?.inp
	lib.append(my_data("cv_start_voltage","Volts",_("Start voltage"),1.0,"QLineEdit"))
	lib.append(my_data("cv_stop_voltage","Volts",_("Stop voltage"),1.0,"QLineEdit"))
	lib.append(my_data("cv_dv_step","Volts",_("dV step"),1.0,"QLineEdit"))
	lib.append(my_data("cv_fx","Hz",_("Frequency"),1.0,"QLineEdit"))


	#sim_info.dat (equlibrium)
	lib.append(my_data("#left_holes","m^{-3}",_("Left hole density"),1.0,"QLineEdit"))
	lib.append(my_data("#left_electrons","m^{-3}",_("Left electron density"),1.0,"QLineEdit"))
	lib.append(my_data("#right_holes","m^{-3}",_("Right hole density"),1.0,"QLineEdit"))
	lib.append(my_data("#right_electrons","m^{-3}",_("Right electron density"),1.0,"QLineEdit"))
	lib.append(my_data("#Vbi","m^{-3}",_("Built in potential"),1.0,"QLineEdit"))
	lib.append(my_data("#electron_affinity_left","eV",_("Electron affinity left"),1.0,"QLineEdit"))
	lib.append(my_data("#electron_affinity_right","eV",_("Electron affinity right"),1.0,"QLineEdit"))

	#tab
	lib.append(my_data("#english_name","name",_("English name"),1.0,"QLineEdit"))

	#server.inp
	lib.append(my_data("gpvdm_core_max_threads","au",_("Number of gpvdm_core threads"),1.0,"QLineEdit"))
	lib.append(my_data("max_gpvdm_instances","au",_("Maximum number of gpvdm_core instances"),1.0,"QLineEdit"))

	lib.append(my_data("server_stall_time","","",1.0,"QLineEdit"))
	lib.append(my_data("server_exit_on_dos_error","","",1.0,"QLineEdit"))
	lib.append(my_data("server_max_run_time","","",1.0,"QLineEdit"))
	lib.append(my_data("server_auto_cpus","","",1.0,"QLineEdit"))
	lib.append(my_data("server_min_cpus","","",1.0,"QLineEdit"))
	lib.append(my_data("server_steel","","",1.0,"QLineEdit"))
	lib.append(my_data("port","au","Cluster port",1.0,"QLineEdit"))
	lib.append(my_data("path_to_src","au",_("Path to source code"),1.0,"QLineEdit"))
	lib.append(my_data("path_to_libs","au",_("Path to compiled libs for cluster"),1.0,"QLineEdit"))
	lib.append(my_data("make_command","au",_("Make command"),1.0,"QLineEdit"))
	lib.append(my_data("exe_name","au",_("exe name"),1.0,"QLineEdit"))
	lib.append(my_data("server_use_dos_disk_cache",_("True/False"),_("Store DoS on disk"),1.0,"gtkswitch"))

	#cluster.inp
	lib.append(my_data("cluster_user_name","au","User name",1.0,"QLineEdit"))
	lib.append(my_data("cluster_ip","au","Cluster IP",1.0,"QLineEdit"))
	lib.append(my_data("cluster_master_ip","au","Cluster master IP",1.0,"QLineEdit"))
	lib.append(my_data("cluster_cluster_dir","au",_("Remote cluster directory"),1.0,"QLineEdit"))
	lib.append(my_data("cluster_node_list","au",_("Remote node list"),1.0,"QLineEdit"))
	lib.append(my_data("cluster_iv","au",_("IV"),1.0,"QLineEdit"))
	lib.append(my_data("cluster_key","au",_("Key"),1.0,"QLineEdit"))

	#triangle mesh editor
	lib.append(my_data("mesh_gen_nx","au",_("x-triangles"),1.0,"QLineEdit"))
	lib.append(my_data("mesh_gen_ny","au",_("y-triangles"),1.0,"QLineEdit"))
	lib.append(my_data("mesh_gen_opp","au",_("Method"),1.0,"QComboBoxLang",defaults=[["node_reduce",_("Node reduce")],["square_mesh_gen",_("No reduce")]]))
	lib.append(my_data("mesh_min_ang","au",_("Min allowable angle"),1.0,"QLineEdit"))

	lib.append(my_data("shape_import_blur","width pixels",_("Gaussian blur"),1.0,"QLineEdit"))
	lib.append(my_data("shape_import_y_norm_percent","percent",_("Percent of histogram to ignore"),1.0,"QLineEdit"))

	lib.append(my_data("gauss_sigma","pixels",_("Sigma of gaussian"),1.0,"QLineEdit"))
	lib.append(my_data("gauss_offset_x","pixels",_("Gaussian offset x"),1.0,"QLineEdit"))
	lib.append(my_data("gauss_offset_y","pixels",_("Gaussian offset y"),1.0,"QLineEdit"))

	#honeycomb
	lib.append(my_data("honeycomb_dx","pixels",_("dx of Honeycomb"),1.0,"QLineEdit"))
	lib.append(my_data("honeycomb_dy","pixels",_("dy of Honeycomb"),1.0,"QLineEdit"))
	lib.append(my_data("honeycomb_line_width","pixels",_("Line width"),1.0,"QLineEdit"))
	lib.append(my_data("honeycomb_x_shift","pixels",_("x shift"),1.0,"QLineEdit"))
	lib.append(my_data("honeycomb_y_shift","pixels",_("y shift"),1.0,"QLineEdit"))
	lib.append(my_data("image_ylen","pixels",_("y size"),1.0,"QLineEdit"))
	lib.append(my_data("image_xlen","pixels",_("x size"),1.0,"QLineEdit"))


	#honeycomb
	lib.append(my_data("shape_import_blur","pixels",_("pixels"),1.0,"QLineEdit"))


	#boundary
	lib.append(my_data("image_boundary_x0","pixels",_("Boundary x0"),1.0,"QLineEdit"))
	lib.append(my_data("image_boundary_x1","pixels",_("Boundary x1"),1.0,"QLineEdit"))
	lib.append(my_data("image_boundary_y0","pixels",_("Boundary y0"),1.0,"QLineEdit"))
	lib.append(my_data("image_boundary_y1","pixels",_("Boundary y1"),1.0,"QLineEdit"))

	#math.inp
	lib.append(my_data("maxelectricalitt_first","au",_("Max Electrical itterations (first step)"),1.0,"QLineEdit"))
	lib.append(my_data("electricalclamp_first","au",_("Electrical clamp (first step)"),1.0,"QLineEdit"))
	lib.append(my_data("math_electrical_error_first","au",_("Desired electrical solver error (first step)"),1.0,"QLineEdit"))
	lib.append(my_data("maxelectricalitt","au",_("Max electrical itterations"),1.0,"QLineEdit"))
	lib.append(my_data("electricalclamp","au",_("Electrical clamp"),1.0,"QLineEdit"))
	lib.append(my_data("electricalerror","au",_("Minimum electrical error"),1.0,"QLineEdit"))



	lib.append(my_data("newton_clever_exit",_("True/False"),"Newton solver clever exit",1.0,"gtkswitch"))
	lib.append(my_data("newton_min_itt","au",_("Newton minimum iterations"),1.0,"QLineEdit"))
	lib.append(my_data("complex_solver_name",_("dll name"),_("Complex matrix solver to use"),1.0,"QLineEdit"))
	lib.append(my_data("math_stop_on_convergence_problem",_("True/False"),_("Quit on convergence problem"),1.0,"gtkswitch"))

	#pos
	lib.append(my_data("pos_max_ittr","au",_("Poisson solver max itterations"),1.0,"QLineEdit"))
	lib.append(my_data("pos_min_error","au",_("Minimum electrical error"),1.0,"QLineEdit"))
	lib.append(my_data("posclamp","au",_("Poisson clamping"),1.0,"QLineEdit"))
	lib.append(my_data("math_enable_pos_solver",_("True/False"),_("Enable poisson solver"),1.0,"gtkswitch"))
	lib.append(my_data("pos_dump_verbosity","au",_("Output verbosity to disk"),1.0,"QComboBoxLang",defaults=[["0",_("Nothing")],[("1"),_("Write everything to disk")]]))

	lib.append(my_data("solver_name",_("dll name"),_("Matrix solver"),1.0,"QComboBoxNewtonSelect",defaults=["umfpack","external_solver","superlu","nr_d","nr_ld"]))

	lib.append(my_data("newton_name",_("dll name"),_("Newton solver to use"),1.0,"QComboBoxNewtonSelect",defaults=["none","newton_2d","newton_norm","newton","poisson_2d"]))
	lib.append(my_data("math_t0","au",_("Slotboom T0"),1.0,"QLineEdit"))
	lib.append(my_data("math_d0","au",_("Slotboom D0"),1.0,"QLineEdit"))
	lib.append(my_data("math_n0","au",_("Slotboom n0"),1.0,"QLineEdit"))
	lib.append(my_data("math_newton_cache","au",_("Use newton cache (experimental)"),1.0,"gtkswitch"))
	lib.append(my_data("kl_in_newton","au",_("Solve Kirchhoff's current law in Newton solver"),1.0,"gtkswitch"))


	#fit.inp
	lib.append(my_data("fit_error_mul","au",_("Fit error multiplyer"),1.0,"QLineEdit"))
	lib.append(my_data("fit_randomize",_("True/False"),_("Randomize fit"),1.0,"gtkswitch"))
	lib.append(my_data("fit_random_reset_ittr","au",_("Number of iterations between random reset"),1.0,"QLineEdit"))
	lib.append(my_data("fit_stall_steps","au",_("Stall steps"),1.0,"QLineEdit"))
	lib.append(my_data("fit_disable_reset_at","au",_("Disable reset at level"),1.0,"QLineEdit"))
	lib.append(my_data("fit_converge_error","au",_("Fit define convergence"),1.0,"QLineEdit"))
	lib.append(my_data("fit_enable_simple_reset","au",_("Enable simplex reset"),1.0,"gtkswitch"))
	lib.append(my_data("fit_enable_simple_reset","au",_("Simplex reset steps"),1.0,"gtkswitch"))
	lib.append(my_data("fit_method","au",_("Fiting method"),1.0,"QComboBox",defaults=["simplex","newton"]))
	lib.append(my_data("fit_simplexmul","au",_("Start simplex step multiplication"),1.0,"QLineEdit"))
	lib.append(my_data("fit_simplex_reset","au",_("Simplex reset steps"),1.0,"QLineEdit"))
	lib.append(my_data("fit_set_error_to_zero_before","au",_("Set error to zero before"),1.0,"QLineEdit"))

	#eqe.inp
	lib.append(my_data("eqe_voltage","au",_("EQE Voltage"),1.0,"QLineEdit"))

	#thermal.inp
	lib.append(my_data("thermal_model_type","au",_("Thermal model type"),1.0,"QComboBoxLang",defaults=[["thermal_hydrodynamic",_("Hydrodynamic")],["thermal_lattice",_("Lattice heat")]], hide_on_false_token=["thermal"]))

	lib.append(my_data("Ty0","Kelvin",_("Device temperature at y_{min}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Ty0_boundry", "neumann"]]))
	lib.append(my_data("Ty0_boundry","au",_("Boundary condition for y_{min}"),1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("heatsink_y0","W m^{-}K^{-1}",_("Conductivity of heat sink y_{min}"),1.0,"QLineEdit", hide_on_false_token=["thermal"],  hide_on_token_eq=[["Ty0_boundry", "neumann"],["Ty0_boundry", "isothermal"]]))
	lib.append(my_data("heatsink_length_y0","m",_("Heat sink length y_{min}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Ty0_boundry", "neumann"],["Ty0_boundry", "isothermal"]]))

	lib.append(my_data("Ty1","Kelvin",_("Device temperature at y_{max}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Ty1_boundry", "neumann"]]))
	lib.append(my_data("Ty1_boundry","au",_("Boundary condition for y_{max}"),1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("heatsink_y1","W m^{-2}K^{-1}",_("Conductivity of heat sink y_{max}"),1.0,"QLineEdit", hide_on_false_token=["thermal"],  hide_on_token_eq=[["Ty1_boundry", "neumann"],["Ty1_boundry", "isothermal"]]))
	lib.append(my_data("heatsink_length_y1","m",_("Heat sink length y_{max}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Ty1_boundry", "neumann"],["Ty1_boundry", "isothermal"]]))

	lib.append(my_data("Tx0","Kelvin",_("Device temperature at x_{min}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tx0_boundry", "neumann"]]))
	lib.append(my_data("Tx0_boundry","au",_("Boundary condition for x_{min}"),1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("heatsink_x0","W m^{-2}K^{-1}",_("Conductivity of heat sink x_{min}"),1.0,"QLineEdit", hide_on_false_token=["thermal"],  hide_on_token_eq=[["Tx0_boundry", "neumann"],["Tx0_boundry", "isothermal"]]))
	lib.append(my_data("heatsink_length_x0","m",_("Heat sink length x_{min}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tx0_boundry", "neumann"],["Tx0_boundry", "isothermal"]]))

	lib.append(my_data("Tx1","Kelvin",_("Device temperature at x_{max}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tx1_boundry", "neumann"]]))
	lib.append(my_data("Tx1_boundry","au",_("Boundary condition for x_{max}"),1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("heatsink_x1","W m^{-2}K^{-1}",_("Conductivity of heat sink x_{max}"),1.0,"QLineEdit", hide_on_false_token=["thermal"],  hide_on_token_eq=[["Tx1_boundry", "neumann"],["Tx1_boundry", "isothermal"]]))
	lib.append(my_data("heatsink_length_x1","m",_("Heat sink length x_{max}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tx1_boundry", "neumann"],["Tx1_boundry", "isothermal"]]))

	lib.append(my_data("Tz0","Kelvin",_("Device temperature at z_{min}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tz0_boundry", "neumann"]]))
	lib.append(my_data("Tz0_boundry","au",_("Boundary condition for z_{min}"),1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("heatsink_z0","W m^{-2}K^{-1}",_("Conductivity of heat sink z_{min}"),1.0,"QLineEdit", hide_on_false_token=["thermal"],  hide_on_token_eq=[["Tz0_boundry", "neumann"],["Tz0_boundry", "isothermal"]]))
	lib.append(my_data("heatsink_length_z0","m",_("Heat sink length z_{min}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tz0_boundry", "neumann"],["Tz0_boundry", "isothermal"]]))

	lib.append(my_data("Tz1","Kelvin",_("Device temperature at z_{max}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tz1_boundry", "neumann"]]))
	lib.append(my_data("Tz1_boundry","au",_("Boundary condition for z_{max}"),1.0,"QComboBoxLang",defaults=[["isothermal",_("Isothermal")],["neumann",_("Neumann (==0)")],["heatsink",_("Heatsink")]], hide_on_false_token=["#thermal"]))
	lib.append(my_data("heatsink_z1","W m^{-2}K^{-1}",_("Conductivity of heat sink z_{max}"),1.0,"QLineEdit", hide_on_false_token=["thermal"],  hide_on_token_eq=[["Tz1_boundry", "neumann"],["Tz1_boundry", "isothermal"]]))
	lib.append(my_data("heatsink_length_z1","m",_("Heat sink length z_{max}"),1.0,"QLineEdit", hide_on_false_token=["#thermal"],  hide_on_token_eq=[["Tz1_boundry", "neumann"],["Tz1_boundry", "isothermal"]]))


	lib.append(my_data("thermal_l",_("True/False"),_("Lattice heat model"),1.0,"gtkswitch",hide_on_token_eq=[["thermal_model_type", "thermal_lattice"]], hide_on_false_token=["thermal"]))
	lib.append(my_data("thermal_e",_("True/False"),_("Electron heat model"),1.0,"gtkswitch",hide_on_token_eq=[["thermal_model_type", "thermal_lattice"]], hide_on_false_token=["thermal"]))
	lib.append(my_data("thermal_h",_("True/False"),_("Hole heat model"),1.0,"gtkswitch",hide_on_token_eq=[["thermal_model_type", "thermal_lattice"]], hide_on_false_token=["thermal"]))

	lib.append(my_data("thermal_max_ittr","au",_("Max thermal solver iterations"),1.0,"QLineEdit"))
	lib.append(my_data("thermal_min_error","au",_("Desired thermal error"),1.0,"QLineEdit"))

	lib.append(my_data("Tliso",_("True/False"),_("Isothermal boundary on left"),1.0,"gtkswitch", hide_on_false_token=["#thermal"]))
	lib.append(my_data("Triso",_("True/False"),_("Isothermal boundary on right"),1.0,"gtkswitch", hide_on_false_token=["#thermal"]))

	#electrical.inp
	lib.append(my_data("electrical_y0_boundry","au",_("Boundary condition for y_{min}"),1.0,"QComboBoxLang",defaults=[["neumann",_("Neumann (==0)")],["interpolate",_("Interpolate")],["constant",_("Constant")]]))
	lib.append(my_data("electrical_y1_boundry","au",_("Boundary condition for y_{max}"),1.0,"QComboBoxLang",defaults=[["neumann",_("Neumann (==0)")],["interpolate",_("Interpolate")],["constant",_("Constant")]]))
	lib.append(my_data("electrical_x0_boundry","au",_("Boundary condition for x_{min}"),1.0,"QComboBoxLang",defaults=[["neumann",_("Neumann (==0)")],["interpolate",_("Interpolate")],["constant",_("Constant")]]))
	lib.append(my_data("electrical_x1_boundry","au",_("Boundary condition for x_{max}"),1.0,"QComboBoxLang",defaults=[["neumann",_("Neumann (==0)")],["interpolate",_("Interpolate")],["constant",_("Constant")]]))

	lib.append(my_data("electrical_y0","V",_("Boundary value for y_{min}"),1.0,"QLineEdit",hide_on_token_eq=[["electrical_y0_boundry", "neumann"],["electrical_y0_boundry", "interpolate"]]))
	lib.append(my_data("electrical_y1","V",_("Boundary value for y_{max}"),1.0,"QLineEdit",hide_on_token_eq=[["electrical_y1_boundry", "neumann"],["electrical_y1_boundry", "interpolate"]]))

	lib.append(my_data("electrical_x0","V",_("Boundary value for x_{min}"),1.0,"QLineEdit",hide_on_token_eq=[["electrical_x0_boundry", "neumann"],["electrical_x0_boundry", "interpolate"]]))
	lib.append(my_data("electrical_x1","V",_("Boundary value for x_{max}"),1.0,"QLineEdit",hide_on_token_eq=[["electrical_x1_boundry", "neumann"],["electrical_x1_boundry", "interpolate"]]))

	lib.append(my_data("thermal_couple_to_electrical_solver",_("True/False"),_("Couple to electrical solver"),1.0,"gtkswitch"))


	#dump.inp
	lib.append(my_data("newton_dump",_("True/False"),_("Dump from newton solver"),1.0,"gtkswitch"))
	lib.append(my_data("plot",_("True/False"),_("Plot bands etc.. "),1.0,"gtkswitch"))
	lib.append(my_data("dump_slices",_("True/False"),_("Dump slices"),1.0,"gtkswitch"))
	lib.append(my_data("dump_dynamic",_("True/False"),_("Dump dynamic"),1.0,"gtkswitch"))
	lib.append(my_data("dump_zip_files",_("True/False"),_("Dump zip files"),1.0,"gtkswitch"))
	lib.append(my_data("dump_write_out_band_structure",_("True/False"),_("Write out band structure"),1.0,"gtkswitch"))
	lib.append(my_data("dump_write_converge",_("True/False"),_("Write newton solver convergence to disk"),1.0,"gtkswitch"))
	lib.append(my_data("dump_print_pos_error",_("True/False"),_("Print poisson solver convergence"),1.0,"gtkswitch"))
	lib.append(my_data("dump_norm_time_to_one",_("True/False"),_("Normalize output x-time to one"),1.0,"gtkswitch"))
	lib.append(my_data("dump_built_in_voltage",_("True/False"),_("Dump the built in voltage."),1.0,"gtkswitch"))
	lib.append(my_data("dump_optical_probe_spectrum",_("True/False"),_("Dump optical probe spectrum"),1.0,"gtkswitch"))
	lib.append(my_data("dump_optics_summary",_("True/False"),_("Dump optical summary"),1.0,"gtkswitch"))
	lib.append(my_data("dump_ray_trace_map",_("True/False"),_("Dump raytrace plots"),1.0,"gtkswitch"))
	lib.append(my_data("dumpitdos",_("True/False"),_("Dump DoS"),1.0,"gtkswitch"))
	lib.append(my_data("dump_file_access_log",_("True/False"),_("Write file access log to disk."),1.0,"gtkswitch"))
	lib.append(my_data("dump_use_cache",_("True/False"),_("Use cache for file writes"),1.0,"gtkswitch"))
	lib.append(my_data("dump_write_headers",_("True/False"),_("Write headers to output files"),1.0,"gtkswitch"))
	lib.append(my_data("dump_first_guess",_("True/False"),_("Write first guess to equations"),1.0,"gtkswitch"))
	lib.append(my_data("dump_log_level","au",_("Log verbocity"),1.0,"QComboBoxLang",defaults=[[("none"),_("None")],["screen",_("Screen")],["disk",_("Disk")],["screen_and_disk",_("Screen and disk")]]))
	lib.append(my_data("dump_log_level","au",_("Log verbocity"),1.0,"QComboBoxLang",defaults=[[("none"),_("None")],["screen",_("Screen")],["disk",_("Disk")],["screen_and_disk",_("Screen and disk")]]))
	lib.append(my_data("dump_dynamic_pl_energy",_("True/False"),_("PL dump Energy"),1.0,"gtkswitch"))
	lib.append(my_data("dump_remove_dos_cache",_("True/False"),_("Clean up DoS cache files"),1.0,"gtkswitch"))
	lib.append(my_data("dump_verbose_electrical_solver_results",_("True/False"),_("Dump verbose electrical solver results"),1.0,"gtkswitch"))

	#pl_ss?.inp
	lib.append(my_data("pl_mode","au",_("Device state"),1.0,"QComboBoxLang",defaults=[[("voc"),_("Voc")],["Jsc",_("Jsc")]]))

	#ray
	lib.append(my_data("ray_wavelength_points","au",_("Wavelength points"),1.0,"QLineEdit"))
	lib.append(my_data("ray_auto_run","au",_("Run the ray tracer"),1.0,"QComboBoxLang",defaults=[[("ray_run_never"),_("Never")],["ray_run_once",_("Once per simulation")],["ray_run_step",_("Each simulation step")]]))

	lib.append(my_data("ray_theta_steps","au",_("Theta steps"),1.0,"QLineEdit"))
	lib.append(my_data("ray_theta_start","Degrees",_("Theta start"),1.0,"QLineEdit"))
	lib.append(my_data("ray_theta_stop","Degrees",_("Theta stop"),1.0,"QLineEdit"))

	lib.append(my_data("ray_phi_steps","au",_("Phi steps"),1.0,"QLineEdit"))
	lib.append(my_data("ray_phi_start","Degrees",_("Phi start"),1.0,"QLineEdit"))
	lib.append(my_data("ray_phi_stop","Degrees",_("Phi stop"),1.0,"QLineEdit"))

	lib.append(my_data("ray_escape_bins","au",_("Escape bins"),1.0,"QLineEdit"))

	lib.append(my_data("ray_auto_wavelength_range",_("True/False"),_("Automatic wavelength range"),1.0,"gtkswitch"))
	lib.append(my_data("ray_lambda_start","nm",_("Start wavelength"),1.0,"QLineEdit",hide_on_true_token="#ray_auto_wavelength_range"))
	lib.append(my_data("ray_lambda_stop","nm",_("Stop wavelength"),1.0,"QLineEdit",hide_on_true_token="#ray_auto_wavelength_range"))
	
	lib.append(my_data("ray_emission_source","au",_("Emit from"),1.0,"QComboBoxLang",defaults=[[("ray_emission_electrical_mesh"),_("Each electrical mesh point")],["ray_emission_single_point",_("Center of each layer")]]))
	lib.append(my_data("text_ray_tracing","au",_("<b>Ray tracing</b>"),1.0,"QLabel"))


	#viewpoint.inp
	lib.append(my_data("viewpoint_enabled",_("True/False"),_("Enable viewpoint"),1.0,"gtkswitch"))
	lib.append(my_data("viewpoint_size","au",_("View point size"),1.0,"QLineEdit"))
	lib.append(my_data("viewpoint_dz","au",_("View point dz"),1.0,"QLineEdit"))
	lib.append(my_data("viewpoint_nx","au",_("Mesh points x"),1.0,"QLineEdit"))
	lib.append(my_data("viewpoint_nz","au",_("Mesh points z"),1.0,"QLineEdit"))



	#led.inp
	lib.append(my_data("led_extract_eff","0.0-1.0",_("LED extraction efficiency"),1.0,"QLineEdit"))

	#device.inp
	#lib.append(my_data("#invert_applied_bias","au",_("Invert applied bias"),1.0,"gtkswitch"))
	#lib.append(my_data("#lcharge","m^{-3}",_("Charge on left contact"),1.0,"QLineEdit"))
	#lib.append(my_data("#rcharge","m^{-3}",_("Charge on right contact"),1.0,"QLineEdit"))

	#parasitic.inp
	lib.append(my_data("Rshunt","Ohms m^{2}",_("Shunt resistance"),1.0,"QLineEdit",min=1e-3,max=1e6))
	lib.append(my_data("Rcontact","Ohms",_("Series resistance"),1.0,"QLineEdit",min=1.0,max=200))
	lib.append(my_data("otherlayers","m",_("Other layers"),1.0,"QLineEdit"))
	lib.append(my_data("test_param","m",_("debug (ignore)"),1.0,"QLineEdit",hidden=True))

	#contacts.inp
	lib.append(my_data("np","m^{-3}",_("Contact charge density"),1.0,"QLineEdit"))

	#mesh?.inp
	lib.append(my_data("remesh_x","au",_("Automatic remesh x"),1.0,"gtkswitch"))
	lib.append(my_data("remesh_y","au",_("Automatic remesh y"),1.0,"gtkswitch"))
	lib.append(my_data("remesh_z","au",_("Automatic remesh z"),1.0,"gtkswitch"))

	#pl?.inp
	lib.append(my_data("pl_emission_enabled",_("True/False"),_("Emission enabled from this layer"),1.0,"gtkswitch"))
	lib.append(my_data("pl_fe_fh","0.0-1.0",_("n_{free} to p_{free} photon generation efficiency"),1.0,"QLineEdit",hide_on_token_eq=[["pl_use_experimental_emission_spectra",True],["pl_emission_enabled",False]]))
	lib.append(my_data("pl_fe_te","0.0-1.0",_("n_{free} to n_{trap} photon generation efficiency"),1.0,"QLineEdit",hide_on_token_eq=[["pl_use_experimental_emission_spectra",True],["pl_emission_enabled",False]]))
	lib.append(my_data("pl_te_fh","0.0-1.0",_("n_{trap} to p_{free} photon generation efficiency"),1.0,"QLineEdit",hide_on_token_eq=[["pl_use_experimental_emission_spectra",True],["pl_emission_enabled",False]]))
	lib.append(my_data("pl_th_fe","0.0-1.0",_("p_{trap} to n_{free} photon generation efficiency"),1.0,"QLineEdit",hide_on_token_eq=[["pl_use_experimental_emission_spectra",True],["pl_emission_enabled",False]]))
	lib.append(my_data("pl_fh_th","0.0-1.0",_("p_{free} to p_{trap} photon generation efficiency"),1.0,"QLineEdit",hide_on_token_eq=[["pl_use_experimental_emission_spectra",True],["pl_emission_enabled",False]]))
	lib.append(my_data("pl_input_spectrum",_("Edit"),_("Experimental emission spectra"),1.0,"gpvdm_select_emission" ,units_widget="QPushButton", hide_on_false_token=["pl_use_experimental_emission_spectra"]))
	lib.append(my_data("icon",_("Icon"),_("Edit"),1.0,"icon_widget" ,units_widget="QPushButton"))
	lib.append(my_data("pl_experimental_emission_efficiency","0.0-1.0",_("Experimental emission efficiency"),1.0,"QLineEdit", hide_on_false_token="pl_use_experimental_emission_spectra"))

#pl_experimental_emission_efficiency

	lib.append(my_data("pl_use_experimental_emission_spectra",_("True/False"),_("Use experimental emission spectra"),1.0,"gtkswitch"))

	#fxdomain?.inp
	lib.append(my_data("is_Vexternal","Volts",_("V_{external}"),1.0,"QLineEdit",hide_on_token_eq=[["load_type","open_circuit"]]))
	lib.append(my_data("fxdomain_Rload","Ohms",_("Load resistor"),1.0,"QLineEdit",hide_on_token_eq=[["load_type","open_circuit"]]))
	lib.append(my_data("fxdomain_points","au",_("fx domain mesh points"),1.0,"QLineEdit",hide_on_token_eq=[["fxdomain_large_signal","small_signal"]]))
	lib.append(my_data("fxdomain_n","au",_("Cycles to simulate"),1.0,"QLineEdit",hide_on_token_eq=[["fxdomain_large_signal","small_signal"]]))
	lib.append(my_data("fxdomain_voltage_modulation_max","V",_("Voltage modulation depth"),1.0,"QLineEdit",hide_on_token_eq=[["fx_modulation_type","optical"],["fxdomain_large_signal","small_signal"]]))

	lib.append(my_data("fx_modulation_type","au",_("Excite with"),1.0,"QComboBoxLang",defaults=[[("voltage"),_("Voltage")],[("optical"),_("Light")]]))
	lib.append(my_data("fxdomain_measure","au",_("Measure"),1.0,"QComboBoxLang",defaults=[[("measure_voltage"),_("Voltage")],[("measure_current"),_("Current")]]))
	lib.append(my_data("fxdomain_light_modulation_depth","au",_("Light modulation depth"),1.0,"QLineEdit",hide_on_token_eq=[["fx_modulation_type","voltage"]]))

	lib.append(my_data("fxdomain_do_fit","au",_("Run fit after simulation"),1.0,"gtkswitch",hide_on_token_eq=[["fxdomain_large_signal","small_signal"],["fxdomain_large_signal","fourier"]]))
	lib.append(my_data("periods_to_fit","au",_("Periods to fit"),1.0,"QLineEdit",hide_on_token_eq=[["fxdomain_large_signal","small_signal"],["fxdomain_large_signal","fourier"]]))

	lib.append(my_data("fxdomain_norm_tx_function","au",_("Normalize transfer functon by light intensity"),1.0,"gtkswitch",hide_on_token_eq=[["fx_modulation_type","voltage"]]))

	lib.append(my_data("fxdomain_r","",_("Re(i)"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_i","V",_("Im(i)"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_Jr","Am^{-2}",_("Re(J)"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_Ji","Am^{-2}",_("Im(J)"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_fx","Hz",_("fx"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_delta_i","s",_("di"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_delta_g","s",_("dmodulation"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_delta_phase","rads",_("dphase"),1.0,"QLineEdit"))
	lib.append(my_data("fxdomain_large_signal","au",_("Simulation type"),1.0,"QComboBoxLang",defaults=[[("large_signal"),_("Large signal")],[("fourier"),_("Fourier")]]))		#,[("small_signal"),_("Small signal")]


 
	#node_list.inp
	lib.append(my_data("#node_list","au",_("Node list"),1.0,"QChangeLog"))

	#crypto.inp
	lib.append(my_data("#iv","au",_("Initialization vector"),1.0,"QLineEdit"))
	lib.append(my_data("#key","au",_("Cryptographic key"),1.0,"QLineEdit"))

	#lumo?.inp
	lib.append(my_data("#function_\d+","au","Function",1.0,"QLineEdit"))
	lib.append(my_data("#function_enable_\d+","au","Enabled",1.0,"QLineEdit"))
	lib.append(my_data("#function_a_\d+","au","a",1.0,"QLineEdit"))
	lib.append(my_data("#function_b_\d+","au","b",1.0,"QLineEdit"))
	lib.append(my_data("#function_c_\d+","au","c",1.0,"QLineEdit"))

	#fit.inp
	lib.append(my_data("simplexmul","au","simplex mull",1.0,"QLineEdit"))
	lib.append(my_data("simplex_reset","au","Reset steps",1.0,"QLineEdit"))

	#
	lib.append(my_data("#Psun","Sun",_("Intensity of the sun"),1.0,"QLineEdit",hidden=True))

	lib.append(my_data("#saturation_n0","#saturation_n0",_("#saturation_n0"),1.0,"QLineEdit"))
	lib.append(my_data("#saturation_rate","#saturation_rate",_("#saturation_rate"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_saturate","#imps_saturate",_("#imps_saturate"),1.0,"QLineEdit"))


	lib.append(my_data("#simplephotondensity","m^{-2}s^{-1}",_("Photon density"),1.0,"QLineEdit"))
	lib.append(my_data("#simple_alpha","m^{-1}",_("Absorption of material"),1.0,"QLineEdit"))
	lib.append(my_data("#simmode","au",_("#simmode"),1.0,"QLineEdit"))

	lib.append(my_data("#function","au",_("#function"),1.0,"QLineEdit"))
	lib.append(my_data("#Vexternal","V",_("start voltage"),1.0,"QLineEdit"))
	lib.append(my_data("#Vmax","V",_("Max voltage"),1.0,"QLineEdit"))
	lib.append(my_data("#start_stop_time","s",_("Time of pause"),1.0,"QLineEdit"))
	lib.append(my_data("#stopstart","1/0",_("Pause between iterations"),1.0,"QComboBox",defaults=["1","0"]))
	lib.append(my_data("#invert_current",_("True/False"),_("Invert output"),1.0,"QLineEdit"))


	lib.append(my_data("#use_capacitor","1/0",_("Use capacitor"),1.0,"QComboBox",defaults=["1","0"]))


	#
	lib.append(my_data("#Rshort_imps","Ohms",_("R_{short}"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_sun","1=1 Sun",_("Backgroud light bias"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_modulation_max","1=1 Sun",_("Modulation depth"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_modulation_fx","Hz",_("Modulation frequency"),1.0,"QLineEdit"))
	lib.append(my_data("#high_sun_scale","au",_("High light multiplyer"),1.0,"QLineEdit"))



	lib.append(my_data("#imps_r","Amps",_("Re(i)"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_i","Amps",_("Im(i)"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_Jr","Amps $m^{-2}$",_("Re(J)"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_Ji","Amps $m^{-2}$",_("Im(J)"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_fx","Hz",_("Frequency"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_delta_i","s",_("Phase shift"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_delta_g","s",_("Phase shift"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_delta_phase","s",_("Phase shift"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_points","s",_("points"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_n","s",_("Wavelengths to simulate"),1.0,"QLineEdit"))
	lib.append(my_data("#imps_Vexternal","Volts",_("External voltage"),1.0,"QLineEdit"))

	lib.append(my_data("#Cext","C",_("External C"),1.0,"QLineEdit"))
	lib.append(my_data("#Rext","Ohms",_("External R"),1.0,"QLineEdit"))

	lib.append(my_data("#Rscope","Ohms",_("Resistance of scope"),1.0,"QLineEdit"))

	#suns_voc
	lib.append(my_data("sun_voc_single_point","True/False",_("Single point"),1.0,"gtkswitch"))
	lib.append(my_data("sun_voc_Psun_start","Suns",_("Start intensity"),1.0,"QLineEdit"))
	lib.append(my_data("sun_voc_Psun_stop","Suns",_("Stop intensity"),1.0,"QLineEdit"))
	lib.append(my_data("sun_voc_Psun_mul","au",_("step multiplier"),1.0,"QLineEdit"))

	#suns_jsc
	lib.append(my_data("sunstart","Suns",_("Start intensity"), 1.0,"QLineEdit"))
	lib.append(my_data("sunstop","Suns",_("Stop intensity"), 1.0,"QLineEdit"))
	lib.append(my_data("sundp","au",_("Step"), 1.0,"QLineEdit"))
	lib.append(my_data("sundpmul","au",_("step multiplier"), 1.0,"QLineEdit"))

	lib.append(my_data("#simplephotondensity","m^{-2}s^{-1}",_("Photon Flux"),1.0,"QLineEdit"))
	lib.append(my_data("#simple_alpha","m^{-1}",_("Absorption"),1.0,"QLineEdit"))
	lib.append(my_data("#xlen","m",_("device width"),1.0,"QLineEdit"))
	lib.append(my_data("#zlen","m",_("device breadth"),1.0,"QLineEdit"))

	lib.append(my_data("#voc_J_to_Jr","au","Ratio of conduction current to recombination current",1.0,"QLineEdit"))

	lib.append(my_data("#voc_i","au",_("Current"),1.0,"QLineEdit"))



	lib.append(my_data("#max_nfree_to_ptrap","m^{-3}s^{-1}","nfree_to_ptrap",1.0,"QLineEdit"))
	lib.append(my_data("#max_pfree_to_ntrap","m^{-3}s^{-1}","max_pfree_to_ntrap",1.0,"QLineEdit"))
	lib.append(my_data("#max_nrelax","m^{-3}s^{-1}","max_nrelax",1.0,"QLineEdit"))
	lib.append(my_data("#max_prelax","m^{-3}s^{-1}","max_prelax",1.0,"QLineEdit"))

	lib.append(my_data("#max_nfree","m^{-3}","max_nfree",1.0,"QLineEdit"))
	lib.append(my_data("#max_pfree","m^{-3}","max_pfree",1.0,"QLineEdit"))
	lib.append(my_data("#max_ntrap","m^{-3}","max_ntrap",1.0,"QLineEdit"))
	lib.append(my_data("#max_ptrap","m^{-3}","max_ptrap",1.0,"QLineEdit"))
	lib.append(my_data("#alpha_max_reduction","m^{-1}","alpha_max_reduction",1.0,"QLineEdit"))
	lib.append(my_data("#alpha_max_increase","m^{-1}","alpha_max_increase",1.0,"QLineEdit"))

	lib.append(my_data("#srh_n_r1","m^{-3}s^{-1}","srh electron rate 1",1.0,"QLineEdit"))
	lib.append(my_data("#srh_n_r2","m^{-3}s^{-1}","srh electron rate 2",1.0,"QLineEdit"))
	lib.append(my_data("#srh_n_r3","m^{-3}s^{-1}","srh electron rate 3",1.0,"QLineEdit"))
	lib.append(my_data("#srh_n_r4","m^{-3}s^{-1}","srh electron rate 4",1.0,"QLineEdit"))

	lib.append(my_data("#srh_p_r1","m^{-3}s^{-1}","srh hole rate 1",1.0,"QLineEdit"))
	lib.append(my_data("#srh_p_r2","m^{-3}s^{-1}","srh hole rate 2",1.0,"QLineEdit"))
	lib.append(my_data("#srh_p_r3","m^{-3}s^{-1}","srh hole rate 3",1.0,"QLineEdit"))
	lib.append(my_data("#srh_p_r4","m^{-3}s^{-1}","srh hole rate 4",1.0,"QLineEdit"))

	lib.append(my_data("#band_bend_max","percent","band bend max",1.0,"QLineEdit"))

	#config.inp
	lib.append(my_data("#gui_config_3d_enabled",_("True/False"),_("Enable 3d effects"),1.0,"gtkswitch"))
	lib.append(my_data("#gui_use_icon_theme",_("True/False"),_("Use icons from OS"),1.0,"gtkswitch"))
	lib.append(my_data("#matlab_interpreter",_("au"),_("Matlab interpreter"),1.0,"QLineEdit"))

	#ce
	lib.append(my_data("ce_start_sun",_("au"),_("Start light intensity"),1.0,"QLineEdit"))
	lib.append(my_data("ce_stop_sun",_("au"),_("Stop light intensity"),1.0,"QLineEdit"))
	lib.append(my_data("ce_number_of_simulations",_("au"),_("Numer of steps"),1.0,"QLineEdit"))
	lib.append(my_data("ce_on_time",_("au"),_("CE light on time"),1.0,"QLineEdit"))
	lib.append(my_data("ce_off_time",_("au"),_("CE light off time"),1.0,"QLineEdit"))

	#fits.data_sets[].config
	lib.append(my_data("time_shift","s","Exp data x shift",1.0,"QLineEdit"))
	lib.append(my_data("fit_shift_y","s","Exp data y shift",1.0,"QLineEdit"))
	lib.append(my_data("start","s","start",1.0,"QLineEdit"))
	lib.append(my_data("stop","s","stop",1.0,"QLineEdit"))
	lib.append(my_data("log_x",_("True/False"),_("log x"),1.0,"gtkswitch"))
	lib.append(my_data("log_y",_("True/False"),_("log y"),1.0,"gtkswitch"))
	lib.append(my_data("log_y_keep_sign",_("True/False"),_("Keep sign for log y "),1.0,"gtkswitch"))
	lib.append(my_data("sim_data",_("filename"),"Fit file name",1.0,"QLineEdit"))
	lib.append(my_data("fit_invert_simulation_y",_("True/False"),_("Invert simulated data (y)"),1.0,"gtkswitch"))
	lib.append(my_data("fit_subtract_lowest_point",_("True/False"),_("Subtract lowest point"),1.0,"gtkswitch"))
	lib.append(my_data("fit_set_first_point_to_zero",_("True/False"),_("Set first point to zero="),1.0,"gtkswitch"))
	lib.append(my_data("fit_against", "au", _("Fit against simulation"),1.0,"QComboBoxLang"))


	#world
	lib.append(my_data("world_margin_x0","decimal",_("margin x0"),1.0,"QLineEdit"))
	lib.append(my_data("world_margin_x1","decimal",_("margin x1"),1.0,"QLineEdit"))
	lib.append(my_data("world_margin_y0","decimal",_("margin y0"),1.0,"QLineEdit"))
	lib.append(my_data("world_margin_y1","decimal",_("margin y1"),1.0,"QLineEdit"))
	lib.append(my_data("world_margin_z0","decimal",_("margin z0"),1.0,"QLineEdit"))
	lib.append(my_data("world_margin_z1","decimal",_("margin z1"),1.0,"QLineEdit"))

	#
	lib.append(my_data("#layer0","m",_("Active layer width"),1.0,"QLineEdit"))
	lib.append(my_data("#stark_saturate","au","Stark saturate",1.0,"QLineEdit"))

	lib.append(my_data("#n_mul","au","n mul",1.0,"QLineEdit"))
	lib.append(my_data("#alpha_mul","m^{-1}","Alpha mul",1.0,"QLineEdit"))


	#time_mesh_config*.inp
	lib.append(my_data("#fs_laser_time","s","Laser start time",1.0,"QLineEdit"))


	#fdtd.inp
	lib.append(my_data("use_gpu","au",_("OpenCL GPU acceleration"),1.0,"gtkswitch"))
	lib.append(my_data("fdtd_lambda_start","m",_("Start wavelength"),1.0,"QLineEdit"))
	lib.append(my_data("fdtd_lambda_stop","m",_("Stop wavelength"),1.0,"QLineEdit"))
	lib.append(my_data("fdtd_lambda_points","m",_("Wavelength steps"),1.0,"QLineEdit"))

	#any files
	lib.append(my_data("dump_verbosity","au",_("Output verbosity to disk"),1.0,"QComboBoxLang",defaults=[["-1",_("Nothing")],["0",_("Key results")],[("1"),_("Write everything to disk")],[("2"),_("Write everything to disk every 2nd step")],[("5"),_("Write everything to disk every 5th step")],[("10"),_("Write everything to disk every 10th step")]]))
	lib.append(my_data("dump_screen_verbosity", "au", _("Output verbosity to screen"),1.0,"QComboBoxLang",defaults=[[("dump_verbosity_everything"),_("Show lots")],["dump_verbosity_key_results",_("Show key results")]]))

	lib.append(my_data("solver_verbosity", "au", _("Solver output verbosity"),1.0,"QComboBoxLang",defaults=[[("solver_verbosity_nothing"),_("Nothing")],["solver_verbosity_at_end",_("As answers are found")],["solver_verbosity_every_step",_("Each step")]]))

	#circuit diagram
	lib.append(my_data("com_R","Ohms",_("Resistor"),1.0,"QLineEdit"))
	lib.append(my_data("com_C","F",_("Capacitor"),1.0,"QLineEdit"))
	lib.append(my_data("com_L","H",_("Inductance"),1.0,"QLineEdit"))
	lib.append(my_data("com_I0","Apms",_("I0"),1.0,"QLineEdit"))
	lib.append(my_data("com_nid","(a.u.)",_("Ideality factor"),1.0,"QLineEdit"))
	lib.append(my_data("com_layer","(a.u.)",_("Layer"),1.0,"QComboBoxLayers"))

	#gl
	lib.append(my_data("xRot","Deg",_("x rotation"),1.0,"QLineEdit"))
	lib.append(my_data("yRot","Deg",_("y rotation"),1.0,"QLineEdit"))
	lib.append(my_data("zRot","Deg",_("z rotation"),1.0,"QLineEdit"))
	lib.append(my_data("x_pos","au",_("x pos"),1.0,"QLineEdit"))
	lib.append(my_data("y_pos","au",_("y pos"),1.0,"QLineEdit"))
	lib.append(my_data("zoom","au",_("zoom"),1.0,"QLineEdit"))

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
			#print(lib[i].token+"$",search_token)
			if bool(re.match(lib[i].token+"$",search_token))==True:
				if lib[i].units=="" and lib[i].info=="":
					return False
				else:
					return lib[i]

		return False

	def find_json(self,token):
		global lib

		search_token=token.strip()

		for line in lib:
			#if line.file_name=="json.inp":
			split_token=line.token.split('.')
			#print(split_token,search_token)
			if split_token[-1]==search_token:
				return line

		return False

	def reverse_lookup(self,english):
		global lib

		for line in lib:
			if english==line.info:
				#print(english,line.info)
				return line

		return False

	def dump_lib(self):
		global lib
		for i in range(0, len(lib)):
			print(">",lib[i].token,"<>",lib[i].info,"<")
			
	def get_lib(self):
		global lib
		return lib
