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

/** @file mesh.c
@brief This builds the electrical mesh
*/

#include "device.h"
#include "mesh.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <lang.h>
#include <shape.h>


void mesh_obj_cpy(struct simulation *sim,struct mesh_obj *out,struct mesh_obj *in)
{
	mesh_cpy(sim,&(out->meshdata_x),&(in->meshdata_x));
	mesh_cpy(sim,&(out->meshdata_y),&(in->meshdata_y));
	mesh_cpy(sim,&(out->meshdata_z),&(in->meshdata_z));
}

void mesh_obj_free(struct simulation *sim,struct mesh_obj *in)
{
	mesh_free(&(in->meshdata_x));
	mesh_free(&(in->meshdata_y));
	mesh_free(&(in->meshdata_z));
}

void mesh_obj_init(struct mesh_obj *in)
{
	mesh_init(&(in->meshdata_x));
	mesh_init(&(in->meshdata_y));
	mesh_init(&(in->meshdata_z));
}


void mesh_obj_load(struct simulation *sim,struct mesh_obj *mesh)
{
	mesh_load_file(sim,&(mesh->meshdata_z),"mesh_z.inp");
	mesh_load_file(sim,&(mesh->meshdata_x),"mesh_x.inp");
	mesh_load_file(sim,&(mesh->meshdata_y),"mesh_y.inp");
}

void mesh_obj_apply_srh_contacts(struct simulation *sim,struct mesh_obj *mesh,struct device *dev)
{
	struct mesh* m=&(mesh->meshdata_y);
	struct mesh_layer* layer=&(m->layers[0]);
	//printf("oh %d",dev->boundry_y0);
	if (dev->boundry_y0==contact_schottky)
	{
		layer->start_at_edge_left=TRUE;
		//layer->n_points++;
		//layer->dmesh=realloc (layer->dmesh, layer->n_points * sizeof(long double));
		m->tot_points++;
		//printf("what\n");
		//getchar();
	}
	//printf("here");
	//getchar();
	//layer->start_at_edge_right=FALSE;
	//mesh_load_file(sim,&(mesh->meshdata_z),"mesh_z.inp");
	//mesh_load_file(sim,&(mesh->meshdata_x),"mesh_x.inp");
	//mesh_load_file(sim,&(mesh->meshdata_y),"mesh_y.inp");
}


