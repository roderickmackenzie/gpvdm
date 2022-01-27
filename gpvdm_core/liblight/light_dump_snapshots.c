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
#include <color.h>


void light_dump_snapshots(struct simulation *sim,char *output_path,struct light *li)
{
	if (li->dump_verbosity<=0)
	{
		return;
	}

	int l;
	char out_dir[PATH_MAX];
	struct dimensions *dim=&li->dim;
	char temp[200];
	char line[400];
	char snapshots_path[PATH_MAX];
	struct dat_file buf;
	dat_file_init(&buf);

	struct dat_file info_file;
	dat_file_init(&info_file);

	struct dat_file wavelengths_buffer;
	dat_file_init(&wavelengths_buffer);
	buffer_malloc(&wavelengths_buffer);

	int dump_number=0;
	int tot_dump_number=0;
	int r;
	int g;
	int b;
	int y;
	double max=0.0;
	int made_dir=FALSE;

	for (l=0;l<dim->llen;l++)
	{

		if (dump_number>=li->dump_verbosity)
		{
			struct snapshots snap;
			snapshots_init(&snap);
			strcpy(snap.type,"snapshots");
			strcpy(snap.icon,"optics2");
			strcpy(snap.plot_type,"2d");
			strcpy(snap.name,"optical_snapshots");
			strcpy(snap.path,output_path);

			dump_make_snapshot_dir(sim,out_dir, tot_dump_number,&snap);
			made_dir=TRUE;
			buffer_malloc(&info_file);

			sprintf(temp,"{\n");
			buffer_add_string(&info_file,temp);

			sprintf(temp,"\t\"wavelength\":%Le\n",(long double )dim->l[l]);
			buffer_add_string(&info_file,temp);

			sprintf(temp,"}");
			buffer_add_string(&info_file,temp);


			//printf("intern: %s\n",info_file.buf);
			buffer_dump_path(sim,out_dir,"data.json",&info_file);
			buffer_free(&info_file);

			wavelength_to_rgb(&r,&g,&b,dim->l[l]);

			if (buffer_set_file_name(sim,NULL,&buf,"light_Gn.csv")==0)
			{
				buffer_malloc(&buf);
				dim_info_to_buf(&buf,dim);
				sprintf(buf.title,"%s %.2Lf nm",_("Absorbed photons at"),dim->l[l]*1e9);
				strcpy(buf.type,"zxy-d");
				strcpy(buf.data_label,_("Generation rate"));
				strcpy(buf.data_units,"m^{-3}");
				sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
				dat_file_add_zxy_double_l(sim,&buf, dim,li->photons_asb,l);
				buffer_dump_path(sim,out_dir,NULL,&buf);
				buffer_free(&buf);
			}

			if (buffer_set_file_name(sim,NULL,&buf,"n.csv")==0)
			{
				buffer_malloc(&buf);
				dim_info_to_buf(&buf,dim);
				strcpy(buf.title,_("Refractive index"));
				strcpy(buf.type,"zxy-d");
				strcpy(buf.data_label,_("Refractive index"));
				strcpy(buf.data_units,"au");
				sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
				dat_file_add_zxy_float_l(sim,&buf, dim,li->n,l);
				buffer_dump_path(sim,out_dir,NULL,&buf);
				buffer_free(&buf);
			}


			if (buffer_set_file_name(sim,NULL,&buf,"alpha.csv")==0)
			{
				buffer_malloc(&buf);
				dim_info_to_buf(&buf,dim);
				strcpy(buf.title,_("Absorption"));
				strcpy(buf.type,"zxy-d");
				strcpy(buf.data_label,_("Absorption"));
				strcpy(buf.data_units,"m^{-1}");
				sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
				dat_file_add_zxy_float_l(sim,&buf, dim,li->alpha,l);
				buffer_dump_path(sim,out_dir,NULL,&buf);
				buffer_free(&buf);
			}

			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			strcpy(buf.title,"Photons absorbed vs position");
			strcpy(buf.type,"xy");
			strcpy(buf.data_label,_("Photons absorbed"));
			strcpy(buf.data_units,"m^{-3} m^{-1}");
			buf.x=1;
			buf.y=dim->ylen;
			buf.z=1;
			buffer_add_info(sim,&buf);
			sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);

			sprintf(temp,"#data\n");
			buffer_add_string(&buf,temp);

			for (y=0;y<dim->ylen;y++)
			{
				sprintf(line,"%Le %le\n",dim->y[y],li->photons_asb[0][0][y][l]);
				buffer_add_string(&buf,line);
			}


			sprintf(temp,"#end\n");
			buffer_add_string(&buf,temp);

			buffer_dump_path(sim,out_dir,"light_photons_abs.dat",&buf);
			//dat_file_reset(&buf);
			buffer_free(&buf);


			buffer_malloc(&buf);
			dim_info_to_buf(&buf,dim);
			strcpy(buf.title,"Position - Normalized photon density");
			strcpy(buf.type,"xy");
			strcpy(buf.data_label,"Normalized photon density");
			strcpy(buf.data_units,"a.u.");
			buf.x=1;
			buf.y=dim->ylen;
			buf.z=1;
			buffer_add_info(sim,&buf);
			sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);

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
				sprintf(line,"%Le %le\n",dim->y[y],li->photons[0][0][y][l]/max);
				buffer_add_string(&buf,line);
			}

			buffer_dump_path(sim,out_dir,"light_photons_norm.dat",&buf);
			buffer_free(&buf);
			//dat_file_reset(&buf);

			sprintf(temp,"%.0Lf\n",dim->l[l]*1e9);
			buffer_add_string(&wavelengths_buffer,temp);

			tot_dump_number++;
		}

		dump_number++;
		if (dump_number>li->dump_verbosity)
		{
			dump_number=0;
		}
	}






	if (made_dir==TRUE)
	{
		join_path(2,snapshots_path,output_path,"optical_snapshots");
		buffer_dump_path(sim,snapshots_path,"wavelengths.dat",&wavelengths_buffer);
	}
	buffer_free(&wavelengths_buffer);

}

