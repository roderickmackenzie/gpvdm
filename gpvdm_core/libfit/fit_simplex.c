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

/** @file fit_simplex.c
@brief simplex down hill fitting
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
#include <inp.h>


int fit_simplex(struct simulation *sim,struct fitvars *fitconfig)
{
	int random_reset_flag=0;

	double last_error=0.0;
	int fit_status=FIT_RUN;
	int algo_status=FIT_RUN;
	double size=0.0;
	int steps_that_dont_reduce_error=0;

	struct multimin data;


	printf_log(sim,"fit_now\n");

	fitconfig->sub_iterations=0;


	multimin_init(&data);

	data.ndim=fitconfig->fitvars;
	data.fn=&my_f;
	data.p0=(void *)sim;
	data.p1=(void *)fitconfig;


	multimin_malloc(&data);

	fit_load_vars(sim,&data,sim->root_simulation_path,fitconfig);

	fit_build_jobs(sim,fitconfig);

	multimin_init_simplex(&data);
	data.n_max=1e9;

	do
	{

		algo_status=simplex_iterate(sim,&data);

		if (algo_status)
		{
			//printf_log(sim,"fit status: Exited due to status error %d\n",algo_status);
			//printf_log(sim,"error: %s\n", gsl_strerror (algo_status));
			printf_log(sim,"fit status: Exited due to status error %d\n",algo_status);
			break;
		}

		//getchar();
		//printf("%le %le\n",lowest_error,data.error);
		//getchar();


		if (algo_status == SIMPLEX_CONVERGED)
		{
			printf_log(sim,"converged to minimum at\n");
			if (fitconfig->converge_error>data.error)
			{
				fit_status=FIT_FINISH;
				break;
			}else
			{
				fit_status=FIT_RESET;
				break;
			}
		}

		if (algo_status != 0)
		{
			printf_log(sim,"fit status: non zero error\n");
			fit_status=FIT_RESET;
			break;
		}

		//This is stall detection
		if (fitconfig->fit_method==FIT_SIMPLEX)
		{
			if (data.error>fitconfig->disable_reset_at)
			{

				if (fitconfig->enable_simple_reset==TRUE)
				{
					if (fitconfig->sub_iterations>fitconfig->simplexreset)
					{
						printf_log(sim,"fit status: simplex reset %d %d %d %d %lf %lf\n",steps_that_dont_reduce_error,fitconfig->stall_steps,fitconfig->sub_iterations,fitconfig->iterations,data.error,fitconfig->disable_reset_at);
						fit_status=FIT_RESET;
					}
				}

				if (steps_that_dont_reduce_error>fitconfig->stall_steps)
				{
					if (fitconfig->randomize==TRUE)
					{
						//if (fitconfig->sub_iterations_two>fitconfig->random_reset_ittr)
						{
							printf_log(sim,"fit status: testing random_reset\n");

							randomize_input_files(sim);

							random_reset_flag=1;
							printf_log(sim,"fit status: random reset %d %d %d %d %lf %lf\n",steps_that_dont_reduce_error,fitconfig->fitvars,fitconfig->sub_iterations,fitconfig->iterations,data.error,fitconfig->disable_reset_at);
							fit_status=FIT_RESET;
							fitconfig->sub_iterations_two=0;

						}
					}

				}


			}else
			{
				if (steps_that_dont_reduce_error>fitconfig->stall_steps)
				{
					printf_log(sim,"fit status: below min error not progressed in 1000 steps %d %d %d %d %lf %lf\n",steps_that_dont_reduce_error,fitconfig->fitvars,fitconfig->sub_iterations,fitconfig->iterations,data.error,fitconfig->disable_reset_at);
					fit_status=FIT_RESET;
					break;
				}
			}
		}

		double last_fit_crashes=get_fit_crashes(sim,fitconfig);
		if (last_fit_crashes>0.0)
		{
				printf_log(sim,"fit status: Detected nan resetting");
				fit_status=FIT_RESET;
				break;
		}


		if (fitconfig->fit_method==FIT_SIMPLEX)
		{
			fit_dump_log(sim,fitconfig,data.error, size);
		}else
		{
			fit_dump_log(sim,fitconfig,data.error, size);
		}

		FILE* out=fopen("fitlog_random_reset_flag.dat","a");
		if (out!=NULL)
		{
			fprintf (out,"%d %d\n", fitconfig->iterations, random_reset_flag);
			fclose(out);
		}
		random_reset_flag=0;

		if (last_error<=data.error)
		{
			steps_that_dont_reduce_error++;
		}else
		{
			steps_that_dont_reduce_error=0;
		}


		printf_log(sim,"fit status: Loop=%d last_error=%le\n",steps_that_dont_reduce_error,data.error);


		last_error=data.error;


		if (fitconfig->converge_error>data.error)
		{
			fit_status=FIT_FINISH;
			printf_log(sim,"fit status: Stopping because I've converged.\n");
		}

		log_time_stamp(sim);

		fitconfig->iterations++;
		fitconfig->sub_iterations++;
		fitconfig->sub_iterations_two++;
		
	}while (fit_status==FIT_RUN);


	printf_log(sim,"fit status: freeing fit memory\n");

	multimin_free(&data);


	printf_log(sim,"fit status: fit_now_exit\n");

return fit_status;
}

