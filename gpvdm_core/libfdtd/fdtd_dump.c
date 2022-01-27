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
#include <color.h>

#include "vec.h"

void fdtd_dump(struct simulation *sim,char *output_path,struct device *dev, struct fdtd_data *data)
{
	long double data_max=1.0;
	char temp[1000];
	struct dat_file buf;
	dat_file_init(&buf);
	char out_dir[PATH_MAX];
	char time_str[20];
	struct dimensions *dim=&(data->dim);
	int r;
	int g;
	int b;


	wavelength_to_rgb(&r,&g,&b,data->lambda);
	struct snapshots snap;
	snapshots_init(&snap);
	strcpy(snap.icon,"fdtd_output");
	strcpy(snap.type,"snapshots");
	strcpy(snap.name,"snapshots");
	strcpy(snap.plot_type,"3d");
	strcpy(snap.path,output_path);

	dump_make_snapshot_dir(sim,out_dir,  data->step,&snap);

	dat_file_init(&buf);
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
	if (buffer_set_file_name(sim,dev,&buf,"Ex.csv")==0)
	{
		buffer_malloc(&buf);
		buf.bin=TRUE;
		time_with_units(time_str,data->time);
		sprintf(buf.title,"%s (%s)",_("Ex - Electric field"),time_str);
		sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
		dim_info_to_buf(&buf,dim);
		strcpy(buf.data_label,_("E-field"));
		strcpy(buf.data_units,"V/m");
		buf.data_max=data_max;
		buf.data_min=-data_max;
		dat_file_add_zxy_float(sim,&buf,dim,data->Ex);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	//////Ey
	if (buffer_set_file_name(sim,dev,&buf,"Ey.csv")==0)
	{
		buffer_malloc(&buf);
		buf.bin=TRUE;
		time_with_units(time_str,data->time);
		sprintf(buf.title,"%s (%s)",_("Ey - Electric field"),time_str);
		sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
		dim_info_to_buf(&buf,dim);
		strcpy(buf.data_label,_("E-field"));
		strcpy(buf.data_units,"V/m");
		//printf("%d %d %d\n",buf.x,buf.y,buf.z);
		buf.data_max=1.0;
		buf.data_min=-1.0;
		dat_file_add_zxy_float(sim,&buf,dim,data->Ey);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	//////Ez
	if (buffer_set_file_name(sim,dev,&buf,"Ez.csv")==0)
	{
		buffer_malloc(&buf);
		buf.bin=TRUE;
		time_with_units(time_str,data->time);
		sprintf(buf.title,"%s (%s)",_("Ez - Electric field"),time_str);
		sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
		dim_info_to_buf(&buf,dim);
		strcpy(buf.data_label,_("E-field"));
		strcpy(buf.data_units,"V/m");
		buf.data_max=data_max;
		buf.data_min=-data_max;
		dat_file_add_zxy_float(sim,&buf,dim,data->Ez);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	//epsilonr
	/*if (buffer_set_file_name(sim,dev,&buf,"epsilon_r.csv")==0)
	{
		buffer_malloc(&buf);
		time_with_units(time_str,data->time);
		sprintf(buf.title,"%s (%s)",_("epsilon_r - Premitivity"),time_str);
		dim_info_to_buf(&buf,dim);
		strcpy(buf.data_label,_("Permitivity"));
		strcpy(buf.data_units,"au");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.data_max=data_max;
		buf.data_min=-data_max;
		dat_file_add_zxy_float(sim,&buf,dim,data->epsilon_r);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}
	//sigma
	//epsilonr
	if (buffer_set_file_name(sim,dev,&buf,"sigma.dat")==0)
	{
		buffer_malloc(&buf);
		time_with_units(time_str,data->time);
		sprintf(buf.title,"%s (%s)",_("sigma - ?"),time_str);
		dim_info_to_buf(&buf,dim);
		strcpy(buf.data_label,_("sigma"));
		strcpy(buf.data_units,"?");
		buf.data_max=data_max;
		buf.data_min=-data_max;
		dat_file_add_zxy_float(sim,&buf,dim,data->sigma);
		buffer_dump_path(sim,out_dir,"sigma.csv",&buf);
		buffer_free(&buf);
	}*/

}
