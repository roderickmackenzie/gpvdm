#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import json

#This example script works with the perovskite solar cell example
#It changes the thickness of a layer and then records FF, Jsc and Voc
#as a function of thickness

class gpvdm_plugin:

	def __init__(self,api):
		#Save the pointer to the API
		self.api=api

		#Set the new simulation dir
		self.sim_dir_name=os.path.join(api.get_sim_path(),"scan")

		#all gpvdm data is stored in json (https://json.org/example.html) files zipped wiht in the .gpvdm file
		#load the json file
		data=api.json_load(os.path.join(api.get_sim_path(),"json.inp"))
		print(api.get_sim_path(),"json.inp")
		data.server.max_gpvdm_instances=False	#Use all CPUs
		data.save()		#save the json file

		#Set the initial layer thicknesses
		layer_thick=10
		while (layer_thick<100):

			#Set the sub sim dir
			sim_path=os.path.join(self.sim_dir_name,str(layer_thick))	#define a sub simulation dir

			#Make the sim dir
			api.mkdir(sim_path)

			#copy the simulation files to the dir
			api.clone(sim_path,api.get_sim_path())

			#Set the layer thicknesses
			#Load the json file again for the sub dir
			data=api.json_load(os.path.join(sim_path,"json.inp"))

			#set the simulation model to JV
			data.sim.simmode="segment0@jv"

			#find the object with name perovskite
			s0=data.epi.find_shape_by_name("PEDOT:PSS")
			s1=data.epi.find_shape_by_name("perovskite")
			s2=data.epi.find_shape_by_name("ZnO")

			#Edit it's thickness
			s1.dy=layer_thick*1e-9	#the thickness is stored in nm

			#Update the y mesh so it matches the electrical layer thickeness
			#if your mesh has mutiple segments then you will have to pick the right layer.
			data.mesh.mesh_y.segments[0].len=s0.dy+(layer_thick)*1e-9+s2.dy

			#save the json file
			data.save()

			#Add the job to the job list
			api.add_job(path=sim_path)

			layer_thick=layer_thick+20	#add 20 to the layer thickness

		#Run the jobs on as many CPUs as you have
		#The finction pointed at by callback is called when the jobs are finished
		api.run(callback=self.finished)

	def finished(self):
		print("Finished!")
		out=[]

		#Find the simulation files
		simulations=self.api.find_simulations(self.sim_dir_name)
		for sub_sim_path in simulations:
			print(sub_sim_path)
			#extract numbers
			data=self.api.json_load(os.path.join(sub_sim_path,"json.inp"))
			s0=data.epi.find_shape_by_name("perovskite")
			if s0==None:
				print("shape not found")

			#load the sim_info.dat file which is a .json file
			with open(os.path.join(sub_sim_path,"sim_info.dat")) as f:
				contents = f.read()
			json_data=json.loads(contents)

			pce=json_data['pce']	#find the pce
			ff=json_data['ff']	#find the ff

			out.append(str(s0.dy)+" "+str(pce)+" "+str(ff))

		#write list to disk
		f = open(os.path.join(self.api.get_sim_path(),"out.txt"), "w")
		for l in out:
			f.write(l+"\n")
		f.close()
		print("exit")