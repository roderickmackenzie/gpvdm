#!/usr/bin/env python3
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

## @package zenith
#  Calculate the zenith angle
#

from math import *

def to_rad(deg):
	return deg*2.0*pi/360

def zenith(lat, day, hour, minute):
	dec=-23.44*cos(to_rad((360/365)*(day+10)))		#https://en.wikipedia.org/wiki/Position_of_the_Sun#Declination_of_the_Sun_as_seen_from_Earth
	dec_rad=to_rad(dec)

	hour_of_day=(hour+minute/60)
	hour_ang=(hour_of_day-12)*15.0		#each hour is 15 degrees
	#print(hour,minute)
	#print(hour_ang)

	rhs=sin(to_rad(lat))*sin(dec_rad)+cos(to_rad(lat))*cos(dec_rad)*cos(to_rad(hour_ang))
	zenith=acos(rhs)		#*360/2/pi
	print(zenith*360/2/pi)
	return zenith
	

