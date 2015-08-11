# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'sm.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindowSM(object):
    def setupUi(self, MainWindowSM):
        MainWindowSM.setObjectName(_fromUtf8("MainWindowSM"))
        MainWindowSM.resize(898, 897)
        self.centralwidget = QtGui.QWidget(MainWindowSM)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.gridLayout_3 = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout_3.setObjectName(_fromUtf8("gridLayout_3"))
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem)
        self.addStudyButton = QtGui.QPushButton(self.centralwidget)
        self.addStudyButton.setObjectName(_fromUtf8("addStudyButton"))
        self.horizontalLayout_2.addWidget(self.addStudyButton)
        self.addRunButton = QtGui.QPushButton(self.centralwidget)
        self.addRunButton.setMaximumSize(QtCore.QSize(100, 16777215))
        self.addRunButton.setObjectName(_fromUtf8("addRunButton"))
        self.horizontalLayout_2.addWidget(self.addRunButton)
        self.saveRunButton = QtGui.QPushButton(self.centralwidget)
        self.saveRunButton.setMaximumSize(QtCore.QSize(100, 16777215))
        self.saveRunButton.setObjectName(_fromUtf8("saveRunButton"))
        self.horizontalLayout_2.addWidget(self.saveRunButton)
        self.gridLayout_3.addLayout(self.horizontalLayout_2, 1, 0, 1, 1)
        self.tabWidget = QtGui.QTabWidget(self.centralwidget)
        self.tabWidget.setObjectName(_fromUtf8("tabWidget"))
        self.tabScenario = QtGui.QWidget()
        self.tabScenario.setObjectName(_fromUtf8("tabScenario"))
        self.gridLayout_2 = QtGui.QGridLayout(self.tabScenario)
        self.gridLayout_2.setObjectName(_fromUtf8("gridLayout_2"))
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.studyView = QtGui.QTableView(self.tabScenario)
        self.studyView.setObjectName(_fromUtf8("studyView"))
        self.gridLayout.addWidget(self.studyView, 1, 0, 1, 1)
        self.plainTextEdit = QtGui.QPlainTextEdit(self.tabScenario)
        self.plainTextEdit.setMaximumSize(QtCore.QSize(300, 16777215))
        self.plainTextEdit.setObjectName(_fromUtf8("plainTextEdit"))
        self.gridLayout.addWidget(self.plainTextEdit, 1, 1, 1, 1)
        self.label = QtGui.QLabel(self.tabScenario)
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.label_5 = QtGui.QLabel(self.tabScenario)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.gridLayout.addWidget(self.label_5, 0, 1, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout, 0, 0, 1, 1)
        self.tabWidget.addTab(self.tabScenario, _fromUtf8(""))
        self.tabRun = QtGui.QWidget()
        self.tabRun.setObjectName(_fromUtf8("tabRun"))
        self.verticalLayout = QtGui.QVBoxLayout(self.tabRun)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.label_2 = QtGui.QLabel(self.tabRun)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout_5.addWidget(self.label_2)
        self.verticalLayout.addLayout(self.horizontalLayout_5)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.runView = QtGui.QTableView(self.tabRun)
        self.runView.setObjectName(_fromUtf8("runView"))
        self.horizontalLayout.addWidget(self.runView)
        self.verticalLayout_2 = QtGui.QVBoxLayout()
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.label_4 = QtGui.QLabel(self.tabRun)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.verticalLayout_2.addWidget(self.label_4)
        self.textEdit_2 = QtGui.QTextEdit(self.tabRun)
        self.textEdit_2.setMaximumSize(QtCore.QSize(16777215, 100))
        self.textEdit_2.setObjectName(_fromUtf8("textEdit_2"))
        self.verticalLayout_2.addWidget(self.textEdit_2)
        self.label_3 = QtGui.QLabel(self.tabRun)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.verticalLayout_2.addWidget(self.label_3)
        self.textEdit = QtGui.QTextEdit(self.tabRun)
        self.textEdit.setObjectName(_fromUtf8("textEdit"))
        self.verticalLayout_2.addWidget(self.textEdit)
        self.horizontalLayout.addLayout(self.verticalLayout_2)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.tabWidget.addTab(self.tabRun, _fromUtf8(""))
        self.gridLayout_3.addWidget(self.tabWidget, 0, 0, 1, 1)
        MainWindowSM.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindowSM)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 898, 21))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        MainWindowSM.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindowSM)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindowSM.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindowSM)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(MainWindowSM)

    def retranslateUi(self, MainWindowSM):
        MainWindowSM.setWindowTitle(_translate("MainWindowSM", "POLARIS Scenario Manager", None))
        self.addStudyButton.setText(_translate("MainWindowSM", "Add Study", None))
        self.addRunButton.setText(_translate("MainWindowSM", "Add Run", None))
        self.saveRunButton.setText(_translate("MainWindowSM", "Save", None))
        self.label.setText(_translate("MainWindowSM", "Studies", None))
        self.label_5.setText(_translate("MainWindowSM", "Default Scenario", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabScenario), _translate("MainWindowSM", "Study", None))
        self.label_2.setText(_translate("MainWindowSM", "Run Attributes", None))
        self.label_4.setText(_translate("MainWindowSM", "Notes", None))
        self.label_3.setText(_translate("MainWindowSM", "Scenario", None))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabRun), _translate("MainWindowSM", "Run", None))

