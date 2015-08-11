import PyQt4.QtGui
import sys
from sm_dialog import SMDialog

app = PyQt4.QtGui.QApplication(sys.argv)
dl = SMDialog()
dl.show()
app.exec_()