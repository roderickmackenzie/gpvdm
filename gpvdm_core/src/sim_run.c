//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/** @file sim_run.c
@brief run the simulation
*/

#include "util.h"
#include "sim.h"
#include "dos.h"
#include "dump.h"
#include "newton_tricks.h"
#include "log.h"
#include "inp.h"
#include "solver_interface.h"
#include "newton_interface.h"
#include "mesh.h"
#include "remesh.h"
#include "lang.h"
#include <plot.h>
#include "device.h"
#include <cal_path.h>
#include <string.h>
#include <contacts.h>
#include <cache.h>
#include <sys/stat.h>
#include "measure.h"
#include <color.h>
#include <ray_fun.h>
#include <light_fun.h>
#include <memory.h>
#include <circuit.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <device_fun.h>
#include <heat.h>
#include <heat_fun.h>
#include <enabled_libs.h>

int run_simulation(struct simulation *sim)
{
	int i;
	int z;
	int x;
	int y;
	struct device cell;
	int enable_electrical=TRUE;
	struct stat st = {0};
	char temp[PATH_MAX];
	char device_file_path[PATH_MAX];

	log_clear(sim);

	printf_log(sim,"%s\n",_("Runing simulation"));

	device_init(sim,&cell);
	cache_init(sim);
	cell.onlypos=FALSE;

	dump_init(sim,&cell);

	set_dump_status(sim,dump_stop_plot, FALSE);
	set_dump_status(sim,dump_print_text, TRUE);
	dump_load_config(sim,&cell);

	cell.kl_in_newton=FALSE;
	struct dimensions *dim=&(cell.ns.dim);


	join_path(2,temp,get_output_path(sim),"error.dat");
	remove_file(sim,temp);

	join_path(2,temp,get_output_path(sim),"equilibrium");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(sim),"solver");
	remove_dir(sim,temp);

	if (get_dump_status(sim,dump_newton)==TRUE)
	{
		join_path(2,temp,get_output_path(sim),"solver");

		if (stat(temp, &st) == -1)
		{
			mkdir(temp, 0700);
		}
	}

	//join_path(2,temp,get_output_path(sim),"snapshots");
	//remove_dir(sim,temp);
	dump_remove_snapshots(sim);

	join_path(2,temp,get_output_path(sim),"optics_output");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(sim),"optical_snapshots");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(sim),"ray_trace");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(sim),"dynamic");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(sim),"frequency");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(sim),"matrix_times.dat");
	remove_file(sim,temp);

	device_load_math_config(sim,&cell);

	///////////////Mesh and epitaxy//////////////////////////

	join_path(2,device_file_path,get_input_path(sim),"epitaxy.inp");
	load_sim_file(sim,&cell);

	if (strcmp(sim->force_sim_mode,"")!=0)
	{
		strcpy(cell.simmode,sim->force_sim_mode);
	}


	if (strcmp(cell.simmode,"opticalmodel@optics")==0)
	{
		enable_electrical=FALSE;
	}

	if (strcmp(cell.simmode,"fdtd@fdtd")==0)
	{
		enable_electrical=FALSE;
	}

	if (strcmp(cell.simmode,"trace@trace")==0)
	{
		enable_electrical=FALSE;
	}

	if (strcmp_end(cell.simmode,"mesh_gen")==0)
	{
		enable_electrical=FALSE;
	}

	epitaxy_load(sim,&(cell.my_epitaxy),device_file_path);


	gen_dos_fd_gaus_fd(sim,&(cell.my_epitaxy));


	mesh_obj_load(sim,&(cell.mesh_data));
	contacts_load(sim,&cell);
	mesh_obj_apply_srh_contacts(sim,&(cell.mesh_data),&cell);

	dim->zlen=cell.mesh_data.meshdata_z.tot_points;
	dim->xlen=cell.mesh_data.meshdata_x.tot_points;
	dim->ylen=cell.mesh_data.meshdata_y.tot_points;

	mesh_build(sim,&cell);
	//mesh_dump_y(sim,dim);
	//getchar();
	device_get_memory(sim,&cell);


	mesh_numerate_points(sim,&cell);

	load_config(sim,&cell);

	contacts_setup(sim,&cell);
	contacts_cal_std_resistance(sim,&cell);


	epitaxy_mask(sim,&cell);

	state_cache_init(sim,&cell);
	state_cache_enable(sim,&cell);
	complex_solver_init(sim,&(cell.msm),cell.complex_solver_name);

	ray_read_config(sim,&(cell.my_image));

	device_build_scene(sim,&(cell));
	printf("here\n");
	device_build_obj_pointer_array(sim,&(cell));

	#ifdef libheat_enabled
		heat_load_config(sim,&(cell.thermal), &(cell));
	#endif

	cell.emission_enabled=FALSE;
	cell.pl_use_experimental_emission_spectra=FALSE;

	for (i=0;i<cell.my_epitaxy.layers;i++)
	{
		if (cell.my_epitaxy.layer[i].pl_enabled==TRUE)
		{
			cell.emission_enabled=TRUE;
		}

		if (cell.my_epitaxy.layer[i].pl_use_experimental_emission_spectra==TRUE)
		{
			cell.pl_use_experimental_emission_spectra=TRUE;
		}

	}



	printf_log(sim,"%s: %d\n",_("Loading DoS layers"),cell.my_epitaxy.electrical_layers);
	i=0;

	cell.mun_symmetric=TRUE;
	cell.mup_symmetric=TRUE;

	for (i=0;i<cell.my_epitaxy.electrical_layers;i++)
	{
		printf_log(sim,"%s %d/%d\n",_("Load DoS"),i,cell.my_epitaxy.electrical_layers);
		//printf("%s\n",cell.my_epitaxy.layer[i].dos_file);
		//getchar();
		load_dos(sim,&cell,cell.my_epitaxy.layer[i].dos_file,i);

		//printf("%d\n",cell.dosn[i].mobility_symmetric);
		//printf("%d\n",cell.dosp[i].mobility_symmetric);
		//getchar();

	}

	epitaxy_setup_interfaces(sim,&(cell));

	solver_init(sim,&(cell.msm),cell.solver_name);

	if (enable_electrical==TRUE)
	{

		if ((dim->xlen>1)||(dim->zlen>1))
		{
			if (strcmp(cell.newton_name,"newton_simple")!=0)
			{
				strcpy(cell.newton_name,"newton_2d");
			}
		}



		newton_init(sim,cell.newton_name);

		#ifdef libcircuit_enabled
			circuit_build_device(sim,&(cell.cir),&cell);
		#endif

		device_alloc_traps(&cell);

		if (get_dump_status(sim,dump_write_converge)==TRUE)
		{
			sim->converge = fopena(get_output_path(sim),"converge.dat","w");
			fclose(sim->converge);

			sim->tconverge=fopena(get_output_path(sim),"tconverge.dat","w");
			fclose(sim->tconverge);
		}



		mesh_cal_layer_widths(&cell);

		long double dy=0.0;
		long double pos=0.0;
		long double value=0.0;
		long double Nad0=0.0;
		long double Nad1=0.0;
		struct epitaxy *epi=&(cell.my_epitaxy);

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					Nad0=get_dos_doping_start(epi,cell.imat[z][x][y]);
					Nad1=get_dos_doping_stop(epi,cell.imat[z][x][y]);
					

					dy=epi->layer[cell.imat_epitaxy[z][x][y]].width;
					pos=cell.ns.dim.ymesh[y]-cell.layer_start[cell.imat[z][x][y]];

					cell.Nad[z][x][y]=Nad0+(Nad1-Nad0)*(pos/dy);
					//printf("%Le %Le\n",cell.layer_start[cell.imat[z][x][y]],cell.Nad[z][x][y]);
				}
				//getchar();
			}

		}
		
		/*for (x=0;x<dim->xlen;x++)
		{
			cell.Nad[0][x][0]=cell.test_param;
		}*/

		update_material_arrays(sim,&cell);

		cell.C=cell.xlen*cell.zlen*epsilon0*cell.epsilonr[0][0][0]/(cell.ylen+cell.other_layers);
		if (get_dump_status(sim,dump_print_text)==TRUE) printf_log(sim,"C=%Le\n",cell.C);
		//printf("%Le\n",cell.C);
		//getchar();
		cell.A=cell.xlen*cell.zlen;
		cell.Vol=cell.xlen*cell.zlen*cell.ylen;

		///////////////////////light model
		char old_model[100];
		gdouble old_Psun=0.0;
		old_Psun=light_get_sun(&cell.mylight);

		light_load_config(sim,&cell.mylight,&cell);



		//printf("%d \n",get_dump_status(sim,dump_optics_verbose));
		//getchar();
		if ((get_dump_status(sim,dump_optics_verbose)==TRUE) || (get_dump_status(sim,dump_optics)==TRUE))
		{
			light_setup_dump_dir(sim,&cell.mylight);
		}

		light_load_dlls(sim,&cell.mylight);

		#ifdef libcircuit_enabled
			circuit_cal_resistance(sim,&(cell.cir),&cell);
		#endif

		//update_arrays(&cell);

		contacts_force_to_zero(sim,&cell);


		get_initial(sim,&cell,TRUE);

		remesh_shrink(sim,&cell);

		if (cell.math_enable_pos_solver==TRUE)
		{
			for (z=0;z<dim->zlen;z++)
			{
				for (x=0;x<dim->xlen;x++)
				{
					solve_pos(sim,&cell,z,x);
				}
			}
		}




		time_init(sim,&cell);

		//matrix_init(&cell.mx);

		solver_realloc(sim,&cell);



		plot_open(sim);


		cell.go_time=FALSE;

		plot_now(sim,&cell,"plot");
		//set_solver_dump_every_matrix(1);

		find_n0(sim,&cell);

		cell.map_start=cell.Ev[0][0][dim->ylen-1];
		cell.map_stop=cell.Ec[0][0][0]+1.0;

		//set_solver_dump_every_matrix(0);


		if (cell.onlypos==TRUE)
		{
			join_path(2,temp,get_output_path(sim),"equilibrium");
			dump_1d_slice(sim,&cell,temp);
			cache_dump(sim);
			cache_free(sim);
			device_free(sim,&cell);
	//		color_cie_load(sim);
			return 0;
		}

	}



	//Load the dll
	if (is_domain(cell.simmode)!=0)
	{
		char gussed_full_mode[200];
		if (guess_whole_sim_name(sim,gussed_full_mode,get_input_path(sim),cell.simmode)==0)
		{
			printf_log(sim,"I guess we are using running %s\n",gussed_full_mode);
			strcpy(cell.simmode,gussed_full_mode);
		}else
		{
			ewe(sim,"I could not guess which simulation to run from the mode %s\n",cell.simmode);
		}


	}



	run_electrical_dll(sim,&cell,strextract_domain(cell.simmode));

	#ifdef libheat_enabled
		if (enable_electrical==FALSE)
		{
			if (cell.thermal.newton_enable_external_thermal==TRUE)
			{
				//printf("Run thermal solver\n");
				heat_solve(sim, &(cell.thermal),&(cell), 0, 0);
			}
		}
	#endif

	//Clean up
	cache_dump(sim);

	cache_free(sim);

	device_free(sim,&cell);

	plot_close(sim);

	newton_interface_free(sim);

	//Free solver dlls
	complex_solver_free(sim,&(cell.msm));
	solver_free(sim,&(cell.msm));
	printf_log(sim,"%s %i %s\n", _("Solved"), cell.odes, _("Equations"));

	measure(sim);
	dump_clean_cache_files(sim);

return cell.odes;
}

