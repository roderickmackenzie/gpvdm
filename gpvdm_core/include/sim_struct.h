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
	char filter_path[PATH_MAX];
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

