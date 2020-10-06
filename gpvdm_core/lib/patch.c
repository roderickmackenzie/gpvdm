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

/** @file patch.c
	@brief Patch an input file from a patch file, used usually for fitting.
*/


#include <stdio.h>
#include <string.h>
#include <util.h>
#include <patch.h>
#include <dump_ctrl.h>
#include <inp.h>
#include <gpvdm_const.h>
#include <log.h>

static int unused __attribute__((unused));

void patch(struct simulation *sim,char *dest,char * patch_file)
{
char *temp;
char token[100];
char file[100];
char newtext[100];
char description[200];

struct inp_file config_file;
inp_init(sim,&config_file);
if (inp_load(sim,&config_file,patch_file)!=0)
{
	ewe(sim,"patch file %s not found\n",patch_file);
}


char filetoedit[200];

printf_log(sim,"Patch %s\n",patch_file);

int found=FALSE;

struct inp_file ifile;
inp_init(sim,&ifile);

do
{
	temp = inp_get_string(sim,&config_file);
	if (temp==NULL)
	{
		break;
	}

	if (strcmp(temp,"#end")==0)
	{
		break;
	}

	strcpy(token,temp);

	if (token[0]!='#')
	{
		ewe(sim,"error token does not begin with # >%s< >%s<\n",token,patch_file);
	}
	else
	{
		found=TRUE;
		strcpy(file,inp_get_string(sim,&config_file));
		strcpy(description,inp_get_string(sim,&config_file));
		strcpy(newtext,inp_get_string(sim,&config_file));

		if (inp_load_from_path(sim,&ifile,dest,file)!=0)
		{
			ewe(sim,"File %s %s not found to patch.\n",dest,file);
		}

		inp_replace(sim,&ifile,token,newtext);

	}

}while(1);

inp_free(sim,&ifile);
inp_free(sim,&config_file);

return;
}
