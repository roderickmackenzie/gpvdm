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

/** @file dump.c
@brief go and dump stuff, what is dumped depends on which options have been set
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <pl.h>
#include <probe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <inp.h>
#include <color.h>
#include <memory.h>
#include <ray_fun.h>
#include <device_fun.h>
#include <heat_fun.h>
#include <contacts.h>
#include <enabled_libs.h>
#include <circuit.h>

static int unused __attribute__((unused));


void dump_clean_cache_files(struct simulation* sim)
{
struct inp_file inp;
char temp[400];
char cach_dir[PATH_MAX];


	if (get_dump_status(sim,dump_remove_dos_cache)==TRUE)
	{
		remove_dir(sim,get_cache_path(sim));
	}

	inp_init(sim,&inp);
	if (inp_load(sim, &inp , "delete_files.inp")==0)
	{

		inp_reset_read(sim,&inp);
		inp_get_string(sim,temp,&inp);
		if (strcmp(temp,"#begin")!=0)
		{
			return;
		}

		while(1)
		{
			inp_get_string(sim,temp,&inp);
			if (strcmp(temp,"#end")==0)
			{
				break;
			}else
			{
				remove(temp);
			}

		}

		inp_free(sim,&inp);
	}



}

void dump_init(struct simulation *sim,struct device* in)
{
in->snapshot_number=0;
set_dump_status(sim,dump_lock, FALSE);
}

void buffer_add_3d_to_2d_projection(struct simulation *sim,struct dat_file *buf,struct device *in,gdouble ***data)
{
int x=0;
int y=0;
int z=0;

gdouble xpos=0.0;
gdouble ypos=0.0;
gdouble zpos=0.0;
long double tot=0.0;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

char string[200];
if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(buf,string);
}

if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			tot=0.0;
			for (y=0;y<dim->ylen;y++)
			{
				tot+=data[z][x][y];
			}

			sprintf(string,"%Le %Le %Le\n",dim->xmesh[x],dim->zmesh[z],tot/((long double)dim->ylen));
			buffer_add_string(buf,string);
		}
	}
}else
if ((dim->xlen>1)&&(dim->ylen>1))
{
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		tot=0.0;
		for (y=0;y<dim->ylen;y++)
		{
			tot+=data[z][x][y];
		}
		sprintf(string,"%Le %Le\n",dim->xmesh[x],tot/((long double)dim->ylen));
		buffer_add_string(buf,string);
	}
}else
{
	x=0;
	z=0;
	tot=0.0;
	for (y=0;y<dim->ylen;y++)
	{
		tot+=data[z][x][y];
	}

	sprintf(string,"%Le %Le\n",dim->xmesh[x],tot/((long double)dim->ylen));
	buffer_add_string(buf,string);
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}

void buffer_add_3d_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble ***data)
{
int x=0;
int y=0;
int z=0;

gdouble xpos=0.0;
gdouble ypos=0.0;
gdouble zpos=0.0;

char string[200];
if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(buf,string);
}

if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				sprintf(string,"%Le %Le %Le %Le\n",dim->xmesh[x],dim->ymesh[y],dim->zmesh[z],data[z][x][y]);
				buffer_add_string(buf,string);
			}
		}
	}
}else
if ((dim->xlen>1)&&(dim->ylen>1))
{
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %Le\n",dim->xmesh[x],dim->ymesh[y],data[z][x][y]);
			buffer_add_string(buf,string);
		}
		buffer_add_string(buf,"\n");
	}
}else
{
	x=0;
	z=0;
	for (y=0;y<dim->ylen;y++)
	{
		sprintf(string,"%Le %Le\n",dim->ymesh[y],data[z][x][y]);
		buffer_add_string(buf,string);
	}
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}


void buffer_add_zxy_rgb_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble ***data)
{
int x=0;
int z=0;
int y=0;

gdouble xpos=0.0;
gdouble zpos=0.0;
long double y_tot=0;

long double X;
long double Y;
long double Z;
int R;
int G;
int B;

struct math_xy luminescence_tot;

char string[200];
struct dimensions XYZ_dim;
long double ***XYZ;
long double max;
dim_init(&XYZ_dim);
dim_cpy(&XYZ_dim,dim);
dim_free_xyz(&XYZ_dim,'y');
XYZ_dim.ylen=3;
dim_alloc_xyz(&XYZ_dim,'y');
XYZ_dim.ymesh[0]=0.0;
XYZ_dim.ymesh[1]=1.0;
XYZ_dim.ymesh[2]=2.0;

malloc_zxy_gdouble(&(XYZ_dim),&XYZ);


	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#data\n");
		buffer_add_string(buf,string);
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{

			inter_init(sim,&luminescence_tot);

			for (y=0;y<dim->ylen;y++)
			{
				inter_append(&luminescence_tot,dim->ymesh[y],data[z][x][y]);
			}

			color_cie_cal_XYZ(sim,&X,&Y,&Z,&luminescence_tot,FALSE);
			XYZ[z][x][0]=X;
			XYZ[z][x][1]=Y;
			XYZ[z][x][2]=Z;

			inter_free(&luminescence_tot);
		}
	}

	max=zx_y_max_gdouble(&XYZ_dim, XYZ,1);
	zxy_div_gdouble(&XYZ_dim, XYZ, max);

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{

			X=XYZ[z][x][0];
			Y=XYZ[z][x][1];
			Z=XYZ[z][x][2];

			color_XYZ_to_rgb(&R,&G,&B,X,Y,Z);

			R*=Y;
			G*=Y;
			B*=Y;

			sprintf(string,"%Le %Le %.2x%.2x%.2x\n",dim->zmesh[z],dim->xmesh[x],R,G,B);
			buffer_add_string(buf,string);

		}

		sprintf(string,"\n");
		buffer_add_string(buf,string);
	}

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#end\n");
		buffer_add_string(buf,string);
	}

zx_y_quick_dump("XYZ", XYZ, &(XYZ_dim));

free_zxy_gdouble(&XYZ_dim,&XYZ);
dim_free(&XYZ_dim);
}

void buffer_add_zxy_data_y_slice(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,long double ***data,int y)
{
int x=0;
int z=0;

gdouble xpos=0.0;
gdouble ypos=0.0;
gdouble zpos=0.0;

char string[200];
if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(buf,string);
}

if ((dim->xlen>1)&&(dim->zlen>1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			sprintf(string,"%Le %Le %Le\n",dim->xmesh[x],dim->zmesh[z],data[z][x][y]);
			buffer_add_string(buf,string);
		}
		buffer_add_string(buf,"\n");
	}
}else
{
	x=0;
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		sprintf(string,"%Le %Le\n",dim->xmesh[x],data[z][x][y]);
		buffer_add_string(buf,string);
	}
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}

void buffer_add_yl_light_data(struct simulation *sim,struct dat_file *buf,struct dim_light *dim,long double ****data,long double shift, int z, int x)
{
int y=0;
int l=0;

long double xpos=0.0;
long double zpos=0.0;

char string[200];

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#data\n");
		buffer_add_string(buf,string);
	}

	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %Le\n",dim->l[l],dim->y[y]-shift,data[z][x][y][l]);

			buffer_add_string(buf,string);
		}

		buffer_add_string(buf,"\n");
	}

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#end\n");
		buffer_add_string(buf,string);
	}

}

void buffer_add_yl_light_data_float(struct simulation *sim,struct dat_file *buf,struct dim_light *dim,float ****data,long double shift, int z, int x)
{
int y=0;
int l=0;

long double xpos=0.0;
long double zpos=0.0;

char string[200];

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#data\n");
		buffer_add_string(buf,string);
	}

	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %e\n",dim->l[l],dim->y[y]-shift,data[z][x][y][l]);

			buffer_add_string(buf,string);
		}

		buffer_add_string(buf,"\n");
	}

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#end\n");
		buffer_add_string(buf,string);
	}

}

void buffer_add_yl_light_data_double(struct simulation *sim,struct dat_file *buf,struct dim_light *dim,double ****data,long double shift, int z, int x)
{
int y=0;
int l=0;

long double xpos=0.0;
long double zpos=0.0;

char string[200];

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#data\n");
		buffer_add_string(buf,string);
	}

	for (l=0;l<dim->llen;l++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %le\n",dim->l[l],dim->y[y]-shift,data[z][x][y][l]);

			buffer_add_string(buf,string);
		}

		buffer_add_string(buf,"\n");
	}

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#end\n");
		buffer_add_string(buf,string);
	}

}

void buffer_add_zx_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble **data)
{
int x=0;
int z=0;

gdouble xpos=0.0;
gdouble zpos=0.0;

char string[200];

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#data\n");
		buffer_add_string(buf,string);
	}


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			sprintf(string,"%Le %Le %Le \n",dim->zmesh[z],dim->xmesh[x],data[z][x]);
			buffer_add_string(buf,string);
		}

		sprintf(string,"\n");
		buffer_add_string(buf,string);
	}

	if (get_dump_status(sim,dump_write_headers)==TRUE)
	{
		sprintf(string,"#end\n");
		buffer_add_string(buf,string);
	}

}

void buffer_add_3d_device_data_int(struct simulation *sim,struct dat_file *buf,struct device *in,int ***data)
{
int x=0;
int y=0;
int z=0;

gdouble xpos=0.0;
gdouble ypos=0.0;
gdouble zpos=0.0;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

char string[200];
if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(buf,string);
}

if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				sprintf(string,"%Le %Le %Le %d\n",dim->xmesh[x],dim->ymesh[y],dim->zmesh[z],data[z][x][y]);
				buffer_add_string(buf,string);
			}
		}
	}
}else
if ((dim->xlen>1)&&(dim->ylen>1))
{
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %d\n",dim->xmesh[x],dim->ymesh[y],data[z][x][y]);
			buffer_add_string(buf,string);
		}
		buffer_add_string(buf,"\n");
	}
}else
{
	x=0;
	z=0;
	for (y=0;y<dim->ylen;y++)
	{
		sprintf(string,"%Le %d\n",dim->ymesh[y],data[z][x][y]);
		buffer_add_string(buf,string);
	}
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}

void buffer_add_2d_device_data_int(struct simulation *sim,struct dat_file *buf,struct device *in,int **data)
{
int x=0;
int z=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

gdouble xpos=0.0;
gdouble zpos=0.0;

char string[200];
if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(buf,string);
}

if ((dim->xlen>1)&&(dim->zlen>1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
				sprintf(string,"%Le %Le %d\n",dim->xmesh[x],dim->zmesh[z],data[z][x]);
				buffer_add_string(buf,string);
		}

		buffer_add_string(buf,"\n");
	}
}else
if ((dim->xlen>1))
{
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		sprintf(string,"%Le %d\n",dim->xmesh[x],data[z][x]);
		buffer_add_string(buf,string);
	}
}else
{
	sprintf(string,"%Le %d\n",dim->ymesh[0],data[0][0]);
	buffer_add_string(buf,string);
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}

void buffer_add_3d_device_data_including_boundaries(struct simulation *sim,struct dat_file *buf,struct device *in,gdouble ***data,long double **left,long double **right)
{
int x=0;
int y=0;
int z=0;

gdouble xpos=0.0;
gdouble ypos=0.0;
gdouble zpos=0.0;

char string[200];
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#data\n");
	buffer_add_string(buf,string);
}

if ((dim->xlen>1)&&(dim->ylen>1)&&(dim->zlen>1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			sprintf(string,"%Le %Le %Le %Le\n",dim->xmesh[x],(long double)0.0,dim->zmesh[z],left[z][x]);
			buffer_add_string(buf,string);

			for (y=0;y<dim->ylen;y++)
			{
				sprintf(string,"%Le %Le %Le %Le\n",dim->xmesh[x],dim->ymesh[y],dim->zmesh[z],data[z][x][y]);
				buffer_add_string(buf,string);
			}

			sprintf(string,"%Le %Le %Le %Le\n",dim->xmesh[x],in->ylen,dim->zmesh[z],right[z][x]);
			buffer_add_string(buf,string);

		}
	}
}else
if ((dim->xlen>1)&&(dim->ylen>1))
{
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		sprintf(string,"%Le %Le %Le\n",dim->xmesh[x],(long double)0.0,left[z][x]);
		buffer_add_string(buf,string);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %Le\n",dim->xmesh[x],dim->ymesh[y],data[z][x][y]);
			buffer_add_string(buf,string);
		}

		sprintf(string,"%Le %Le %Le\n",dim->xmesh[x],in->ylen,right[z][x]);
		buffer_add_string(buf,string);

		buffer_add_string(buf,"\n");
	}
}else
{
	x=0;
	z=0;
	sprintf(string,"%Le %Le\n",(long double)0.0,left[z][x]);
	buffer_add_string(buf,string);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(string,"%Le %Le\n",dim->ymesh[y],data[z][x][y]);
		buffer_add_string(buf,string);
	}

	sprintf(string,"%Le %Le\n",in->ylen,right[z][x]);
	buffer_add_string(buf,string);

}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}


void dump_write_to_disk(struct simulation *sim,struct device* in)
{
char temp[200];
char postfix[100];
char sim_name[PATH_MAX];
char out_dir[PATH_MAX];
struct dat_file buf;
buffer_init(&buf);
struct heat *thermal=&(in->thermal);

strextract_name(sim_name,in->simmode);


int dumped=FALSE;
FILE* out;

	sprintf(postfix,"%d",in->snapshot_number);
	//if ((get_dump_status(sim,dump_pl)==TRUE)||(get_dump_status(sim,dump_energy_slice_switch)==TRUE)||(get_dump_status(sim,dump_1d_slices)==TRUE)||(get_dump_status(sim,dump_optical_probe_spectrum)==TRUE))
	//{
	long double fx=0.0;


	long double voltage=0.0;
	if (in->ncontacts==0)
	{
		voltage=get_equiv_V(sim,in);
	}else
	{
		voltage=contact_get_active_contact_voltage(sim,in);
	}

	//make the snapshots dir
	dump_make_snapshot_dir(sim,out_dir,in->output_path ,"snapshots", in->snapshot_number);

	buffer_init(&buf);
	buffer_malloc(&buf);

	sprintf(temp,"{\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"voltage\":%Le,\n",(long double )voltage);
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"time\":%Le\n",(long double )in->time);
	buffer_add_string(&buf,temp);

	sprintf(temp,"}");
	buffer_add_string(&buf,temp);


	buffer_dump_path(sim,out_dir,"data.json",&buf);
	buffer_free(&buf);


	if ((in->dump_1d_slice_zpos!=-1)&&(in->dump_1d_slice_xpos!=-1))
	{
		dump_device_map(sim,out_dir,in);
		dumped=TRUE;
	}


	dump_1d_slice(sim,in,out_dir);

	circuit_dump_snapshot(sim,in,out_dir);

	#ifdef libheat_enabled
		heat_dump(sim,out_dir,thermal);
	#endif
	dumped=TRUE;

	if (get_dump_status(sim,dump_energy_slice_switch)==TRUE)
	{
		dump_energy_slice(sim,out_dir,in);
		dumped=TRUE;
	}



	if (dumped==TRUE)
	{
		in->snapshot_number++;
	}


}


