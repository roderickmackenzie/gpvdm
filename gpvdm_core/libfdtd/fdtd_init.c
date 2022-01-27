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

/** @file fdtd_init.c
	@brief Init the fdtd struct.
*/

#include <enabled_libs.h>
#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inp.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>

#include "vec.h"

void fdtd_init(struct fdtd_data *data)
{
	struct dimensions *dim=&(data->dim);
	data->src_code=NULL;
	data->max_time=-1.0;
	data->max_ittr=-1;
	data->src_start=-1;
	data->src_stop=-1;
	data->lambda=-1;
	data->stop=1e-6;
	data->time=0.0;
	data->use_gpu=FALSE;
	data->excitation_mesh_point_x=-1;
	data->excitation_mesh_point_y=-1;
	data->step=0;
	data->escape=-1;
	dim_light_init(dim);

	data->Ex=NULL;
	data->Ex=NULL;
	data->Ey=NULL;
	data->Ez=NULL;

	data->Hx=NULL;
	data->Hy=NULL;
	data->Hz=NULL;

	data->Ex_last=NULL;
	data->Ey_last=NULL;
	data->Ez_last=NULL;

	data->Ex_last_last=NULL;
	data->Ey_last_last=NULL;
	data->Ez_last_last=NULL;

	data->Hx_last=NULL;
	data->Hy_last=NULL;
	data->Hz_last=NULL;

	data->epsilon_r=NULL;
	data->sigma=NULL;

	data->obj=NULL;

	data->excitation_type=-1;
	data->pulse_length=-1;
	data->excite_Ex=FALSE;
	data->excite_Ey=FALSE;
	data->excite_Ez=FALSE;

}
