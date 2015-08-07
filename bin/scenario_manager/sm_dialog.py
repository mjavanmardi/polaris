from PyQt4 import QtCore, QtGui
from sm_ui import Ui_MainWindowSM
try:
	_fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
	_fromUtf8 = lambda s: s
import sqlite3
import datetime 
import time
from data_models import *
#some useful tutorials:
#http://www.qgisworkshop.org/html/workshop/python_in_qgis_tutorial2.html
#http://www.qgis.org/api/classQgsVectorDataProvider.html
#http://www.qgis.org/pyqgis-cookbook/index.html



# create the dialog for the plugin builder
class SMDialog(QtGui.QMainWindow):
	def __init__(self):
		QtGui.QMainWindow.__init__(self)
		# Set up the user interface from Designer.		
		self.ui = Ui_MainWindowSM()
		self.ui.setupUi(self)  
		self.conn = sqlite3.connect("sm.sqlite")
		self.ui.addRunButton.clicked.connect(self.addRun)
		self.ui.addStudyButton.clicked.connect(self.addStudy)
		self.study = None
		self.qtdb = QtSql.QSqlDatabase.addDatabase("QSQLITE")
		self.qtdb.setDatabaseName("sm.sqlite")
		print self.qtdb.open()
		self.studyModel = studyModel(); self.ui.studyView.setModel(self.studyModel)
		self.ui.studyView.hideColumn(2)
		self.ui.studyView.hideColumn(4)
		self.runModel = runModel();  self.ui.runView.setModel(self.runModel)
		self.ui.runView.hideColumn(0)
		self.ui.runView.hideColumn(5)
		self.ui.runView.hideColumn(6)
		self.connect(self.studyModel, QtCore.SIGNAL("primeInsert(QSqlRecord)"), self.insertStudyRow)
	def addStudy(self):
		max_id = self.conn.execute("select max(id) from study").fetchone()[0]
		if max_id is None:
			max_id = 0
		d = str(datetime.datetime.now())[0:16]
		# self.conn.execute("insert into study values (?,?,?,?,?)",(max_id+1, max_id+1, None, d, None,))
		# self.conn.commit()
		self.study = max_id+1
		# self.ui.runView.dataChanged()
		QtCore.QSqlRecord record = self.studyModel.record();
		record.setValue(1,QVariant(max_id+1));
		record.setValue(2,QVariant(max_id+1));
		record.setValue(2,QVariant(tr("")));
		record.setValue(2,QVariant(tr(d)));
		self.studyModel.insertRows(self.studyModel.rowCount(), 1, record)
		# self.studyModel.record(1).value("id")
		# index = QtCore.QModelIndex()
		# index.column = 1;
		# print self.studyModel.data(,1)
	def insertStudyRow(self, record):
		print "LALALA"
		# print record
	def addRun(self):
		if self.study is None:
			self.addStudy()
		scenario = self.conn.execute("select default_scenario from study where id=?",(self.study,)).fetchone()[0]
		max_id = self.conn.execute("select max(id) from run").fetchone()[0]
		if max_id is None:
			max_id = 1
		d = str(datetime.datetime.now())[0:16]
		self.conn.execute("insert into run values (?,?,?,?,?,?,?)",(max_id+1,self.study,None,max_id+1, d, scenario,None,))
		self.conn.commit()
