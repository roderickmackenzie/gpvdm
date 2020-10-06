#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#This is an example of changing two layer widths at the same time
import os
import sys

class gpvdm_plugin:

	def __init__(self,api):
		#So finished can access the api
		self.api=api

		#Set the simulation dir
		self.sim_dir_name=os.path.join(api.get_sim_path(),"scan_example_2")

		#Allow gpvdm to use all the CPUs you have
		api.edit(os.path.join(api.get_sim_path(),"server.inp"),"#max_gpvdm_instances","false")

		#Set the initial layer thicknesses
		layer0_thick=10
		layer1_thick=10
		while (layer0_thick<100):
			while (layer1_thick<100):

				#Set the sub sim dir
				sim_path=os.path.join(self.sim_dir_name,str(layer0_thick),str(layer1_thick))			#define a sub simulation dir

				#Make the sim dir
				api.mkdir(sim_path)

				#copy the simulation files to the dir
				api.clone(sim_path)

				#Set the layer thicknesses
				#Look up which shape?.inp file stores the geometry of the layer
				shape_file=api.shape_name_to_file_name(sim_path,"P3HT:PCBM0")
				#edit the shape file to match the desired thickness
				api.edit(os.path.join(sim_path,shape_file),"#shape_dy",str(layer0_thick)+"e-9")

				#Look up which shape?.inp file stores the geometry of the layer
				shape_file=api.shape_name_to_file_name(sim_path,"P3HT:PCBM1")
				#edit the shape file to match the desired thickness
				api.edit(os.path.join(sim_path,shape_file),"#shape_dy",str(layer1_thick)+"e-9")

				#Update the y mesh so it matches the electrical layer thickeness
				api.edit(os.path.join(sim_path,"mesh_y.inp"),"#mesh_layer_length0",str(layer0_thick+layer1_thick)+"e-9")

				#Add the job to the job list
				api.add_job(path=sim_path)

				layer1_thick=layer1_thick+20

			layer1_thick=10
			layer0_thick=layer0_thick+20

		api.run(callback=self.finished)			#Run the jobs on as many CPUs as you have, tell the server what to do when finished

	def finished(self):
		print("Finished!")
		out=[]

		#Find the simulation files
		simulations=self.api.find_simulations(self.sim_dir_name)
		for sub_sim_path in simulations:
			#extract numbers
			shape_file=self.api.shape_name_to_file_name(sub_sim_path,"P3HT:PCBM0")
			layer0=self.api.get(os.path.join(sub_sim_path,shape_file),"#shape_dy")

			shape_file=self.api.shape_name_to_file_name(sub_sim_path,"P3HT:PCBM1")
			layer1=self.api.get(os.path.join(sub_sim_path,shape_file),"#shape_dy")

			pce=self.api.get(os.path.join(sub_sim_path,"sim_info.dat"),"#pce")
			ff=self.api.get(os.path.join(sub_sim_path,"sim_info.dat"),"#ff")
			#width/layer0 thick/layer1 thick/pce/ff

			out.append(str(layer0)+" "+str(layer1)+" "+str(pce)+" "+str(ff))

		#write list to disk
		f = open(os.path.join(self.api.get_sim_path(),"out.txt"), "w")
		for l in out:
			f.write(l+"\n")
		f.close()