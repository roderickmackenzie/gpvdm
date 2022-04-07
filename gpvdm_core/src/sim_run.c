//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright 2008-2022 Roderick C. I. MacKenzie https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// 

/** @file sim_run.c
@brief run the simulation
*/
#include <enabled_libs.h>
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
#include <exciton.h>
#include <exciton_fun.h>
#include <json.h>
#include <server.h>
#include <light_srcs.h>
#include <world.h>
#include <detector.h>
#include <singlet.h>
#include <singlet_fun.h>
#include <optical_mode_fun.h>

int device_run_simulation(struct simulation *sim, struct device *dev)
{

	int i;
	int z;
	int x;
	int y;
	struct stat st = {0};
	char temp[PATH_MAX];
	char json_path[PATH_MAX];
	int layers;
	int world_items;
	int enable_transfer_matrix_light=TRUE;
	FILE *file;

	struct json_obj *json_world;
	struct json_obj *json_detectors;
	struct json_obj *json_epi;
	struct json_obj *json_layer;
	struct json_obj *json_dos;
	struct json_obj *json_mesh;
	struct json_obj *json_ray;
	struct json_obj *json_light_sources;

	struct epitaxy *epi;
	log_clear(sim);

	printf_log(sim,"%s\n",_("Runing simulation"));

	join_path(2,json_path,dev->input_path,"sim.json");
	json_load(sim,&(dev->config),json_path);

	dev->onlypos=FALSE;

	dump_init(sim,dev);


	//json_dump_obj(&(dev->config.obj));
	set_dump_status(sim,dump_stop_plot, FALSE);
	set_dump_status(sim,dump_print_text, TRUE);
	dump_load_config(sim,dev);

	dev->kl_in_newton=FALSE;
	struct dimensions *dim=&(dev->ns.dim);

	clean_sim_dir(sim,dev);

	if (get_dump_status(sim,dump_newton)==TRUE)
	{
		join_path(2,temp,dev->output_path,"solver");

		if (stat(temp, &st) == -1)
		{
			mkdir(temp, 0700);
		}
	}

	device_load_math_config(sim,dev);

	///////////////Mesh and epitaxy//////////////////////////

	load_sim_file(sim,dev);

	if (strcmp(sim->force_sim_mode,"")!=0)
	{
		strcpy(dev->simmode,sim->force_sim_mode);
	}

	
	if (strcmp_end(dev->simmode,"@optics")==0)
	{
		dev->electrical_simulation_enabled=FALSE;
		dev->drift_diffision_simulations_enabled=FALSE;
	}

	if (strcmp_end(dev->simmode,"@exciton")==0)
	{
		dev->electrical_simulation_enabled=FALSE;
		dev->drift_diffision_simulations_enabled=FALSE;
	}

	if (strcmp_end(dev->simmode,"@fdtd")==0)
	{
		dev->electrical_simulation_enabled=FALSE;
		dev->drift_diffision_simulations_enabled=FALSE;
		enable_transfer_matrix_light=FALSE;
	}

	if (strcmp(dev->simmode,"trace@trace")==0)
	{
		dev->electrical_simulation_enabled=FALSE;
		dev->drift_diffision_simulations_enabled=FALSE;
	}

	if (strcmp_end(dev->simmode,"mesh_gen")==0)
	{
		dev->electrical_simulation_enabled=FALSE;
		dev->drift_diffision_simulations_enabled=FALSE;
	}

	/*if (strcmp_end(dev->simmode,"mesh_gen_electrical")==0)
	{
		dev->drift_diffision_simulations_enabled=FALSE;
	}*/


	json_epi=json_obj_find(&(dev->config.obj), "epitaxy");
	if (json_epi==NULL)
	{
		ewe(sim,"Object epitaxy not found\n");
	}
	epi=&(dev->my_epitaxy);
	layers=epitaxy_load(sim,epi,json_epi);
	mode_load(sim,&(dev->mode),dev);

	if (layers==0)
	{
		dev->electrical_simulation_enabled=FALSE;
	}

	if (layers>0)
	{
		if (dev->drift_diffision_simulations_enabled==TRUE)
		{
			dos_cache_setup(sim,&(sim->doscache),&(dev->config.obj));
			gen_dos_fd_gaus_fd(sim,epi,json_epi);
		}

		json_mesh=json_obj_find(&(dev->config.obj), "mesh");
		if (json_mesh==NULL)
		{
			ewe(sim,"Mesh object not found\n");
		}
		mesh_obj_load(sim,&(dev->mesh_data),json_mesh);

		contacts_load(sim,dev);
		mesh_obj_apply_srh_contacts(sim,&(dev->mesh_data),dev);

		device_to_dim(sim,dim,dev);



		mesh_build(sim,dev);
		//mesh_dump_y(sim,dim);
		//getchar();
		device_get_memory(sim,dev);


		mesh_numerate_points(sim,dev);

		load_config(sim,dev);

		contacts_setup(sim,dev);
		contacts_cal_std_resistance(sim,dev);


		epitaxy_mask(sim,dev);
	}

	json_world=json_obj_find(&(dev->config.obj), "world");
	if (json_world==NULL)
	{
		ewe(sim,"Object world not found\n");
	}
	world_items=world_load(sim,&(dev->w),json_world);

	if (layers+world_items<1)
	{
		ewe(sim,"Object no objects in the world\n");
	}

	json_detectors=json_obj_find(&(dev->config.obj), "detectors");
	if (json_detectors==NULL)
	{
		ewe(sim,"Object detectors not found\n");
	}

	detectors_load(sim,&(dev->w),json_detectors);

	state_cache_init(sim,dev);
	state_cache_enable(sim,dev);


	complex_solver_get_mem(sim,&(dev->msm));

	json_ray=json_obj_find_by_path(sim,&(dev->config.obj), "ray.segment0");
	ray_read_config(sim,&(dev->my_image),&(dev->w),json_ray);

	device_build_scene(sim,dev);

	device_build_obj_pointer_array(sim,dev);

	#ifdef libheat_enabled
		heat_load_config(sim,&(dev->thermal), dev);
	#endif

	#ifdef libexciton_enabled
		exciton_load_config(sim,&(dev->ex), dev);
	#endif

	dev->emission_enabled=FALSE;
	dev->pl_use_experimental_emission_spectra=FALSE;

	for (i=0;i<epi->layers;i++)
	{
		if (epi->layer[i].pl_enabled==TRUE)
		{
			dev->emission_enabled=TRUE;
		}

		if (epi->layer[i].pl_use_experimental_emission_spectra==TRUE)
		{
			dev->pl_use_experimental_emission_spectra=TRUE;
		}

	}

	//Load light sources
	json_light_sources=json_obj_find(&(dev->config.obj), "light_sources");

	if (json_light_sources==NULL)
	{
		ewe(sim,"Object json_light_sources not found\n");
	}


	light_srcs_load(sim,&(dev->lights),json_light_sources);


	dev->mun_symmetric=TRUE;
	dev->mup_symmetric=TRUE;


	epitaxy_load_dos_files(sim,dev,epi,json_epi);

	epitaxy_setup_interfaces(sim,dev);

	solver_get_mem(sim,&(dev->msm));

	if (strcmp(dev->solver_type,"circuit")==0)
	{
		strcpy(dev->newton_name,"newton_simple");
	}


	if (dev->electrical_simulation_enabled==TRUE)
	{

		if ((dim->xlen>1)||(dim->zlen>1))
		{
			if (strcmp(dev->newton_name,"newton_simple")!=0)
			{
				if (strcmp(dev->newton_name,"poisson_2d")!=0)
				{
					strcpy(dev->newton_name,"newton_2d");
				}
			}
		}



		newton_init(sim,dev,dev->newton_name);

		#ifdef libcircuit_enabled
			circuit_build_device(sim,&(dev->cir),dev);
		#endif

		device_alloc_traps(dev);

		if (get_dump_status(sim,dump_write_converge)==TRUE)
		{
			file = fopena(get_output_path(dev),"converge.dat","w");
			fclose(file);

			file=fopena(get_output_path(dev),"tconverge.dat","w");
			fclose(file);

			file=fopena(get_output_path(dev),"ion_converge.dat","w");
			fclose(file);
		}



		mesh_cal_layer_widths(dev);


		//getchar();

		update_material_arrays(sim,dev);

		device_interface_doping(sim,dev);

		dev->C=dev->xlen*dev->zlen*epsilon0*dev->epsilonr[0][0][0]/(dev->ylen+dev->other_layers);
		if (get_dump_status(sim,dump_print_text)==TRUE) printf_log(sim,"C=%Le\n",dev->C);
		//printf("%Le\n",dev->C);
		//getchar();
		dev->A=dev->xlen*dev->zlen;
		dev->Vol=dev->xlen*dev->zlen*dev->ylen;

		///////////////////////light model

		gdouble old_Psun=0.0;
		old_Psun=light_get_sun(&dev->mylight);

		if (strcmp_end(dev->simmode,"@eqe")==0)
		{
			dev->mylight.use_flat_sepctrum=TRUE;
		}

		light_load_config(sim,&dev->mylight,dev);

		if (dev->mylight.dump_verbosity>-1)
		{
			light_setup_dump_dir(sim,get_output_path(dev),&dev->mylight);
		}

		light_load_dlls(sim,&dev->mylight);

		#ifdef libcircuit_enabled
			circuit_cal_resistance(sim,&(dev->cir),dev);
		#endif

		//update_arrays(dev);

		contacts_force_to_zero(sim,dev);

		int y;


		get_initial(sim,dev,TRUE);

		remesh_shrink(sim,dev);

		if (solve_pos(sim,dev)==-1)
		{
			goto solver_exit;
		}


		time_init(sim,dev);

		//matrix_init(&dev->mx);

		solver_realloc(sim,dev);



		plot_open(sim);


		dev->go_time=FALSE;

		plot_now(sim,dev,"plot");
		//set_solver_dump_every_matrix(1);

		if (dev->drift_diffision_simulations_enabled==TRUE)
		{
			dev->map_start=dev->Ev[0][0][dim->ylen-1];
			dev->map_stop=dev->Ec[0][0][0]+1.0;
		}
		//set_solver_dump_every_matrix(0);


		/*if (dev->onlypos==TRUE)
		{
			join_path(2,temp,get_output_path(sim),"equilibrium");
			dump_1d_slice(sim,dev,temp);
			cache_dump(sim);
			cache_free(sim);
			device_free(sim,dev);
	//		color_cie_load(sim);
			return 0;
		}*/

	}


	run_electrical_dll(sim,dev,strextract_domain(dev->simmode));

	#ifdef libheat_enabled
		if (dev->electrical_simulation_enabled==FALSE)
		{
			if (dev->thermal.newton_enable_external_thermal==TRUE)
			{
				//printf("Run thermal solver\n");
				heat_solve(sim, &(dev->thermal),dev, 0, 0);
			}
		}
	#endif

	//Clean up
	solver_exit:

	device_free(sim,dev);

	plot_close(sim);

	newton_interface_free(sim,dev);

	//Free solver dlls
	complex_solver_free(sim,&(dev->msm));
	solver_free(sim,&(dev->msm));
	printf_log(sim,"%s %i %s\n", _("Solved"), dev->odes, _("Equations"));

	//measure(sim,dev->output_path,dev->input_path);

return dev->odes;
}

