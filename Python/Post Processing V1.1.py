import matplotlib.pyplot as plt
import numpy as np

f = open("C:\Users\Andy\Desktop\Programs\C ++\Projects\N-body Sim\NBody Sim v1.0\Data.txt")

numBodies = int(f.readline()[:-1])
iterations = int(f.readline()[:-1])




class Body:
    def __init__(self):
        self.Px = []
        self.Py = []
        self.Vx = []
        self.Vy = []
        
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
        self.AppendVy(float(data[3][:-1]))
        

def main():
    
    bodyList = []
    for i in range(numBodies):
        bodyList.append(Body())

    
    for i in range(iterations - 1):
        for j in range(numBodies):
            bodyList[j].ReadData(f)
        f.readline()

    plt.clf()
    for i in range(numBodies):
        plt.plot(bodyList[i].GetPx(),bodyList[i].GetPy())
    plt.grid()
    plt.show()
    plt.xlim(-500,500)
    plt.ylim(-500,500)
    f.close()
    
if __name__ == "__main__":
    main()