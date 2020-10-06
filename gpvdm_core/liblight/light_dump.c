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



/** @file light_dump.c
	@brief Dumps optical fields from the light model.
*/

#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "gpvdm_const.h"
#include "dump_ctrl.h"
#include "light.h"
#include "dat_file.h"
#include <cal_path.h>
#include <lang.h>
#include <light_fun.h>
#include <dump.h>



void light_setup_dump_dir(struct simulation *sim,struct light *li)
{

	int l;
	FILE *out;
	char out_dir[PATH_MAX];
	struct dim_light *dim=&li->dim;

	join_path(2,out_dir,get_output_path(sim),"optical_output");
	//sprintf(out_dir,"%s/optical_output/",get_output_path(sim));

	gpvdm_mkdir(out_dir);

	out=fopena(out_dir,"wavelengths.dat","w");

	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%.0Lf\n",dim->l[l]*1e9);
	}

	fclose(out);

	strcpy(li->dump_dir,out_dir);
}

void light_dump(struct simulation *sim,struct light *li)
{
	struct dat_file buf;
	struct dim_light *dim=&li->dim;
	//struct epitaxy *epi=li->epi;


	buffer_init(&buf);

	buffer_malloc(&buf);
	dim_light_info_to_buf(&buf,dim);
	strcpy(buf.title,"Photon density");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Wavelength");
	get_wavelength_dim(buf.x_units,&buf.x_mul,dim->l[dim->llen-1]);
	strcpy(buf.data_label,"Density");
	strcpy(buf.data_units,"m^{-3}");
	buf.x=dim->llen;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	buffer_add_yl_light_data(sim,&buf,dim,li->photons, 0.0, 0 , 0);

	buffer_dump_path(sim,li->dump_dir,"light_2d_photons.dat",&buf);
	buffer_free(&buf);



	buffer_malloc(&buf);
	dim_light_info_to_buf(&buf,dim);
	strcpy(buf.title,"Absorbed Photon density");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Wavelength");
	get_wavelength_dim(buf.x_units,&buf.x_mul,dim->l[dim->llen-1]);
	strcpy(buf.data_label,"Density");
	strcpy(buf.data_units,"m^{-3}");
	buf.x=dim->llen;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	buffer_add_yl_light_data(sim,&buf,dim,li->photons_asb,0.0, 0 , 0);

	buffer_dump_path(sim,li->dump_dir,"light_2d_photons_asb.dat",&buf);
	buffer_free(&buf);

	if (get_dump_status(sim,dump_optics_verbose)==TRUE)
	{
		light_dump_verbose_2d(sim,li);
	}

}

void light_dump_sim_info(struct simulation *sim,struct light *li)
{
	light_dump_stats(sim,li);

	long double ret=0.0;
	ret=light_calculate_photons_absorbed_in_active_layer(li);
	FILE *out;
	out=fopena(get_output_path(sim),"sim_info.dat","w");
	fprintf(out,"#light_photons_in_active_layer\n%Le\n",ret);
	fprintf(out,"#end");
	fclose(out);

}

void light_dump_1d(struct simulation *sim,struct light *li, int l,char *ext)
{
//	printf("%d\n",li->dump_level);

if (li->dump_level==0)
{
	return;
}

char line[1024];
char out_name[200];
char temp_name[400];
char temp[1024];
int y;

char name[400];
double max=0.0;

struct dim_light *dim=&li->dim;
//struct epitaxy *epi=li->epi;

struct dat_file data_photons_norm;
struct dat_file data_1d_photons_tot;

buffer_init(&data_photons_norm);
buffer_init(&data_1d_photons_tot);

struct dat_file buf;

buffer_init(&buf);


if (get_dump_status(sim,dump_optics)==TRUE)
{

	if (l==0)
	{
		light_dump_shapshots(sim,li);

		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,_("Electron generation rate"));
		strcpy(buf.type,"zxy-d");
		strcpy(buf.data_label,_("Generation rate"));
		strcpy(buf.data_units,"m^{-3}");
		buf.x=dim->xlen;
		buf.y=dim->ylen;
		buf.z=dim->zlen;
		buffer_add_info(sim,&buf);
		dat_file_add_zxy_long_double_light_data(sim,&buf,li->Gn, dim);
		buffer_dump_path(sim,"","light_Gn.dat",&buf);
		buffer_free(&buf);

		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,_("Optical heating"));
		strcpy(buf.type,"zxy-d");
		strcpy(buf.data_label,_("Optical heating"));
		strcpy(buf.data_units,"W m^{-3}");
		buf.x=dim->xlen;
		buf.y=dim->ylen;
		buf.z=dim->zlen;
		buffer_add_info(sim,&buf);
		dat_file_add_zxy_long_double_light_data(sim,&buf,li->Htot, dim);
		buffer_dump_path(sim,"","light_Htot.dat",&buf);
		buffer_free(&buf);

		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,_("Photon density"));
		strcpy(buf.type,"zxy-d");
		strcpy(buf.data_label,_("Photon density"));
		strcpy(buf.data_units,"W m^{-3}");
		buf.x=dim->xlen;
		buf.y=dim->ylen;
		buf.z=dim->zlen;
		buffer_add_info(sim,&buf);
		dat_file_add_zxy_long_double_light_data(sim,&buf,li->photons_tot, dim);
		buffer_dump_path(sim,"","light_photons.dat",&buf);
		buffer_free(&buf);

		///////////


		max=inter_array_get_max(li->photons_tot[0][0],dim->ylen);

		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,"Normalized photon density");
		strcpy(buf.type,"xy");
		strcpy(buf.data_label,"Photon desntiy");
		strcpy(buf.data_units,"a.u.");
		buf.x=1;
		buf.y=dim->ylen;
		buf.z=1;
		buffer_add_info(sim,&buf);

		buffer_add_string(&buf,"#data\n");

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le\n",dim->y[y],li->photons_tot[0][0][y]/max);
			buffer_add_string(&buf,line);
		}

		buffer_add_string(&buf,"#end");

		sprintf(out_name,"light_1d_photons_tot_norm%s.dat",ext);
		buffer_dump_path(sim,li->dump_dir,out_name,&buf);
		buffer_free(&buf);

		printf("Write.....%s %s\n",li->dump_dir,out_name);

		buffer_malloc(&data_1d_photons_tot);
		sprintf(temp_name,"light_1d_photons_tot%s.dat",ext);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le\n",dim->y[y],li->photons_tot[0][0][y]);
			buffer_add_string(&data_1d_photons_tot,line);
		}

		buffer_dump_path(sim,li->dump_dir,temp_name,&data_1d_photons_tot);
		buffer_free(&data_1d_photons_tot);


		max=inter_array_get_max(li->Gn[0][0],dim->ylen);

		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,"Normalized photons absorbed");
		strcpy(buf.type,"xy");
		strcpy(buf.data_label,"Absorbed photons");
		strcpy(buf.data_units,"a.u.");
		buf.x=1;
		buf.y=dim->ylen;
		buf.z=1;
		buffer_add_info(sim,&buf);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le\n",dim->y[y],li->Gn[0][0][y]/max);
			buffer_add_string(&buf,line);
		}

		sprintf(out_name,"light_1d_photons_tot_abs_norm%s.dat",ext);
		buffer_dump_path(sim,li->dump_dir,out_name,&buf);
		buffer_free(&buf);


		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,_("Electron generation rate"));
		strcpy(buf.type,"xy");
		strcpy(buf.data_label,_("Generation rate"));
		strcpy(buf.data_units,"m^{-3}");
		buf.x=1;
		buf.y=dim->ylen;
		buf.z=1;
		buffer_add_info(sim,&buf);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le\n",dim->y[y],li->Gn[0][0][y]);
			buffer_add_string(&buf,line);
		}

		buffer_dump_path(sim,li->dump_dir,"light_1d_Gn.dat",&buf);
		buffer_free(&buf);


		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,_("Hole generation rate"));
		strcpy(buf.type,"xy");
		strcpy(buf.data_label,_("Generation rate"));
		strcpy(buf.data_units,"m^{-3}");
		buf.x=1;
		buf.y=dim->ylen;
		buf.z=1;
		buffer_add_info(sim,&buf);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le\n",dim->y[y],li->Gp[0][0][y]);
			buffer_add_string(&buf,line);
		}

		buffer_dump_path(sim,li->dump_dir,"light_1d_Gp.dat",&buf);
		buffer_free(&buf);




		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.y_mul=1e9;
		strcpy(buf.title,"Wavelength - Reflected light");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"Wavelength");
		strcpy(buf.data_label,"Reflected light");
		strcpy(buf.y_units,"nm");
		strcpy(buf.data_units,"a.u.");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=dim->llen;
		buf.z=1;
		buffer_add_info(sim,&buf);
		buffer_add_xy_data(sim,&buf,dim->l, li->reflect, dim->llen);
		buffer_dump_path(sim,get_output_path(sim),"reflect.dat",&buf);
		buffer_free(&buf);

		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.y_mul=1e9;
		strcpy(buf.title,"Wavelength - Transmitted light");
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,"Wavelength");
		strcpy(buf.data_label,"Transmitted light");
		strcpy(buf.y_units,"nm");
		strcpy(buf.data_units,"a.u.");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=dim->llen;
		buf.z=1;
		buffer_add_info(sim,&buf);
		buffer_add_xy_data(sim,&buf,dim->l, li->transmit, dim->llen);
		buffer_dump_path(sim,get_output_path(sim),"transmit.dat",&buf);
		buffer_free(&buf);

	}


	buffer_malloc(&buf);
	dim_light_info_to_buf(&buf,dim);
	strcpy(buf.title,"Position - Normalized photon density");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,"Normalized photon density");
	strcpy(buf.data_units,"a.u.");
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	char name_photons_norm[200];
	sprintf(name_photons_norm,"light_1d_%.0Lf_photons%s_norm.dat",dim->l[l]*1e9,ext);

	max=0.0;
	for (y=0;y<dim->ylen;y++)
	{
		if (li->photons[0][0][y][l]>max)
		{
			max=li->photons[0][0][y][l];
		}
	}

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y],li->photons[0][0][y][l]/max);
		buffer_add_string(&buf,line);
	}

	buffer_dump_path(sim,li->dump_dir,name_photons_norm,&buf);
	buffer_free(&buf);




	if (get_dump_status(sim,dump_optics_verbose)==TRUE)
	{
		light_dump_verbose_1d(sim,li,l,ext);

		buffer_malloc(&buf);
		dim_light_info_to_buf(&buf,dim);
		strcpy(buf.title,"Photons absorbed vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.data_label,_("Photons absorbed"));
		strcpy(buf.data_units,"m^{-3} m^{-1}");
		buf.x=1;
		buf.y=dim->ylen;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le\n",dim->y[y],li->photons_asb[0][0][y][l]);
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_photons_abs%s.dat",dim->l[l]*1e9,ext);
		buffer_dump_path(sim,li->dump_dir,name,&buf);
		buffer_free(&buf);



	}



}

}

