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

/** @file josn_dump.c
	@brief Json file decoder
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include "inp.h"
#include "util.h"
#include "code_ctrl.h"
#include "gpvdm_const.h"
#include <log.h>
#include <cal_path.h>
#include "lock.h"
#include <json.h>

void json_string_init(struct json_string *in)
{
	in->data=NULL;
	in->len=0;
	in->pos=0;
	in->compact=FALSE;

}

void json_string_free(struct json_string *in)
{
	if (in->data!=NULL)
	{
		free(in->data);
	}

	json_string_init(in);
}

void json_cat_str(struct json_string *buf,char *data)
{
	int n=0;
	int str_len=strlen(data);
	int extend=1024*1000;
	int bytes_left=buf->len-buf->pos;
	int bytes_left_after=bytes_left-str_len;
	if (bytes_left_after<extend)
	{
		buf->len+=extend*2;
		buf->data=realloc(buf->data,buf->len*sizeof(char));
		//printf("alloca\n");
	}
	n=sprintf((buf->data+buf->pos),"%s",data);
	if (buf->compact==TRUE)
	{
		if (n>2)
		{
			if (buf->data[buf->pos+n-1]=='\n')
			{
				buf->data[buf->pos+n-1]=0;
				n--;
			}
		}
	}
	buf->pos+=n;

}

