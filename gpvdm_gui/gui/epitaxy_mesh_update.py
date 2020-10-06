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

## @package epitaxy_mesh_update
#  If there is only one y layer in the mesh, and you update it attempt to remesh it.
#

import os

#epitaxy
from epitaxy import epitaxy_get_dy
from epitaxy import epitaxy_get_dos_file
from epitaxy import epitaxy_get_layers


from mesh import get_mesh


def epitaxy_mesh_update():
	mesh=get_mesh().y
	if len(mesh.layers)==1:
		tot=0.0
		for i in range(0,epitaxy_get_layers()):
			width=float(epitaxy_get_dy(i))
			dos_layer=epitaxy_get_dos_file(i)
			print(dos_layer,width)
			if dos_layer.startswith("dos")==True:
				tot=tot+width

		mesh.layers[0].thick=tot
		mesh.save()



