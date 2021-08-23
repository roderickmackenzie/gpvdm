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

## @package gl_photons
#  Shows photons on the device
#

import sys

from OpenGL.GL import *
from OpenGL.GLU import *

import random
import numpy as np
from math import pi,acos,sin,cos

from gl_scale import scale_get_xmul
from gl_scale import scale_get_ymul
from gl_scale import scale_get_zmul

from gl_base_object import gl_base_object
from triangle import vec

from gl_scale import scale_get_device_y
from gl_scale import scale_get_device_x
from gl_scale import scale_get_device_z
from gpvdm_json import gpvdm_data

class gl_photons():

	def draw_photon(self,x,y,z,up,r,g,b):

		length=0.9
		glColor4f(r, g, b, 1.0)

		glLineWidth(3)
		wx=np.arange(0, length , 0.025)
		wy=np.sin(wx*3.14159*8)*0.2
		
		start_y=y+length
		stop_y=y

		glBegin(GL_LINES)
		for i in range(1,len(wx)):
			glVertex3f(x, start_y-wx[i-1], z+wy[i-1])
			glVertex3f(x, start_y-wx[i], z+wy[i])

		glEnd()

		if up==False:
			glBegin(GL_TRIANGLES)

			glVertex3f(x-0.1, stop_y,z)
			glVertex3f(x+0.1, stop_y ,z)
			glVertex3f(x,stop_y-0.1 ,z)

			glEnd()
		else:
			glBegin(GL_TRIANGLES)

			glVertex3f(x-0.1, start_y,z)
			glVertex3f(x+0.1, start_y ,z)
			glVertex3f(x,start_y+0.1 ,z)

			glEnd()

	def draw_photon_sheet(self,source,x0,z0):
		up_photons=False
		device_top=scale_get_device_y()
		dx=scale_get_device_x()

		if source.light_illuminate_from=="y0":
			y=-1.0
		elif source.light_illuminate_from=="y1":
			y=device_top+0.5
			up_photons=True
		else:
			return

		suns=self.suns#*source.virtual_spectra.light_spectra.segments[0].light_multiplyer
		if suns!=0:
			if suns<=0.01:
				den=dx/5
			elif suns<=0.1:
				den=dx/8
			elif suns<=1.0:
				den=dx/10
			elif suns<=10.0:
				den=dx/20
			else:
				den=dx/25
			x=np.arange(x0+den/2.0, x0+gpvdm_data().mesh.mesh_x.get_len()*scale_get_xmul() , den)
			z=np.arange(z0+den/2.0, z0+gpvdm_data().mesh.mesh_z.get_len()*scale_get_zmul() , den)

			for i in range(0,len(x)):
				for ii in range(0,len(z)):
					self.draw_photon(x[i],y,z[ii],not up_photons,0.0,1.0,0.0)

	def draw_photons(self,x0,z0):
		if self.false_color==True:
			return
		done=[]
		for source in gpvdm_data().light_sources.lights.segments:
			if source.light_illuminate_from not in done:
				self.draw_photon_sheet(source,x0,z0)
				done.append(source.light_illuminate_from)

		if self.emission==True and self.ray_model==False:
			den=1.2
			x=np.arange(x0, x0+scale_get_device_x() , den)
			z=np.arange(z0, z0+scale_get_device_z() , den)

			for i in range(0,len(x)):
				for ii in range(0,len(z)):
					self.draw_photon(x[i]+0.1,y+0.1,z[ii],True,0.0, 0.0, 1.0)

