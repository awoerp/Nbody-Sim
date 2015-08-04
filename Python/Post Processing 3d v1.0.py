#works with "NBodySim V1.4"


import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBodySim v1.5\Nbody v1.5\Data.txt")

numBodies = int(f.readline()[:-1])
iterations = int(f.readline()[:-1])
window = 250
MAX_PLANET_SIZE = 20000.
activeBodies = []

t = np.array(range(0,5000))
angle = np.linspace(0,100,400)
phi = 15 * np.cos(2.*3.1415/250. * t)
class Body:
    def __init__(self):
        self.Px = []
        self.Py = []
        self.Pz = []
        self.Vx = []
        self.Vy = []
        self.Vz = []
        self.mass = []
        
    def GetPx(self):
        return self.Px
    
    def GetPy(self):
        return self.Py
        
    def GetPz(self):
        return self.Pz
    
    def GetMass(self,n):
        return self.mass[n]
    
    def isActive(self):
        return self.active
        
    def AppendPx(self,val):
        self.Px.append(val)
    
    def AppendPy(self,val):
        self.Py.append(val)
        
    def AppendPz(self,val):
        self.Pz.append(val)
        
    def AppendVx(self,val):
        self.Vx.append(val)
        
    def AppendVy(self,val):
        self.Vy.append(val)
        
    def AppendVz(self,val):
        self.Vz.append(val)
    
    def ReadData(self, inFile):
        rawData = inFile.readline()
        data = rawData.split(' ')
        
        self.AppendPx(float(data[0]))
        self.AppendPy(float(data[1]))
        self.AppendPz(float(data[2]))
        self.mass.append(float(data[6]))
            
        
        
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
    f.readline()
    

def animate(n):
    plt.cla()
    
    for i in bodyList:
        x = i.GetPx()
        y = i.GetPy()
        z = i.GetPz()
        ax.plot(x[:n],y[:n],z[:n],'--', lw = 2)
        ax.scatter(x[n],y[n],z[n], s = MAX_PLANET_SIZE * i.GetMass(n) / 100000)
        ax.grid(False)
    plt.xlim(-window,window)
    plt.ylim(-window,window)
    ax.view_init(phi[n],angle[n])

    
fig = plt.figure()
ax = fig.gca(projection = '3d')


anim = animation.FuncAnimation(fig,animate,frames = 400)

anim.save('C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBodySim v1.5\Nbody v1.5\Test.MP4',fps = 25,bitrate = 4500)

f.close()
plt.show()



"""   
    
if __name__ == "__main__":
    main()
    
"""