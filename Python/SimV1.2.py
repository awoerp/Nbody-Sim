#Partially works with "SimV1.2"

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\SimV1.2\TestData.txt")

numBodies = int(f.readline()[:-1])
iterations = int(f.readline()[:-1])
MAX_PLANET_SIZE = 500.

window = 300

numBodiesList = []


class Body:
    def __init__(self):
        self.Px = []
        self.Py = []
        self.Vx = []
        self.Vy = []
        self.mass = 0
        
    def GetPx(self):
        return self.Px
    
    def GetPy(self):
        return self.Py
        
    def AppendPx(self,val):
        self.Px.append(val)
    
    def AppendPy(self,val):
        self.Py.append(val)
        
    def AppendVx(self,val):
        self.Vx.append(val)
        
    def AppendVy(self,val):
        self.Vy.append(val)
    
    def ReadData(self, inFile):
        rawData = inFile.readline()
        data = rawData.split(' ')
        
        self.AppendPx(float(data[0]))
        self.AppendPy(float(data[1]))
        self.AppendVx(float(data[2]))
        self.AppendVy(float(data[3]))
        self.Mass = float(data[4][:-1])
        
    def ReadMass(self, inFile):
        temp = float(inFile.readline()[:-1])
        self.mass = temp
    
    def GetMass(self):
        return self.mass
        




bodyList = []
for i in range(numBodies):
    bodyList.append(Body())

def RemoveBody(index):
    print("Removing ", index, " Body")
    for i in range(index, numBodies - 1):
        bodyList[i] = bodyList[i+1]
    




for i in range(numBodies):
    bodyList[i].ReadMass(f)
    
for i in range(iterations - 1):
    test = f.readline()
    while test[0:2] == "Bo":
        RemoveBody(int(test[-3:-1]))
        numBodies = numBodies - 1
        test = f.readline()
    
    numBodiesList.append(numBodies)
                   
    for j in range(numBodies):
        bodyList[j].ReadData(f)

    

def animate(n):
    plt.cla()
    num = numBodiesList[n]
    if n == 0:
        print("Start")
    print(num)
    for i in bodyList[:num]:
        
        x = i.GetPx()
        y = i.GetPy()
        plt.plot(x[:n],y[:n],'--', lw = 2)
        print(x[n],y[n])
        plt.scatter(x[n],y[n], s = MAX_PLANET_SIZE * i.GetMass()/ 100000)
    print("")
    plt.xlim(-window, window)
    plt.ylim(-window, window)
    plt.grid()

    
fig = plt.figure()

anim = animation.FuncAnimation(fig,animate,frames = 10)

#anim.save('C:\Users\Andy\Desktop\Programs\Python\Nbody Sim\Oribiting.MP4',fps = 24,bitrate = 4500)

plt.show()

"""   
    
if __name__ == "__main__":
    main()
    
"""