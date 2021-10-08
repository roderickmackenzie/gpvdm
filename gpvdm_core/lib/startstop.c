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

/** @file startstop.c
	@brief Pause the simulation if desired.
*/


#include <enabled_libs.h>
#include <stdio.h>
#include <time.h>
#include "sim.h"

static int unused __attribute__((unused));

void stop_start(struct simulation *sim,struct device *in)
{
struct timespec delay;

if (in->stop_start==TRUE)
{
	getchar();
}



if (in->start_stop_time!=0.0)
{
	double sec=(int)in->start_stop_time;
	double ns=(in->start_stop_time-(double)sec)*1e9;
	delay.tv_sec = (long int)sec;
	delay.tv_nsec = (long int)ns;

	if(nanosleep(&delay, NULL) < 0 )
	{
		ewe(sim,"Nano sleep failed \n");
	}
}


}
