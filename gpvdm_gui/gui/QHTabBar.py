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

## @package QHTabBar
#  A horizontal toolbar tab because QT does not have one.
#

#qt
from PyQt5.QtWidgets import QMainWindow, QTextEdit, QAction, QApplication, QMenu
from PyQt5.QtGui import QIcon, QPainter, QFont, QColor,QMouseEvent
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QSizePolicy,QVBoxLayout,QPushButton,QDialog,QFileDialog,QToolBar,QLabel,QComboBox, QTabWidget,QStatusBar,QMenuBar, QTabBar, QStylePainter, QStyleOptionTab,QStyle
from PyQt5.QtCore import pyqtSignal
from icon_lib import icon_get
from gpvdm_json import gpvdm_data
import json
class QHTabBar(QTabBar):
	menu_click = pyqtSignal(QMouseEvent,int)
	paste = pyqtSignal()

	delete = pyqtSignal()
	rename = pyqtSignal()

	def menu_build(self):
		self.main_menu = QMenu(self)

		action=self.main_menu.addAction(icon_get("edit-copy"),_("Copy"))
		action.triggered.connect(self.do_copy)

		action=self.main_menu.addAction(icon_get("edit-paste"),_("Paste"))
		action.triggered.connect(self.do_paste)

		action=self.main_menu.addAction(icon_get("edit-delete"),_("Delete"))
		action.triggered.connect(self.do_delete)

		action=self.main_menu.addAction(icon_get("rename"),_("Rename"))
		action.triggered.connect(self.do_rename)
		#

	def do_delete(self):
		self.delete.emit()

	def do_rename(self):
		self.rename.emit()

	def do_copy(self):
		lines=None
		if self.obj_search_path!=None and self.obj_id!=None:
			search_path=eval(self.obj_search_path)
			obj=search_path.find_object_by_id(self.obj_id)
			lines=obj.gen_json()

		if lines!=None:
			lines[0]="\"data\": "+lines[0].split(":")[1]
			all_data=[]
			all_data.append("{")
			all_data.append("\"data_type\": \""+self.data_type+"\",")

			all_data.extend(lines)
			all_data.append("}")

			cb = QApplication.clipboard()
			cb.clear(mode=cb.Clipboard )
			cb.setText("\n".join(all_data), mode=cb.Clipboard)

	def do_paste(self):
		lines = QApplication.clipboard().text()
		#try:
		read_data=json.loads(lines)
		#except:
		#	return
		self.paste_data=read_data['data']
		self.paste.emit()

	def show_menu_tab(self,event,tab_number):
		self.main_menu.exec_(event.globalPos())

	def __init__(self,build_tb=False):
		QTabBar.__init__(self)

		if build_tb==True:
			self.box=QVBoxLayout()
			self.box.setSpacing(0)
			self.box.setContentsMargins(0, 0, 0, 0)
			self.setLayout(self.box)
			self.box_tb0=QToolBar()
			self.box_tb0.setIconSize(QSize(32, 32))
			self.box.addWidget(self.box_tb0)
			self.box_tb1=QToolBar()
			self.box_tb1.setIconSize(QSize(32, 32))
			self.box.addWidget(self.box_tb1)
			

			self.tb_copy = QAction(icon_get("edit-copy"), wrap_text(_("Copy"),3), self)
			self.box_tb0.addAction(self.tb_copy)

			self.tb_paste = QAction(icon_get("edit-paste"), wrap_text(_("Paste"),3), self)
			self.box_tb1.addAction(self.tb_paste)
			self.tb_copy.triggered.connect(self.do_copy)
			self.tb_paste.triggered.connect(self.do_paste)


		self.menu_build()

		self.data_type="none"
		self.paste_data=None

		self.obj_search_path=None
		self.obj_id=None


		self.setStyleSheet("QTabBar::tab { height: 35px; width: 140px; }")

	def paintEvent(self, event):
		painter = QStylePainter(self)
		option = QStyleOptionTab()

		#painter.begin(self)
		for index in range(self.count()):
			self.initStyleOption(option, index)
			tabRect = self.tabRect(index)
			tabRect.moveLeft(10)
			painter.drawControl(QStyle.CE_TabBarTabShape, option)
			painter.drawText(tabRect, Qt.AlignVCenter | Qt.TextDontClip, self.tabText(index))
		#painter.end()

	def mousePressEvent(self, event):
		if event.button() == Qt.RightButton:
			tab_index=self.tabAt(event.pos())
			#self.obj_id=tab.uid
			#self.menu_click.emit(event,self.tabAt(event.pos()))
			self.show_menu_tab(event,tab_index)
		QTabBar.mousePressEvent(self, event)


