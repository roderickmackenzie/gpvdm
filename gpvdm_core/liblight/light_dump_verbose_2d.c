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

/** @file light_dump_vervose_2d.c
	@brief Dumps the optical fields in 2D.
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

void light_dump_verbose_2d(struct simulation *sim,struct light *li)
{
	FILE *out;
	int x=0;
	int y=0;
	int z=0;
	int l=0;
	struct dat_file buf;
	char line[1024];
	char temp[1024];
	struct dim_light *dim=&li->dim;
	struct epitaxy *epi=li->epi;
	long double device_start =epi->device_start;
	buffer_init(&buf);

	out=fopena(li->dump_dir,"light_2d_Ep.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			fprintf(out,"%Le %Le %Le\n",dim->l[l],dim->y[y]-device_start,gpow(gpow(li->Ep[z][x][y][l],2.0)+gpow(li->Epz[z][x][y][l],2.0),0.5));

		}

	fprintf(out,"\n");
	}
	fclose(out);

	out=fopena(li->dump_dir,"light_2d_En.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			fprintf(out,"%Le %Le %Le\n",dim->l[l],dim->y[y]-device_start,gpow(gpow(li->En[z][x][y][l],2.0)+gpow(li->Enz[z][x][y][l],2.0),0.5));
		}

	fprintf(out,"\n");
	}
	fclose(out);

	out=fopena(li->dump_dir,"light_2d_E_mod.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			fprintf(out,"%Le %Le %Le\n",dim->l[l],dim->y[y]-device_start,gpow(gpow(li->Ep[z][x][y][l]+li->En[z][x][y][l],2.0)+gpow(li->Enz[z][x][y][l]+li->Epz[z][x][y][l],2.0),1.0));
		}

	fprintf(out,"\n");
	}
	fclose(out);



	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Refractive index (real) coefficient");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=dim->llen;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le %Le\n",dim->l[l],dim->y[y]-device_start,li->n[z][x][y][l]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,li->dump_dir,"2d_n.dat",&buf);
	buffer_free(&buf);


	out=fopena(li->dump_dir,"light_lambda_sun.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%Le %Le\n",dim->l[l],li->sun[l]);
	}
	fclose(out);

	out=fopena(li->dump_dir,"light_lambda_sun_norm.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%Le %Le\n",dim->l[l],li->sun_norm[l]);
	}
	fclose(out);

	out=fopena(li->dump_dir,"light_lambda_sun_photons.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%Le %Le\n",dim->l[l],li->sun_photons[l]);
	}
	fclose(out);

	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Optical absorption coefficient");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=dim->llen;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le %Le\n",dim->l[l],dim->y[y]-device_start,li->alpha[z][x][y][l]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,li->dump_dir,"2d_alpha.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Optical absorption coefficient");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=dim->llen;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(line,"%Le %Le %Le\n",dim->l[l],dim->y[y]-device_start,li->n[z][x][y][l]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,li->dump_dir,"light_lambda_n.dat",&buf);
	buffer_free(&buf);

	out=fopena(li->dump_dir,"light_sun_wavelength_E.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%Le %Le\n",dim->l[l],li->sun_E[l]);
	}
	fclose(out);


}
