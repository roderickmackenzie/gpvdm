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

		#all gpvdm data is stored in json (https://json.org/example.html) files zipped wiht in the .gpvdm file
		data=api.json_load(os.path.join(api.get_sim_path(),"json.inp"))			#load the json file
		data.server.max_gpvdm_instances=False	#Use all CPUs
		data.save()		#save the json file

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
				#Load the json file again for the sub dir
				data=api.json_load(os.path.join(sim_path,"json.inp"))
				#find the object with name P3HT:PCBM0
				s=data.epi.find_shape_by_name("P3HT:PCBM0")
				#Edit it's thickness
				s.dy=layer0_thick*1e-9

				#find the object with name P3HT:PCBM1
				s=data.epi.find_shape_by_name("P3HT:PCBM1")
				#Edit it's thickness
				s.dy=layer1_thick*1e-9

				#Update the y mesh so it matches the electrical layer thickeness
				data.mesh.mesh_y.segments[0].len=(layer0_thick+layer1_thick)*1e-9

				#save the json file
				data.save()

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
			data=self.api.json_load(os.path.join(sub_sim_path,"json.inp"))
			s0=data.epi.find_shape_by_name("P3HT:PCBM0")
			s1=data.epi.find_shape_by_name("P3HT:PCBM1")

			pce=self.api.get(os.path.join(sub_sim_path,"sim_info.dat"),"#pce")
			ff=self.api.get(os.path.join(sub_sim_path,"sim_info.dat"),"#ff")

			out.append(str(s0.dy)+" "+str(s1.dy)+" "+str(pce)+" "+str(ff))

		#write list to disk
		f = open(os.path.join(self.api.get_sim_path(),"out.txt"), "w")
		for l in out:
			f.write(l+"\n")
		f.close()