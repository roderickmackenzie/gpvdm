//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/** @file zip_buffer.c
	@brief Writing and reading zip buffer
*/

#include <stdio.h>
#include <stdlib.h>

	#include <zlib.h>

#include <code_ctrl.h>
#include <sim.h>
#include <inp.h>
#include <util.h>
#include <dat_file.h>
#include <epitaxy.h>
#include <lang.h>

static int unused __attribute__((unused));

#include "dump.h"
#include "log.h"
#include "cal_path.h"


void write_zip_buffer(struct simulation *sim,char *outfile,long double *buf,int buf_len)
{
		gzFile file;
		file = gzopen (outfile, "w9b");
		gzwrite (file, (char*)buf, buf_len*sizeof(gdouble));
		gzclose (file);
	FILE * yes;
	yes = fopen (outfile, "ab");
	int temp1=buf_len*sizeof(gdouble);
	fwrite ((char*)&temp1, sizeof(int),1,yes);
	fclose (yes);

	//out = fopen(outfile, "wb");
	//fwrite((char*)buf, buf_len*sizeof(gdouble), 1, out);
	//fclose(out);

}


int read_zip_buffer(struct simulation *sim,char *file_name,long double **buf)
{
	int len;

	FILE *tl=fopen(file_name,"rb");
	if (tl==NULL)
	{
		return -1;
	}

	fseek(tl, -4, SEEK_END);
	if (fread((char*)&len, 4, 1, tl)==0)
	{
		return -1;
	}

	fclose(tl);

		gzFile file_in;
		file_in = gzopen (file_name, "rb");
		if (file_in==Z_NULL)
		{
			ewe(sim,_("File not found\n"));
		}



	int buf_len=len/sizeof(long double);

	(*buf)=(long double *)malloc(sizeof(long double)*buf_len);


		gzread (file_in, (char*)(*buf), len);
		gzclose(file_in);

	return buf_len;
}


