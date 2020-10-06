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
#include "inp.h"
#include "log.h"
#include "dump.h"
#include <cal_path.h>

void dump_load_config(struct simulation* sim,struct device *in)
{
	int dump;
	struct dimensions *dim=&in->ns.dim;

	struct inp_file inp;
	inp_init(sim,&inp);


	inp_load_from_path(sim,&inp,get_input_path(sim),"dump.inp");

	inp_check(sim,&inp,1.57);

	dump=inp_search_english(sim,&inp,"#plot");
	set_dump_status(sim,dump_plot,dump);

	dump=inp_search_english(sim,&inp,"#newton_dump");
	set_dump_status(sim,dump_newton, dump);

	dump=inp_search_english(sim,&inp,"#dump_dynamic");
	set_dump_status(sim,dump_dynamic, dump);

	in->stop_start=inp_search_english(sim,&inp,"#startstop");

	in->dumpitdos=inp_search_english(sim,&inp,"#dumpitdos");

	inp_search_string(sim,&inp,in->plot_file,"#plotfile");

	inp_search_gdouble(sim,&inp,&(in->start_stop_time),"#start_stop_time");

	dump=inp_search_english(sim,&inp,"#dump_optics");
	set_dump_status(sim,dump_optics, dump);

	dump=inp_search_english(sim,&inp,"#dump_optics_verbose");
	set_dump_status(sim,dump_optics_verbose, dump);

	dump=inp_search_english(sim,&inp,"#dump_norm_time_to_one");
	set_dump_status(sim,dump_norm_time_to_one, dump);

	dump=inp_search_english(sim,&inp,"#dump_norm_y_axis");
	set_dump_status(sim,dump_norm_y_axis, dump);

	set_dump_status(sim,dump_energy_slice_switch, TRUE);

	inp_search_int(sim,&inp,&(in->dump_energy_slice_xpos),"#dump_energy_slice_xpos");
	if (in->dump_energy_slice_xpos<0)
	{
			set_dump_status(sim,dump_energy_slice_switch, FALSE);
	}
	if (in->dump_energy_slice_xpos>=dim->xlen) in->dump_energy_slice_xpos=0;

	inp_search_int(sim,&inp,&(in->dump_energy_slice_ypos),"#dump_energy_slice_ypos");
	if (in->dump_energy_slice_xpos<0)
	{
			set_dump_status(sim,dump_energy_slice_switch, FALSE);
	}
	if (in->dump_energy_slice_ypos>=dim->ylen) in->dump_energy_slice_ypos=0;

	inp_search_int(sim,&inp,&(in->dump_energy_slice_zpos),"#dump_energy_slice_zpos");
	if (in->dump_energy_slice_xpos<0)
	{
			set_dump_status(sim,dump_energy_slice_switch, FALSE);
	}
	if (in->dump_energy_slice_zpos>=dim->zlen) in->dump_energy_slice_zpos=0;


	set_dump_status(sim,dump_1d_slices, TRUE);

	inp_search_int(sim,&inp,&(in->dump_1d_slice_xpos),"#dump_1d_slice_xpos");
	if (in->dump_1d_slice_xpos<0) in->dump_1d_slice_xpos=-1;

	inp_search_int(sim,&inp,&(in->dump_1d_slice_zpos),"#dump_1d_slice_zpos");
	if (in->dump_1d_slice_zpos<0) in->dump_1d_slice_zpos=-1;


	dump=inp_search_english(sim,&inp,"#dump_verbose_electrical_solver_results");
	set_dump_status(sim,dump_1d_slices, dump);


	dump=inp_search_english(sim,&inp,"#dump_print_newtonerror");
	set_dump_status(sim,dump_print_newtonerror, dump);

	dump=inp_search_english(sim,&inp,"#dump_write_converge");
	set_dump_status(sim,dump_write_converge, dump);

	dump=inp_search_english(sim,&inp,"#dump_print_converge");
	set_dump_status(sim,dump_print_converge, dump);

	dump=inp_search_english(sim,&inp,"#dump_print_pos_error");
	set_dump_status(sim,dump_print_pos_error, dump);

	dump=inp_search_english(sim,&inp,"#dump_zip_files");
	set_dump_status(sim,dump_zip_files, dump);

	dump=inp_search_english(sim,&inp,"#dump_write_out_band_structure");
	set_dump_status(sim,dump_write_out_band_structure, dump);

	dump=inp_search_english(sim,&inp,"#dump_first_guess");
	set_dump_status(sim,dump_first_guess, dump);

	dump=inp_search_english(sim,&inp,"#dump_optical_probe");
	set_dump_status(sim,dump_optical_probe, dump);

	dump=inp_search_english(sim,&inp,"#dump_optical_probe_spectrum");
	set_dump_status(sim,dump_optical_probe_spectrum, dump);


	sim->log_level=inp_search_english(sim,&inp,"#dump_log_level");

	dump=inp_search_english(sim,&inp,"#dump_print_text");
	set_dump_status(sim,dump_print_text, dump);

	dump=inp_search_english(sim,&inp,"#dump_info_text");
	set_dump_status(sim,dump_info_text, dump);

	dump=inp_search_english(sim,&inp,"#dump_ray_trace_map");
	set_dump_status(sim,dump_ray_trace_map, dump);

	dump=inp_search_english(sim,&inp,"#dump_use_cache");
	set_dump_status(sim,dump_use_cache, dump);

	dump=inp_search_english(sim,&inp,"#dump_write_headers");
	set_dump_status(sim,dump_write_headers, dump);

	dump=inp_search_english(sim,&inp,"#dump_remove_dos_cache");
	set_dump_status(sim,dump_remove_dos_cache, dump);

	inp_search_gdouble(sim,&inp,&(in->dump_dynamic_pl_energy), "#dump_dynamic_pl_energy");


	inp_free(sim,&inp);


}
