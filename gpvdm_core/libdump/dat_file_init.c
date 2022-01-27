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

/** @file buffer.c
@brief used to save output files to disk with a nice header, so the user knows what was writtne to them
*/

#include <enabled_libs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "dat_file.h"
#include "gpvdm_const.h"
#include "code_ctrl.h"
#include "cal_path.h"
#include "dump.h"
#include <log.h>
#include <lang.h>
//<strip>
#include <lock.h>
//</strip>

void dat_file_init(struct dat_file *in)
{
	in->write_to_zip=FALSE;
	in->norm_x_axis=FALSE;
	in->norm_y_axis=FALSE;
	in->data_max=NAN;
	in->data_min=NAN;
	in->len=0;
	in->max_len=0;
	in->max_len+=10240;
	strcpy(in->title,"");
	strcpy(in->type,"xy");
	strcpy(in->title,"");
	in->x_mul=1.0;
	in->y_mul=1.0;
	in->z_mul=1.0;
	in->x_offset=0.0;
	in->y_offset=0.0;
	in->z_offset=0.0;
	in->data_mul=1.0;
	strcpy(in->x_label,"");
	strcpy(in->y_label,"");
	strcpy(in->z_label,"");
	strcpy(in->data_label,"");
	strcpy(in->x_units,"");
	strcpy(in->y_units,"");
	strcpy(in->z_units,"");
	strcpy(in->rgb,"");
	strcpy(in->data_units,"");
	strcpy(in->section_one,"");
	strcpy(in->section_two,"");
	in->logscale_x=0;
	in->logscale_y=0;
	in->logscale_z=0;
	in->logscale_data=0;
	in->x=0;
	in->y=0;
	in->z=0;
	in->time= -1.0;
	in->Vexternal= -1.0;
	in->dump_gnu_plot_file=FALSE;
	strcpy(in->file_name,"");
	strcpy(in->cols,"");
	in->bin=FALSE;
	in->srh_bands=-1;
}

