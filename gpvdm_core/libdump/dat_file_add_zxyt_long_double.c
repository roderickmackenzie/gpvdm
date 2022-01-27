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

/** @file dump.c
@brief go and dump stuff, what is dumped depends on which options have been set
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <pl.h>
#include <probe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <inp.h>
#include <color.h>
#include <memory.h>
#include <ray_fun.h>
#include <device_fun.h>
#include <heat_fun.h>
#include <contacts.h>
#include <enabled_libs.h>
#include <dos.h>

static int unused __attribute__((unused));


void dat_file_add_zxyt_long_double(struct simulation *sim,struct dat_file *buf,struct device *dev)
{
	int x=0;
	int y=0;
	int z=0;
	int size=0;
	int pos=0;
	int band=0;
	struct shape *s;
	struct dimensions *dim=&dev->ns.dim;

	strcpy(buf->cols,"zxyEd");
	buf->bin=TRUE;
	buf->srh_bands=dim->srh_bands;
	buffer_add_json(sim,buf);

	pos=buf->len;
	size=(dim->zlen+dim->xlen+dim->ylen)*sizeof(float);					//axies
	size+=dim->zlen*dim->xlen*dim->ylen*sizeof(float);					//Ec
	size+=dim->zlen*dim->xlen*dim->ylen*sizeof(float);					//nf
	size+=dim->zlen*dim->xlen*dim->ylen*dim->srh_bands*sizeof(float);	//Energy of trap (e)
	size+=dim->zlen*dim->xlen*dim->ylen*dim->srh_bands*sizeof(float);	//trap density (e)

	size+=dim->zlen*dim->xlen*dim->ylen*sizeof(float);					//Ev
	size+=dim->zlen*dim->xlen*dim->ylen*sizeof(float);					//pf
	size+=dim->zlen*dim->xlen*dim->ylen*dim->srh_bands*sizeof(float);	//Energy of trap (h)
	size+=dim->zlen*dim->xlen*dim->ylen*dim->srh_bands*sizeof(float);	//trap density (h)

	dat_file_increase_buffer(buf,size);

	for (z=0;z<dim->zlen;z++)
	{
		*((float *)(buf->buf+pos))=dim->z[z];
		pos+=sizeof(float);
	}

	for (x=0;x<dim->xlen;x++)
	{
		*((float *)(buf->buf+pos))=dim->x[x];
		pos+=sizeof(float);
	}

	for (y=0;y<dim->ylen;y++)
	{
		*((float *)(buf->buf+pos))=dim->y[y];
		pos+=sizeof(float);
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				s=dev->obj_zxy[z][x][y]->s;

				*((float *)(buf->buf+pos))=dev->Ec[z][x][y];
				pos+=sizeof(float);

				*((float *)(buf->buf+pos))=dev->n[z][x][y];
				pos+=sizeof(float);

				for (band=0;band<dim->srh_bands;band++)
				{
					*((float *)(buf->buf+pos))=dev->Ec[z][x][y]+dos_get_band_energy_n(s,band);
					pos+=sizeof(float);

					*((float *)(buf->buf+pos))=dev->nt[z][x][y][band];
					pos+=sizeof(float);
				}

				*((float *)(buf->buf+pos))=dev->Ev[z][x][y];
				pos+=sizeof(float);

				*((float *)(buf->buf+pos))=dev->p[z][x][y];
				pos+=sizeof(float);

				for (band=0;band<dim->srh_bands;band++)
				{
					*((float *)(buf->buf+pos))=dev->Ev[z][x][y]-dos_get_band_energy_p(s,band);
					pos+=sizeof(float);

					*((float *)(buf->buf+pos))=dev->pt[z][x][y][band];
					pos+=sizeof(float);
				}
			}
		}
	}


}

