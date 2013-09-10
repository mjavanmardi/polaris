# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_polaris.ui'
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

class Ui_Polaris(object):
    def setupUi(self, Polaris):
        Polaris.setObjectName(_fromUtf8("Polaris"))
        Polaris.resize(943, 531)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/plugins/polaris/icons/logo.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Polaris.setWindowIcon(icon)
        self.mainWidget = QtGui.QWidget(Polaris)
        self.mainWidget.setObjectName(_fromUtf8("mainWidget"))
        self.horizontalLayout_4 = QtGui.QHBoxLayout(self.mainWidget)
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.tabWidget = QtGui.QTabWidget(self.mainWidget)
        self.tabWidget.setObjectName(_fromUtf8("tabWidget"))
        self.horizontalLayout.addWidget(self.tabWidget)
        self.horizontalLayout_4.addLayout(self.horizontalLayout)
        Polaris.setCentralWidget(self.mainWidget)
        self.menubar = QtGui.QMenuBar(Polaris)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 943, 21))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        self.menuRecent_Databases = QtGui.QMenu(self.menuFile)
        self.menuRecent_Databases.setObjectName(_fromUtf8("menuRecent_Databases"))
        self.menuTools = QtGui.QMenu(self.menubar)
        self.menuTools.setObjectName(_fromUtf8("menuTools"))
        Polaris.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(Polaris)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        Polaris.setStatusBar(self.statusbar)
        self.actionDatabase = QtGui.QAction(Polaris)
        self.actionDatabase.setObjectName(_fromUtf8("actionDatabase"))
        self.actionA = QtGui.QAction(Polaris)
        self.actionA.setObjectName(_fromUtf8("actionA"))
        self.actionB = QtGui.QAction(Polaris)
        self.actionB.setObjectName(_fromUtf8("actionB"))
        self.actionConnect_TestCase = QtGui.QAction(Polaris)
        self.actionConnect_TestCase.setObjectName(_fromUtf8("actionConnect_TestCase"))
        self.actionDraw = QtGui.QAction(Polaris)
        self.actionDraw.setObjectName(_fromUtf8("actionDraw"))
        self.actionQuit = QtGui.QAction(Polaris)
        self.actionQuit.setObjectName(_fromUtf8("actionQuit"))
        self.actionNew_XY_DB_Plot = QtGui.QAction(Polaris)
        self.actionNew_XY_DB_Plot.setObjectName(_fromUtf8("actionNew_XY_DB_Plot"))
        self.actionNew_XY_DB_Plot_1 = QtGui.QAction(Polaris)
        self.actionNew_XY_DB_Plot_1.setObjectName(_fromUtf8("actionNew_XY_DB_Plot_1"))
        self.actionTravel_Time_Layer = QtGui.QAction(Polaris)
        self.actionTravel_Time_Layer.setObjectName(_fromUtf8("actionTravel_Time_Layer"))
        self.menuRecent_Databases.addAction(self.actionA)
        self.menuRecent_Databases.addAction(self.actionB)
        self.menuFile.addAction(self.actionDatabase)
        self.menuFile.addAction(self.menuRecent_Databases.menuAction())
        self.menuFile.addAction(self.actionConnect_TestCase)
        self.menuFile.addAction(self.actionDraw)
        self.menuFile.addAction(self.actionQuit)
        self.menuTools.addAction(self.actionNew_XY_DB_Plot)
        self.menuTools.addAction(self.actionNew_XY_DB_Plot_1)
        self.menuTools.addAction(self.actionTravel_Time_Layer)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuTools.menuAction())

        self.retranslateUi(Polaris)
        self.tabWidget.setCurrentIndex(-1)
        QtCore.QMetaObject.connectSlotsByName(Polaris)

    def retranslateUi(self, Polaris):
        Polaris.setWindowTitle(QtGui.QApplication.translate("Polaris", "POLARIS Aanalyzer", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("Polaris", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuRecent_Databases.setTitle(QtGui.QApplication.translate("Polaris", "Recent Databases", None, QtGui.QApplication.UnicodeUTF8))
        self.menuTools.setTitle(QtGui.QApplication.translate("Polaris", "Tools", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDatabase.setText(QtGui.QApplication.translate("Polaris", "Open Database", None, QtGui.QApplication.UnicodeUTF8))
        self.actionA.setText(QtGui.QApplication.translate("Polaris", "a", None, QtGui.QApplication.UnicodeUTF8))
        self.actionB.setText(QtGui.QApplication.translate("Polaris", "b", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConnect_TestCase.setText(QtGui.QApplication.translate("Polaris", "Connect TestCase", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDraw.setText(QtGui.QApplication.translate("Polaris", "Draw", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuit.setText(QtGui.QApplication.translate("Polaris", "Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_XY_DB_Plot.setText(QtGui.QApplication.translate("Polaris", "New XY DB Plot", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_XY_DB_Plot_1.setText(QtGui.QApplication.translate("Polaris", "New XY DB Plot 1", None, QtGui.QApplication.UnicodeUTF8))
        self.actionTravel_Time_Layer.setText(QtGui.QApplication.translate("Polaris", "Travel Time Layer", None, QtGui.QApplication.UnicodeUTF8))

import resources_rc
