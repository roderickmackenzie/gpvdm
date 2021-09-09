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
#include <color.h>


void light_dump_snapshots(struct simulation *sim,char *output_path,struct light *li)
{
	if (li->dump_verbosity<=0)
	{
		return;
	}

	int l;
	char out_dir[PATH_MAX];
	struct dim_light *dim=&li->dim;
	char temp[200];
	char line[400];
	char snapshots_path[PATH_MAX];
	struct dat_file buf;
	buffer_init(&buf);

	struct dat_file info_file;
	buffer_init(&info_file);

	struct dat_file wavelengths_buffer;
	buffer_init(&wavelengths_buffer);
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
			dump_make_snapshot_dir(sim,out_dir,output_path ,"optical_snapshots", tot_dump_number,"2d");
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

			buffer_malloc(&buf);
			dim_light_info_to_buf(&buf,dim);
			sprintf(buf.title,"%s %.2Lf nm",_("Absorbed photons at"),dim->l[l]*1e9);
			strcpy(buf.type,"zxy-d");
			strcpy(buf.data_label,_("Generation rate"));
			strcpy(buf.data_units,"m^{-3}");
			buf.x=dim->xlen;
			buf.y=dim->ylen;
			buf.z=dim->zlen;
			buffer_add_info(sim,&buf);
			sprintf(buf.rgb,"%.2x%.2x%.2x",r,g,b);
			dat_file_add_zxy_from_zxyl_double_light_data(sim,&buf,li->photons_asb, dim,l);
			dat_file_add_zxy_long_double_light_data(sim,&buf,li->Gn, dim);
			buffer_dump_path(sim,out_dir,"light_Gn.dat",&buf);
			buffer_free(&buf);

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
			dim_light_info_to_buf(&buf,dim);
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

