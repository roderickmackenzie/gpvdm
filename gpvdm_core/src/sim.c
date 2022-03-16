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

/** @file sim.c
@brief init sim structure
*/


#include <enabled_libs.h>
#include <sim.h>
#include <string.h>
#include <server.h>
#include <hard_limit.h>
#include <dos.h>
#include <color.h>
#include <solver_interface.h>
#include <lock.h>

void sim_init(struct simulation *sim)
{
//plotting
	sim->gnuplot= NULL;
	sim->gnuplot_time= NULL;
	sim->log_level= -1;

	//newton solver dll
	strcpy(sim->force_sim_mode,"");

	//matrix solver dll
	strcpy(sim->solver_name,"");

	//complex matrix solver dll
	sim->dll_complex_matrix_handle=NULL;
	sim->dll_complex_matrix_solve=NULL;
	sim->dll_complex_matrix_solver_free=NULL;
	sim->dll_complex_matrix_init=NULL;
	strcpy(sim->complex_solver_name,"");

	//fit vars
	sim->last_total_error=-1.0;
	sim->gui=FALSE;
	sim->fitting=FIT_NOT_FITTING;
	sim->html=FALSE;
	sim->bytes_written=0;
	sim->bytes_read=0;
	sim->files_read=0;
	sim->files_written=0;
	sim->T0=300.0;
	sim->D0=243.75;
	sim->n0=1e20;

	sim->math_stop_on_convergence_problem=FALSE;

	//sim->x_matrix_offset=0;

	sim->cache_len=-1;
	sim->cache_max=-1;
	sim->cache=NULL;

	sim->mindbustx=FALSE;

	sim->connection=NULL;

	sim->error_log=NULL;
	sim->error_log_size=0;
	sim->error_log_size_max=0;
	sim->errors_logged=0;
	strcpy(sim->server.dbus_finish_signal,"");

	server2_init(sim,&(sim->server));

	//run control

	strcpy(sim->command_line_path,"");
	hard_limit_init(sim,&(sim->hl));
	dos_cache_init(&(sim->doscache));

	int i;
	for (i=0;i<100;i++)
	{
		sim->dump_array[i]=FALSE;
	}

}


void sim_free(struct simulation *sim)
{
	solver_unload_dll(sim);
	complex_solver_unload_dll(sim);
	server_shut_down(sim,&(sim->server));

	server2_free(sim,&sim->server);

	color_cie_free(sim);
	hard_limit_free(sim,&(sim->hl));
	dos_cache_dump(&(sim->doscache));
	dos_cache_free(&(sim->doscache));


	errors_free(sim);
}
