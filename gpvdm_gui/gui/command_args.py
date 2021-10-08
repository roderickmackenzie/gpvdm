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


## @package command_args
#  Handle command line arguments.
#

import sys
import os

from import_archive import import_archive
from util import gpvdm_copy_src

from scan_io import scan_io
from const_ver import const_ver
from import_archive import import_scan_dirs
from make_man import make_man
from scan_tree import tree_gen

from cal_path import get_exe_command
from dat_file import dat_file
from plot_io import plot_load_info
from scan_plot import scan_gen_plot_data
from server_io import server_find_simulations_to_run
from clean_sim import clean_sim_dir
from win_lin import running_on_linux
from inp import inp_update_token_value
from device_lib_io import device_lib_replace
from cal_path import test_arg_for_sim_file
from cal_path import set_sim_path
from util_zip import archive_decompress
from util_zip import archive_compress
from scan_tree import tree_load_flat_list


from gui_enable import set_gui
from gui_enable import gui_get

from materials_io import archive_materials

import i18n
_ = i18n.language.gettext

import argparse
parser = argparse.ArgumentParser(epilog=_("Additional information about gpvdm is available at")+" https://www.gpvdm.com"+"\n"+_("Report bugs to:")+" roderick.mackenzie@nottingham.ac.uk")
parser.add_argument("--version", help=_("displays the current version"), action='store_true')
parser.add_argument("--ver", help=_("displays the current version"), action='store_true')
parser.add_argument("--replace", help=_("replaces file in device lib --replace file.inp path_to_device_lib"), nargs=2)
parser.add_argument("--clean", help=_("cleans the current simulation directory deleting .dat files but not  scan dirs"), action='store_true')
parser.add_argument("--export", help=_("export a simulation to a gz file"), nargs=1)
parser.add_argument("--makeman", help=_("Generate the manual pages referring to the output files.."), action='store_true')
parser.add_argument("--importscandirs", help=_("Only imports the scan directories."), nargs=1)
parser.add_argument("--cleanscandirs", help=_("Deletes the content of all scan directories."), nargs=1)
parser.add_argument("--patch", help=_("Patch a .gpvdm file with an older .gpvdm file."), nargs=2)
parser.add_argument("--patchfile", help=_("Patch an .inp file with an older .inp file. usage --patchfile dest_file base_file input_file"), nargs=3)
parser.add_argument("--importfile", help=_("usage --import abc.gpvdm ./path/to/output/ "), nargs=2)
parser.add_argument("--dumptab", help=_("Dumps simulation parameters as jpg, usage: --dump-tab output_path"), nargs=1)
parser.add_argument("--clonesrc", help=_("Clone the source code."), action='store_true')
parser.add_argument("--editvalue", help=_("edits a value in a .gpvdm archive. Usage --edit-value /path/to/sim.gpvdm #token_to_change new_value "), nargs=3)
parser.add_argument("--scanplot", help=_("Runs an oplot file, usage --scanplot /path/to/oplot/file.oplot "), nargs=1)
parser.add_argument("--load", help=_("Loads a simulation --load /path/containing/simulation/sim.gpvdm"), nargs=1)
parser.add_argument("--unpack", help=_("Extract the sim.gpvdm archive --unpack"), action='store_true')
parser.add_argument("--pack", help=_("Extract the sim.gpvdm archive --pack"), action='store_true')
parser.add_argument("--matcompress", help=_("Compresses the materials dir"), action='store_true')

if test_arg_for_sim_file()==False:
	args = parser.parse_args()

def command_args(argc,argv):
	if test_arg_for_sim_file()!=False:
		return

	if argc>=2:
		if args.version:
			print(const_ver())
			sys.exit(0)
		elif args.ver:
			print(const_ver())
			sys.exit(0)
		elif args.importscandirs:
			import_scan_dirs(os.getcwd(),args.importscandirs[0])
			exit(0)
		elif args.replace:
			device_lib_replace(args.replace[0],dir_name=args.replace[1])
			exit(0)
		elif args.clean:
			clean_sim_dir()
			sys.exit(0)
		elif args.makeman:
			make_man()
			sys.exit(0)
		elif args.cleanscandirs:
			scan=scans_io(os.getcwd())
			scan.clean_all()
			sys.exit(0)
		elif args.importfile:
			import_archive(args.importfile[0],os.path.join(os.getcwd(),"sim.gpvdm"),False)
			sys.exit(0)
		elif args.patch:
			import_archive(args.patch[0],args.patch[1],True)
			sys.exit(0)
		elif args.matcompress:
			archive_materials(os.path.join(os.getcwd(),"materials"))
			sys.exit(0)
		elif args.clonesrc:
			gpvdm_copy_src(clone-src[0])
			sys.exit(0)
		elif args.editvalue:
			inp_update_token_value(args.editvalue[0], args.editvalue[1], args.editvalue[2])
			sys.exit(0)
		elif args.load:
			set_sim_path(os.path.dirname(args.load[0]))
			#print("a")
		elif args.unpack:
			archive_decompress(os.path.join(os.getcwd(),"sim.gpvdm"),remove_gpvdm_file=False)
			sys.exit(0)
		elif args.pack:
			archive_compress(os.path.join(os.getcwd(),"sim.gpvdm"))
			sys.exit(0)
		elif args.scanplot:
			plot_token=dat_file()
			oplot_file=args.scan-plot[0]
			if plot_load_info(plot_token,oplot_file)==True:
				print("file0=",plot_token.file0,"<")
				plot_files, plot_labels, save_file = scan_gen_plot_data(plot_token,os.path.dirname(oplot_file))
				print("written data to",save_file)
			else:
				print("Problem loading oplot file")
			sys.exit(0)



