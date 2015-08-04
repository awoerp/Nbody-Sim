#works with "NBodySim V1.3"


import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBodySim V1.3\Data.txt")

numBodies = int(f.readline()[:-1])
iterations = int(f.readline()[:-1])
window = 75
MAX_PLANET_SIZE = 100000.
activeBodies = []


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

def animate(n):
    plt.cla()
    
    for i in bodyList:
        x = i.GetPx()
        y = i.GetPy()
        if(activeBodies[n] <= 16):
            plt.plot(x[:n],y[:n],'--', lw = 2)
        plt.scatter(x[n],y[n], s = MAX_PLANET_SIZE * i.GetMass(n) / 100000)
    
    plt.text(-window + window / 10,window - window/10, "Active Bodies: %s" % (activeBodies[n]))
    plt.xlim(-window, window)
    plt.ylim(-window, window)
    plt.grid()

    
fig = plt.figure()

anim = animation.FuncAnimation(fig,animate,frames = 500)

#anim.save('C:\Users\Andy\Desktop\Programs\Python\Nbody Sim\Oribiting.MP4',fps = 24,bitrate = 4500)

f.close()
plt.show()



"""   
    
if __name__ == "__main__":
    main()
    
"""