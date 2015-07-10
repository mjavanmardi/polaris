from PyQt4 import QtCore, QtGui, QtSql
from PyQt4.QtCore import Qt, QVariant
class studyModel(QtSql.QSqlTableModel):
	def __init__(self, parent = None):
		super(studyModel, self).__init__(parent)
		self.setEditStrategy(QtSql.QSqlTableModel.OnFieldChange)
		self.setTable("study")
		self.select()

		# self.setHeaderData(0, Qt.Horizontal, "ID");
		# self.setHeaderData(1, Qt.Horizontal, "Name");
		# self.setHeaderData(3, Qt.Horizontal, "Date");
		
class runModel(QtSql.QSqlTableModel):
	def __init__(self, parent = None):
		super(runModel, self).__init__(parent)
		self.setEditStrategy(QtSql.QSqlTableModel.OnFieldChange)
		self.setTable("run")
		self.select()