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

/** @file optics_plugin.c
	@brief Plugin to run optics as stand alone simulation.
*/

#include <sim.h>
#include <dump.h>
#include <dump.h>
#include <inp.h>
#include <dat_file.h>
#include <light_interface.h>
#include <dynamic_store.h>
#include <light.h>
#include <dat_file.h>
#include "optics_plugin.h"
#include <device.h>
#include <cal_path.h>
#include <fdtd.h>
#include <light_fun.h>


static int unused __attribute__((unused));

void sim_optics(struct simulation *sim,struct device *in)
{
	struct inp_file inp;

	dumpfiles_turn_on_dir(sim,"optical_output");
	set_dump_status(sim,dump_lock, FALSE);
	set_dump_status(sim,dump_optics, TRUE);
	set_dump_status(sim,dump_optics_verbose, TRUE);


	light_load_config(sim,&in->mylight,in);

	light_setup_dump_dir(sim,&in->mylight);

	light_load_dlls(sim,&in->mylight);


	in->mylight.disable_transfer_to_electrical_mesh=TRUE;

	inp_init(sim,&inp);


	light_set_sun(&in->mylight,1.0);


	light_solve_and_update(sim,in,&in->mylight,0.0);

	light_dump(sim,&in->mylight);


	light_dump_sim_info(sim,&in->mylight);



}
