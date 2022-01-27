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

/** @file cal_j0.c
@brief Calculates j0 for the didoes
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


void circuit_apply_photo_generation(struct simulation * sim,struct circuit *cir,struct device *dev)
{
	int x=0;
	int y=0;
	int z=0;
	int l=0;
	int nlink=0;

	//int n=0;
	//struct circuit_node *node;
	//struct circuit_node *nodes=cir->nodes;
	struct light *li=&(dev->mylight);
	struct dimensions *dim=&(dev->ns.dim);
	struct dimensions *ldim=&(li->dim);
	//struct dimensions *dim=&(dev->ns.dim);
	//struct epitaxy *epi=&(dev->my_epitaxy);
	//struct circuit_link *links=(cir->links);
	struct circuit_link *link;
	struct object *obj;
	//struct component *com;
	//double *tot;
	double sum;

	for (nlink=0;nlink<cir->links_len;nlink++)
	{
		sum=0.0;

		link=&(cir->links[nlink]);

		if (link->enable_Isc==TRUE)
		{
			x=link->x;
			z=link->z;
			for (y=0;y<ldim->ylen;y++)
			{
				obj=li->obj[z][x][y];
				l=obj->s->epi_index;
				//printf("%d %d\n",l,link->layer);
				if (l==link->layer)
				{
					sum+=Qe*(li->Gn[z][x][y]+li->Gp[z][x][y])*ldim->dy*dim->dX[x]*dim->dZ[z]*li->Dphotoneff/2.0;
					//printf("%d %d %d %le %Le\n",z,x,y,sum,li->Gn[z][x][y]);
				}//else
				//{
				//	printf("%d\n",y);
				//}

			}
		//getchar();
		//printf("%c %d %d %d %d %le %Le %Le\n",link->type,link->enable_Isc,nlink,x,z,sum,dim->dx[x],dim->dZ[z]);
		}
		link->Isc=sum;


	}

	/*for (y=0;y<ldim->ylen;y++)
	{
		printf("%d %Le\n",y,li->Gn[z][x][y]);
	}
getchar();*/
}

