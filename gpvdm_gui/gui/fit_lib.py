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

## @package fit_lib
#  This is the backend to handle fitting
#


import sys
import os
import shutil

import i18n
_ = i18n.language.gettext
import zipfile
from PIL import Image
from util_latex import latex
from token_lib import tokens
from gpvdm_json import gpvdm_data
from fit_lib_anal_results import fit_lib_anal_results
from search import find_sims

class fit_lib(fit_lib_anal_results):


	def gen_plots(self,scan_dir,plot_file,copy_fit_file=True):
		sims=find_sims(scan_dir)
		
		for s in sims:
			dest_plot_file=os.path.join(s,plot_file)
			dest_eps_file=os.path.join(s,plot_file)+".eps"
			dest_graph_file=os.path.join(s,"graph.jpg")
			dest_tex_file=os.path.join(s,"tab.tex")
			dest_jpg_tab=os.path.join(s,"tab.jpg")

			final_jpg=os.path.join(scan_dir,os.path.basename(s)+".jpg")


			data=gpvdm_data()
			data.load(os.path.join(s,"sim.json"))
			
			lx=latex()
			lx.document_start()
			lx.tab_start(["Parameter","Value","Units"])

			for latex_line in data.light.dump_as_latex(token_lib=tokens()):
				lx.tab_add_row([latex_line.text,"$"+latex_line.value+"$","$"+latex_line.units+"$"])

			for latex_line in data.parasitic.dump_as_latex(token_lib=tokens()):
				lx.tab_add_row([latex_line.text,"$"+latex_line.value+"$","$"+latex_line.units+"$"])

			for l in data.epi.layers:
				if l.shape_dos.enabled==True:
					for latex_line in l.shape_dos.dump_as_latex(token_lib=tokens()):
						ok=False
						for v in data.fits.vars.segments:
							print(latex_line.token, v.json_var.split("/")[-1])
							if latex_line.token == v.json_var.split("/")[-1]:
								ok=True
								break
						if ok==True:
							lx.tab_add_row([latex_line.text,"$"+latex_line.value+"$","$"+latex_line.units+"$"])


			for c in data.epi.contacts.segments:
				for latex_line in c.dump_as_latex(token_lib=tokens()):
					lx.tab_add_row([latex_line.text+" "+c.shape_name,"$"+latex_line.value+"$","$"+latex_line.units+"$"])


			lx.tab_end()
			lx.latex_document_end()
			lx.save(dest_tex_file)
			lx.make_jpg()



			#print(dest_jpg_file,dest_plot_file,dest_eps_file)
			if copy_fit_file==True:
				shutil.copy(plot_file, dest_plot_file)
			orig_dir=os.getcwd()
			os.chdir(s)
			os.system("gnuplot "+dest_plot_file+" > "+dest_eps_file)
			os.system("gs -dNOPAUSE -r600 -dEPSCrop -sDEVICE=jpeg -sOutputFile="+dest_graph_file+" "+dest_eps_file+" -c quit")
			os.chdir(orig_dir)

			try:
				im1 = Image.open(dest_graph_file)
				im2 = Image.open(dest_jpg_tab)
				dst = Image.new('RGB', (im1.width + im2.width, im1.height))
				dst.paste(im1, (0, 0))
				dst.paste(im2, (im1.width, 0))
				dst.save(final_jpg)
			except:
				pass

	def gen_fitlog_plot(self,plot_file,scan_dir):
		sims=[]
		lines="plot "
		for root, dirs, files in os.walk(scan_dir):
			for name in files:
				full_name=os.path.join(root, name)
				if full_name.endswith("fitlog.csv"):		
					lines=lines+"\'"+full_name+"\' with lp,"

		lines=lines.rstrip()[:-1]
		f=open(plot_file,'w')
		f.write(lines)
		f.close()

