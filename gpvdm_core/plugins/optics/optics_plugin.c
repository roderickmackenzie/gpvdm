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
#include <device_fun.h>

static int unused __attribute__((unused));

void sim_optics(struct simulation *sim,struct device *in)
{
	struct inp_file inp;

	set_dump_status(sim,dump_lock, FALSE);
	in->mylight.dump_verbosity=1;

	light_load_config(sim,&in->mylight,in);

	light_setup_dump_dir(sim,get_output_path(in),&in->mylight);

	//printf("%s %s\n",in->mylight.dump_dir,get_output_path(in));
	//getchar();

	light_load_dlls(sim,&in->mylight);


	in->mylight.disable_transfer_to_electrical_mesh=TRUE;

	inp_init(sim,&inp);


	light_set_sun(&in->mylight,1.0);

	light_solve_and_update(sim,in,&in->mylight,0.0);



	light_dump(sim,&in->mylight);


	light_dump_sim_info(sim,in->output_path,&in->mylight,in);



}
