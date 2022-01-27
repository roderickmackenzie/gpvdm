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

/** @file cal_resistance.c
@brief Calculates resistance for the shunt resistance of the diodes
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


void circuit_cal_resistance(struct simulation * sim,struct circuit *cir,struct device *dev)
{
	return;
	int x=0;
	int y=0;
	int z=0;
	int l=0;
	//int n=0;
	//struct circuit_node *node;
	//struct circuit_node *nodes=cir->nodes;
	struct light *li=&(dev->mylight);
	struct dimensions *ldim=&(li->dim);
	//struct dimensions *dim=&(dev->ns.dim);
	struct epitaxy *epi=&(dev->my_epitaxy);
	//struct circuit_link *links=(cir->links);
	struct circuit_link *link;
	struct object *obj;

	double tot=0.0;

	if (cir->enabled==FALSE)
	{
		return;
	}

	if (cir->abstract_circuit==TRUE)
	{
		return;
	}


	long double diode_length=0.0;

	for (l=0;l<epi->layers;l++)
	{

		if (strcmp(epi->layer[l].s.com.component,"diode")==0)
		{
			diode_length+=epi->layer[l].width;
		}
	}

	for (l=0;l<cir->links_len;l++)
	{
		link=&(cir->links[l]);
		if (link->id==1)		//I have defined the down links to have an ID of 1
		{
			tot=0.0;
			x=link->x;
			z=link->z;

			for (y=0;y<ldim->ylen;y++)
			{
					obj=li->obj[z][x][y];
					//if (obj->epi_layer!=-1)
					//{
						if (epi->layer[obj->epi_layer].electrical_layer==TRUE)
						{
							//printf("%d %s\n",obj->epi_layer,obj->name);
							tot+=ldim->dy;
						}
					//}
			}

			long double ratio=tot/diode_length;
			link->R=link->R*ratio+0.24390*(1.0-ratio);
			if (link->R<1e-5)
			{
				link->R=1e-5;
			}

			//printf("%le %d %Lf\n",link->R,link->id,ratio);

		}
	}



circuit_plot_resistance(sim,cir,dev);
//getchar();
}

