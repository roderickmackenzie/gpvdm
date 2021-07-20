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



#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "epitaxy.h"
#include "lang.h"
#include "log.h"
#include <cal_path.h>
#include <light_fun.h>
#include <json.h>
#include <device_fun.h>

/** @file light_src.c
	@brief Loads the light configuration files.
*/

void light_src_dump(struct simulation *sim,char *path,struct light_src *in,char *file_name)
{
	struct dat_file buf;

	buffer_init(&buf);

	buffer_malloc(&buf);
	buf.data_mul=1.0;
	buf.y_mul=1e9;
	strcpy(buf.title,"Wavelength - Light intensity");
	strcpy(buf.type,"xy");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Light intensity");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"W m^{-2} m^{-1}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=in->spectra_tot.len;
	buf.z=1;
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,in->spectra_tot.x, in->spectra_tot.data, in->spectra_tot.len);
	buffer_dump_path(sim,path,file_name,&buf);
	buffer_free(&buf);
}

void light_src_cpy(struct simulation *sim,struct light_src *out, struct light_src *in)
{
	int i;
	out->nspectra=in->nspectra;
	if (in->spectra!=NULL)
	{
		out->spectra=malloc(sizeof(struct math_xy)*out->nspectra);
		for (i=0;i<in->nspectra;i++)
		{
			inter_copy(&(out->spectra[i]),&(in->spectra[i]),TRUE);
			out->light_multiplyer[i]=in->light_multiplyer[i];
		}
	}

	inter_copy(&(out->spectra_tot),&(in->spectra_tot),TRUE);

	//filter
	out->filter_enabled=in->filter_enabled;
	out->filter_dB=in->filter_dB;
	strcpy(out->filter_path,in->filter_path);
	inter_copy(&(out->filter_read),&(in->filter_read),TRUE);

}

void light_src_build_spectra_tot(struct simulation *sim,struct light_src *in, long double min, long double max, int len)
{
	int i;
	int l;
	//inter_dump(sim,&(in->spectra[i]));
	//printf("%Le %Le %d\n",min,max,len);
	//getchar();

	inter_init_mesh(&(in->spectra_tot),len,min,max);

	for (l=0;l<in->spectra_tot.len;l++)
	{
		in->spectra_tot.data[l]=0.0;
	}

	for (i=0;i<in->nspectra;i++)
	{
		for (l=0;l<in->spectra_tot.len;l++)
		{
			in->spectra_tot.data[l]+=inter_get_hard(&(in->spectra[i]),in->spectra_tot.x[l])*in->light_multiplyer[i];
		}
	}

	if (in->filter_enabled==TRUE)
	{
		if (in->filter_read.len>0)
		{
			for (l=0;l<in->spectra_tot.len;l++)
			{
				in->spectra_tot.data[l]*=inter_get_hard(&(in->filter_read),in->spectra_tot.x[l]);
			}
		}
	}

	long double Power=inter_intergrate(&(in->spectra_tot));
	printf_log(sim,"%s %Le Wm^{-2}\n",_("Power density of the optical spectra:"),Power);
}

void light_src_cal_min_max(struct simulation *sim,long double  *min,long double  *max,struct light_src *in)
{
	int i;
	for (i=0;i<in->nspectra;i++)
	{
		int left=0;
		int right=0;
		math_xy_get_left_right_start(&(in->spectra[i]),&left,&right, 0.02);

		if ((*min==-1.0)||(*min<in->spectra[i].x[left]))
		{
			*min=in->spectra[i].x[left];
		}

		if ((*max==-1.0)||(*max>in->spectra[i].x[right]))
		{
			*max=in->spectra[i].x[right];
		}
	}

}

void light_src_init(struct simulation *sim,struct light_src *in)
{
	in->nspectra=-1.0;
	in->spectra=NULL;
	inter_init(sim,&(in->spectra_tot));
	in->filter_enabled=FALSE;
	strcpy(in->filter_path,"");
	inter_init(sim,&(in->filter_read));
	in->filter_dB=-1.0;

}

void light_src_free(struct simulation *sim, struct light_src *in)
{
	int i;
	inter_free(&(in->spectra_tot));
	if (in->spectra!=NULL)
	{
		for (i=0;i<in->nspectra;i++)
		{
			inter_free(&(in->spectra[i]));
		}
		free(in->spectra);
		in->spectra=NULL;
	}

	in->nspectra=-1;
	strcpy(in->filter_path,"");
	inter_free(&(in->filter_read));
	in->filter_dB=-1.0;

}


void light_src_load(struct simulation *sim,struct light_src *in, struct json_obj *json_light_src)
{
	int x;
	char filter_path[PATH_MAX];
	struct json_obj *json_filter;
	int i;
	struct json_obj *obj;
	struct json_obj *json_light_spectra;
	int segments;
	char temp_str[100];
	char spectrum_name[200];
	char file_path[PATH_MAX];
	DIR *theFolder;

	theFolder = opendir(get_spectra_path(sim));
	if (theFolder==NULL)
	{
		ewe(sim,_("Optical spectra directory not found\n"));
	}
	closedir (theFolder);

	json_light_spectra=json_obj_find(json_light_src, "light_spectra");

	json_get_int(sim, json_light_spectra, &segments,"segments");
	if (segments>0)
	{
		in->spectra=malloc(sizeof(struct math_xy)*segments);
		in->nspectra=segments;

		for (i=0;i<segments;i++)
		{
			sprintf(temp_str,"segment%d",i);
			obj=json_obj_find(json_light_spectra, temp_str);
			if (obj==NULL)
			{
				ewe(sim,"Object %s not found\n",temp_str);
			}

			json_get_string(sim, obj, spectrum_name,"light_spectrum");
			json_get_double(sim, obj, &(in->light_multiplyer[i]),"light_multiplyer");

			join_path(3,file_path,get_spectra_path(sim),spectrum_name,"spectra.inp");

			if (isfile(file_path)!=0)
			{
				ewe(sim,"%s: %s\n",_("File not found"),file_path);
			}
			inter_init(sim,&(in->spectra[i]));
			inter_load(sim,&(in->spectra[i]),file_path);
			inter_sort(&(in->spectra[i]));

			inter_mod(&(in->spectra[i]));
			//printf("%d %Le %Le\n",i,in->spectra_tot.x[l],in->spectra_tot.data[l]);
			//getchar();

		}


	}

	//Load the filter

	json_filter=json_obj_find(json_light_src, "light_filter");
	if (json_filter==NULL)
	{
		ewe(sim,"json_filter not found\n");
	}

	json_get_english(sim, json_filter, &(in->filter_enabled),"filter_enabled");

	if (in->filter_enabled==TRUE)
	{
		json_get_string(sim, json_filter, filter_path,"filter_material");
		json_get_long_double(sim, json_filter, &(in->filter_dB),"filter_db");

		join_path(3,in->filter_path,get_materials_path(sim),filter_path,"alpha.gmat");

		if (isfile(in->filter_path)==0)
		{
			printf_log(sim,"%s %s\n",_("Light: Load filter"),in->filter_path);
			inter_init(sim,&(in->filter_read));
			inter_load(sim,&(in->filter_read),in->filter_path);
			inter_sort(&(in->filter_read));
			inter_mod(&(in->filter_read));
			inter_norm(&(in->filter_read),1.0);

			for (x=0;x<in->filter_read.len;x++)
			{
				in->filter_read.data[x]=powl(1.0-in->filter_read.data[x],10*in->filter_dB);
				//printf("%Le\n",in->filter_read.data[x]);
			}
			//getchar();
		}
	}



}

