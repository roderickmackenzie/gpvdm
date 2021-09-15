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

#include <stdio.h>
#include <ray.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <ray_fun.h>
#include <dat_file.h>
#include <string.h>
#include <color.h>
#include <dump.h>
#include <util.h>
#include <detector.h>


/** @file detector_dump_bins.c
	@brief Ray tracing for the optical model, this should really be split out into it's own library.
*/


void detector_dump_bins(struct simulation *sim,struct image *in)
{
	int x;
	int y;
	char temp[200];

	struct dat_file buf;

	buffer_init(&buf);

	buffer_malloc(&buf);
	buf.x_mul=1.0;
	buf.y_mul=1e9;
	strcpy(buf.title,"Angle vs. Wavelength");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Wavelength");
	strcpy(buf.y_label,"Angle");
	strcpy(buf.data_label,"Intensity");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"Degrees");
	strcpy(buf.data_units,"Counts");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=in->escape_bins;
	buf.y=in->ray_wavelength_points;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (x=0;x<in->escape_bins;x++)
	{
		for (y=0;y<in->ray_wavelength_points;y++)
		{
			sprintf(temp,"%Le %e %Le\n",in->angle[x],in->lam[y],in->ang_escape[y][x]);
			buffer_add_string(&buf,temp);
		}

		buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,"","escape_bins_raw.dat",&buf);
	buffer_free(&buf);


}

