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


## @package json_cluster
#  Store the cluster information
#


import sys
import os
import shutil
import json
from json_base import json_base


class json_cluster_config(json_base):

	def __init__(self):
		json_base.__init__(self,"config")
		self.var_list=[]
		self.var_list.append(["cluster_iv","none"])
		self.var_list.append(["cluster_key","none"])
		self.var_list.append(["cluster_user_name","none"])
		self.var_list.append(["cluster_ip","127.0.0.1"])
		self.var_list.append(["cluster_cluster_dir","none"])
		self.var_list.append(["cluster_master_ip","127.0.0.1"])
		self.var_list.append(["cluster_node_list",""])
		self.var_list_build()



class json_cluster_node(json_base):

	def __init__(self):
		json_base.__init__(self,"cluster_node")
		self.var_list=[]
		self.var_list.append(["english_name","Cluster"])
		self.var_list.append(["icon","server"])
		self.var_list.append(["enabled",True])
		self.var_list.append(["config",json_cluster_config()])
		self.var_list.append(["id",self.random_id()])
		self.var_list_build()


class json_cluster(json_base):

	def __init__(self):
		json_base.__init__(self,"cluster",segment_class=True)
		#self.segments.append(json_cluster_node())

	def load_from_json(self,json):
		self.segments=[]
		segs=json['segments']
		for i in range(0,segs):
			a=json_cluster_node()
			simulation_name="segment"+str(i)
			a.load_from_json(json[simulation_name])
			self.segments.append(a)

