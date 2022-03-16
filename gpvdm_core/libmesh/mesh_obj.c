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

/** @file mesh.c
@brief This builds the electrical mesh
*/

#include "device.h"
#include "mesh.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <lang.h>
#include <shape.h>
#include <json.h>

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


void mesh_obj_load(struct simulation *sim,struct mesh_obj *mesh,struct json_obj *json_mesh)
{
	struct json_obj *json_mesh_xyz;
	struct json_obj *json_mesh_config;
	json_mesh_xyz=json_obj_find(json_mesh, "mesh_z");
	if (json_mesh_xyz==NULL)
	{
		ewe(sim,"Mesh z not found\n");
	}
	mesh_load_file(sim,&(mesh->meshdata_z),json_mesh_xyz);

	json_mesh_xyz=json_obj_find(json_mesh, "mesh_x");
	if (json_mesh_xyz==NULL)
	{
		ewe(sim,"Mesh x not found\n");
	}
	mesh_load_file(sim,&(mesh->meshdata_x),json_mesh_xyz);

	json_mesh_xyz=json_obj_find(json_mesh, "mesh_y");
	if (json_mesh_xyz==NULL)
	{
		ewe(sim,"Mesh y not found\n");
	}
	mesh_load_file(sim,&(mesh->meshdata_y),json_mesh_xyz);


	json_mesh_config=json_obj_find(json_mesh, "config");
	if (json_mesh_config==NULL)
	{
		ewe(sim,"Mesh y not found\n");
	}

	json_get_english(sim, json_mesh_config, &(mesh->meshdata_z.remesh),"remesh_z");
	json_get_english(sim, json_mesh_config, &(mesh->meshdata_x.remesh),"remesh_x");
	json_get_english(sim, json_mesh_config, &(mesh->meshdata_y.remesh),"remesh_y");

}

void mesh_obj_apply_srh_contacts(struct simulation *sim,struct mesh_obj *mesh,struct device *dev)
{
	struct mesh* m=&(mesh->meshdata_y);
	//struct mesh_layer* layer=&(m->layers[0]);
	//printf("oh %d",dev->boundry_y0);
	if (dev->boundry_y0==contact_schottky)
	{
		//layer->start_at_edge_left=TRUE;
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


