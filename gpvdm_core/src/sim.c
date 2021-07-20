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
	printf("SIM FREE!!!!!!!!!\n");
}
