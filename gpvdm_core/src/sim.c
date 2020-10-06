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



#include <sim.h>
#include <string.h>
#include <server.h>

void sim_init(struct simulation *sim)
{
//plotting
	sim->gnuplot= NULL;
	sim->gnuplot_time= NULL;
	sim->converge= NULL;
	sim->tconverge= NULL;
	sim->log_level= -1;

	//newton solver dll
	sim->dll_solve_cur=NULL;
	sim->dll_solver_realloc=NULL;
	sim->dll_solver_free_memory=NULL;
	sim->dll_solver_handle=NULL;
	strcpy(sim->force_sim_mode,"");

	//matrix solver dll
	sim->dll_matrix_handle=NULL;
	sim->dll_matrix_solve=NULL;
	sim->dll_matrix_solver_free=NULL;

	//complex matrix solver dll
	sim->dll_complex_matrix_handle=NULL;
	sim->dll_complex_matrix_solve=NULL;
	sim->dll_complex_matrix_solver_free=NULL;

	//fit vars
	sim->last_total_error=-1.0;
	sim->gui=FALSE;
	sim->fitting=FALSE;
	sim->dumpfile=NULL;
	sim->dumpfiles=-1;
	sim->html=FALSE;
	sim->bytes_written=0;
	sim->bytes_read=0;
	sim->files_read=0;
	sim->files_written=0;
	sim->T0=300.0;
	sim->D0=243.75;
	sim->n0=1e20;
	//sim->x_matrix_offset=0;

	sim->cache_len=-1;
	sim->cache_max=-1;
	sim->cache=NULL;

	sim->mindbustx=FALSE;

	#ifdef dbus
		sim->connection=NULL;
	#endif


	sim->error_log=NULL;
	sim->error_log_size=0;
	sim->error_log_size_max=0;
	sim->errors_logged=0;
	strcpy(sim->server.dbus_finish_signal,"");

	server2_init(sim,&(sim->server));

	//run control

	strcpy(sim->command_line_path,"");

}

