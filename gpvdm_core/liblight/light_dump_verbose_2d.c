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
	struct dimensions *dim=&li->dim;
	struct epitaxy *epi=li->epi;
	long double device_start =epi->device_start;
	dat_file_init(&buf);

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
			sprintf(line,"%Le %Le %e\n",dim->l[l],dim->y[y]-device_start,li->n[z][x][y][l]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,li->dump_dir,"2d_n.dat",&buf);
	buffer_free(&buf);



	/*out=fopena(li->dump_dir,"light_lambda_sun_photons.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%Le %Le\n",dim->l[l],li->sun_photons_y0[l]);
	}
	fclose(out);*/

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
			sprintf(line,"%Le %Le %e\n",dim->l[l],dim->y[y]-device_start,li->alpha[z][x][y][l]);
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
			sprintf(line,"%Le %Le %e\n",dim->l[l],dim->y[y]-device_start,li->n[z][x][y][l]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,li->dump_dir,"light_lambda_n.dat",&buf);
	buffer_free(&buf);

	/*out=fopena(li->dump_dir,"light_sun_wavelength_E.dat","w");
	for (l=0;l<dim->llen;l++)
	{
		fprintf(out,"%Le %Le\n",dim->l[l],li->sun_E[l]);
	}
	fclose(out);*/


}
