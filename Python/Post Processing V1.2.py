#works with "Nbody Simv1.1 -- Don't Touch


import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBody Sim v1.1 -- Don't Touch\Data1.txt")

numBodies = int(f.readline()[:-1])
iterations = int(f.readline()[:-1])
window = 300
MAX_PLANET_SIZE = 500.



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
    def ReadMass(self, inFile):
        temp = float(inFile.readline()[:-1])
        self.mass = temp
    
    def GetMass(self):
        return self.mass
        

bodyList = []
for i in range(numBodies):
    bodyList.append(Body())

for i in range(numBodies):
    bodyList[i].ReadMass(f)
    
for i in range(iterations - 1):
    for j in range(numBodies):
        bodyList[j].ReadData(f)
    f.readline()

def animate(n):
    plt.cla()
    
    for i in bodyList:
        x = i.GetPx()
        y = i.GetPy()
        plt.plot(x[:n],y[:n],'--', lw = 2)
        plt.scatter(x[n],y[n], s = MAX_PLANET_SIZE * i.GetMass() / 100000)

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