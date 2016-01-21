import csv
import matplotlib.pyplot as plt
import numpy as np
import sys, string, os
from random import *

def getData(fileName):
    carSpeedProfiles = dict()
    with open(fileName) as csvfile:
        data = csv.reader(csvfile,delimiter = ';')
        for row in data:
            if(int(row[0]) in carSpeedProfiles):
                time,position,state = carSpeedProfiles[int(row[0])]
                time.append(float(row[1]))
                position.append(float(row[2]))
                state.append(float(row[3]))
            else:
                time = [float(row[1])]
                position = [float(row[2])]
                state = [float(row[3])]
            carSpeedProfiles[int(row[0])] = (time,position,state)
    return carSpeedProfiles



def plotData(data,i):
    time,position,state = data[i]
    timeS = time[1:]
    speed = np.subtract(position[1:],position[:len(position)-1])
    plt.subplot(3,1,1)
    plt.title('Position - Vehicle id : '+str(i))
    plt.plot(time[:],position[:],label='Vehicle number : '+str(i))
    plt.subplot(3,1,2)
    plt.title('Speed - Vehicle id : '+str(i))
    plt.plot(timeS[:],speed[:],label='Speed - Vehicle number : '+str(i))
    plt.subplot(3,1,3)
    plt.title('State - Vehicle id : '+str(i))
    plt.plot(time[:],state[:],label='State - Vehicle number : '+str(i))
    #plt.show()

#os.system("integrated_capacity_based_model.exe")
data = getData('output.txt')
n = randint(0,len(data)-1)
for i in range(1):
    #plotData(data,92+i)
    plotData(data,n)
plt.show()
