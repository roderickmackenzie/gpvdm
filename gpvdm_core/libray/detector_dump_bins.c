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

	dat_file_init(&buf);

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

