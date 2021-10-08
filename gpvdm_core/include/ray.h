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

/** @file ray.h
@brief ray tracing header files.
*/
#ifndef ray_h
#define ray_h
#include <enabled_libs.h>
#include <vec.h>
#include <i.h>
#include <sim_struct.h>
#include <triangle.h>
#include <dim.h>
#include <shape_struct.h>
#include <object.h>

#include <gpvdm_const.h>

#include <pthread.h>

struct ray
{
	struct vec xy;
	struct vec xy_end;
	struct vec dir;
	int state;
	int bounce;
	int obj_uid_start;		//The ray started in
	int parent;
	int uid;
	double mag;
};


struct ray_worker
{
	struct ray *rays;
	int nrays;
	int nray_max;
	int top_of_done_rays;
	int l;
		int working;
		pthread_t thread_han;
	int worker_n;
};

struct ray_src
{
	double x;
	double y;
	double z;

	int theta_steps;
	double theta_start;
	double theta_stop;

	int phi_steps;
	double phi_start;
	double phi_stop;

	int epi_layer;		//epi layer
	int light;			//light source
	int emission_source;	//single point or mesh


};

struct image
{
	int enabled;
	int worker_max;
	struct ray_worker *worker;

	struct ray_src *ray_srcs;
	int n_ray_srcs;

	double y_escape_level;
	long double *angle;
	long double **ang_escape;
	int ray_wavelength_points;
	double *lam;
	int escape_bins;

	double ray_lambda_start;
	double ray_lambda_stop;
	int ray_auto_wavelength_range;

	//benchmarking
	int tot_rays;
	double start_time;

	//run control
	int ray_auto_run;
	int dump_snapshots;

	struct dimensions viewpoint_dim;
	long double ***viewpoint_image;

};


#endif
