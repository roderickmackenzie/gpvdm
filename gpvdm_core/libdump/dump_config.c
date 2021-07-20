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

/** @file dump_config.c
@brief read the dump config file
*/


#include "sim.h"
#include "dump.h"
#include "log.h"
#include "dump.h"
#include <cal_path.h>
#include <json.h>

void dump_load_config(struct simulation* sim,struct device *in)
{
	int dump;
	struct dimensions *dim=&in->ns.dim;
	struct json_obj *obj;
	obj=json_obj_find(&(in->config.obj), "dump");
	if (obj==NULL)
	{
		ewe(sim,"Object light not found\n");
	}


	json_get_english(sim,obj, &(dump),"plot");
	set_dump_status(sim,dump_plot,dump);

	json_get_english(sim,obj, &(dump),"newton_dump");
	set_dump_status(sim,dump_newton, dump);

	json_get_english(sim,obj, &(dump),"dump_dynamic");
	set_dump_status(sim,dump_dynamic, dump);

	json_get_english(sim,obj, &(in->stop_start),"startstop");

	json_get_english(sim,obj, &(in->dumpitdos),"dumpitdos");

	json_get_string(sim, obj, in->plot_file,"plotfile");

	json_get_long_double(sim,obj, &(in->start_stop_time),"start_stop_time");

	json_get_english(sim,obj, &(dump),"dump_optics");
	set_dump_status(sim,dump_optics, dump);

	json_get_english(sim,obj, &(dump),"dump_norm_time_to_one");
	set_dump_status(sim,dump_norm_time_to_one, dump);

	json_get_english(sim,obj, &(dump),"dump_norm_y_axis");
	set_dump_status(sim,dump_norm_y_axis, dump);

	set_dump_status(sim,dump_energy_slice_switch, TRUE);

	json_get_int(sim,obj, &in->dump_energy_slice_xpos,"dump_energy_slice_xpos");
	if (in->dump_energy_slice_xpos<0)
	{
			set_dump_status(sim,dump_energy_slice_switch, FALSE);
	}
	if (in->dump_energy_slice_xpos>=dim->xlen) in->dump_energy_slice_xpos=0;

	json_get_int(sim,obj, &in->dump_energy_slice_ypos,"dump_energy_slice_ypos");
	if (in->dump_energy_slice_xpos<0)
	{
			set_dump_status(sim,dump_energy_slice_switch, FALSE);
	}
	if (in->dump_energy_slice_ypos>=dim->ylen) in->dump_energy_slice_ypos=0;

	json_get_int(sim,obj, &in->dump_energy_slice_zpos,"dump_energy_slice_zpos");
	if (in->dump_energy_slice_xpos<0)
	{
			set_dump_status(sim,dump_energy_slice_switch, FALSE);
	}
	if (in->dump_energy_slice_zpos>=dim->zlen) in->dump_energy_slice_zpos=0;


	set_dump_status(sim,dump_1d_slices, TRUE);

	json_get_int(sim,obj, &in->dump_1d_slice_xpos,"dump_1d_slice_xpos");
	if (in->dump_1d_slice_xpos<0) in->dump_1d_slice_xpos=-1;

	json_get_int(sim,obj, &in->dump_1d_slice_zpos,"dump_1d_slice_zpos");
	if (in->dump_1d_slice_zpos<0) in->dump_1d_slice_zpos=-1;


	json_get_english(sim,obj, &(dump),"dump_verbose_electrical_solver_results");
	set_dump_status(sim,dump_1d_slices, dump);



	json_get_english(sim,obj, &(dump),"dump_write_converge");
	set_dump_status(sim,dump_write_converge, dump);


	json_get_english(sim,obj, &(dump),"dump_zip_files");
	set_dump_status(sim,dump_zip_files, dump);

	json_get_english(sim,obj, &(dump),"dump_write_out_band_structure");
	set_dump_status(sim,dump_write_out_band_structure, dump);

	json_get_english(sim,obj, &(dump),"dump_first_guess");
	set_dump_status(sim,dump_first_guess, dump);

	json_get_english(sim,obj, &(dump),"dump_optical_probe");
	set_dump_status(sim,dump_optical_probe, dump);

	json_get_english(sim,obj, &(dump),"dump_optical_probe_spectrum");
	set_dump_status(sim,dump_optical_probe_spectrum, dump);

	json_get_english(sim,obj, &(sim->log_level),"dump_log_level");

	json_get_english(sim,obj, &(dump),"dump_log_level");
	set_dump_status(sim,dump_print_text, dump);

	json_get_english(sim,obj, &(dump),"dump_info_text");
	set_dump_status(sim,dump_info_text, dump);

	json_get_english(sim,obj, &(dump),"dump_ray_trace_map");
	set_dump_status(sim,dump_ray_trace_map, dump);

	json_get_english(sim,obj, &(dump),"dump_use_cache");
	set_dump_status(sim,dump_use_cache, dump);

	json_get_english(sim,obj, &(dump),"dump_write_headers");
	set_dump_status(sim,dump_write_headers, dump);

	json_get_english(sim,obj, &(dump),"dump_remove_dos_cache");
	set_dump_status(sim,dump_remove_dos_cache, dump);

	json_get_long_double(sim,obj, &(in->dump_dynamic_pl_energy),"dump_dynamic_pl_energy");


}
