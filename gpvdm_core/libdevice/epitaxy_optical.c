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

/** @file epitaxy_optical.c
	@brief Load the epitaxy structure.
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include <shape.h>
#include <contacts.h>

/**
 * @brief Do the maths for the optical parts of the epitaxy
 *
 */

/**
 * @brief Get the height of the layers which are optically active. i.e. the full height of the device.
 *
 */
gdouble epitaxy_get_optical_length(struct epitaxy *in)
{
int i=0;
gdouble tot=0.0;

for (i=0;i<in->layers;i++)
{
	tot+=in->layer[i].width;
}

return tot;
}


long double epitaxy_get_optical_problem_start(struct epitaxy *in)
{
int i=0;
gdouble pos=0.0;
for (i=0;i<in->layers;i++)
{

	//printf("%d\n",in->layer[i].solve_optical_problem);
	//getchar();
	if (in->layer[i].solve_optical_problem==TRUE)
	{
		return pos;
	}
	pos+=in->layer[i].width;

}

return -1;
}


long double epitaxy_get_optical_problem_stop(struct epitaxy *in)
{
int i=0;
gdouble pos=0.0;
int found=FALSE;
for (i=0;i<in->layers;i++)
{


	if (in->layer[i].solve_optical_problem==TRUE)
	{
		found=TRUE;
	}

	if ((in->layer[i].solve_optical_problem==FALSE)&&(found==TRUE))
	{
		return pos;
	}

	pos+=in->layer[i].width;

	//printf("%d %d %Le %Le\n",i,in->layer[i].solve_optical_problem,pos,in->layer[i].width);
	//getchar();
}

if (found==TRUE)
{
	return pos;
}


return -1;
}

