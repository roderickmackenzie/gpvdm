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
	struct json_obj *obj;
	obj=json_obj_find(&(in->config.obj), "dump");
	if (obj==NULL)
	{
		ewe(sim,"Object dump not found in dump_config.c 2\n");
	}


	json_get_english(sim,obj, &(dump),"plot");
	set_dump_status(sim,dump_plot,dump);

	json_get_english(sim,obj, &(dump),"newton_dump");
	set_dump_status(sim,dump_newton, dump);

	json_get_english(sim,obj, &(dump),"dump_dynamic");
	set_dump_status(sim,dump_dynamic, dump);

	json_get_english(sim,obj, &(in->stop_start),"startstop");

	json_get_string(sim, obj, in->plot_file,"plotfile");

	json_get_long_double(sim,obj, &(in->start_stop_time),"start_stop_time");

	json_get_english(sim,obj, &(dump),"dump_norm_time_to_one");
	set_dump_status(sim,dump_norm_time_to_one, dump);

	json_get_english(sim,obj, &(dump),"dump_norm_y_axis");
	set_dump_status(sim,dump_norm_y_axis, dump);


	set_dump_status(sim,dump_1d_slices, TRUE);


	json_get_english(sim,obj, &(dump),"dump_verbose_electrical_solver_results");
	set_dump_status(sim,dump_1d_slices, dump);



	json_get_english(sim,obj, &(dump),"dump_write_converge");
	set_dump_status(sim,dump_write_converge, dump);

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
