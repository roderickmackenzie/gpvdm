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

/** @file nodes.c
@brief Functions which work on nodes
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


void circuit_node_set_type(struct simulation * sim, struct circuit_node *node, int type)
{
	node->type=type;
	if (node->type==CIR_UNKNOWN)
	{
		node->V=1.0;
		node->matrix_pos=-1;
	}else
	if (node->type==CIR_CHANGE_X)
	{
		node->matrix_pos=-1;
	}else
	if (node->type==CIR_KNOWN)
	{
		node->matrix_pos=-1;
	}

}

struct circuit_node *circuit_add_node(struct simulation * sim,struct circuit *cir,int x,int y,int z,int type)
{
	int n=0;
	struct circuit_node *node;

	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(cir->nodes[n]);
		if ((node->x==x)&&(node->y==y)&&(node->z==z))
		{
			return NULL;
		}
	}

	node=&(cir->nodes[cir->nodes_len]);
	//printf("%d\n",cir->nodes_len);

	node->nlinks=0;

	circuit_node_set_type(sim, node,type);

	node->x=x;
	node->y=y;
	node->z=z;
	node->x_pos=0.0;
	node->y_pos=0.0;
	node->z_pos=0.0;
	node->V_last=0.0;
	node->selected=FALSE;
	node->node_index=cir->nodes_len;

	cir->nodes_len++;

	if (cir->nodes_len>=cir->nodes_max)
	{
		cir->nodes_max+=1000;
		cir->nodes=realloc(cir->nodes,sizeof(struct circuit_node)*cir->nodes_max);
		node=&(cir->nodes[cir->nodes_len-1]);
	}

	return node;
}


int circuit_find_node_by_xyz(struct simulation * sim,struct circuit *cir,int x,int y,int z)
{
	int n;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;
	for (n=0;n<cir->nodes_len;n++)
	{
		//printf("%x %d %ld\n",nodes,n,cir->nodes_len);
		node=&(nodes[n]);
		if ((node->x==x)&&(node->y==y)&&(node->z==z))
		{
			return n;
		}
	}

	return -1;
}

void circuit_nodes_select(struct simulation * sim,struct circuit *cir,int value)
{
	int n;

	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);
		node->selected=value;
	}
}

void circuit_nodes_flood_fill(struct simulation * sim,struct circuit *cir,int node_index)
{
	int l;
	int other_end;
	struct circuit_node *node;
	struct circuit_link *link;
	struct circuit_link *links=cir->links;
	struct circuit_node *nodes=cir->nodes;

	node=&(nodes[node_index]);
	node->selected=TRUE;

	for (l=0;l<node->nlinks;l++)
	{
		link=&(links[node->links[l]]);
		other_end=link_get_other_end(cir,link,node_index);
		if (other_end!=-1)
		{
			if (nodes[other_end].selected==FALSE)
			{
				circuit_nodes_flood_fill(sim,cir,other_end);
			}
		}
	}

	return;
}

struct circuit_node * circuit_nodes_find_min_xyz(struct simulation * sim,struct circuit *cir)
{
	int n;
	int dxyz=0;
	int min_dxyz=0;
	struct circuit_node * min_node=NULL;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	if (cir->nodes_len>0)
	{
		node=&(nodes[0]);
		min_dxyz=(node->x*node->x)+(node->y*node->y)+(node->z*node->z);
		min_node=node;
	}

	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);
		dxyz=(node->x*node->x)+(node->y*node->y)+(node->z*node->z);

		if (min_dxyz>dxyz)
		{
			min_node=node;
			min_dxyz=dxyz;
		}

	}

	return min_node;
}

struct circuit_node * circuit_nodes_selected_find_maxx_maxy_maxz(struct simulation * sim,struct circuit *cir)
{
	int n;

	struct circuit_node *max_node=NULL;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	int first=TRUE;
	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);

		if (node->selected==TRUE)
		{
			if (first==TRUE)
			{
				max_node=node;
				first=FALSE;
			}

			if (max_node->x<=node->x)
			{
				if (max_node->y<=node->y)
				{
					if (max_node->z<=node->z)
					{
						max_node=node;
					}
				}
			}
		}

	}

	return max_node;
}

struct circuit_node * circuit_nodes_find_node_x_plus_one(struct simulation * sim,struct circuit *cir,struct circuit_node *base_node)
{
	int n;
	int min_delta=1000;
	int delta=0;
	struct circuit_node *next_node=NULL;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	//int first=TRUE;
	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);

		if (base_node->z==node->z)
		{
			if (base_node->y==node->y)
			{
				if (base_node->x<node->x)
				{
					delta=fabs(base_node->x-node->x);
					if (delta<min_delta)
					{
						next_node=node;
						min_delta=delta;
					}
				}
			}
		}

	}

	return next_node;
}

struct circuit_node * circuit_nodes_selected_find_miny(struct simulation * sim,struct circuit *cir,struct circuit_node *base_node)
{
	int n;
	struct circuit_node *next_node=base_node;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	//int first=TRUE;
	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);

		if (base_node->z==node->z)
		{
			if (base_node->x==node->x)
			{
				if (node->y<next_node->y)
				{
					next_node=node;
				}
			}
		}

	}

	return next_node;
}
