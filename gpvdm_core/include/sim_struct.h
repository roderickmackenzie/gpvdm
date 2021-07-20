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

/** @file sim_struct.h
@brief define the sim structure, the sim structure is used to keep all simulation parameters which are physicaly part of the device. Such as dll locations.
*/


#ifndef sim_struct_h
#define sim_struct_h

#include <enabled_libs.h>
#include <stdio.h>
#include <server_struct.h>
#include "cache_struct.h"
#include <hard_limit_struct.h>
//#ifdef dbus
//	#include <dbus/dbus.h>
//#endif

	#include <sys/mman.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>

#include <dirent.h>
#include <i_struct.h>
#include <dos_struct.h>

//<strip>


#include <gpvdm_const.h>
//</strip>

/*struct dumpfiles_struct
{
char file_name[100];
char path_name[100];
int write_out;
};*/

struct logging
{
	int log_level;
	char path[PATH_MAX];
	int html;
};

struct simulation
{
	//plotting
	FILE *gnuplot;
	FILE *gnuplot_time;
	//dump
	int dump_array[100];
	int dumpfiles;
	//struct dumpfiles_struct *dumpfile;

	//paths
	char root_simulation_path[PATH_MAX];

	//struct logging log;
	int log_level;
	//char path[PATH_MAX];
	int html;

	char plugins_path[PATH_MAX];
	char lang_path[PATH_MAX];
	char share_path[PATH_MAX];
	char exe_path[PATH_MAX];
	char exe_path_dot_dot[PATH_MAX];
	char materials_path[PATH_MAX];
	char cie_color_path[PATH_MAX];
	char emission_path[PATH_MAX];
	char spectra_path[PATH_MAX];
	char home_path[PATH_MAX];
	char shape_path[PATH_MAX];
	char cache_path[PATH_MAX];
	char cache_path_for_fit[PATH_MAX];
	char gpvdm_local_path[PATH_MAX];
	char tmp_path[PATH_MAX];
	char command_line_path[PATH_MAX];


	//solver name
	char solver_name[20];
	char complex_solver_name[20];

	//Matrix solver dll	- external
	void (*dll_matrix_init)();
	void (*dll_matrix_solve)();
	void (*dll_matrix_solver_free)();
	void *dll_matrix_handle;

	//Complex matrix solver dll	- internal
	void (*dll_complex_matrix_init)();
	void (*dll_complex_matrix_solve)();
	void (*dll_complex_matrix_solver_free)();
	void *dll_complex_matrix_handle;

	//Solve dlls
	char force_sim_mode[100];

	//Fitting vars
	double last_total_error;
	int fitting;
	struct server_struct server;

	int gui;

	long int bytes_written;
	long int bytes_read;
	long int files_read;
	long int files_written;

	long double T0;
	long double D0;
	long double n0;

	int math_stop_on_convergence_problem;

	int cache_len;
	int cache_max;
	struct cache_item *cache;

	//gui
	int mindbustx;
	//#ifdef dbus
	//	DBusConnection *connection;
	//#endif

	//#ifdef windows
	//	HANDLE connection;
	//#endif
	void *connection;

	struct math_xy cie_x;
	struct math_xy cie_y;
	struct math_xy cie_z;

	char *error_log;
	int error_log_size;
	int error_log_size_max;
	int errors_logged;


	struct hard_limit hl;
	struct dos_cache doscache;

};

#endif

