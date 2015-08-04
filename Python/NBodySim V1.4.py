#works with "NBodySim V1.4"


import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBody Simv1.4\Data.txt")

numBodies = int(f.readline()[:-1])
iterations = int(f.readline()[:-1])
window = 500
MAX_PLANET_SIZE = 5000.
activeBodies = []
centerX      = []
centerY      = []
deviation    = []


class Body:
    def __init__(self):
        self.Px = []
        self.Py = []
        self.Vx = []
        self.Vy = []
        self.mass = []
        self.active = []
        
    def GetPx(self):
        return self.Px
    
    def GetPy(self):
        return self.Py
    
    def GetMass(self,n):
        return self.mass[n]
    
    def isActive(self):
        return self.active
        
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
        self.mass.append(float(data[4]))
        self.active.append(int(data[5][:-1]))
            
        
        
    def ReadMass(self, inFile):
        temp = float(inFile.readline()[:-1])
        self.mass.append(temp)

        

bodyList = []
for i in range(numBodies):
    bodyList.append(Body())

for i in range(numBodies):
    bodyList[i].ReadMass(f)
    
for i in range(iterations - 1):
    for j in range(numBodies):
        bodyList[j].ReadData(f)
    activeBodies.append(int(f.readline()[:-1]))
    data = f.readline()
    splitData = data.split(' ')
    centerX.append(float(splitData[0]))
    centerY.append(float(splitData[1]))
    deviationX = float(splitData[2])
    deviationY = float(splitData[3][:-1])
    if( deviationX > deviationY):
        deviation.append(deviationX)
    else:
        deviation.append(deviationY)
    f.readline()
    

def animate(n):
    plt.cla()
    
    for i in bodyList:
        x = i.GetPx()
        y = i.GetPy()
        if(activeBodies[n] <= 18):
            plt.plot(x[:n],y[:n],'--', lw = 2)
        plt.scatter(x[n],y[n], s = MAX_PLANET_SIZE * i.GetMass(n) / 100000)
        plt.scatter(centerX[n],centerY[n],marker = '+', s = 100)
    x = centerX[n]
    y = centerY[n]
    d = deviation[n]

    plt.text(x - d + 2*d / 50, y + d- 2*d /25, "Active Bodies: %s" % (activeBodies[n]))
    
    plt.xlim(x - d, x + d)
    plt.ylim(y - d, y + d)

    plt.grid()

    
fig = plt.figure()

anim = animation.FuncAnimation(fig,animate,frames = iterations - 1)

#anim.save('C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBody Simv1.4\Test2.MP4',fps = 100,bitrate = 4500)

f.close()
plt.show()



"""   
    
if __name__ == "__main__":
    main()
    
"""