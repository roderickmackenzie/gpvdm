#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys

class gpvdm_plugin:

	def __init__(self,api):
		self.api=api					#So finished can access the api
		api.mkdir("scan")					#make a new scandir
		for i in range(1,8):				#count from 1 to 20
			sim_path="scan/"+str(i)			#define a sub simulation dir
			api.mkdir(sim_path)				#make the dir
			api.clone(sim_path)				#copy the simulation files to the dir
			print(str(1e-7*float(i)))			#Print some text
			data=api.json_load(os.path.join(sim_path,"json.inp"))
			s=data.epi.find_shape_by_name("P3HT:PCBM")
			s.shape_dos.mue_y=1e-7*float(i)
			s.shape_dos.muh_y=1e-7*float(i)*2.0
			data.save()
			api.add_job(path=sim_path)			#Add the job to the job list
		api.run(callback=self.finished)			#Run the jobs on as many CPUs as you have, tell the server what to do when finished

	def finished(self):
		print("Finished!")
		self.api.build_multiplot("scan")			#Build the plot files
		#self.api.graph_from_tokens("output_plot.dat","scan","dos0.inp","#mueffe","sim_info.dat","#voc")