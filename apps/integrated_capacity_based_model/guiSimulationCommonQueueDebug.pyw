#!/usr/bin/python
import sys
import collections
import csv
import time
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from math import *
from random import *

class SimuWindow(QWidget):
    def __init__(self,w,h,data,dt,numberOfCars):
        self.dt=dt
        self.data = data #Data is an array of pixmaps
        self.numberOfCars=numberOfCars
        self.isPlaying = False
        super(SimuWindow,self).__init__()
        self.setWindowTitle('POLARIS capacity model simulation')
        self.layout = QGridLayout(self)

        #Widget where the network is displayed
        self.label = QLabel()
        self.label.resize(w,h)

        #A slider to browse the pixmaps
        self.slider = QSlider(Qt.Horizontal)
        self.slider.setMinimum(0)
        self.slider.setMaximum(len(data)-1)

        #A slider to selct the ration simulation time / real time
        self.doubleBox = QDoubleSpinBox()
        self.doubleBox.setValue(1)

        self.rateLabel = QLabel('Ratio:Simulation_time:Real_time')

        #A button to play and stop the simulation
        self.playStopButton = QPushButton('Play')

        #A list of Variables updated in real time for user information
        self.listVar = QListWidget()
        self.listVar.addItem('Total number of cars : ' + str(self.numberOfCars))
        self.listVar.addItem('dt : '+str(self.dt)+'s')
        self.listVar.addItem('Current time : '+str(0)+'s')

        self.layout.addWidget(self.label,3,1,1,3)
        self.layout.addWidget(self.doubleBox,2,1)
        self.layout.addWidget(self.rateLabel,2,2)
        self.layout.addWidget(self.playStopButton,2,3)
        self.layout.addWidget(self.slider,1,1,1,3)
        self.layout.addWidget(self.listVar,1,4,3,1)

        #Conncection of the dynamic widgets
        self.slider.valueChanged[int].connect(self.refreshPixmap)
        self.playStopButton.clicked.connect(self.playStop)

        #We display the first pixmap and ze compute the display time
        if len(data) != 0:
                a = time.clock()
                self.refreshPixmap(0)
                self.refreshListVar(0)
                QApplication.processEvents()
                self.dispTime = (time.clock()-a)

        self.show()
    #Refreshing the current pixmap
    def refreshPixmap(self,pixmapId):
        self.label.setPixmap(self.data[pixmapId])
        self.slider.setSliderPosition(pixmapId)
        self.refreshListVar(pixmapId)
    #Stop if playing, play if stopped
    def playStop(self):
        if self.isPlaying:
            self.playStopButton.setText('Play')
            self.isPlaying = False
        else:
            it = self.slider.value()
            self.playStopButton.setText('Stop')
            self.isPlaying = True
            sleepTime = max(0.,self.dt / float(self.doubleBox.text())-self.dispTime)
            while self.isPlaying and it < len(self.data):
                self.refreshPixmap(it)
                QApplication.processEvents()
                time.sleep(sleepTime)
                it = it + 1
            self.isPlaying = False
            self.playStopButton.setText('Play')
    #Refresh the list of the variables displayed
    def refreshListVar(self,it):
        self.listVar.takeItem(2)
        self.listVar.addItem('Current time : '+str(self.dt*it)+'s')
        self.listVar.repaint()
    def keyPressEvent(self,event):
        #Setting 'v' as the play and stop keyboard button
        if type(event)==QKeyEvent and event.key() == Qt.Key_V:
            self.playStop()
        #Setting 'r' as reset button
        if type(event)==QKeyEvent and event.key() == Qt.Key_R and not self.isPlaying:
            self.refreshPixmap(0)

class Road:
    def __init__(self,a,_b,_numberOfLanes,_laneWidth):
        self.a=a
        b=_b
        self.numberOfLanes=_numberOfLanes
        self.laneWidth = _laneWidth
        #u is the local unite director vector of the road
        self.u=( (b[0]-a[0])/sqrt((b[0]-a[0])*(b[0]-a[0])+(b[1]-a[1])*(b[1]-a[1])) , (b[1]-a[1])/sqrt((b[0]-a[0])*(b[0]-a[0])+(b[1]-a[1])*(b[1]-a[1])) )
        #u is the local tangent vector of the road
        self.ut = (-self.u[1],self.u[0])
    #Return the car's coordinates in the abstract coordinate system
    def carCoord(self,l,numberLane):
        ux,uy = self.u
        utx,uty = self.ut
        return(self.a[0]+l*ux+self.laneWidth*(self.numberOfLanes -1-numberLane)*utx,self.a[1]+l*uy+self.laneWidth*(self.numberOfLanes-1-numberLane)*uty)
        
    
#Parsing the output file
def getCarsData(fileName):
    carSpeedProfiles = dict()
    numberOfCars=0
    with open(fileName) as csvfile:
        data = csv.reader(csvfile,delimiter = ';')
        for row in data:
            if int(row[4])!=-1:
                carId=(int(row[0]))
                if carId > numberOfCars:
                    numberOfCars = carId
                position=min(100.,float(row[6]))
                roadId=(int(row[4]))
                iqId=(float(row[5]))
                state = (int(row[3]))
                if iqId == -1:
                    iqId=0
                setData=(carId,position,roadId,iqId,state)
                if(float(row[1]) in carSpeedProfiles):
                    carSpeedProfiles[float(row[1])].append(setData)
                else:
                    carSpeedProfiles[float(row[1])] = [setData]
    return carSpeedProfiles, numberOfCars+1

#Converter from abstract coordinate system to windows coordinate system
def adaptCoord(x,y,dA,dB,hA,hB,w,h):
    return(dA+hA*x,h-(dB+hB*y))


#Drawing the test network on pixmap
def drawNetwork(pixmap,dA,dB,hA,hB,w,h,bgcolor):
    sizeTup = (dA,dB,hA,hB,w,h)
    pixmap.fill(bgcolor)
    painter = QPainter(pixmap)
    painter.setPen(QColor(0,0,0))
    painter.drawLine(*(adaptCoord(0,0,*sizeTup)+adaptCoord(214,0,*sizeTup)))
    painter.drawLine(*(adaptCoord(0,8,*sizeTup)+adaptCoord(100,8,*sizeTup)))
    painter.drawLine(*(adaptCoord(114,4,*sizeTup)+adaptCoord(214,4,*sizeTup)))
    painter.drawLine(*(adaptCoord(0,10,*sizeTup)+adaptCoord(100,10,*sizeTup)))
    painter.drawLine(*(adaptCoord(0,14,*sizeTup)+adaptCoord(100,14,*sizeTup)))
    painter.drawLine(*(adaptCoord(114,6,*sizeTup)+adaptCoord(214,6,*sizeTup)))
    painter.drawLine(*(adaptCoord(114,14,*sizeTup)+adaptCoord(214,14,*sizeTup)))
    painter.drawLine(*(adaptCoord(100,14,*sizeTup)+adaptCoord(100,114,*sizeTup)))
    painter.drawLine(*(adaptCoord(108,14,*sizeTup)+adaptCoord(108,114,*sizeTup)))
    painter.drawLine(*(adaptCoord(110,14,*sizeTup)+adaptCoord(110,114,*sizeTup)))
    painter.drawLine(*(adaptCoord(114,14,*sizeTup)+adaptCoord(114,114,*sizeTup)))
    painter.drawLine(*(adaptCoord(108,14,*sizeTup)+adaptCoord(110,14,*sizeTup)))
    painter.drawLine(*(adaptCoord(100,8,*sizeTup)+adaptCoord(100,10,*sizeTup)))
    painter.drawLine(*(adaptCoord(114,4,*sizeTup)+adaptCoord(114,6,*sizeTup)))
    del painter
    return pixmap

#Drawing a car on pixmap
def drawCar(pixmap,road,l,numberLane,sizeTup,color):
    (x,y) = road.carCoord(l,numberLane)
    (xt,yt) = (x+road.laneWidth*(road.u[0]+road.ut[0]),y+road.laneWidth*(road.u[1]+road.ut[1]))
    painter = QPainter(pixmap)
    (rx,ry) = adaptCoord(x,y,*sizeTup)
    (rxt,ryt) = adaptCoord(xt,yt,*sizeTup)
    painter.fillRect(rx,ry,rxt-rx,ryt-ry,color)
    #print(rx,ry,rxt-rx,ryt-ry)
    return pixmap

#Get the simulation time step
def getDt(od):
    keyArray = []
    for key in od:
        keyArray.append(key)
    dt = keyArray[1]-keyArray[0]
    return dt

def main():
    
    app = QApplication(sys.argv)

    dA = 100
    dB = 300
    hA=3
    hB=3
    w=800
    h=800
    
    testNetwork = dict()
    testNetwork[0] = Road((0,0),(100,0),2,4)
    testNetwork[1] = Road((100,14),(0,14),1,4)
    testNetwork[2] = Road((100,114),(100,14),2,4)
    testNetwork[3] = Road((114,14),(114,114),1,4)
    testNetwork[4] = Road((214,14),(114,14),2,4)
    testNetwork[5] = Road((114,0),(214,0),1,4)
    
    data,numberOfCars = getCarsData('output.txt')
    #Sorting data's keys by increasing order
    od=collections.OrderedDict(sorted(data.items()))
    dt=getDt(od)

    colors = []
    testWidget = QWidget()
    #Get background color
    bgcolor = testWidget.palette().color(QPalette.Background)
    #Creating a random color for each car
    for i in range(numberOfCars):
        r = randint(0,255)
        g = randint(0,255)
        b = randint(0,255)
        colors.append(QColor(r,g,b))

    #Generating the pixmaps
    frames=[]
    for timeStep in od:
        pixmap = QPixmap(QSize(w,h))
        pixmap = drawNetwork(pixmap,dA,dB,hA,hB,w,h,bgcolor)
        param = (dA,dB,hA,hB,w,h)
        for car in data[timeStep]:
            carId,position,roadId,iqId,state = car
            color = colors[carId]
            if state == 2:
                color = QColor(0,0,0)
            if state == 3:
                color = QColor(255,0,0)
            pixmap = drawCar(pixmap,testNetwork[roadId],position,iqId,param,color)
        frames.append(pixmap)
    #Building the window
    window = SimuWindow(w,h,frames,dt,numberOfCars)
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
