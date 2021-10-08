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

## @package gl_color
#  Handle colors for the OpenGL display, can also do false color for color picking.
#

import sys
from OpenGL.GL import *

class color():
	def __init__(self,obj):
		self.r=obj.r
		self.g=obj.g
		self.b=obj.b
		self.alpha=obj.alpha
		self.obj=obj

	def __str__(self):
		return "obj.name="+self.obj.name+" r="+str(self.r)+" g="+str(self.g)+" b="+str(self.b)

	def __eq__(self, other):
		if other.r==self.r and other.g==self.g and other.b==self.b:
			return True
		return False



class gl_color:

	def __init__(self):
		self.false_color=False

	def set_false_color(self,value):
		self.false_color=value
		if self.false_color==True:
			glDisable(GL_LIGHTING)
		else:
			glEnable(GL_LIGHTING)

	def cal_color(self,obj,r_override=None,g_override=None,b_override=None):
		r=obj.r
		g=obj.g
		b=obj.b
		alpha=obj.alpha

		lit=True

		if r==None or g==None or b==None:
			r=255
			g=0
			b=0
			alpha=0.7

		if type(r)==int:
			r=r/255
			g=g/255
			b=b/255
			alpha=alpha

		if self.false_color==True:
			return obj.r_false,obj.g_false,obj.b_false
		else:
			if r_override!=None:
				r=r_override

			if g_override!=None:
				g=g_override

			if b_override!=None:
				b=b_override

			if alpha==-1:
				 return r, g, b, 1.0
			else:
				return r, g, b, alpha

	def set_color(self,obj,r_override=None,g_override=None,b_override=None):

		if self.false_color==True:
			r,g,b= self.cal_color(obj,r_override=r_override,g_override=g_override,b_override=b_override)
			glColor3f(r,g,b)
		else:
			r,g,b,alpha= self.cal_color(obj,r_override=r_override,g_override=g_override,b_override=b_override)
			glColor4f(r,g,b,alpha)


