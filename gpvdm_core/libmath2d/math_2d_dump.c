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
	buffer_init(&buf);

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



