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

/** @file dump_map.c
@brief dump a map charge carrier density map of the device, not really used any more due to python back end needs rewritign anyway
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "sim.h"
#include "dump.h"
#include "dat_file.h"
#include <cal_path.h>
#include <exp.h>
#include <lang.h>
#include <i.h>
#include <math_2d.h>
#include <memory.h>

static int unused __attribute__((unused));


void dump_device_map(struct simulation *sim,char* out_dir,struct device *in)
{
struct dimensions *dim=&in->ns.dim;

if (in->circuit_simulation==TRUE)
{
	return;
}

if (dim->srh_bands<=1) return;

if (in->dump_1d_slice_xpos>=dim->xlen) return;
if (in->dump_1d_slice_zpos>=dim->zlen) return;

int x=0;
int y=0;
struct dat_file buf;
struct epitaxy *epi=&(in->my_epitaxy);
char temp[1000];
int i=0;
long double Vexternal=get_equiv_V(sim,in);
int Epoints=0;
struct math_2d map;
struct shape *s;
s=in->obj_zxy[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][0]->s;

long double delta_E=dos_get_band_energy_n(s,1)-dos_get_band_energy_n(s,0);
long double start=in->map_start;
long double stop=in->map_stop;
Epoints=(int)abs(stop-start)/delta_E;
int band=0;
long double E=0;
int vpos=0;
struct newton_state *ns=&(in->ns);
math_2d_init(&map);
math_2d_malloc(&map,dim->ylen,Epoints);
math_2d_init_y_mesh(&map,start, stop);
//printf("%Le %Le %Le\n",stop,start,delta_E);
//printf("%ld %ld\n",dim->ylen,Epoints);
buffer_init(&buf);


//Electrons
buffer_malloc(&buf);
buf.y_mul=1e9;
buf.x_mul=1.0;
strcpy(buf.title,"Charge carrier density - position");
strcpy(buf.type,"heat");
strcpy(buf.x_label,_("Position"));
strcpy(buf.y_label,_("Energy"));
strcpy(buf.x_units,"nm");
strcpy(buf.y_units,"eV");

strcpy(buf.data_label,_("Charge density"));
strcpy(buf.data_units,"m^{-3} eV^{-1}");

buf.logscale_x=0;
buf.logscale_y=0;
buf.logscale_data=TRUE;

buf.x=Epoints;
buf.y=dim->ylen;
buf.z=1;

buf.Vexternal=Vexternal;
buf.time=in->time;

buffer_add_info(sim,&buf);

math_2d_set_value(&map,0.0);
for (band=0;band<dim->srh_bands;band++)
{

	for (y=0;y<dim->ylen;y++)
	{
		s=in->obj_zxy[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]->s;
		E=in->Ec[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]+dos_get_band_energy_n(s,band);
		//printf("%Le\n",in->Ec[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]);
		//printf("%Le\n",dos_get_band_energy_n(epi,band,in->imat[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]));
		//printf("%Le eV\n",E);
		//printf("%ld\n",map.y_len);
		//for (i=0;i<map.y_len;i++)
		//{
		//	printf("%Le\n",map.y_mesh[i]);
		//}
		//getchar();
		vpos=search(map.y_mesh,map.y_len,E);
		map.data[y][vpos]=in->nt[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y][band];
	}

}

for (y=0;y<Epoints;y++)
{

	for (x=0;x<dim->ylen;x++)
	{
		sprintf(temp,"%Le %Le %Le\n",map.y_mesh[y],dim->ymesh[x],map.data[x][y]);
		buffer_add_string(&buf,temp);
	}

	buffer_add_string(&buf,"\n");
}


buffer_dump_path(sim,out_dir,"nt_map.dat",&buf);
buffer_free(&buf);





//Holes
buffer_malloc(&buf);

buf.y_mul=1e9;
buf.x_mul=1.0;
strcpy(buf.title,"Charge carrier density - position");
strcpy(buf.type,"heat");
strcpy(buf.x_label,_("Position"));
strcpy(buf.y_label,_("Energy"));
strcpy(buf.data_label,_("Charge density"));
strcpy(buf.x_units,"nm");
strcpy(buf.y_units,"eV");

strcpy(buf.data_label,_("Charge density"));
strcpy(buf.data_units,"m^{-3} eV^{-1}");

buf.logscale_x=0;
buf.logscale_y=0;
buf.logscale_data=TRUE;

buf.x=Epoints;
buf.y=dim->ylen;
buf.z=1;

buf.Vexternal=Vexternal;
buf.time=in->time;

buffer_add_info(sim,&buf);

math_2d_set_value(&map,0.0);
for (band=0;band<dim->srh_bands;band++)
{

	for (y=0;y<dim->ylen;y++)
	{
		s=in->obj_zxy[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]->s;
		E=in->Ev[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]-dos_get_band_energy_p(s,band);
		vpos=search(map.y_mesh,map.y_len,E);
		map.data[y][vpos]=in->pt[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y][band];
	}

}

for (y=0;y<Epoints;y++)
{

	for (x=0;x<dim->ylen;x++)
	{
		sprintf(temp,"%Le %Le %Le\n",map.y_mesh[y],dim->ymesh[x],map.data[x][y]);
		buffer_add_string(&buf,temp);
	}

	buffer_add_string(&buf,"\n");
}



buffer_dump_path(sim,out_dir,"pt_map.dat",&buf);

buffer_free(&buf);


//Charte total
buffer_malloc(&buf);
buf.y_mul=1e9;
buf.x_mul=1.0;
strcpy(buf.title,"Total charge - position");
strcpy(buf.type,"heat");
strcpy(buf.x_label,_("Position"));
strcpy(buf.y_label,_("Energy"));
strcpy(buf.x_units,"nm");
strcpy(buf.y_units,"eV");

strcpy(buf.data_label,_("Charge density"));
strcpy(buf.data_units,"m^{-3} eV^{-1}");

buf.logscale_x=0;
buf.logscale_y=0;
buf.logscale_data=TRUE;

buf.x=Epoints;
buf.y=dim->ylen;
buf.z=1;

buf.Vexternal=Vexternal;
buf.time=in->time;

buffer_add_info(sim,&buf);


math_2d_set_value(&map,0.0);
for (band=0;band<dim->srh_bands;band++)
{

	for (y=0;y<dim->ylen;y++)
	{
		s=in->obj_zxy[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]->s;
		E=in->Ev[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]-dos_get_band_energy_p(s,band);
		vpos=search(map.y_mesh,map.y_len,E);
		map.data[y][vpos]=in->nt[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y][band];

		E=in->Ec[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y]+dos_get_band_energy_n(s,band);
		vpos=search(map.y_mesh,map.y_len,E);
		map.data[y][vpos]=in->pt[in->dump_1d_slice_zpos][in->dump_1d_slice_xpos][y][band];
	}

}

for (y=0;y<Epoints;y++)
{

	for (x=0;x<dim->ylen;x++)
	{
		sprintf(temp,"%Le %Le %Le\n",map.y_mesh[y],dim->ymesh[x],map.data[x][y]);
		buffer_add_string(&buf,temp);
	}

	buffer_add_string(&buf,"\n");
}


buffer_dump_path(sim,out_dir,"npt_map.dat",&buf);
buffer_free(&buf);


math_2d_free(&map);
}



