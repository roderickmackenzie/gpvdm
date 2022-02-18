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

/** @file fit_error.c
@brief calculate the fitting error.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <util.h>
#include <i.h>
#include <log.h>

	#define _GNU_SOURCE
	#include <dlfcn.h>

static double last_constraints_error=-1.0;
static double last_fit_names_error=-1.0;

#include "fit.h"
#include "cal_path.h"

static int unused __attribute__((unused));


double get_all_error(struct simulation *sim,struct fitvars *myfit)
{

int i;
last_fit_names_error=0.0;

	for (i=0;i<myfit->data_sets;i++)
	{
		if (myfit->data_set[i].enabled>0)
		{
			//printf("one\n");

			myfit->data_set[i].error=fit_get_sim_error(sim,myfit,i,FALSE);
			//printf("two\n");

			printf_log(sim,"Last %s error = %le\n",myfit->data_set[i].fit_name,myfit->data_set[i].error);
			last_fit_names_error+=myfit->data_set[i].error;
		}


	}

	printf_log(sim,"Total error = %le\n",last_fit_names_error);


last_constraints_error=get_constraints_error(sim,myfit);

printf_log(sim,"last_constraints_error = %le\n",last_constraints_error);


//printf_log(sim,"last_fit_crashes = %le\n",last_fit_crashes);

sim->last_total_error=last_fit_names_error+last_constraints_error;
//+last_fit_crashes;

printf_log(sim,"total error = %le\n",sim->last_total_error);

#ifndef mindump
FILE *fiterror=fopen("fiterror.dat","w");
fprintf(fiterror,"%le\n",sim->last_total_error);
fclose(fiterror);
#endif

return sim->last_total_error;
}
