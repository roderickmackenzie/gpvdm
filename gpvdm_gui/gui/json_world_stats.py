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


## @package json_world_stats
#  Used to calculate the size of the world
#

from triangle import vec

class json_world_stats():

	def get_world_size(self):
		my_min=vec()
		my_min.x=1e6
		my_min.y=1e6
		my_min.z=1e6

		my_max=vec()
		my_max.x=-1e6
		my_max.y=-1e6
		my_max.z=-1e6
		for l in self.epi.layers:
			my_min,my_max=l.get_min_max(my_min,my_max)

		for l in self.world.world_data.segments:
			my_min,my_max=l.get_min_max(my_min,my_max)
			
		return my_min,my_max


