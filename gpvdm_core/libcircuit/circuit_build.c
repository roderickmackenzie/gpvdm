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

/** @file circuit_build.c
@brief This builds the circuit mesh
*/

#include "device.h"
#include <mesh.h>
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <circuit.h>
#include <matrix.h>
#include <memory.h>
#include <contacts.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <exp.h>
#include <dump.h>
#include <lang.h>
#include <json.h>
#include <device_fun.h>

void circuit_build_nodes(struct simulation * sim,struct circuit *cir,struct device *dev)
{

	int x=0;
	int y=0;
	int z=0;
	int l=0;
	long double y_pos=0.0;
	struct circuit_node *node;
	struct epitaxy *epi=&(dev->my_epitaxy);
	struct dimensions *dim=&(dev->ns.dim);
	struct object *obj;
	//struct component *com;
	long double dy;
	int add_final_point=FALSE;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			y=0;
			y_pos=0.0;
			//com=&(epi->layer[l].s.com);

			for (l=0;l<epi->layers;l++)
			{
				dy=epi->layer[l].width/2.0;
				obj=dev->obj_zx_layer[z][x][l];
				//printf("%d %d %d\n",l,dev->mask_epitaxy[z][x][l],obj->uid);
				if ((epi->layer[l].layer_type==LAYER_ACTIVE))	//||(epi->layer[l].layer_type==LAYER_CONTACT)
				{
					if (obj->uid!=0)
					{
							//upper component
							node=circuit_add_node(sim,cir,x,y,z,CIR_UNKNOWN);
							if (node!=NULL)
							{
								node->z_pos=dim->z[z];
								node->x_pos=dim->x[x];
								node->y_pos=y_pos;
								y++;
							}

							y_pos+=dy;

							//lower component
							node=circuit_add_node(sim,cir,x,y,z,CIR_UNKNOWN);
							if (node!=NULL)
							{
								node->z_pos=dim->z[z];
								node->x_pos=dim->x[x];
								node->y_pos=y_pos;
								y++;
							}

							y_pos+=dy;


					}else
					{
						y_pos+=dy;
						y_pos+=dy;
						y+=2;
					}

				}
			}

			//This is a special case for a device with one layer
			add_final_point=FALSE;

			if (y==2)
			{
				add_final_point=TRUE;
			}

			if (add_final_point==TRUE)
			{
				node=circuit_add_node(sim,cir,x,y,z,CIR_UNKNOWN);
				if (node!=NULL)
				{
					node->z_pos=dim->z[z];
					node->x_pos=dim->x[x];
					node->y_pos=y_pos;
					y++;
				}
			}
			//getchar();
			//Then a final node at the end

			//getchar();
		}

	}

}

void circuit_build_device(struct simulation * sim,struct circuit *cir,struct device *dev)
{

	int x=0;
	int y=0;
	int z=0;
	int start;
	int stop;
	int l=0;
	int y0=-1;
	int y1=-1;
	int y2=-1;

	int x0=-1;
	int x1=-1;

	int z0=-1;
	int z1=-1;

	//int sub_mesh=0;
	//int x_submesh_point_count=0;
	//int no_x=FALSE;
	//int x_link_y_shift=0;
	long double dx=0.0;
	long double dy=0.0;
	long double dz=0.0;
	struct json_obj *json_circuit;
	long double link_resistance;

	char full_path[PATH_MAX];
	struct inp_file inp;

	cir->unknowns=0;
	struct epitaxy *epi=&(dev->my_epitaxy);
	struct dimensions *dim=&(dev->ns.dim);
	struct circuit_link local_link;
	struct component *com;
	struct object *obj;
	struct shape *s;
	//struct mesh *mesh_x;
	int added_layers=0;

	cir->enabled=TRUE;

	if (strcmp(dev->newton_name,"newton_simple")!=0)
	{
		cir->enabled=FALSE;
		return;
	}

	//mesh_x=&(dev->mesh_data.meshdata_x);

	/*for (l=0;l<mesh_x->nlayers;l++)
	{
		printf("%d\n",mesh_x->layers[l].n_points);
	}

	getchar();*/

	/*for (x=0;x<dim->xlen;x++)
	{
		printf("%d %Le %Le\n",x,dim->xmesh[x],dim->dx[x]);
	}
	getchar();*/


	json_circuit=json_obj_find(&(dev->config.obj), "circuit");

	if (circuit_load_config(sim,cir,json_circuit,epi)!=0)
	{

		circuit_build_nodes(sim,cir,dev);

		for (z=0;z<dim->zlen;z++)
		{
			//sub_mesh=0;
			//x_submesh_point_count=0;
			for (x=0;x<dim->xlen;x++)
			{
				y=0;
				added_layers=0;
				for (l=0;l<epi->layers;l++)
				{
					//no_x=FALSE;

					obj=dev->obj_zx_layer[z][x][l];

					s=obj->s;
					com=&(s->com);

					dx=dim->dX[x];
					dz=dim->dZ[z];
					dy=epi->layer[l].width/2.0;
					//printf("%d %s %s\n",l,s->name,com->component);
					//getchar();
					y0=added_layers*2;
					y1=added_layers*2+1;
					y2=added_layers*2+2;
					if ((epi->layer[l].layer_type==LAYER_ACTIVE))	//||(epi->layer[l].layer_type==LAYER_CONTACT)
					{
						if (strcmp(com->component,"none")!=0)
						{
							if (strcmp(com->component,"resistance")==0)
							{
								start=circuit_find_node_by_xyz(sim,cir,x,y0,z);
								stop=circuit_find_node_by_xyz(sim,cir,x,y1,z);
								if ((start!=-1)&&(stop!=-1))
								{
									local_link.start=start;
									local_link.stop=stop;
									local_link.layer=l;
									local_link.id=0;
									local_link.dl=dy;
									local_link.x=x;
									local_link.z=z;

									local_link.type='R';
									local_link.R=com->series_y*dy/(dx*dz);
									//circuit_printf_links(sim,cir);
									//printf("%le %Le %Le %Le %Le\n",local_link.R,com->series,dy,dx,dz);
									//getchar();
									local_link.n0=0.0;
									local_link.I0=0.0;
									local_link.enable_Isc=com->electrical_enable_generation;
									local_link.Isc=0.0;
									circuit_add_link(sim,cir,&local_link);
								}

								start=circuit_find_node_by_xyz(sim,cir,x,y1,z);
								stop=circuit_find_node_by_xyz(sim,cir,x,y2,z);
								if ((start!=-1)&&(stop!=-1))
								{
									local_link.start=start;
									local_link.stop=stop;
									local_link.layer=l;
									local_link.id=0;
									local_link.dl=dy;
									local_link.x=x;
									local_link.z=z;

									local_link.type='R';
									local_link.R=com->series_y*dy/(dx*dz);
									//circuit_printf_links(sim,cir);
									//printf("%le\n",local_link.rho);
									//getchar();
									local_link.n0=0.0;
									local_link.I0=0.0;
									local_link.enable_Isc=FALSE;
									local_link.Isc=0.0;
									circuit_add_link(sim,cir,&local_link);
								}

							}else
							if (strcmp(com->component,"diode")==0)
							{

								start=circuit_find_node_by_xyz(sim,cir,x,y0,z);
								int diode_start=start;
								stop=circuit_find_node_by_xyz(sim,cir,x,y1,z);
								if ((start!=-1)&&(stop!=-1))
								{
									local_link.start=start;
									local_link.stop=stop;
									local_link.layer=l;
									local_link.id=0;
									local_link.dl=dy;
									local_link.x=x;
									local_link.z=z;

									local_link.type='D';
									local_link.layer=l;
									local_link.R=0.0;
									local_link.n0=com->n_ideality;
									local_link.I0=com->J0*dx*dz;
									local_link.enable_Isc=com->electrical_enable_generation;
									local_link.Isc=0.0;
									circuit_add_link(sim,cir,&local_link);
								}

								//series
								start=circuit_find_node_by_xyz(sim,cir,x,y1,z);
								int diode_mid=start;
								stop=circuit_find_node_by_xyz(sim,cir,x,y2,z);
								int diode_stop=stop;

								if ((start!=-1)&&(stop!=-1))
								{
									local_link.start=start;
									local_link.stop=stop;
									local_link.layer=l;
									local_link.id=0;
									local_link.dl=dy;
									local_link.x=x;
									local_link.z=z;

									local_link.type='R';
									local_link.R=com->series_y*2.0*dy/(dx*dz);
									local_link.n0=0.0;
									local_link.I0=0.0;
									local_link.enable_Isc=FALSE;
									local_link.Isc=0.0;
									circuit_add_link(sim,cir,&local_link);
								}

								//shunt
								if ((diode_start!=-1)&&(diode_stop!=-1))
								{
									local_link.start=diode_start;
									local_link.stop=diode_mid;
									local_link.layer=l;
									local_link.id=1;
									local_link.dl=dy;
									local_link.x=x;
									local_link.z=z;

									local_link.type='R';
									local_link.R=com->shunt*2.0*dy/(dx*dz);

									//printf("%Le %le\n",com->shunt,local_link.R);
									//getchar();

									local_link.n0=0.0;
									local_link.I0=0.0;
									local_link.enable_Isc=FALSE;
									local_link.Isc=0.0;
									circuit_add_link(sim,cir,&local_link);
								}
								

							}else
							{
								printf_log(sim,"Shape: %s\n",s->name);
								ewe(sim,"I don't know what component this is %s %s\n",s->name,com->component);
							}
						}

						added_layers++;
					}
					
				}
			}
		}

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				int y_level=0;
				for (y=0;y<epi->layers*2+1;y++)
				{
					l=y/2;
					if (l>=epi->layers)
					{
						l=epi->layers-1;
					}
					obj=dev->obj_zx_layer[z][x][l];

					s=obj->s;
					com=&(s->com);

					x0=x;
					x1=x+1;

					start=circuit_find_node_by_xyz(sim,cir,x0,y_level,z);
					stop=circuit_find_node_by_xyz(sim,cir,x1,y_level,z);

					if ((epi->layer[l].layer_type==LAYER_ACTIVE))	//||(epi->layer[l].layer_type==LAYER_CONTACT)
					{
						if (strcmp(com->component,"none")!=0)
						{
							if ((start!=-1)&&(stop!=-1))
							{

								dx=dim_dl_xyz(dim, x0, 0, z, x1, 0, z);
								dy=epi->layer[l].width;
								dz=dim->dZ[z];

								local_link.id=0;
								local_link.dl=dx;
								local_link.start=start;
								local_link.stop=stop;
								local_link.type='R';
								local_link.layer=l;
								long double val=com->series_x;
								//val=1e-10;
								local_link.R=val*dx/(dy*dz);
								//printf("%le\n", local_link.R);
								//getchar();
								local_link.enable_Isc=FALSE;
								local_link.Isc=0.0;

								circuit_add_link(sim,cir,&local_link);
							}
						}

						z0=z;
						z1=z+1;

						start=circuit_find_node_by_xyz(sim,cir,x,y_level,z0);
						stop=circuit_find_node_by_xyz(sim,cir,x,y_level,z1);
						if (strcmp(com->component,"none")!=0)
						{
							if ((start!=-1)&&(stop!=-1))
							{

								dx=dim->dX[x];
								dy=epi->layer[l].width;
								dz=dim_dl_xyz(dim, x, 0, z0, x, 0, z1);

								local_link.id=0;
								local_link.dl=dz;
								local_link.start=start;
								local_link.stop=stop;
								local_link.type='R';
								local_link.layer=l;
								//val=1e-10;
								local_link.R=com->series_z*dz/(dx*dy);
								//printf("%le\n", local_link.R);
								//getchar();
								local_link.enable_Isc=FALSE;
								local_link.Isc=0.0;

								circuit_add_link(sim,cir,&local_link);
							}
						}

						y_level++;
					}

				}
			}

		}
	}
////

	int cross_link=FALSE;

	if (cross_link==TRUE)
	{
		circuit_nodes_select(sim,cir,FALSE);
		struct circuit_node *node0=NULL;
		struct circuit_node *node1=NULL;

		node0=circuit_nodes_find_min_xyz(sim,cir);
		//printf("%d\n",min_node);
		//printf("%d %d %d\n",node0->x,node0->y,node0->z);

		join_path(2, full_path, get_input_path(dev), "circuit.inp");

		inp_init(sim,&inp);
		inp_load(sim,&inp,full_path);
		inp_check(sim,&inp,1.0);
		inp_search_gdouble(sim,&inp,&link_resistance,"#cross_link");
		inp_free(sim,&inp);

		while(1)
		{
			circuit_nodes_select(sim,cir,FALSE);
			circuit_nodes_flood_fill(sim,cir,node0->node_index);

			node0=circuit_nodes_selected_find_maxx_maxy_maxz(sim,cir);
			circuit_nodes_select(sim,cir,FALSE);
			//node0->selected=TRUE;
			//break;
			node1=circuit_nodes_find_node_x_plus_one(sim,cir,node0);
			if (node1==NULL)
			{
				break;
			}
			circuit_nodes_flood_fill(sim,cir,node1->node_index);
			//node1->selected=TRUE;
			circuit_nodes_select(sim,cir,FALSE);
			node1=circuit_nodes_selected_find_miny(sim,cir,node1);
			node1->selected=TRUE;

			for (z=0;z<dim->zlen;z++)
			{
				start=circuit_find_node_by_xyz(sim,cir,node0->x,node0->y,z);
				stop=circuit_find_node_by_xyz(sim,cir,node1->x,node1->y,z);

				if ((start!=-1)&&(stop!=-1))
				{
					//add link
					local_link.id=0;
					local_link.dl=1.0;//dim_dl_xyz(dim, x, 0, z, x+1, 0, z);
					local_link.start=start;
					local_link.stop=stop;
					local_link.type='R';
					local_link.R=link_resistance;
					local_link.enable_Isc=FALSE;
					local_link.Isc=0.0;

					circuit_add_link(sim,cir,&local_link);
				}
			}

		}
		//
	}

////
	circuit_apply_voltages(sim,cir,dev);
	circuit_calculate_matrix_pos(sim,cir);
	circuit_malloc(sim,cir);
	circuit_dump(sim,dev,cir);
	circuit_dump_gnuplot(sim,dev,cir);

	//circuit_printf_links(sim,cir);
	printf_log(sim,"unknowns=%d\n",cir->unknowns);
	//getchar();

	//struct circuit_node *node;
	//struct circuit_node *nodes=cir->nodes;

	//circuit_printf_links(sim,cir);
	//getchar();

	//getchar();
}



