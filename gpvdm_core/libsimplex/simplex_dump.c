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

/** @file simplex_dump.c
@brief Dump simplex status
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <simplex.h>
#include <log.h>
#include <sim_struct.h>

void multimin_dump(struct simulation *sim,struct multimin *data)
{
#ifdef simplex_verbose
	int s;
	int d;

	/*	FILE *out;
		out=fopen("one.dat","w");
		fprintf(out,"0.0 0.0\n\n");

		fprintf(out,"50.0 50.0\n\n");

		fprintf(out,"%f %f\n\n",data->center[0],data->center[1]);


		for (s=0;s<data->nsimplex;s++)
		{
			fprintf(out,"%f %f\n",data->p[s][0],data->p[s][1]);
		}
			fprintf(out,"%f %f\n",data->p[0][0],data->p[0][1]);
		fclose(out);

	printf("\n");*/

		for (d=0;d<data->ndim;d++)
		{
			for (s=0;s<data->nsimplex;s++)
			{
				printf_log(sim,"%e ",data->p[s][d]);
			}

			printf_log(sim,"\n");
		}

		for (s=0;s<data->nsimplex;s++)
		{
			printf_log(sim,"y[%d] = %e \n",s,data->y[s]);
		}

		for (d=0;d<data->ndim;d++)
		{
			printf_log(sim,"center[%d] = %e \n",d,data->center[d]);
		}


		printf_log(sim,"0worst=%d 1worst=%d best='%d'\n",data->i_hi0,data->i_hi1,data->i_lo);

		printf_log(sim,"error=%e\n",data->error);

	//printf("\n");

	//getchar();
#endif
}
