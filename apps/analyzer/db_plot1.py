# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_db_plot1.ui'
#
# Created: Tue Sep 10 15:21:44 2013
#      by: PyQt4 UI code generator 4.8.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_DBPlot1(object):
    def setupUi(self, DBPlot1):
        DBPlot1.setObjectName(_fromUtf8("DBPlot1"))
        DBPlot1.resize(640, 480)
        self.verticalLayout = QtGui.QVBoxLayout(DBPlot1)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.groupBox = QtGui.QGroupBox(DBPlot1)
        self.groupBox.setEnabled(True)
        self.groupBox.setMaximumSize(QtCore.QSize(350, 150))
        self.groupBox.setTitle(_fromUtf8(""))
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.gridLayout_2 = QtGui.QGridLayout(self.groupBox)
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.plotButton = QtGui.QPushButton(self.groupBox)
        self.plotButton.setMaximumSize(QtCore.QSize(50, 16777215))
        self.plotButton.setObjectName(_fromUtf8("plotButton"))
        self.gridLayout_2.addWidget(self.plotButton, 3, 0, 1, 1)
        self.label_2 = QtGui.QLabel(self.groupBox)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.gridLayout_2.addWidget(self.label_2, 1, 0, 1, 1)
        self.comboX = QtGui.QComboBox(self.groupBox)
        self.comboX.setMinimumSize(QtCore.QSize(200, 0))
        self.comboX.setMaximumSize(QtCore.QSize(250, 16777215))
        self.comboX.setObjectName(_fromUtf8("comboX"))
        self.gridLayout_2.addWidget(self.comboX, 1, 1, 1, 1)
        self.label_3 = QtGui.QLabel(self.groupBox)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.gridLayout_2.addWidget(self.label_3, 2, 0, 1, 1)
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout_2.addWidget(self.label, 0, 0, 1, 1)
        self.comboTable = QtGui.QComboBox(self.groupBox)
        self.comboTable.setMinimumSize(QtCore.QSize(200, 0))
        self.comboTable.setMaximumSize(QtCore.QSize(250, 16777215))
        self.comboTable.setObjectName(_fromUtf8("comboTable"))
        self.gridLayout_2.addWidget(self.comboTable, 0, 1, 1, 1)
        self.comboY = QtGui.QComboBox(self.groupBox)
        self.comboY.setMinimumSize(QtCore.QSize(200, 0))
        self.comboY.setMaximumSize(QtCore.QSize(250, 16777215))
        self.comboY.setObjectName(_fromUtf8("comboY"))
        self.gridLayout_2.addWidget(self.comboY, 2, 1, 1, 1)
        self.whereText = QtGui.QPlainTextEdit(self.groupBox)
        self.whereText.setMaximumBlockCount(0)
        self.whereText.setObjectName(_fromUtf8("whereText"))
        self.gridLayout_2.addWidget(self.whereText, 3, 1, 1, 1)
        self.verticalLayout.addWidget(self.groupBox)

        self.retranslateUi(DBPlot1)
        QtCore.QMetaObject.connectSlotsByName(DBPlot1)

    def retranslateUi(self, DBPlot1):
        DBPlot1.setWindowTitle(QtGui.QApplication.translate("DBPlot1", "Form", None, QtGui.QApplication.UnicodeUTF8))
        self.plotButton.setText(QtGui.QApplication.translate("DBPlot1", "Plot", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("DBPlot1", "X", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("DBPlot1", "Y", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("DBPlot1", "Table", None, QtGui.QApplication.UnicodeUTF8))
        self.whereText.setDocumentTitle(QtGui.QApplication.translate("DBPlot1", "Where Statement", None, QtGui.QApplication.UnicodeUTF8))

