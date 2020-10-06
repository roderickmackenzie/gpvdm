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

/** @file shape.c
	@brief Load the shape files.
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <string.h>
#include <inp.h>
#include <util.h>
#include <cal_path.h>
#include <i.h>
#include <triangles.h>
#include <triangle.h>


void epitaxy_shapes_load(struct simulation *sim,struct epitaxy *in)
{
	int l=0;
	int ii=0;
	char build[100];
	strcpy(build,"");
	int pos=0;
	int len=0;
	struct shape *s;
	long double y_pos=0.0;

	for (l=0;l<in->layers;l++)
	{
		//printf("%d\n",l);
		//getchar();
		in->layer[l].nshape=0;
		if (strcmp(in->shape_file[l],"none")!=0)
		{
			len=strlen(in->shape_file[l])+1;
			for (ii=0;ii<len;ii++)
			{
				if ((in->shape_file[l][ii]==',')||(ii==len-1))
				{

					s=shape_load_file(sim,in,&(in->layer[l].shapes[in->layer[l].nshape]),build,0.0);
					s->epi_index=l;
					if (s->flip_y==FALSE)
					{
						s->y0=in->layer[l].y_start+s->y0;		//Starting from zero
					}else
					{
						s->y0=in->layer[l].y_stop-s->y0;		//Starting from top of layer
					}

					if (s!=NULL)
					{
						in->layer[l].nshape++;


						s->epi_index=l;
					}

					build[0]=0;
					pos=0;
				}else
				{
					build[pos]=in->shape_file[l][ii];
					build[pos+1]=0;
					pos++;
				}


			}
		}

	}

}


