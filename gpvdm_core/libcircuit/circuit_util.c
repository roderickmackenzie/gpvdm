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

/** @file circuit_util.c
@brief Utility functions circuit solver
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

void circuit_calculate_matrix_pos(struct simulation * sim,struct circuit *cir)
{
	int n=0;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	cir->unknowns=0;
	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);
		if (node->type==CIR_UNKNOWN)
		{
			node->matrix_pos=cir->unknowns++;
		}
	}
}



int circuit_cal_max_y_node_level(struct simulation * sim,struct circuit *cir,struct device *dev)
{
	int n=0;
	int max=0;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;

	for (n=0;n<cir->nodes_len;n++)
	{
		node=&(nodes[n]);
		if (node->y>max)
		{
			max=node->y;
		}

	}

	return max;
}

void circuit_cal_device_resistance(struct simulation * sim,struct device *dev)
{
	int l=0;
	struct epitaxy *epi=&(dev->my_epitaxy);
	long double dl=0.0;
	long double Rtot=0.0;
	long double R=0.0;
	printf("Layer resistance\n");
	printf("----------------\n");

	for (l=0;l<epi->layers;l++)
	{
		dl=dev->my_epitaxy.layer[l].width;
		R=(dl*dev->my_epitaxy.layer[l].s.com.series_y)/dev->area;
		printf("%s %Lf\n",dev->my_epitaxy.layer[l].s.name,R);
		Rtot+=R;
	}
	printf("----------------\n");

	printf("Rtot=%Lf\n",Rtot);
	printf("----------------\n");

}
