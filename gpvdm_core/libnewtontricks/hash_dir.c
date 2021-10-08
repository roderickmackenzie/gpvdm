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

/** @file hash.c
	@brief Hashing function for fast lookup in tables, but the arrays are now linear so you don't need to hash.
*/

//<stripall>

#include <stdio.h>
#include "sim.h"
#include "inp.h"
#include "cal_path.h"
#include "list.h"
#include "md5.h"

void hash_dir(struct simulation *sim,char *out)
{
	/*int do_md5=FALSE;
	struct md5 sum;
	md5_init(&sum);

	int i=0;
	char *buffer;
	unsigned int len;
	long l;
	char newcheck[100];
	struct list files;
	struct inp_file inp;
	inp_listdir(sim, get_input_path(sim),&files);


	for (i=0;i<files.len;i++)
	{
		do_md5=FALSE;

		if (strcmp(files.names[i],"light.inp")==0)
		{
			do_md5=TRUE;
		}

		if (is_numbered_file(files.names[i],"dos")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"mesh_x.inp")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"mesh_y.inp")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"mesh_z.inp")==0)
		{
			do_md5=TRUE;
		}

		if (strcmp(files.names[i],"epitaxy.inp")==0)
		{
			do_md5=TRUE;
		}

		if (do_md5==TRUE)
		{
			inp_read_buffer(sim,&buffer, &l,files.names[i]);
			len=(unsigned int)l;
			
			md5_update(&sum,buffer,len);

			free(buffer);
		}else
		if (strcmp(files.names[i],"contacts.inp")==0)
		{
			inp_init(sim,&inp);
			inp_load(sim,&inp,files.names[i]);
			inp_replace(sim,&inp,"#contact_voltage0", "");
			inp_replace(sim,&inp,"#contact_voltage1", "");
			inp_replace(sim,&inp,"#contact_voltage2", "");
			inp_replace(sim,&inp,"#contact_voltage3", "");
			inp_replace(sim,&inp,"#contact_voltage4", "");
			inp_replace(sim,&inp,"#contact_voltage5", "");
			inp_replace(sim,&inp,"#contact_voltage6", "");
			inp_replace(sim,&inp,"#contact_voltage7", "");
			md5_update(&sum,inp.data,inp.fsize);
			inp_free_no_save(sim,&inp);
		}


	}


	list_free(&files);

	md5_to_str(out,&sum);*/
}

