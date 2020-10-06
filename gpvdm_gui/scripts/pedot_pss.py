#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys

class gpvdm_plugin:

	def __init__(self,api):
		self.api=api
		self.scan_name="scan_layer_thick"
		self.plot()
		return
		api.mkdir(self.scan_name)
		sims=["10e-9","50e-9","100e-9","200e-9","250e-9","300e-9", "350e-9","400e-9", "425e-9","450e-9"]
		for h in sims:
			sim_path=self.scan_name+"/"+h
			api.mkdir(sim_path)
			api.clone(sim_path)
			print(h)
			api.edit(sim_path+"/light.inp","#Psun","0.0")
			api.edit(sim_path+"/shape2.inp","#shape_dy",h)
			api.add_job(path=sim_path)			#Add the job to the job list
		api.run(callback=self.finished)			#Run the jobs on as many CPUs as you have, tell the server what to do when finished

	def finished(self):
		print("Finished!")
		#self.api.build_multiplot("scan")			#Build the plot files
		#self.api.graph_from_tokens("output_plot.dat","scan","dos0.inp","#mueffe","sim_info.dat","#voc")

	def plot(self):
		self.api.graph_from_tokens("Rp_pmax.dat",self.scan_name,"object_stats.dat","#Rp6","sim_info.dat","#Pmax")
		self.api.graph_from_tokens("Rp_jsc.dat",self.scan_name,"object_stats.dat","#Rp6","sim_info.dat","#jsc")

		self.api.graph_from_tokens("Rq_pmax.dat",self.scan_name,"object_stats.dat","#Rq6","sim_info.dat","#Pmax")
		self.api.graph_from_tokens("Rq_jsc.dat",self.scan_name,"object_stats.dat","#Rq6","sim_info.dat","#jsc")

		self.api.graph_from_tokens("Ra_pmax.dat",self.scan_name,"object_stats.dat","#Ra6","sim_info.dat","#Pmax")
		self.api.graph_from_tokens("Ra_jsc.dat",self.scan_name,"object_stats.dat","#Ra6","sim_info.dat","#jsc")
		self.api.build_multiplot(self.scan_name)