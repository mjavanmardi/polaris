from PyQt4 import QtCore, QtGui
from polaris_dialog import PolarisDialog
if __name__ == '__main__':
    import sys

    app = QtGui.QApplication(sys.argv)
    window = PolarisDialog()
    window.show()
    sys.exit(app.exec_())
