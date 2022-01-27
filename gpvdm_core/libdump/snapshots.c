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

/** @file dump_snapshot_utils.c
@brief Utils to handle the snapshots dir.
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <pl.h>
#include <probe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <inp.h>
#include <util.h>

static int unused __attribute__((unused));

void snapshots_init(struct snapshots *snap)
{
	strcpy(snap->icon,"cover_flow");
	strcpy(snap->type,"snapshots");
	strcpy(snap->plot_type,"2d");
	strcpy(snap->name,"snapshots");
	strcpy(snap->path,"none");
}

void dump_remove_snapshots(struct simulation* sim, char *output_path)
{
	char test_dir[PATH_MAX];
	char test_file[PATH_MAX];

	DIR *d;
	struct dirent *dir;

	d = opendir(output_path);
	if (d)
	{

		while ((dir = readdir(d)) != NULL)
		{
			join_path(2,test_dir,output_path,dir->d_name);
			if (isdir(test_dir)==0)
			{
				join_path(3,test_file,output_path,dir->d_name,"data.json");

				if (isfile(test_file)==0)
				{
					printf("delete: %s\n",test_dir);

					remove_dir(sim,test_dir);
				}
			}
		}
		closedir(d);
	}

}

void dump_make_snapshot_dir(struct simulation *sim,char *ret_path, int number,struct snapshots *snap)
{

	struct dat_file buf;
	char temp[200];
	char sub_dir[200];
	char main_snapshots_dir[PATH_MAX];

	dat_file_init(&buf);

	join_path(2,main_snapshots_dir,snap->path,snap->name);
	gpvdm_mkdir(main_snapshots_dir);

	buffer_malloc(&buf);

	buffer_add_string(&buf,"{\n");
	sprintf(temp," \"icon\":\"%s\",\n",snap->icon);
	buffer_add_string(&buf,temp);
	buffer_add_string(&buf," \"item_type\":\"snapshots\",\n");
	sprintf(temp," \"default_plot_type\":\"%s\"\n",snap->plot_type);
	buffer_add_string(&buf,temp);
	buffer_add_string(&buf,"}\n");
	buffer_dump_path(sim,main_snapshots_dir,"data.json",&buf);
	buffer_free(&buf);


	sprintf(sub_dir,"%d",number);
	join_path(2,ret_path,main_snapshots_dir,sub_dir);
	gpvdm_mkdir(ret_path);


}


