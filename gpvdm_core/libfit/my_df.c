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

/** @file my_df.c
@brief the dfunction we are trying to minimize.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <i.h>
#include <server.h>
#include "fit.h"
#include <dump_ctrl.h>
#include <timer.h>
#include <util.h>
#include <rand.h>
#include <inp.h>
#include <gpvdm_const.h>
#include <cal_path.h>
#include <log.h>


static int unused __attribute__((unused));


#ifdef remove
void  my_df (const gsl_vector *v, void *params,  gsl_vector *df)
{

	struct inp_file config_file;
	inp_init(local_sim,&config_file);
	double e0=0.0;
	double e1=0.0;
	double val;
	double delta=0.0;
	double grad=0.0;
	int fitcount=0;

	for (fitcount=0;fitcount<fitconfig->fitvars;fitcount++)
	{
		inp_load_from_path(local_sim,&config_file,get_input_path(local_sim),fitconfig->fititem[fitcount].fit_file);
		val = gsl_vector_get(v, fitcount);
		inp_replace_double(local_sim,&config_file,fitconfig->fititem[fitcount].fit_token, val);
	}

	inp_free(local_sim,&config_file);

	duplicate(local_sim);
	e0=fit_run_sims(local_sim,fitconfig);

	for (fitcount=0;fitcount<fitconfig->fitvars;fitcount++)
	{
		inp_load_from_path(local_sim,&config_file,get_input_path(local_sim),fitconfig->fititem[fitcount].fit_file);
		val = gsl_vector_get(v, fitcount);
		delta=fabs(val*0.01);

		inp_replace_double(local_sim,&config_file,fitconfig->fititem[fitcount].fit_token, val+delta);
		inp_free(local_sim,&config_file);

		duplicate(local_sim);


		e1=fit_run_sims(local_sim,fitconfig);
		grad=(e1-e0)/delta;
		printf_log(local_sim,"grad=%le %le %le %le\n",grad,e1,e0,delta);
		//getchar();
		gsl_vector_set(df, fitcount, grad);

		inp_load_from_path(local_sim,&config_file,get_input_path(local_sim),fitconfig->fititem[fitcount].fit_file);
		inp_replace_double(local_sim,&config_file,fitconfig->fititem[fitcount].fit_token, val);
		inp_free(local_sim,&config_file);
	}



}

void my_fdf (const gsl_vector *x, void *params, double *f, gsl_vector *df)
{
  *f = my_f(x, params);
  my_df(x, params, df);
}
#endif
