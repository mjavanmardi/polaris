from PyQt4 import QtCore, QtGui
from ui_polaris import Ui_Polaris
from geo_tools import *
import os
import matplotlib
import numpy as np
import pylab as P
import random
import math
import mpl
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
from matplotlib.figure import Figure
try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s
# create the dialog for the plugin builder
class PolarisDialog(QtGui.QMainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self.conn = None
        self.tabs = {}
        self.layouts = {}
        # Set up the user interface from Designer.        
        self.ui = Ui_Polaris()
        self.ui.setupUi(self)
        self.recent_db = set()
        #self.ui.analyseButton.clicked.connect(self.analyse)
        self.ui.actionConnect_TestCase.triggered.connect(self.connect)
        self.ui.actionDraw.triggered.connect(self.draw_tt)
        self.ui.actionQuit.triggered.connect(self.fileQuit)
        self.connect()
        self.draw_all()
        #self.draw_tt()
    def fileQuit(self):
        self.close()
    def create_plot_tab(self,tab_tag, tab_name):
        self.tabs[tab_tag] = QtGui.QWidget()
        self.tabs[tab_tag].setObjectName(_fromUtf8(tab_name))
        layout_tag = tab_tag+"Layout"
        self.layouts[layout_tag] = QtGui.QVBoxLayout(self.tabs[tab_tag])
        self.layouts[layout_tag].setObjectName(_fromUtf8(layout_tag))
        self.ui.tabWidget.addTab(self.tabs[tab_tag], _fromUtf8(tab_name))                
    def load_recent(self):
        home = os.path.expanduser("~")
        home += "/.polaris"
        if not os.path.exists(home):
            os.makedirs(home)
            with open(home+'/recent', 'w') as fh: #create empty file
                pass
        with open(home+'/recent', 'r') as fh:
             self.recent_db.add(item)

    def draw_tt(self):
        x = []
        y = []
        tt = self.conn.execute("select * from TTime_Distribution order by TTime_minutes").fetchall()
        for item in tt:
            tt,count = item
            x.append(int(tt))
            y.append(int(count))
            #y.append(math.log10(int(count)))   
        self.create_plot_tab("ttTab", "Travel Time")
        fig = mpl.MyMplCanvas(self.tabs["ttTab"], width=5, height=4, dpi=100)
        self.layouts["ttTabLayout"].addWidget(fig.mpl_toolbar)
        self.layouts["ttTabLayout"].addWidget(fig)       
        fig.axes.plot(x,y, '*-')
        fig.axes.set_xlabel("Trevel Time (min)")
        fig.axes.set_ylabel("Vehicle Count")
    def treat_null(self, value):
        if value is None:
            return 0
        else:
            return value
    def draw_mode(self):
        data = self.conn.execute("select * from Mode_Distribution").fetchall()
        types = []
        autos = []
        transits = []
        for item in data:
            type, auto, transit, total = map(self.treat_null, item)
            type = type[0]
            autos.append(auto)
            types.append(type)
            transits.append(transit)
        self.create_plot_tab("atTab", "Activity Type")
        fig = mpl.MyMplCanvas(self.tabs["atTab"], width=5, height=4, dpi=100)
        self.layouts["atTabLayout"].addWidget(fig.mpl_toolbar)
        self.layouts["atTabLayout"].addWidget(fig)       

        N = len(types)
        ind = np.arange(N)  # the x locations for the groups
        width = 0.35       # the width of the bars
        fig.axes.set_xticks(ind+width)
        fig.axes.set_xticklabels( types )
        ar = fig.axes.bar(ind, autos, width, color='r')
        tr = fig.axes.bar(ind+width, transits, width, color='y')
        fig.axes.legend( (ar[0], tr[0]), ('Auto', 'Transit') )
        fig.axes.set_xlabel("Activity Type")
        fig.axes.set_ylabel("Trip Count")
    def draw_all(self):
        self.draw_tt()
        self.draw_mode()
    def connect(self):
        self.conn = sqlite3.connect("../test_case/chicago-Supply.sqlite") 
        self.conn.execute("attach database \'../test_case/chicago-Demand.sqlite\' as Demand") 
        #QtGui.QMessageBox.about(self, "My message box", "Connected")     
        self.ui.statusbar.showMessage("Connected to test_case/chicago-Supply.sqlite")        
    def analyse(self):
        pass
        conn = sqlite3.connect("../chicago-Supply.sqlite")
        c = conn.cursor()
        try:
            conn.enable_load_extension(1)
            conn.load_extension('./spatialite4.dll')
            total_nz = conn.execute("select count(*) from zone").fetchone()[0]
            self.ui.lineTotalZones.setText(str(total_nz))
            total_pop = conn.execute("select sum(pop_per) from ZoneLandUse").fetchone()[0]
            self.ui.lineTotal.setText(str(total_pop))
            conn.execute("drop table if exists EvacZones")
            conn.execute("create temporary table EvacZones as select * from zone, EvacPoly where Within(zone.geo, Transform(EvacPoly.geo, 26916)) and EvacPoly.id=1")
            evac_zones = conn.execute("select * from EvacZones").fetchall()
            self.ui.lineEvacZones.setText(str(len(evac_zones)))
            pop_hh_sum = conn.execute("select sum(pop_hh) from ZoneLandUse where zone in (select zone from EvacZones)").fetchone()[0]
            pop_per_sum = conn.execute("select sum(pop_per) from ZoneLandUse where zone in (select zone from EvacZones)").fetchone()[0]
            emp_tot_sum = conn.execute("select sum(emp_tot) from ZoneLandUse where zone in (select zone from EvacZones)").fetchone()[0]
            emp_ret_sum = conn.execute("select sum(emp_ret) from ZoneLandUse where zone in (select zone from EvacZones)").fetchone()[0]
            self.ui.lineEvacPop.setText(str(pop_hh_sum))
            self.ui.lineEvacEmp.setText(str(emp_tot_sum))
            self.ui.lineEvacHH.setText(str(pop_hh_sum))
            self.ui.lineEvacRet.setText(str(emp_ret_sum))
            rows=conn.execute("update HexGrid set geo=(select HexagonalGrid(Transform(geo,26916),1000) from EvacPoly where id=1)")
            conn.execute("delete from AEP")
            n_polys = int(conn.execute("select NumGeometries(geo) from HexGrid").fetchone()[0])
            self.ui.lineAEP.setText(str(n_polys))
            for i in range(1,n_polys+1):
                conn.execute("insert into AEP values (?,?,?,?,?)",(i,i,10,0,None,))
                conn.execute("update AEP set geo=(select Transform(Centroid(GeometryN(geo,?)),4326) from HexGrid) where pkuid=?", (i,i,))
            rt = ('LINK', 'GEO', 'LINK')
            pt = ('AEP', 'GEO', 'pkuid')
            conn.execute('select DiscardGeometryColumn(\'AEPLink\',\'geo\')')
            conn.execute('drop table if exists AEPLink')
            conn.execute('create table AEPLink (AEP int, link int)')
            conn.execute("select AddGeometryColumn('AEPLink', 'geo', 26916, 'LINESTRING')")
            res =  match(conn, rt, pt)
            count = 0
            conn.execute("create table if not exists EvacRoute (origins int, dest int)")
            # delete all of the routes layers
            for row in c.execute("select * from EvacRoute").fetchall():
                origin, dest = row
                name = "route_%s_%s"%(str(origin), str(dest))
                delete_table_layer(conn, name, 'Geometry')
            conn.execute("delete from EvacRoute")
            node_to_set = set()
            node_from = 15389
            for item in res:
                    if item[1] == None:
                            continue;
                    count+=1
                    link = item[1]
                    aep = item[0]
                    node_to = c.execute("select node_a from Link where link = ?", (link,)).fetchone()[0]
                    if node_to not in node_to_set:
                        node_to_set.add(node_to)
                        name = "route_%s_%s"%(str(node_from), str(node_to))
                        conn.execute("insert into EvacRoute values (?,?)",(node_from, node_to,))
                        conn.execute("create table %s as select * from HwyLink_net where NodeFrom=? and NodeTo=?"%(name), (node_from, node_to,))
                        conn.execute("select RecoverGeometryColumn(\'%s\', \'%s\', 26916, \'LINESTRING\', 'XY')"%(name, 'Geometry'))
                    conn.execute("insert into AEPLink values (?,?,?)", (aep, link, None,)) 
                    conn.execute("update AEPLink set geo="
                                 "(select Transform(MakeLine(Transform(AEP.geo,26916),ClosestPoint(Link.geo, Transform(AEP.geo,26916))),26916) from AEP, Link where AEP.pkuid=? and Link.Link=?) "
                                 "where "
                                 "AEPLink.AEP=? AND AEPLink.Link=?"
                                 , item+item )
                #conn.execute('delete from &s where Geometry is NULL')
            conn.commit()
            conn.close()
        except:
            conn.close()
            self.close()
            raise
        
        
        
         
