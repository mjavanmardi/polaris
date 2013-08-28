from PyQt4.QtCore import *
from PyQt4.QtGui import *
from qgis.core import *
from polaris_dialog import PolarisDialog
# initialize Qt resources from file resouces.py
import resources_rc

class Polaris:
    def __init__(self, iface):
        # save reference to the QGIS interface
        self.iface = iface

    def initGui(self):
        # create action that will start plugin configuration
        self.action = QAction(QIcon(":/plugins/polaris/icons/logo.png"), "POLARIS", self.iface.mainWindow())
        self.action.setWhatsThis("Configuration for POLARIS plugin")
        self.action.setStatusTip("This is status tip")
        QObject.connect(self.action, SIGNAL("triggered()"), self.run)

        # add toolbar button and menu item
        self.iface.addToolBarIcon(self.action)
        self.iface.addPluginToMenu("&POLARIS", self.action)

        # connect to signal renderComplete which is emitted when canvas rendering is done
        QObject.connect(self.iface.mapCanvas(), SIGNAL("renderComplete(QPainter *)"), self.renderTest)

    def unload(self):
        # remove the plugin menu item and icon
        self.iface.removePluginMenu("&POLARIS",self.action)
        self.iface.removeToolBarIcon(self.action)

        # disconnect form signal of the canvas
        QObject.disconnect(self.iface.mapCanvas(), SIGNAL("renderComplete(QPainter *)"), self.renderTest)

    def run(self):
        pass
        # create and show a configuration dialog or something similar
        # print "IndTools: run called!"
        self.dlg = PolarisDialog()
        self.dlg.show()

    def renderTest(self, painter):
        pass
        # use painter for drawing to map canvas
        # print "IndTools: renderTest called!"