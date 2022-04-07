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

/** @file time.c
	@brief Meshing in time domain.
*/


#include <stddef.h>
#include <lang.h>
#include <exp.h>
#include "sim.h"
#include <cal_path.h>
#include <contacts.h>
#include <log.h>
#include <light_fun.h>
#include <circuit.h>
#include <time_mesh.h>
#include <memory.h>
#include <enabled_libs.h>
#include <json.h>

#define TM_BUFFER_MAX_LEN 2000

static int unused __attribute__((unused));



void time_mesh_save(struct simulation *sim,struct device *in)
{

int i;
FILE *out;
struct time_mesh *tm=&(in->tm);
out=fopen("mesh_save.dat","w");
if (out==NULL)
{
	ewe(sim,"can not save time mesh file\n");
}

fprintf(out, "%d\n",tm->tm_mesh_len);

for (i=0;i<tm->tm_mesh_len;i++)
{
	fprintf(out, "%Le %Le %Le %Le %Le\n",tm->tm_time_mesh[i],tm->tm_laser[i],tm->tm_sun[i],tm->tm_voltage[i],tm->tm_fs_laser[i]);
}
fprintf(out, "#ver\n");
fprintf(out, "1.0\n");
fprintf(out, "#end\n");

fclose(out);

}

void time_load_mesh(struct simulation *sim,struct device *in,struct json_obj *json_pulse)
{

int i;
int ii=0;
struct time_mesh *tm=&(in->tm);
char temp[200];
long double start_time=0.0;
long double fs_laser_time=0.0;
int segments=0;
long double read_len=0.0;
long double dt=0.0;
long double v_start=0.0;
long double v_stop=0.0;
long double mul=0.0;
long double laser_start=0.0;
long double laser_stop=0.0;
long double sun_start=0.0;
long double sun_stop=0.0;
long double laser_pulse_width=0.0;
long double time=0.0;
long double end_time=0;
int fired=FALSE;
long double v=0.0;
long double sun=0.0;
long double dv=0.0;
long double laser=0.0;
long double dlaser=0.0;
long double dsun=0.0;
int round=0;

struct json_obj *json_config;
struct json_obj *json_mesh;
struct json_obj *json_mesh_seg;

tm->tm_mesh_pos=0;

if (in->mylight.pulse_width==-1)
{
	ewe(sim,_("You must load the optical plugin before making the time mesh"));
}else
{
	laser_pulse_width=in->mylight.pulse_width;
}

json_config=json_obj_find(json_pulse, "config");

json_get_long_double(sim,json_config,&start_time,"start_time");
json_get_long_double(sim,json_config,&fs_laser_time,"fs_laser_time");

json_mesh=json_obj_find(json_pulse, "mesh");

json_get_int(sim,json_mesh,&segments,"segments");
long double dt_last=0.0;
for (round=0;round<2;round++)
{
	fired=FALSE;
	ii=0;
	time=start_time;


	for (i=0;i<segments;i++)
	{

		sprintf(temp,"segment%d",i);
		json_mesh_seg=json_obj_find(json_mesh, temp);
		if (json_mesh_seg==NULL)
		{
			ewe(sim,"segment not found\n");
		}
		json_get_long_double(sim,json_mesh_seg,&read_len,"len");
		json_get_long_double(sim,json_mesh_seg,&dt,"dt");
		dt_last=dt;
		json_get_long_double(sim,json_mesh_seg,&v_start,"voltage_start");
		json_get_long_double(sim,json_mesh_seg,&v_stop,"voltage_stop");
		json_get_long_double(sim,json_mesh_seg,&mul,"mul");
		json_get_long_double(sim,json_mesh_seg,&sun_start,"sun_start");
		json_get_long_double(sim,json_mesh_seg,&sun_stop,"sun_stop");
		json_get_long_double(sim,json_mesh_seg,&laser_start,"laser_start");
		json_get_long_double(sim,json_mesh_seg,&laser_stop,"laser_stop");



		if ((dt!=0.0)&&(mul!=0.0))
		{
			v=v_start;
			laser=laser_start;
			sun=sun_start;
			end_time=time+read_len;


			while(time<end_time)
			{
				dv=(v_stop-v_start)*dt/read_len;
				dlaser=(laser_stop-laser_start)*dt/read_len;
				dsun=(sun_stop-sun_start)*dt/read_len;
				if (round==1)
				{
					tm->tm_time_mesh[ii]=time;
					tm->tm_dt[ii]=dt_last;
					tm->tm_laser[ii]=laser;
					tm->tm_sun[ii]=sun;//+light_get_sun(&(in->mylight));
					tm->tm_voltage[ii]=v;
					tm->tm_fs_laser[ii]=0.0;
				}
				time=time+dt;
				dt_last=dt;	
				v=v+dv;
				laser=laser+dlaser;
				sun=sun+dsun;
				if (round==1)
				{
					if (fired==FALSE)
					{
						if ((time>fs_laser_time)&&(fs_laser_time!= -1.0))
						{
							fired=TRUE;
							if (round==1)
							{
								tm->tm_fs_laser[ii]=laser_pulse_width/dt;
							}
						}
					}
					//printf("%Le %Le %d\n",time,tm->tm_fs_laser[ii],fired);
				}
				ii++;
				dt=dt*mul;
			}
		}
	}
	//getchar();
	if (round==0)
	{
		malloc_1d((void **)&(tm->tm_time_mesh), ii, sizeof(long double));
		malloc_1d((void **)&(tm->tm_dt), ii, sizeof(long double));
		malloc_1d((void **)&(tm->tm_sun), ii, sizeof(long double));
		malloc_1d((void **)&(tm->tm_voltage), ii, sizeof(long double));
		malloc_1d((void **)&(tm->tm_laser), ii, sizeof(long double));
		malloc_1d((void **)&(tm->tm_fs_laser), ii, sizeof(long double));
	}
}

tm->tm_mesh_len=ii;


in->time=tm->tm_time_mesh[0];
in->dt=tm->tm_dt[0];


tm->tm_use_mesh=TRUE;

}


void time_init(struct simulation *sim,struct device *in)
{

time_store(sim,in);

in->time=0.0;
}

void time_store(struct simulation *sim,struct device *in)
{
int x;
int y;
int z;
int band;
struct dimensions *dim=&in->ns.dim;
struct newton_state *ns=&(in->ns);

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			if (in->drift_diffision_simulations_enabled==TRUE)
			{
				in->nlast[z][x][y]=in->n[z][x][y];
				in->plast[z][x][y]=in->p[z][x][y];

				if (ns->Nion_enabled==TRUE)
				{
					in->Nion_last[z][x][y]=in->Nion[z][x][y];
				}

				if (ns->singlet_enabled==TRUE)
				{
					in->Ns_last[z][x][y]=in->Ns[z][x][y];
					in->Nt_last[z][x][y]=in->Nt[z][x][y];
					in->Nsd_last[z][x][y]=in->Nsd[z][x][y];
					in->Ntd_last[z][x][y]=in->Ntd[z][x][y];
				}

				for (band=0;band<dim->srh_bands;band++)
				{
					in->ntlast[z][x][y][band]=in->nt[z][x][y][band];
					in->ptlast[z][x][y][band]=in->pt[z][x][y][band];
				}
			}
		}
	}
}

if (ns->singlet_enabled==TRUE)
{
	in->Nho_last=in->Nho;
}

contacts_time_step(sim,in);

#ifdef libcircuit_enabled
	circuit_time_step(sim,&(in->cir));
#endif

//in->VCext_last=in->VCext;
in->Ilast=get_I(in);
}

void device_timestep(struct simulation *sim,struct device *in)
{
time_store(sim,in);
struct time_mesh *tm=&(in->tm);
//printf("%d %d\n",tm->tm_use_mesh,in->go_time);
//getchar();
if (in->go_time==TRUE)
{
	if (tm->tm_use_mesh==TRUE)
	{
		if (tm->tm_mesh_pos<(tm->tm_mesh_len-1))
		{
			tm->tm_mesh_pos++;
			in->dt=tm->tm_dt[tm->tm_mesh_pos];
			in->time=tm->tm_time_mesh[tm->tm_mesh_pos];
		}
	}else
	{
		in->time+=in->dt;
	}
}
}

int time_test_last_point(struct device *in)
{
	struct time_mesh *tm=&(in->tm);
	if (tm->tm_mesh_pos<(tm->tm_mesh_len-1))
	{
		return FALSE;
	}else
	{
		return TRUE;
	}
}

gdouble time_get_voltage(struct device *in)
{
	struct time_mesh *tm=&(in->tm);
	return tm->tm_voltage[tm->tm_mesh_pos];
}

gdouble time_get_fs_laser(struct device *in)
{
	struct time_mesh *tm=&(in->tm);
	return tm->tm_fs_laser[tm->tm_mesh_pos];
}

gdouble time_get_sun(struct device *in)
{
	struct time_mesh *tm=&(in->tm);
	return tm->tm_sun[tm->tm_mesh_pos];
}

gdouble time_get_laser(struct device *in)
{
	struct time_mesh *tm=&(in->tm);
	return tm->tm_laser[tm->tm_mesh_pos];
}

void time_memory_free(struct time_mesh *tm)
{

	free_1d((void **)&(tm->tm_time_mesh),sizeof(long double));
	free_1d((void **)&(tm->tm_dt),sizeof(long double));
	free_1d((void **)&(tm->tm_sun),sizeof(long double));
	free_1d((void **)&(tm->tm_voltage),sizeof(long double));
	free_1d((void **)&(tm->tm_laser),sizeof(long double));
	free_1d((void **)&(tm->tm_fs_laser),sizeof(long double));

	tm->tm_mesh_len=-1;
	tm->tm_use_mesh=-1;
	tm->tm_mesh_pos=-1;
}

void time_mesh_init(struct time_mesh *tm)
{
	tm->tm_time_mesh=NULL;
	tm->tm_dt=NULL;
	tm->tm_sun=NULL;
	tm->tm_voltage=NULL;
	tm->tm_laser=NULL;
	tm->tm_fs_laser=NULL;

	tm->tm_mesh_len=-1;
	tm->tm_use_mesh=-1;
	tm->tm_mesh_pos=-1;
}


void time_mesh_cpy(struct time_mesh *out,struct time_mesh *in)
{

	cpy_1d_alloc( (void **)&(out->tm_time_mesh), (void **)&(in->tm_time_mesh), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_dt), (void **)&(in->tm_dt), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_sun), (void **)&(in->tm_sun), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_voltage), (void **)&(in->tm_voltage), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_laser), (void **)&(in->tm_laser), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_fs_laser), (void **)&(in->tm_fs_laser), TM_BUFFER_MAX_LEN,sizeof(long double));

	out->tm_mesh_len=in->tm_mesh_len;
	out->tm_use_mesh=in->tm_use_mesh;
	out->tm_mesh_pos=in->tm_mesh_pos;
}
