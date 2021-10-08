# 
#   General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#   model for 1st, 2nd and 3rd generation solar cells.
#   Copyright (C) 2008-2022 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#   
#   https://www.gpvdm.com
#   
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License v2.0, as published by
#   the Free Software Foundation.
#   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   


## @package http
#  This is used to fetch http files from the web, mainly used for updates under windows.
#

#import sys
import os
#import shutil
#import commands
#import urllib2
import socket
from socket import setdefaulttimeout
from socket import socket
from socket import error
from socket import AF_INET
from socket import SOCK_STREAM
from socket import SOL_SOCKET
from socket import SO_REUSEADDR
from socket import getdefaulttimeout
from urllib.parse import urlparse
import re
#import os


socket.setdefaulttimeout = 1.0
os.environ['no_proxy'] = '127.0.0.1,localhost'
linkRegex = re.compile('<a\s*href=[\'|"](.*?)[\'"].*?>')
CRLF = "\r\n\r\n"


def get_data_from_web(address):
	setdefaulttimeout(4.0)
	url = urlparse(address) #urlparse.
	HOST = url.netloc
	PORT = 80
	message=""

	try:
		s = socket(AF_INET, SOCK_STREAM)
	except:
		s = None

	s.settimeout(4.0)

	s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

	try:
		s.connect((HOST, PORT))
	except:
		s.close()
		s = None
	if s!=None:
		request="GET "+address+" HTTP/1.0" +CRLF
		request=request.encode('utf-8')
		
		s.send(request)
		data=""
		result = s.recv(10000)
		result=result.decode('utf-8')
		
		data=data+result

		while (len(result) > 0):
			result = s.recv(10000)
			result=result.decode('utf-8')
			data=data+result

		s.shutdown(1)
		s.close()
		message=data.split('\r\n\r\n', 1)[-1]

	return message

