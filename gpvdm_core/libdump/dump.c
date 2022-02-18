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
long double tot=0.0;

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

			sprintf(string,"%Le %Le %Le\n",dim->x[x],dim->z[z],tot/((long double)dim->ylen));
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
		sprintf(string,"%Le %Le\n",dim->x[x],tot/((long double)dim->ylen));
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

	sprintf(string,"%Le %Le\n",dim->x[x],tot/((long double)dim->ylen));
	buffer_add_string(buf,string);
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}

void dat_file_add_zxy_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble ***data)
{
	dat_file_add_zxy(sim,buf,dim,(void ***)data,GPVDM_LONG_DOUBLE);
}



void buffer_add_zxy_rgb_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,gdouble ***data)
{
int x=0;
int z=0;
int y=0;

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
dim_malloc_xyz(&XYZ_dim,'y');
XYZ_dim.y[0]=0.0;
XYZ_dim.y[1]=1.0;
XYZ_dim.y[2]=2.0;

malloc_zxy_long_double(&(XYZ_dim),&XYZ);


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
				inter_append(&luminescence_tot,dim->y[y],data[z][x][y]);
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

			sprintf(string,"%Le %Le %.2x%.2x%.2x\n",dim->z[z],dim->x[x],R,G,B);
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

free_zxy_long_double(&XYZ_dim,&XYZ);
dim_free(&XYZ_dim);
}

void buffer_add_zxy_data_y_slice(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,long double ***data,int y)
{
int x=0;
int z=0;

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
			sprintf(string,"%Le %Le %Le\n",dim->x[x],dim->z[z],data[z][x][y]);
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
		sprintf(string,"%Le %Le\n",dim->x[x],data[z][x][y]);
		buffer_add_string(buf,string);
	}
}

if (get_dump_status(sim,dump_write_headers)==TRUE)
{
	sprintf(string,"#end\n");
	buffer_add_string(buf,string);
}

}

void buffer_add_yl_light_data(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,long double ****data,long double shift, int z, int x)
{
int y=0;
int l=0;

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

void buffer_add_yl_light_data_float(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,float ****data,long double shift, int z, int x)
{
int y=0;
int l=0;

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

void buffer_add_yl_light_data_double(struct simulation *sim,struct dat_file *buf,struct dimensions *dim,double ****data,long double shift, int z, int x)
{
int y=0;
int l=0;

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
			sprintf(string,"%Le %Le %Le \n",dim->z[z],dim->x[x],data[z][x]);
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
				sprintf(string,"%Le %Le %Le %d\n",dim->x[x],dim->y[y],dim->z[z],data[z][x][y]);
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
			sprintf(string,"%Le %Le %d\n",dim->x[x],dim->y[y],data[z][x][y]);
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
		sprintf(string,"%Le %d\n",dim->y[y],data[z][x][y]);
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
struct dimensions *dim=&in->ns.dim;


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
				sprintf(string,"%Le %Le %d\n",dim->x[x],dim->z[z],data[z][x]);
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
		sprintf(string,"%Le %d\n",dim->x[x],data[z][x]);
		buffer_add_string(buf,string);
	}
}else
{
	sprintf(string,"%Le %d\n",dim->y[0],data[0][0]);
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

char string[200];
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
			sprintf(string,"%Le %Le %Le %Le\n",dim->x[x],(long double)0.0,dim->z[z],left[z][x]);
			buffer_add_string(buf,string);

			for (y=0;y<dim->ylen;y++)
			{
				sprintf(string,"%Le %Le %Le %Le\n",dim->x[x],dim->y[y],dim->z[z],data[z][x][y]);
				buffer_add_string(buf,string);
			}

			sprintf(string,"%Le %Le %Le %Le\n",dim->x[x],in->ylen,dim->z[z],right[z][x]);
			buffer_add_string(buf,string);

		}
	}
}else
if ((dim->xlen>1)&&(dim->ylen>1))
{
	z=0;
	for (x=0;x<dim->xlen;x++)
	{
		sprintf(string,"%Le %Le %Le\n",dim->x[x],(long double)0.0,left[z][x]);
		buffer_add_string(buf,string);

		for (y=0;y<dim->ylen;y++)
		{
			sprintf(string,"%Le %Le %Le\n",dim->x[x],dim->y[y],data[z][x][y]);
			buffer_add_string(buf,string);
		}

		sprintf(string,"%Le %Le %Le\n",dim->x[x],in->ylen,right[z][x]);
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
		sprintf(string,"%Le %Le\n",dim->y[y],data[z][x][y]);
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
dat_file_init(&buf);
struct world *w=&(in->w);
struct heat *thermal=&(in->thermal);

strextract_name(sim_name,in->simmode);

int dumped=FALSE;
long double voltage=0.0;

	sprintf(postfix,"%d",in->snapshot_number);


	if (in->ncontacts==0)
	{
		voltage=get_equiv_V(sim,in);
	}else
	{
		voltage=contact_get_active_contact_voltage(sim,in);
	}

	//make the snapshots dir
	struct snapshots snap;
	snapshots_init(&snap);
	strcpy(snap.type,"snapshots");
	strcpy(snap.plot_type,"2d3d");
	strcpy(snap.name,"snapshots");
	strcpy(snap.path,in->output_path);
	dump_make_snapshot_dir(sim,out_dir, in->snapshot_number,&snap);

	dat_file_init(&buf);
	buffer_malloc(&buf);

	sprintf(temp,"{\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"voltage\":%Le,\n",(long double )voltage);
	buffer_add_string(&buf,temp);

	sprintf(temp,"\t\"time\":%Le,\n",(long double )in->time);
	buffer_add_string(&buf,temp);
	buffer_add_string(&buf,"\t\"item_type\":\"snapshots_sub_dir\"\n");
	sprintf(temp,"}");
	buffer_add_string(&buf,temp);


	buffer_dump_path(sim,out_dir,"data.json",&buf);
	buffer_free(&buf);


	dump_1d_slice(sim,in,out_dir);
	#ifdef libcircuit_enabled
		circuit_dump_snapshot(sim,in,out_dir);
	#endif

	#ifdef libheat_enabled
		heat_dump(sim,out_dir,thermal);
	#endif
	dumped=TRUE;




	if (dumped==TRUE)
	{
		in->snapshot_number++;
	}


}


