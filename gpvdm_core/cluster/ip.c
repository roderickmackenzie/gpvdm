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

/** @file ip.c
@brief functions to figure out IP addresses
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
#include<pthread.h>

static char my_ip[20];

char* get_my_ip()
{
	return my_ip;
}

int cal_my_ip(int sock)
{

/*   int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    //Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;

    //Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name , interface , IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    strcpy(my_ip,inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr));

  return 0;*/

	int z;
	struct sockaddr_in adr_inet;
	unsigned int len_inet=0;

	len_inet = sizeof adr_inet;

	z = getsockname(sock, (struct sockaddr *)&adr_inet, &len_inet);
	if ( z == -1)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	strcpy(my_ip,inet_ntoa(adr_inet.sin_addr));
	return 0;
}

int get_ip_from_sock(char *out,int sock)
{
	int z;
	struct sockaddr_in adr_inet;
	unsigned int len_inet=0;

	len_inet = sizeof adr_inet;

	z = getpeername(sock, (struct sockaddr *)&adr_inet, &len_inet);
	if ( z == -1)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	strcpy(out,inet_ntoa(adr_inet.sin_addr));
	return 0;
}
