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

/** @file links.c
@brief Functions which work on links
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



int link_get_other_end(struct circuit *cir,struct circuit_link *link,int node)
{
	if (link->start==node)
	{
		return link->stop;
	}

	if (link->stop==node)
	{
		return link->start;
	}

	return -1;
}

struct circuit_link *circuit_add_link(struct simulation * sim,struct circuit *cir,struct circuit_link *in_link)
{
	struct circuit_link *link;
	struct circuit_node *nodes=cir->nodes;

	link=&(cir->links[cir->links_len]);

	memcpy(link, in_link, sizeof(struct circuit_link) );
	link->i=0.0;

	nodes[link->start].links[nodes[link->start].nlinks]=cir->links_len;
	nodes[link->start].nlinks++;

	nodes[link->stop].links[nodes[link->stop].nlinks]=cir->links_len;
	nodes[link->stop].nlinks++;

	cir->links_len++;

	if (cir->links_len>=cir->links_max)
	{
		cir->links_max+=1000;
		cir->links=realloc(cir->links,sizeof(struct circuit_link)*cir->links_max);
	}

	return link;
}

int circuit_find_link(struct simulation * sim,struct circuit *cir,int z0,int x0,int y0, int z1,int x1,int y1)
{
	int n0;
	int n1;
	int l;


	n0=circuit_find_node_by_xyz(sim,cir,x0,y0,z0);
	n1=circuit_find_node_by_xyz(sim,cir,x1,y1,z1);
	if ((n0==-1)||(n1==-1))
	{
		return -1;
	}

	struct circuit_link *links=(cir->links);
	for (l=0;l<cir->links_len;l++)
	{

		if (links[l].type=='R')
		{
			if ((links[l].start==n0)&&(links[l].stop==n1))
			{
				return l;
			}

			if ((links[l].stop==n0)&&(links[l].start==n1))
			{
				return l;
			}

		}

	}

	return -1;
}
