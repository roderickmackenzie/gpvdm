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

/** @file time.c
	@brief Meshing in time domain.
*/


#include <stddef.h>
#include <lang.h>
#include <exp.h>
#include "sim.h"
#include "inp.h"
#include <cal_path.h>
#include <contacts.h>
#include <log.h>
#include <light_fun.h>
#include <circuit.h>
#include <time_mesh.h>
#include <memory.h>
#include <enabled_libs.h>

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

void time_load_mesh(struct simulation *sim,struct device *in,int number)
{

int i;
struct time_mesh *tm=&(in->tm);
struct inp_file inp;
char mesh_file[200];
gdouble start_time=0.0;
gdouble fs_laser_time=0.0;
int segments=0;
gdouble read_len=0.0;
gdouble dt=0.0;
gdouble v_start=0.0;
gdouble v_stop=0.0;
gdouble mul=0.0;
gdouble read_sun=0.0;
gdouble read_laser=0.0;
gdouble laser_pulse_width=0.0;
gdouble time=0.0;
int ii=0;
gdouble end_time=0;
int fired=FALSE;
gdouble v=0.0;
gdouble dv=0.0;

malloc_1d((void **)&(tm->tm_time_mesh), TM_BUFFER_MAX_LEN, sizeof(long double));
malloc_1d((void **)&(tm->tm_sun), TM_BUFFER_MAX_LEN, sizeof(long double));
malloc_1d((void **)&(tm->tm_voltage), TM_BUFFER_MAX_LEN, sizeof(long double));
malloc_1d((void **)&(tm->tm_laser), TM_BUFFER_MAX_LEN, sizeof(long double));
malloc_1d((void **)&(tm->tm_fs_laser), TM_BUFFER_MAX_LEN, sizeof(long double));

tm->tm_mesh_pos=0;

if (in->mylight.pulse_width==-1)
{
	ewe(sim,_("You must load the optical plugin before making the time mesh"));
}else
{
laser_pulse_width=in->mylight.pulse_width;
}

sprintf(mesh_file,"time_mesh_config%d.inp",number);

inp_init(sim,&inp);
inp_load_from_path(sim,&inp,get_input_path(sim),mesh_file);
inp_check(sim,&inp,1.1);

inp_reset_read(sim,&inp);

inp_get_string(sim,&inp);
sscanf(inp_get_string(sim,&inp),"%Le",&start_time);

inp_get_string(sim,&inp);
sscanf(inp_get_string(sim,&inp),"%Le",&fs_laser_time);

inp_get_string(sim,&inp);
sscanf(inp_get_string(sim,&inp),"%d",&segments);
time=start_time;


for (i=0;i<segments;i++)
{
	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&read_len);

	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&dt);

	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&v_start);

	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&v_stop);

	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&mul);

	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&read_sun);

	inp_get_string(sim,&inp);
	sscanf(inp_get_string(sim,&inp),"%Le",&read_laser);

	if ((dt!=0.0)&&(mul!=0.0))
	{
		v=v_start;
		end_time=time+read_len;


		while(time<end_time)
		{
			dv=(v_stop-v_start)*dt/read_len;
			tm->tm_time_mesh[ii]=time;

			tm->tm_laser[ii]=read_laser;
			tm->tm_sun[ii]=read_sun+light_get_sun(&(in->mylight));
			tm->tm_voltage[ii]=v;
			tm->tm_fs_laser[ii]=0.0;
			time=time+dt;
			v=v+dv;


			if (fired==FALSE)
			{
				if ((time>fs_laser_time)&&(fs_laser_time!= -1.0))
				{
					fired=TRUE;
					tm->tm_fs_laser[ii]=laser_pulse_width/dt;
				}
			}

			ii++;
			dt=dt*mul;
		}
	}
}

tm->tm_mesh_len=ii;


in->time=tm->tm_time_mesh[0];
in->dt=tm->tm_time_mesh[1]-tm->tm_time_mesh[0];

inp_free(sim,&inp);

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

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			in->nlast[z][x][y]=in->n[z][x][y];
			in->plast[z][x][y]=in->p[z][x][y];

			in->Nion_last[z][x][y]=in->Nion[z][x][y];

			for (band=0;band<dim->srh_bands;band++)
			{
				in->ntlast[z][x][y][band]=in->nt[z][x][y][band];
				in->ptlast[z][x][y][band]=in->pt[z][x][y][band];
			}
		}
	}
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
			//in->time=tm->tm_time_mesh[tm->tm_mesh_pos];
			if (tm->tm_mesh_pos==(tm->tm_mesh_len-1))
			{
				in->dt=(tm->tm_time_mesh[tm->tm_mesh_pos]-tm->tm_time_mesh[tm->tm_mesh_pos-1]);
			}else
			{
				in->dt=(tm->tm_time_mesh[tm->tm_mesh_pos+1]-tm->tm_time_mesh[tm->tm_mesh_pos]);
			}

			in->time+=in->dt;

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
	cpy_1d_alloc( (void **)&(out->tm_sun), (void **)&(in->tm_sun), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_voltage), (void **)&(in->tm_voltage), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_laser), (void **)&(in->tm_laser), TM_BUFFER_MAX_LEN,sizeof(long double));
	cpy_1d_alloc( (void **)&(out->tm_fs_laser), (void **)&(in->tm_fs_laser), TM_BUFFER_MAX_LEN,sizeof(long double));

	out->tm_mesh_len=in->tm_mesh_len;
	out->tm_use_mesh=in->tm_use_mesh;
	out->tm_mesh_pos=in->tm_mesh_pos;
}
