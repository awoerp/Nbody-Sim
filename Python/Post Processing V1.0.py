import matplotlib.pyplot as plt
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBody Sim v1.0\Data.txt")

P1x = []
P1y = []
Vx1 = []
Vy1 = []

P2x = []
P2y = []
Vx2 = []
Vy2 = []

P3x = []
P3y = []
Vx3 = []
Vy3 = []

step = 0
t = range(5000)

for i in range(5000):
    data = f.readline()
    splitData = data.split(' ')
    P1x.append(float(splitData[0]))
    P1y.append(float(splitData[1]))
    Vx1.append(float(splitData[2]))
    Vy1.append(float(splitData[3][:-2]))
    
    data = f.readline()
    splitData = data.split(' ')
    P2x.append(float(splitData[0]))
    P2y.append(float(splitData[1]))
    Vx2.append(float(splitData[2]))
    Vy2.append(float(splitData[3][:-2]))

   
    data = f.readline()
    splitData = data.split(' ')
    P3x.append(float(splitData[0]))
    P3y.append(float(splitData[1]))
    Vx2.append(float(splitData[2]))
    Vy2.append(float(splitData[3][:-2]))
    
    data = f.readline()
plt.clf()  
plt.plot(P1x,P1y)
plt.plot(P2x,P2y)
plt.plot(P3x,P3y)
plt.xlim(0,100)
plt.ylim(0,100)
plt.grid()


plt.show()