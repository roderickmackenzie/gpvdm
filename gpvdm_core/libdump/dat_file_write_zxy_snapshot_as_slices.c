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
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <probe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <inp.h>
#include <color.h>
#include <memory.h>
#include <device_fun.h>
#include <heat_fun.h>
#include <contacts.h>
#include <enabled_libs.h>
#include <circuit.h>

static int unused __attribute__((unused));

//This will generate a 3D snapshot for 3D data.
void dat_file_write_zxy_snapshot_as_slices(struct simulation *sim,char * path, struct dat_file *buf,struct dimensions *dim,long double ***data)
{
	int x;
	int y;
	int z;
	long double ***temp=NULL;
	struct snapshots snap;
	struct dat_file buf_out;
	struct dimensions dim_out;
	char shapshot_ret_path[PATH_MAX];
	char file_name_no_ext[PATH_MAX];
	char file_name_zx[100];
	char file_name_zy[100];
	int dim_sum;
	dat_file_init(&buf_out);
	int ret;

	dim_sum=0;

	if (dim->xlen>1)
	{
		dim_sum++;
	}

	if (dim->ylen>1)
	{
		dim_sum++;
	}

	if (dim->zlen>1)
	{
		dim_sum++;
	}
	
	if (dim_sum<=2)
	{
		dat_file_add_zxy_data(sim,buf,dim,data);
		buffer_dump_path(sim,path,NULL,buf);
	}else
	{
		snapshots_init(&snap);

		strcpy(file_name_no_ext,buf->file_name);
		remove_file_ext(file_name_no_ext);
		//join_path(2,shapshot_path,path,file_name_no_ext);

		strcpy(snap.type,"snapshots");
		strcpy(snap.plot_type,"2d3d");
		strcpy(snap.name,file_name_no_ext);
		strcpy(snap.path,path);

		ret = snprintf(file_name_zx,100,"%s_zx.csv",file_name_no_ext);
		if (ret<0)
		{
			ewe(sim,"error snprintf");
		}

		ret = snprintf(file_name_zy,100,"%s_zy.csv",file_name_no_ext);
		if (ret<0)
		{
			ewe(sim,"error snprintf");
		}

		//Dump zx slice
		dim_init(&dim_out);
		dim_cpy(&dim_out,dim);

		dim_free_xyz(&dim_out,'y');
		dim_out.ylen=1;
		dim_malloc_xyz(&dim_out,'y');

		malloc_zxy_long_double(&dim_out, &temp);

		for (y=0;y<dim->ylen;y++)
		{
			dump_make_snapshot_dir(sim, shapshot_ret_path, y,&snap);

			dim_out.y[0]=dim->y[y];
			dim_out.dY[0]=dim->dY[y];

			buffer_malloc(&buf_out);
			dim_info_to_buf(&buf_out,&dim_out);


			strcpy(buf_out.title,buf->title);
			strcpy(buf_out.data_label,buf->data_label);
			strcpy(buf_out.data_units,buf->data_units);

			for (z=0;z<dim->zlen;z++)
			{
				for (x=0;x<dim->xlen;x++)
				{
					temp[z][x][0]=data[z][x][y];
				}
			}
			//buf_out.bin=TRUE;
			dat_file_add_zxy_data(sim,&buf_out,&dim_out,temp);

			buffer_dump_path(sim,shapshot_ret_path,file_name_zx,&buf_out);
			buffer_free(&buf_out);


		}

		free_zxy_long_double(&dim_out, &temp);
		dim_free(&dim_out);

		//Dump zy slice
		dim_init(&dim_out);
		dim_cpy(&dim_out,dim);

		dim_free_xyz(&dim_out,'x');
		dim_out.xlen=1;
		dim_malloc_xyz(&dim_out,'x');

		malloc_zxy_long_double(&dim_out, &temp);

		for (x=0;x<dim->xlen;x++)
		{
			dump_make_snapshot_dir(sim, shapshot_ret_path, x,&snap);

			dim_out.x[0]=dim->x[x];
			dim_out.dX[0]=dim->dX[x];

			buffer_malloc(&buf_out);
			dim_info_to_buf(&buf_out,&dim_out);


			strcpy(buf_out.title,buf->title);
			strcpy(buf_out.data_label,buf->data_label);
			strcpy(buf_out.data_units,buf->data_units);

			for (z=0;z<dim->zlen;z++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					temp[z][0][y]=data[z][x][y];
				}
			}
			//buf_out.bin=TRUE;
			dat_file_add_zxy_data(sim,&buf_out,&dim_out,temp);

			buffer_dump_path(sim,shapshot_ret_path,file_name_zy,&buf_out);
			buffer_free(&buf_out);


		}

		free_zxy_long_double(&dim_out, &temp);
		dim_free(&dim_out);
	}


}
