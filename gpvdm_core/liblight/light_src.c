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

void light_src_dump(struct simulation *sim,char *path,struct light_src *in)
{
	struct dat_file buf;
	char file_name[200];
	sprintf(file_name,"light_src_%s.csv",in->id);
	dat_file_init(&buf);

	if (buffer_set_file_name(sim,NULL,&buf,file_name)==0)
	{
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
		dat_file_add_xy_data(sim,&buf,in->spectra_tot.x, in->spectra_tot.data, in->spectra_tot.len);
		buffer_dump_path(sim,path,NULL,&buf);
		buffer_free(&buf);
	}

	sprintf(file_name,"light_src_no_filter%s.csv",in->id);
	dat_file_init(&buf);

	if (buffer_set_file_name(sim,NULL,&buf,file_name)==0)
	{
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
		buf.y=in->spectra_tot_no_filter.len;
		buf.z=1;
		dat_file_add_xy_data(sim,&buf,in->spectra_tot_no_filter.x, in->spectra_tot_no_filter.data, in->spectra_tot_no_filter.len);
		buffer_dump_path(sim,path,NULL,&buf);
		buffer_free(&buf);
	}

	sprintf(file_name,"light_src_filter_%s.csv",in->id);
	dat_file_init(&buf);

	if (buffer_set_file_name(sim,NULL,&buf,file_name)==0)
	{
		buffer_malloc(&buf);
		buf.data_mul=1.0;
		buf.y_mul=1e9;
		strcpy(buf.title,"Wavelength - Attenuation");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"Wavelength");
		strcpy(buf.data_label,"Attenuation");
		strcpy(buf.y_units,"nm");
		strcpy(buf.data_units,"au");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->filter_read.len;
		buf.z=1;
		dat_file_add_xy_data(sim,&buf,in->filter_read.x, in->filter_read.data, in->filter_read.len);
		buffer_dump_path(sim,path,NULL,&buf);
		buffer_free(&buf);
	}

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
	inter_copy(&(out->spectra_tot_no_filter),&(in->spectra_tot_no_filter),TRUE);

	//filter
	out->filter_enabled=in->filter_enabled;
	out->filter_dB=in->filter_dB;
	out->filter_invert=in->filter_invert;

	out->local_ground_view_factor=in->local_ground_view_factor;
	strcpy(out->filter_path,in->filter_path);
	inter_copy(&(out->filter_read),&(in->filter_read),TRUE);

	//external interface
	out->n=in->n;
	out->external_interface_enabled=in->external_interface_enabled;
	strcpy(out->id,in->id);

	//config
	strcpy(out->illuminate_from,in->illuminate_from);
	out->x0=in->x0;
	out->y0=in->y0;
	out->z0=in->z0;

	out->theta_steps=in->theta_steps;
	out->theta_start=in->theta_start;
	out->theta_stop=in->theta_stop;

	out->phi_steps=in->phi_steps;
	out->phi_start=in->phi_start;
	out->phi_stop=in->phi_stop;

	out->use_flat_sepctrum=in->use_flat_sepctrum;			
}

void light_src_build_spectra_tot(struct simulation *sim,struct light_src *in, long double min, long double max, int len)
{
	int i;
	int l;
	long double Power=0.0;
	long double Power_no_filter=0.0;

	inter_init_mesh(&(in->spectra_tot),len,min,max);

	for (l=0;l<in->spectra_tot.len;l++)
	{
		in->spectra_tot.data[l]=0.0;
	}

	for (i=0;i<in->nspectra;i++)
	{
		for (l=0;l<in->spectra_tot.len;l++)
		{
			in->spectra_tot.data[l]+=inter_get_hard(&(in->spectra[i]),in->spectra_tot.x[l])*in->light_multiplyer[i]*in->local_ground_view_factor;
		}
	}
	//FILE * out;
	inter_copy(&(in->spectra_tot_no_filter),&(in->spectra_tot),TRUE);

	if (in->filter_enabled==TRUE)
	{
		//out=fopen("filter2.dat","w");
		if (in->filter_read.len>0)
		{
			for (l=0;l<in->spectra_tot.len;l++)
			{
				//fprintf(out,"%Le %Le %Le %Le\n",in->spectra_tot.x[l],inter_get_hard(&(in->filter_read),in->spectra_tot.x[l]),in->spectra_tot.data[l],in->spectra_tot.data[l]*inter_get_hard(&(in->filter_read),in->spectra_tot.x[l]));
				in->spectra_tot.data[l]*=inter_get_hard(&(in->filter_read),in->spectra_tot.x[l]);
			}
		}
		//fclose(out);
	}

	Power=inter_intergrate(&(in->spectra_tot));
	Power_no_filter=inter_intergrate(&(in->spectra_tot_no_filter));
	printf_log(sim,"%s %Le (%Le) Wm^{-2}\n",_("Power density of the optical spectra (no filter):"),Power,Power_no_filter);
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
	inter_init(sim,&(in->spectra_tot_no_filter));
	in->filter_enabled=FALSE;
	strcpy(in->filter_path,"");
	inter_init(sim,&(in->filter_read));
	in->filter_dB=-1.0;
	in->filter_invert=-1.0;
	in->local_ground_view_factor=-1.0;
	in->n=-1.0;
	in->external_interface_enabled=FALSE;
	strcpy(in->id,"");

	strcpy(in->illuminate_from,"");
	in->x0=-1.0;
	in->y0=-1.0;
	in->z0=-1.0;

	in->theta_steps=-1;
	in->theta_start=-1.0;
	in->theta_stop=-1.0;

	in->phi_steps=-1;
	in->phi_start=-1.0;
	in->phi_stop=-1.0;

	in->use_flat_sepctrum=FALSE;			
}

void light_src_free(struct simulation *sim, struct light_src *in)
{
	int i;
	inter_free(&(in->spectra_tot));
	inter_free(&(in->spectra_tot_no_filter));
	if (in->spectra!=NULL)
	{
		for (i=0;i<in->nspectra;i++)
		{
			inter_free(&(in->spectra[i]));
		}
		free(in->spectra);
		in->spectra=NULL;
	}

	inter_free(&(in->filter_read));

	light_src_init(sim,in);

}


void light_src_load(struct simulation *sim,struct light_src *in, struct json_obj *json_light_src)
{
	int x;
	int l;
	char filter_path[PATH_MAX];
	struct json_obj *json_filters;
	struct json_obj *json_external_interface;
	int i;
	struct json_obj *obj;
	struct json_obj *json_light_spectra;
	struct json_obj *virtual_spectra;

	struct math_xy filter;

	inter_init(sim,&filter);

	int segments;
	char temp_str[100];
	char spectrum_name[200];
	char file_path[PATH_MAX];
	int filter_invert;
	long double val;
	long double W_max;
	long double photon_den_max;
	long double E;
	long double lam;

	json_get_string(sim, json_light_src, in->id,"id");

	json_get_long_double(sim, json_light_src, &(in->lstart),"lstart");
	json_get_long_double(sim, json_light_src, &(in->lstop),"lstop");
	json_get_string(sim, json_light_src, in->illuminate_from,"light_illuminate_from");

	json_get_long_double(sim, json_light_src, &(in->x0),"x0");
	json_get_long_double(sim, json_light_src, &(in->y0),"y0");
	json_get_long_double(sim, json_light_src, &(in->z0),"z0");


	json_get_int(sim, json_light_src, &(in->theta_steps),"ray_theta_steps");
	json_get_long_double(sim, json_light_src, &(in->theta_start),"ray_theta_start");
	json_get_long_double(sim, json_light_src, &(in->theta_stop),"ray_theta_stop");

	json_get_int(sim, json_light_src, &(in->phi_steps),"ray_phi_steps");
	json_get_long_double(sim, json_light_src, &(in->phi_start),"ray_phi_start");
	json_get_long_double(sim, json_light_src, &(in->phi_stop),"ray_phi_stop");
	json_get_long_double(sim, json_light_src, &(in->local_ground_view_factor),"filter_local_ground_view_factor");
	in->local_ground_view_factor=(1.0-cos(2.0*M_PI*in->local_ground_view_factor/360.0))/2.0;

	virtual_spectra=json_obj_find(json_light_src, "virtual_spectra");

	json_light_spectra=json_obj_find(virtual_spectra, "light_spectra");


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

			if (in->use_flat_sepctrum==TRUE)
			{
				
				math_xy_get_max_and_pos(&(in->spectra[i]),&W_max, &lam);
				E=hp*cl/lam;
				photon_den_max=W_max/E;

				for (l=0;l<in->spectra[i].len;l++)
				{
					E=hp*cl/in->spectra[i].x[l];
					in->spectra[i].data[l]=photon_den_max*E;
				}

			}
			//printf("%d %Le %Le\n",i,in->spectra_tot.x[l],in->spectra_tot.data[l]);
			//getchar();

		}


	}

	//Load the filter

	json_filters=json_obj_find(virtual_spectra, "light_filters");
	if (json_filters==NULL)
	{
		ewe(sim,"json_filters not found\n");
	}

	json_get_int(sim, json_filters, &segments,"segments");

	//If there is a spectra defined then use its wavelength range as the base of the filter
	if (in->nspectra>0)
	{
		inter_copy(&in->filter_read,&(in->spectra[0]),TRUE);
		math_xy_set_value(&in->filter_read,1.0);
	}

	int filter_enabled=FALSE;
	in->filter_enabled=FALSE;
	int first_load=TRUE;
	FILE *out;
	out=fopen("filter_test.dat","w");
	for (i=0;i<segments;i++)
	{
		sprintf(temp_str,"segment%d",i);
		obj=json_obj_find(json_filters, temp_str);
		if (obj==NULL)
		{
			ewe(sim,"Object %s not found\n",temp_str);
		}

		json_get_english(sim, obj, &(filter_enabled),"filter_enabled");


		if (filter_enabled==TRUE)
		{
			in->filter_enabled=TRUE;
			json_get_string(sim, obj, filter_path,"filter_material");
			json_get_long_double(sim, obj, &(in->filter_dB),"filter_db");
			json_get_english(sim, obj, &filter_invert,"filter_invert");

			join_path(3,in->filter_path,get_filter_path(sim),filter_path,"filter.inp");


			if (isfile(in->filter_path)==0)
			{
				printf_log(sim,"%s %s\n",_("Light: Load filter"),in->filter_path);
				inter_load(sim,&filter,in->filter_path);
				inter_sort(&filter);
				inter_mod(&filter);
				inter_norm(&filter,1.0);
				if (first_load==TRUE)
				{
					if (in->nspectra==0)
					{
						inter_copy(&in->filter_read,&filter,TRUE);
						math_xy_set_value(&in->filter_read,1.0);
					}
					first_load=FALSE;
				}

				for (x=0;x<in->filter_read.len;x++)
				{
					val=inter_get_hard(&filter,in->filter_read.x[x]);

					if (filter_invert==TRUE)
					{
						val=1.0-val;
					}
					//printf("%Le %Le %Le\n",val,powl(1.0-val,10*in->filter_dB),in->filter_dB);
					in->filter_read.data[x]*=powl((1.0-val),10.0*in->filter_dB);

					fprintf(out,"%Le %Le\n",in->filter_read.x[x],in->filter_read.data[x]);
				}

				inter_free(&filter);
				//getchar();
			}
		}

	}

	fclose(out);
	//Reflection of external interface

	json_external_interface=json_obj_find(virtual_spectra, "external_interface");
	if (json_external_interface==NULL)
	{
		ewe(sim,"json_external_interface not found\n");
	}

	json_get_english(sim, json_external_interface, &(in->external_interface_enabled),"enabled");
	json_get_long_double(sim, json_external_interface, &(in->n),"light_external_n");

}

