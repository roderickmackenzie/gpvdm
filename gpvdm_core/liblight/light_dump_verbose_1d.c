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

/** @file ligh_dump_verbose_1d.c
	@brief Dumps 1D optical fields from light model.
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

void light_dump_verbose_1d(struct simulation *sim,struct light *li, int l,char *ext)
{
	return;
	char line[1024];
	char temp[1024];
	char name_photons[200];
	char name_light_1d_Ep[200];
	char name_light_1d_En[200];
	char name_pointing[200];
	char name_E_tot[200];
	char name_r[200];
	char name_t[200];
	char name[200];

	int x=0;
	int z=0;
	int y=0;

	struct epitaxy *epi=li->epi;
	long double device_start=epi->device_start;

	//int max=0;
	struct dat_file data_photons;
	struct dat_file data_light_1d_Ep;
	struct dat_file data_light_1d_En;
	//struct dat_file data_pointing;
	//struct dat_file data_E_tot;

	struct dat_file data_r;
	struct dat_file data_t;

	struct dat_file buf;
	struct dimensions *dim=&li->dim;

	dat_file_init(&data_light_1d_Ep);
	dat_file_init(&data_light_1d_En);
	//dat_file_init(&data_pointing);
	//dat_file_init(&data_E_tot);
	dat_file_init(&buf);

	dat_file_init(&data_r);
	dat_file_init(&data_t);
	dat_file_init(&data_photons);

	buffer_malloc(&data_photons);
	buffer_malloc(&data_light_1d_Ep);
	buffer_malloc(&data_light_1d_En);
	//buffer_malloc(&data_pointing);
	//buffer_malloc(&data_E_tot);
	buffer_malloc(&data_r);
	buffer_malloc(&data_t);

	sprintf(name_photons,"light_1d_%.0Lf_photons%s.dat",dim->l[l]*1e9,ext);

	sprintf(name_light_1d_Ep,"light_1d_%.0Lf_Ep%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_light_1d_En,"light_1d_%.0Lf_En%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_pointing,"light_1d_%.0Lf_pointing%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_E_tot,"light_1d_%.0Lf_E_tot%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_r,"light_1d_%.0Lf_r%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_t,"light_1d_%.0Lf_t%s.dat",dim->l[l]*1e9,ext);

	//
	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %e\n",dim->y[y]-device_start,li->photons[z][x][y][l]);
		buffer_add_string(&data_photons,line);

		sprintf(line,"%Le %e %e %e\n",dim->y[y]-device_start,pow(pow(li->Ep[z][x][y][l],2.0)+pow(li->Epz[z][x][y][l],2.0),0.5),li->Ep[z][x][y][l],li->Epz[z][x][y][l]);
		buffer_add_string(&data_light_1d_Ep,line);

		sprintf(line,"%Le %e %e %e\n",dim->y[y]-device_start,pow(pow(li->En[z][x][y][l],2.0)+pow(li->Enz[z][x][y][l],2.0),0.5),li->En[z][x][y][l],li->Enz[z][x][y][l]);
		buffer_add_string(&data_light_1d_En,line);

		//sprintf(line,"%Le %e\n",dim->y[y]-device_start,li->pointing_vector[z][x][y][l]);
		//buffer_add_string(&data_pointing,line);

		//sprintf(line,"%Le %e %e\n",dim->y[y]-device_start,li->E_tot_r[z][x][y][l],li->E_tot_i[z][x][y][l]);
		//buffer_add_string(&data_E_tot,line);

		sprintf(line,"%Le %Le %Le %Le\n",dim->y[y]-device_start,gcabs(li->r[z][x][y][l]),gcreal(li->r[z][x][y][l]),gcimag(li->r[z][x][y][l]));
		buffer_add_string(&data_r,line);

		sprintf(line,"%Le %Le %Le %Le\n",dim->y[y]-device_start,gcabs(li->t[z][x][y][l]),gcreal(li->t[z][x][y][l]),gcimag(li->t[z][x][y][l]));
		buffer_add_string(&data_t,line);

	}

	buffer_dump_path(sim,li->dump_dir,name_photons,&data_photons);

	buffer_dump_path(sim,li->dump_dir,name_light_1d_Ep,&data_light_1d_Ep);
	buffer_dump_path(sim,li->dump_dir,name_light_1d_En,&data_light_1d_En);
	//buffer_dump_path(sim,li->dump_dir,name_pointing,&data_pointing);
	//buffer_dump_path(sim,li->dump_dir,name_E_tot,&data_E_tot);
	buffer_dump_path(sim,li->dump_dir,name_r,&data_r);
	buffer_dump_path(sim,li->dump_dir,name_t,&data_t);



	buffer_free(&data_photons);

	buffer_free(&data_light_1d_Ep);
	buffer_free(&data_light_1d_En);
	//buffer_free(&data_pointing);
	//buffer_free(&data_E_tot);
	buffer_free(&data_r);
	buffer_free(&data_t);

	buffer_malloc(&buf);
	dim_info_to_buf(&buf,dim);
	strcpy(buf.title,"Real refractive index (n) vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,_("Real refractive index"));
	strcpy(buf.data_units,"au");
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %e\n",dim->y[y],li->n[0][0][y][l]);
		buffer_add_string(&buf,line);
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(name,"light_1d_%.0Lf_n%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,name,&buf);
	buffer_free(&buf);




	buffer_malloc(&buf);
	dim_info_to_buf(&buf,dim);
	strcpy(buf.title,"Absorption vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,_("Absorption"));
	strcpy(buf.data_units,"m^{-1}");
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %e\n",dim->y[y],li->alpha[0][0][y][l]);
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(name,"light_1d_%.0Lf_alpha%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"|Electric field| vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("|Electric field|"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"V/m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,gpow(gpow(li->Ep[z][x][y][l]+li->En[z][x][y][l],2.0)+gpow(li->Enz[z][x][y][l]+li->Epz[z][x][y][l],2.0),0.5));
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"light_1d_%.0Lf_E%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,temp,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Transmittance vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("Transmittance"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"au");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,gcabs(li->t[z][x][y][l]));
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"light_1d_%.0Lf_t%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,temp,&buf);
	buffer_free(&buf);



	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Reflectance vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("Reflectance"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"au");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,gcabs(li->r[z][x][y][l]));
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"light_1d_%.0Lf_r%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,temp,&buf);
	buffer_free(&buf);

}
