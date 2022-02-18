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

/** @file clean_sim_dir.c
@brief run the simulation
*/
#include <enabled_libs.h>
#include "util.h"
#include "sim.h"
#include "dos.h"
#include "dump.h"
#include "log.h"
#include "inp.h"
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
#include <device_fun.h>

void clean_sim_dir(struct simulation *sim, struct device *dev)
{
	char temp[PATH_MAX];

	join_path(2,temp,dev->output_path,"error.dat");
	remove_file(sim,temp);

	join_path(2,temp,dev->output_path,"equilibrium");
	remove_dir(sim,temp);

	join_path(2,temp,dev->output_path,"solver");
	remove_dir(sim,temp);

	dump_remove_snapshots(sim,dev->output_path);

	join_path(2,temp,get_output_path(dev),"optics_output");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"optical_snapshots");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"trap_map");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"ray_trace");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"dynamic");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"frequency");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"exciton_output");
	remove_dir(sim,temp);

	join_path(2,temp,get_output_path(dev),"matrix_times.dat");
	remove_file(sim,temp);



}

