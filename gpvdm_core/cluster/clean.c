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

/** @file clean.c
@brief clean the cluster to get rid of old files
*/
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "inp.h"
#include "util.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include "inp.h"
#include "tx_packet.h"

int cmp_master_clean(int sock,struct tx_struct *data)
{
	char *dir;
	if (cmpstr_min(data->id,"gpvdm_master_clean")==0)
	{
		dir=calpath_get_store_path();
		printf("I want to delete %s\n",dir);
		if (strlen(dir)>4)		// / is not allowed!
		{
			remove_dir(dir);
		}

		struct tx_struct packet;
		tx_struct_init(&packet);
		tx_set_id(&packet,"gpvdm_slave_clean");
		broadcast_to_nodes(&packet);

		jobs_reset();
		return 0;
	}

return -1;
}

int cmp_slave_clean(int sock,struct tx_struct *data)
{
	char *dir;
	if (cmpstr_min(data->id,"gpvdm_slave_clean")==0)
	{
		dir=calpath_get_store_path();
		printf("I want to delete %s\n",dir);
		if (strlen(dir)>4)		// / is not allowed!
		{
			remove_dir(dir);
		}
		return 0;
	}

return -1;
}

