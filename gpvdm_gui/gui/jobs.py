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



import os
from inp import inp_search_token_value
from status_icon import status_icon_stop


#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QIcon,QPalette
from PyQt5.QtWidgets import QWidget, QVBoxLayout,QProgressBar,QLabel,QDesktopWidget,QToolBar,QHBoxLayout,QAction, QSizePolicy,  QTableWidgetItem,QComboBox,QDialog,QAbstractItemView

from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget

from server import server_get

from gpvdm_tab import gpvdm_tab
import time

## @package jobs
#  A jobs viewer widget.
#

class jobs_view(QWidget):

	def __init__(self):
		QWidget.__init__(self)

		self.main_vbox=QVBoxLayout()
	
		self.tab = gpvdm_tab()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)

#		self.tab.cellChanged.connect(self.tab_changed)
		
		self.main_vbox.addWidget(self.tab)

		self.setLayout(self.main_vbox)

		self.myserver=server_get()
		self.myserver.jobs_update.connect(self.callback_cluster_get_jobs)

		self.show()

	def callback_cluster_get_jobs(self):

		self.tab.clear()
		self.tab.setColumnCount(7)
		self.tab.setRowCount(0)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([ _("name"), _("path"), _("ip"), _("start"), _("stop"), _("CPUs"), _("status")])


		for job in self.myserver.jobs:
			start_time=time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(job.start_time))
			self.tab.add([job.name,job.path,job.ip,start_time,job.stop,str(job.cpus),str(job.status)])








