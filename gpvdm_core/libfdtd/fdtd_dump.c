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

/** @file fdtd_dump.c
	@brief Dumps the fdtd fields.
*/

#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <lang.h>
#include <cal_path.h>
#include <dat_file.h>
#include <inp.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>
#include <dump.h>


#include "vec.h"

void fdtd_dump(struct simulation *sim,char *output_path,struct fdtd_data *data)
{
	int z=0;
	int x=0;
	int y=0;
	char temp[1000];
	struct dat_file buf;
	FILE *out;
	buffer_init(&buf);
	char out_dir[PATH_MAX];
	char lambda_name[20];
	char time_str[20];

	sprintf(lambda_name,"snapshots_%.0f",data->lambda*1e9);
	//strcpy(out_dir,get_output_path(sim));
	//dump_make_snapshot_dir_with_name(sim,out_dir ,(double)data->time, 0.0, (long double )(1.0/data->lambda),data->step,lambda_name);

	dump_make_snapshot_dir(sim,out_dir,output_path ,lambda_name, data->step,"2d");

	buffer_init(&buf);
	buffer_malloc(&buf);

	sprintf(temp,"{\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"fx\":%Le,\n",(long double )(1.0/data->lambda));
	buffer_add_string(&buf,temp);

	sprintf(temp,"}");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,out_dir,"data.json",&buf);
	buffer_free(&buf);

	//////Ex
	buffer_malloc(&buf);
	buf.z_mul=1e9;
	buf.x_mul=1e9;
	buf.y_mul=1e9;
	time_with_units(time_str,data->time);
	sprintf(buf.title,"%s (%s)",_("Ex - Electric field"),time_str);
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,_("X position"));
	strcpy(buf.y_label,_("Y position"));

	strcpy(buf.data_label,_("E-field"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("nm"));


	strcpy(buf.data_units,"V/m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=data->xlen;
	buf.y=data->ylen;
	buf.data_max=1.0;
	buf.data_min=-1.0;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{

			for (y=0;y<data->ylen;y++)
			{
				sprintf(temp,"%le %le %le\n",data->x_mesh[x],data->y_mesh[y],data->Ex[z][x][y]);
				buffer_add_string(&buf,temp);
			}

			buffer_add_string(&buf,"\n");

		}

	}

	buffer_dump_path(sim,output_path,"Ex.dat",&buf);
	buffer_dump_path(sim,out_dir,"Ex.dat",&buf);
	buffer_free(&buf);

	//////Ey
	buffer_malloc(&buf);
	buf.z_mul=1e9;
	buf.x_mul=1e9;
	buf.y_mul=1e9;
	time_with_units(time_str,data->time);
	sprintf(buf.title,"%s (%s)",_("Ey - Electric field"),time_str);
	strcpy(buf.type,"3d");
	strcpy(buf.x_label,_("X position"));
	strcpy(buf.y_label,_("Y position"));

	strcpy(buf.data_label,_("E-field"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("nm"));


	strcpy(buf.data_units,"V/m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=data->xlen;
	buf.y=data->ylen;
	buf.data_max=1.0;
	buf.data_min=-1.0;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{

			for (y=0;y<data->ylen;y++)
			{
				sprintf(temp,"%le %le %le\n",data->x_mesh[x],data->y_mesh[y],data->Ey[z][x][y]);
				buffer_add_string(&buf,temp);
			}

			buffer_add_string(&buf,"\n");

		}

	}

	buffer_dump_path(sim,output_path,"Ey.dat",&buf);
	buffer_dump_path(sim,out_dir,"Ey.dat",&buf);
	buffer_free(&buf);


	//////Ez
	buffer_malloc(&buf);
	buf.z_mul=1e9;
	buf.x_mul=1e9;
	buf.y_mul=1e9;
	time_with_units(time_str,data->time);
	sprintf(buf.title,"%s (%s)",_("Ez - Electric field"),time_str);
	strcpy(buf.type,"3d");
	strcpy(buf.x_label,_("X position"));
	strcpy(buf.y_label,_("Y position"));

	strcpy(buf.data_label,_("E-field"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("nm"));


	strcpy(buf.data_units,"V/m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=data->xlen;
	buf.y=data->ylen;
	buf.data_max=1.0;
	buf.data_min=-1.0;
	buf.z=1;
	buffer_add_info(sim,&buf);

	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{

			for (y=0;y<data->ylen;y++)
			{
				sprintf(temp,"%le %le %le\n",data->x_mesh[x],data->y_mesh[y],data->Ez[z][x][y]);
				buffer_add_string(&buf,temp);
			}

			buffer_add_string(&buf,"\n");

		}

	}

	buffer_dump_path(sim,output_path,"Ez.dat",&buf);
	buffer_dump_path(sim,out_dir,"Ez.dat",&buf);
	buffer_free(&buf);

	out=fopen("./epsilonr.dat","w");
	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{

			for (y=0;y<data->ylen;y++)
			{
				fprintf(out,"%le %le %le\n",data->x_mesh[x],data->y_mesh[y],data->epsilon_r[z][x][y]);
			}

			fprintf(out,"\n");
		}


	}

	fclose(out);

	out=fopen("./sigma.dat","w");
	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{

			for (y=0;y<data->ylen;y++)
			{
				fprintf(out,"%le %le %le\n",data->x_mesh[x],data->y_mesh[y],data->sigma[z][x][y]);
			}

			fprintf(out,"\n");
		}


	}

	fclose(out);

}
