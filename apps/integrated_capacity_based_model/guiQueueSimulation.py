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
        
class Car:
    def __init__(self,ide,initPos,carac):
        #maxAcc,maxDec,beta,delta,gamma,tau,dz = carac
        self.ide=ide
        self.pos=initPos
        self.speed=0.
        self.carac=carac
    def moveFreeFlow(self,d,dt,maxSpeed):
        #Free-flow model
        if d <= 0:
            self.speed = 0
            return
        maxAcc,maxDec,beta,delta,gamma,tau,dz = self.carac
        alpha = 0
        #Computing threshold distance
        nBraking = (self.speed/(beta*maxDec*dt))
        dBraking = nBraking*dt*(self.speed-nBraking*beta*maxDec*dt/2)
        nBrakingE = (self.speed/(maxDec*dt))
        dBrakingE = nBrakingE*dt*(self.speed-nBrakingE*maxDec*dt/2)
        if(d<=dBraking):
            #Brake
            if(d<=dBrakingE):
                alpha = self.speed * self.speed / (2*dBrakingE)
                alpha = -max(0,min(maxDec,alpha))
            else:
                beta = beta*gamma
                alpha = -1 * beta * maxDec
            #print(alpha)
        else:
            #Accelerate
            if maxSpeed <= self.speed:
                alpha = 0
            else:
                alpha = maxAcc*(1-self.speed/maxSpeed*(1-delta))
        self.speed = max(0,min(maxSpeed,self.speed+dt*alpha))
        self.pos = self.pos + self.speed * dt
    def movQueuing(self,xFront,vFront,dt,maxSpeed):
        #Queue following model
        maxAcc,maxDec,beta,delta,gamma,tau,dz = self.carac
        safeDistance=tau*vFront+maxSpeed/(2*maxDec)*(self.speed-vFront)+dz
        error = (xFront-self.pos) - safeDistance
        dv = min(self.speed+maxAcc*dt,max(self.speed-1*maxDec*dt,error/dt))
        self.speed = max(0,min(maxSpeed,dv))
        self.pos = self.pos + self.speed * dt
        

#Converter from abstract coordinate system to windows coordinate system
def adaptCoord(x,y,dA,dB,hA,hB,w,h):
    return(dA+hA*x,h-(dB+hB*y))


#Drawing the test network on pixmap
def drawNetwork(pixmap,dA,dB,hA,hB,w,h,bgcolor):
    sizeTup = (dA,dB,hA,hB,w,h)
    pixmap.fill(bgcolor)
    painter = QPainter(pixmap)
    painter.setPen(QColor(0,0,0))
    painter.drawLine(*(adaptCoord(0,0,*sizeTup)+adaptCoord(100,0,*sizeTup)))
    painter.drawLine(*(adaptCoord(0,8,*sizeTup)+adaptCoord(100,8,*sizeTup)))
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

def main():
    
    app = QApplication(sys.argv)

    #Window parameters
    dA = 100
    dB = 300
    hA = 6
    hB = 6
    w = 800
    h = 800

    dt = 0.5 #Time step
    maxSpeed = 13.8 #Max speed on the road

    dz=5. #Distance between two vehicles stopped
    tau=2. #Car+driver reaction time
    maxDec=6.23 #Max deceleration of the car
    maxAcc=3.31 #Max acceleration of the car 
    beta=0.58 #Free-flow parameter
    delta=0.38 #Free-flow parameter
    gamma=0.69 #Free-flow parameter
    
    carac = maxAcc,maxDec,beta,delta,gamma,tau,dz
   
    #Building the test road 
    testNetwork = dict()
    testNetwork[0] = Road((0,0),(100,0),2,4)
    
    #Create cars (id,initialPosition,carac)
    cara=Car(0,40,carac)
    carb=Car(1,35,carac)
    carc=Car(2,30,carac)
    card=Car(3,25,carac)
    care=Car(4,20,carac)
    carf=Car(5,15,carac)
    carg=Car(6,10,carac)
    carh=Car(7,5,carac)
    cari=Car(8,0,carac)
    data=[cara,carb,carc,card,care,carf,carg,carh,cari]
    carNb = len(data)   
 
    colors = []
    testWidget = QWidget()
    #Get background color
    bgcolor = testWidget.palette().color(QPalette.Background)
    #Creating a random color for each car
    for i in range(len(data)):
        r = randint(0,255)
        g = randint(0,255)
        b = randint(0,255)
        colors.append(QColor(r,g,b))

    #Generating the pixmaps
    frames=[]
    passed = 0
    while len(data) != 0: #While all the cars are not out yet
        carPasses=[]
        pixmap = QPixmap(QSize(w,h))
        pixmap = drawNetwork(pixmap,dA,dB,hA,hB,w,h,bgcolor)
        param = (dA,dB,hA,hB,w,h)
        i=0
        while i  < (len(data)):
            car = data[i]
            carPasses.append(car.ide)
            if i==0:
                #The first car of the queue runs in free flow
                dToStop=0
                #Implementation of a brake point for cara
                #Stays at 70m for 10s
                if car.ide == 0 and passed < 10: 
                    dToStop = 70-car.pos
                else:
                    dToStop = 104
                if car.ide == 0 and car.speed == 0:
                    passed = passed + dt
                car.moveFreeFlow(dToStop,dt,maxSpeed)
                color = colors[car.ide]
                pixmap = drawCar(pixmap,testNetwork[0],car.pos,0,param,color)
                #Removing the first car of the queue when it's out
                if car.pos >= 104:
                    data.remove(car)
                    i=0
                else:
                    i = i+1
            else:
                #The other cars use the queue following model
                car.movQueuing(data[i-1].pos,data[i-1].speed,dt,maxSpeed)
                color = colors[car.ide]
                pixmap = drawCar(pixmap,testNetwork[0],car.pos,0,param,color)
                i=i+1
        frames.append(pixmap)
    #Building the window
    window = SimuWindow(w,h,frames,dt,carNb)
    
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
