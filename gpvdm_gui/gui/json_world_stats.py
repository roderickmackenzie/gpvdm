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


## @package json_world_stats
#  Used to calculate the size of the world
#

from triangle import vec

class json_world_stats():

	def get_world_size(self):
		my_min=vec()
		my_max=vec()

		if self.world.config.world_automatic_size==False:
			my_min.x=self.world.config.world_x0
			my_min.y=self.world.config.world_y0
			my_min.z=self.world.config.world_z0

			my_max.x=self.world.config.world_x1
			my_max.y=self.world.config.world_y1
			my_max.z=self.world.config.world_z1
		else:

			my_min.x=1e6
			my_min.y=1e6
			my_min.z=1e6

			my_max.x=-1e6
			my_max.y=-1e6
			my_max.z=-1e6
			for l in self.epi.layers:
				my_min,my_max=l.get_min_max(my_min,my_max)

			for l in self.world.world_data.segments:
				my_min,my_max=l.get_min_max(my_min,my_max)

			for source in self.light_sources.lights.segments:
				if source.light_illuminate_from=="xyz":
					my_min,my_max=l.get_min_max(my_min,my_max)

			dx0=(self.world.config.world_margin_x0-1.0)*(my_max.x-my_min.x)
			dx1=(self.world.config.world_margin_x1-1.0)*(my_max.x-my_min.x)

			dy0=(self.world.config.world_margin_y0-1.0)*(my_max.y-my_min.y)
			dy1=(self.world.config.world_margin_y1-1.0)*(my_max.y-my_min.y)

			dz0=(self.world.config.world_margin_z0-1.0)*(my_max.z-my_min.z)
			dz1=(self.world.config.world_margin_z1-1.0)*(my_max.z-my_min.z)

			my_min.x=my_min.x-dx0
			my_min.y=my_min.y-dy0
			my_min.z=my_min.z-dz0

			my_max.x=my_max.x+dx1
			my_max.y=my_max.y+dy1
			my_max.z=my_max.z+dz1

		return my_min,my_max


