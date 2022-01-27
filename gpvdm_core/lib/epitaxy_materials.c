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

/** @file epitaxy_materials.c
	@brief Load the materials into the epitaxy structure.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "util.h"
#include "gpvdm_const.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "epitaxy.h"
#include <lang.h>
#include "log.h"
#include <cal_path.h>
#include <dat_file.h>
#include <color.h>



static int unused __attribute__((unused));

void epitaxy_load_emission(struct simulation *sim,struct epi_layer *layer)
{
	char temp_file[100];
	struct dat_file buf;
	long double max;
	long double X;
	long double Y;
	long double Z;
	int R;
	int G;
	int B;
	int max_pos;

	if (inp_isfile(sim,layer->pl_spectrum_file)!=0)
	{
		ewe(sim,"The emission file %s does not exist",layer->pl_spectrum_file);
	}

	inter_load(sim,&(layer->pl_spectrum),layer->pl_spectrum_file);
	inter_sort(&(layer->pl_spectrum));

	max=math_xy_get_max(&(layer->pl_spectrum));

	inter_div_long_double(&(layer->pl_spectrum),max);
	math_xy_mul_long_double(&(layer->pl_spectrum),layer->pl_experimental_emission_efficiency);

	max_pos=inter_get_max_pos(&(layer->pl_spectrum));
	layer->peak_wavelength=layer->pl_spectrum.x[max_pos];


	int min_pos=0;
	for (min_pos=0;min_pos<layer->pl_spectrum.len;min_pos++)
	{
		if (layer->pl_spectrum.data[min_pos]>max*0.05)
		{
			break;
		}
	}


	for (max_pos=layer->pl_spectrum.len-1;max_pos>0;max_pos--)
	{
		if (layer->pl_spectrum.data[max_pos]>max*0.05)
		{
			break;
		}
	}

	//printf("%ld %ld\n",layer->pl_spectrum.len,max_pos);
	//getchar();
	inter_chop(&(layer->pl_spectrum),layer->pl_spectrum.x[min_pos], layer->pl_spectrum.x[max_pos]);

	color_cie_cal_XYZ(sim,&X,&Y,&Z,&(layer->pl_spectrum),FALSE);

	color_XYZ_to_rgb(&R,&G,&B,X,Y,Z);

	dat_file_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.y_mul=1e9;

	sprintf(buf.title,"The input emission spectrum for layer %s",layer->s.name);

	strcpy(buf.type,"linegraph");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Probability");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"a.u.");


	sprintf(buf.rgb,"%.2x%.2x%.2x",R,G,B);

	buf.logscale_x=0;
	buf.logscale_y=0;
	dat_file_add_xy_data(sim,&buf,layer->pl_spectrum.x, layer->pl_spectrum.data, layer->pl_spectrum.len);
	sprintf(temp_file,"emission_input_%d.dat",layer->layer_number);
	buffer_dump_path(sim,"",temp_file,&buf);
	buffer_free(&buf);

}




