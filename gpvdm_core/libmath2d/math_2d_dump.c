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




/** @file math_2d_dump.c
	@brief Dump data from 2d math structures
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sim_struct.h>
#include <memory.h>

#include "math_2d.h"
#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <log.h>
#include <lang.h>
#include <dat_file.h>

void math_2d_save(struct simulation *sim,char *file_name,struct math_2d *data)
{
	int x;
	int y;
	char temp[1000];
	struct dat_file buf;
	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1;
	buf.x_mul=1;
	buf.z_mul=1;
	sprintf(buf.title,"%s - %s",_("Math_2d output"),_("Position"));
	strcpy(buf.x_label,_("x-position"));
	strcpy(buf.y_label,_("y-position"));
	strcpy(buf.z_label,_("z-position"));
	strcpy(buf.data_label,_("Values"));
	strcpy(buf.x_units,"Pixels");
	strcpy(buf.y_units,"Pixels");
	strcpy(buf.z_units,"Pixels");
	strcpy(buf.data_units,"au");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=0.0;
	buf.Vexternal=0.0;
	buf.x=data->x_len;
	buf.y=data->y_len;
	buf.z=0.0;
	buffer_add_info(sim,&buf);
	for (y=0;y<data->y_len;y++)
	{
		for (x=0;x<data->x_len;x++)
		{
			sprintf(temp,"%d %d %Le\n",x,y,data->data[x][y]);
			buffer_add_string(&buf,temp);
		}
		buffer_add_string(&buf,"\n");

	}
	buffer_dump_path(sim,"",file_name,&buf);
	buffer_free(&buf);
}



