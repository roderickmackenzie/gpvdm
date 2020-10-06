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
