# -*- coding: utf-8 -*-
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

## @package spctral2
#  After R. Bird and C. Riordan 1984
#

import os
import sys

from math import *

from dat_file import dat_file
from cal_path import get_atmosphere_path
from zenith import zenith

class spctral2():
	def __init__(self):

		self.day=80		#winter equinox
		self.lat=51		#london
		self.hour=12
		self.min=0

		self.P=1.0		#Preasure 1bar
		self.aod=0.27	#AOD
		self.W= 1.42	#precip water
		self.No2_un=0.0

		file_name = os.path.join(get_atmosphere_path(), "spctral2", "etr.inp")
		#print(file_name)
		self.etr=dat_file()
		self.etr.y_mul=1e9
		self.etr.y_units="nm"
		self.etr.data_mul=1.0
		self.etr.data_units="W m^{-3} m^{-1}"

		self.etr.load(file_name)

		file_name = os.path.join(get_atmosphere_path(), "spctral2", "h2o.inp")
		self.aw=dat_file()
		self.aw.load(file_name)

		file_name = os.path.join(get_atmosphere_path(), "spctral2", "o3.inp")
		self.ao=dat_file()
		self.ao.load(file_name)

		file_name = os.path.join(get_atmosphere_path(), "spctral2", "uni_abs.inp")
		self.au=dat_file()
		self.au.load(file_name)

		file_name = os.path.join(get_atmosphere_path(), "spctral2", "no2.inp")
		self.no2_data=dat_file()
		self.no2_data.load(file_name)

	def calc(self,json_data):
		self.lat=json_data.spctral2_lat

		self.day=json_data.spctral2_day		#80 winter equinox
		self.hour=json_data.spctral2_hour
		self.min=json_data.spctral2_minute

		self.P=json_data.spctral2_preasure
		self.aod=json_data.spctral2_aod
		self.W=json_data.spctral2_water	#precip water
		self.No2_un=json_data.spctral2_no2

		self.debug=True
		self.cal_earth_sun_distance()
		
		#zenith
		self.Z_rad=zenith(self.lat, self.day, self.hour, self.min)
		self.Z_deg=self.Z_rad*360/2/pi
		print("Zenith",self.Z_deg)

		if self.Z_deg>90.0:
			self.Iglobal=dat_file()
			self.Iglobal.copy(self.etr)
			self.Iglobal.file_name="Iglobal.dat"
			self.Iglobal.key_text="Global"

			self.Is=dat_file()
			self.Is.copy(self.etr)
			self.Is.file_name="Idiffuse.dat"
			self.Is.key_text="Diffuse"

			self.Id=dat_file()
			self.Id.copy(self.etr)
			self.Id.file_name="Idirect.dat"
			self.Id.key_text="Direct"
			return

		#self.D=1.0
		#se0lf.Z_deg=48.2
		#self.Z_rad=(self.Z_deg/360.0)*2.0*pi
		#Direct

		self.Tr=dat_file()
		self.Tr.copy(self.etr)
		self.cal_rayleigh()
		if self.debug==True:
			self.Tr.save("Tr.dat")

		self.Ta=dat_file()
		self.Ta.copy(self.etr)
		self.tau_a=dat_file()
		self.tau_a.copy(self.etr)
		self.cal_arosol()
		if self.debug==True:
			self.Ta.save("Ta.dat")

		self.Tw=dat_file()
		self.Tw.copy(self.etr)
		self.cal_water()

		self.To=dat_file()
		self.To.copy(self.etr)
		self.cal_ozone()
		if self.debug==True:
			self.To.save("To.dat")

		self.Tu=dat_file()
		self.Tu.copy(self.etr)
		self.cal_mixed_gas()
		if self.debug==True:
			self.Tu.save("Tu.dat")

		self.Tno2=dat_file()
		self.Tno2.copy(self.etr)
		self.cal_Tno2()
		if self.debug==True:
			self.Tno2.save("Tno.dat")

		self.Id=self.etr*self.D*self.Tr*self.Ta*self.Tw*self.To*self.Tu*self.Tno2
		self.Id.file_name="Idirect.dat"
		self.Id.key_text="Direct"


		#Diffuse
		self.omega=dat_file()
		self.omega.copy(self.etr)
		self.cal_omega()
		if self.debug==True:
			self.omega.save("omega.dat")

		self.Taa=dat_file()
		self.Taa.copy(self.etr)
		self.cal_Taa()
		if self.debug==True:
			self.Taa.save("Taa.dat")

		self.Cs=dat_file()
		self.Cs.copy(self.etr)
		self.cal_Cs()
		if self.debug==True:
			self.Cs.save("Cs.dat")

		self.Ir=self.etr*self.D*cos(self.Z_rad)*self.To*self.Tu*self.Tw*self.Taa*(1.0-self.Tr.pow(0.95))*self.Cs
		if self.debug==True:
			self.Ta.save("Ir.dat")

		self.Tas=dat_file()
		self.Tas.copy(self.etr)
		self.cal_Tas()
		if self.debug==True:
			self.Tas.save("Tas.dat")

		self.Fs=dat_file()
		self.Fs.copy(self.etr)
		self.cal_Fs()

		self.Ia=self.etr*self.D*cos(self.Z_rad)*self.To*self.Tu*self.Tw*self.Taa*self.Tr.pow(1.5)*(1.0-self.Tas)*self.Fs*self.Cs
		#Ig

		self.TuP=dat_file()
		self.TuP.copy(self.etr)
		self.cal_mixed_gas_P()
		if self.debug==True:
			self.TuP.save("TuP.dat")

		self.TwP=dat_file()
		self.TwP.copy(self.etr)
		self.cal_water_P()
		if self.debug==True:
			self.TwP.save("TwP.dat")

		self.TaaP=dat_file()
		self.TaaP.copy(self.etr)
		self.cal_TaaP()
		if self.debug==True:
			self.TaaP.save("Taap.dat")

		self.TrP=dat_file()
		self.TrP.copy(self.etr)
		self.cal_rayleigh_P()
		if self.debug==True:
			self.TrP.save("TrP.dat")

		self.TasP=dat_file()
		self.TasP.copy(self.etr)
		self.cal_TasP()
		if self.debug==True:
			self.TasP.save("TasP.dat")

		FsP=1.0- 0.5*exp((self.AFS + self.BFS/1.8)/ 1.8 )
		self.r_s=self.TuP*self.TwP*self.TaaP*(0.5*(1.0-self.TrP)+(1-FsP)*self.TrP*(1.0-self.TasP))
		if self.debug==True:
			self.r_s.save("r_s.dat")

		r_g=0.2						#Mohamed
		self.Ig=(self.Id*cos(self.Z_rad)+self.Ir+self.Ia)*self.r_s*r_g*self.Cs/(1.0-self.r_s*r_g)
		if self.debug==True:
			self.Ig.save("Ig.dat")


		self.Is=self.Ir+self.Ia+self.Ig
		self.Is.file_name="Idiffuse.dat"
		self.Is.key_text="Diffuse"

		self.Ir.save("Ir.dat")
		self.Ia.save("Ia.dat")
		self.Ig.save("Ig.dat")

		self.Is.save("Is.dat")
		self.Id.save("Id.dat")

		self.Iglobal=self.Is+self.Id
		self.Iglobal.file_name="Iglobal.dat"
		self.Iglobal.key_text="Global"
		self.Iglobal.save("Iglobal.dat")


	def cal_earth_sun_distance(self):
		# Earth-Sun Correction factor
		psi = 2 * pi * (self.day - 1) / 365		#eq 2-3
		self.D = 1.00011 + 0.034221 * cos(psi) + 0.00128 * sin(psi) + 0.00719 * cos(2 * psi) + 0.000077 * sin(2 * psi)		#eq 2-2

	def cal_rayleigh(self):
		P0=1.013								#In bar not mb
		# relative air mass (2-5)
		self.M = pow(cos(self.Z_rad) + 0.15 * pow(93.885 - self.Z_deg, -1.253), -1.0)

		# pressure corrected air mass
		self.M_dash = self.M * self.P / P0

		for y in range(0,self.Tr.y_len):
			lam=self.Tr.y_scale[y]*1e6		#in um
			self.Tr.data[0][0][y] = exp((-self.M_dash) / ((pow(lam, 4.0)) * (115.6406 - (1.3366 / pow(lam,2.0)))))	#(2-4)

	def cal_rayleigh_P(self):
		for y in range(0,self.Tr.y_len):
			lam=self.TrP.y_scale[y]*1e6		#in um
			self.TrP.data[0][0][y] = exp((-1.8) / ((pow(lam, 4.0)) * (115.6406 - (1.3366 / pow(lam,2.0)))))	#(2-4)


	def cal_arosol(self):
		#So this ecpression comes from the origonal source code, and by reading the test in combinaion with the wiki page on the Angstrom exponent
		alpha=1.140
		for y in range(0,self.Ta.y_len):
			lam=self.Ta.y_scale[y]*1e6		#in um
			self.tau_a.data[0][0][y]=self.aod *pow((lam / 0.5), -alpha)
			self.Ta.data[0][0][y] = exp(- self.M * self.tau_a.data[0][0][y])


	def cal_water(self):
		for y in range(0,self.Tw.y_len):
			lam=self.Tw.y_scale[y]*1e6		#in um
			aw=self.aw.data[0][0][y]
			self.Tw.data[0][0][y] = exp((-0.2385 * aw * self.W * self.M) / pow(1 + 20.07 * aw * self.W * self.M,  0.45) )

	def cal_water_P(self):
		for y in range(0,self.TwP.y_len):
			lam=self.TwP.y_scale[y]*1e6		#in um
			aw=self.aw.data[0][0][y]
			self.TwP.data[0][0][y] = exp((-0.2385 * aw * self.W * 1.8) / pow(1 + 20.07 * aw * self.W * 1.8,  0.45) )

	def cal_ozone(self):
		max_ozone_height=22.0
		Mo = (1 + max_ozone_height / 6370) / pow((pow(cos(self.Z_rad), 2.0) + 2 * max_ozone_height / 6370), 0.5)  # effective ozone mass
		for y in range(0,self.To.y_len):
			lam=self.To.y_scale[y]*1e6		#in um
			ao=self.ao.data[0][0][y]
			self.To.data[0][0][y] = exp(-ao * 0.395 * Mo)
		#print(str(self.To))

	def cal_mixed_gas(self):
		for y in range(0,self.Tu.y_len):
			lam=self.Tu.y_scale[y]*1e6		#in um
			au=self.au.data[0][0][y]
			self.Tu.data[0][0][y] = exp((-1.41 * au * self.M_dash) / pow((1.0 + 118.93 * au * self.M_dash),  0.45))

	def cal_mixed_gas_P(self):
		for y in range(0,self.TuP.y_len):
			lam=self.TuP.y_scale[y]*1e6		#in um
			au=self.au.data[0][0][y]
			self.TuP.data[0][0][y] = exp((-1.41 * au * 1.8) / pow((1.0 + 118.93 * au * 1.8),  0.45))

	def cal_omega(self):
		omega_04=0.945
		omega_dash=0.095
		for y in range(0,self.omega.y_len):
			lam=self.omega.y_scale[y]*1e6		#in um
			self.omega.data[0][0][y] = omega_04*exp(-omega_dash*pow(log(lam/0.4),2.0))

	def cal_Taa(self):
		for y in range(0,self.Taa.y_len):
			lam=self.Taa.y_scale[y]*1e6		#in um
			omega=self.omega.data[0][0][y]
			tau_a=self.tau_a.data[0][0][y]

			self.Taa.data[0][0][y] = exp(-(1-omega)*tau_a*self.M)

	def cal_TaaP(self):
		for y in range(0,self.TaaP.y_len):
			lam=self.TaaP.y_scale[y]*1e6		#in um
			omega=self.omega.data[0][0][y]
			tau_a=self.tau_a.data[0][0][y]

			self.TaaP.data[0][0][y] = exp(-(1-omega)*tau_a*1.8)


	def cal_Cs(self):
		for y in range(0,self.Cs.y_len):
			lam=self.Cs.y_scale[y]*1e6		#in um
			if lam<0.45:			
				self.Cs.data[0][0][y] = pow(lam+0.55,1.8)
			else:
				self.Cs.data[0][0][y] = 1.0				

	def cal_Tas(self):
		for y in range(0,self.Tas.y_len):
			lam=self.Tas.y_scale[y]*1e6		#in um
			omega=self.omega.data[0][0][y]
			tau_a=self.tau_a.data[0][0][y]

			self.Tas.data[0][0][y] = exp(-omega*tau_a*self.M)

	def cal_TasP(self):
		for y in range(0,self.TasP.y_len):
			lam=self.TasP.y_scale[y]*1e6		#in um
			omega=self.omega.data[0][0][y]
			tau_a=self.tau_a.data[0][0][y]

			self.TasP.data[0][0][y] = exp(-omega*tau_a*1.8)


	def cal_Fs(self):
		self.ALG=log(1.0-0.65)
		self.AFS=self.ALG*(1.459+self.ALG*(0.1595+self.ALG*0.4129))
		self.BFS=self.ALG*(0.0783+self.ALG*(-0.3824-self.ALG*0.5874))
		self.Fs=1.0-0.5*exp((self.AFS+self.BFS*cos(self.Z_rad))*cos(self.Z_rad))

	def cal_Tno2(self):
		m_n=pow(cos(self.Z_rad)+6.0230e2*pow(self.Z_deg,0.5)*pow(117.960-self.Z_deg,-3.4536),-1.0)
		for y in range(0,self.Tno2.y_len):
			lam=self.Tno2.y_scale[y]*1e6		#in um
			An=self.no2_data.data[0][0][y]
			self.Tno2.data[0][0][y] = exp(-m_n*self.No2_un*An )


