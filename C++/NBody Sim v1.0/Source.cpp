
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <ctime>


using namespace std;

const int MAX_BODIES     = 10;
const int MAX_TIME_STEPS = 6000;
const int DYNAMIC_VALUES = 4;

const int MAX_DISTANCE   = 100;
const int MAX_VELOCITY   = 5;
const int MAX_MASS       = 10000;

enum SimType      {RANDOM,FILE_INPUT};
enum DynamicsData {Px, Py, Vx, Vy};

class Body
{
private:
   float mass;
   float data[MAX_TIME_STEPS][DYNAMIC_VALUES]; // sets up a 2D array with rows as time and columns as dynamics Data
   int currentTimePosition;
   int bodyNumber;

public:


   Body()
   {
      mass = 0;
      data[0][Px] = 0;
      data[0][Py] = 0;
      data[0][Vx] = 0;
      data[0][Vx] = 0;
      currentTimePosition = 0;
      
   }

   Body(float initialMass, float Pxi, float Pyi, float Vxi, float Vyi)
   {
      mass = initialMass;
      data[0][Px] = Pxi;
      data[0][Py] = Pyi;
      data[0][Vx] = Vxi;
      data[0][Vx] = Vxi;
      currentTimePosition = 0;
      
   }

   void SetPx(int time, float val)
   {
      data[time][Px] = val;
   }

   void SetPy(int time, float val)
   {
      data[time][Py] = val;
   }

   void SetVx(int time, float val)
   {
      data[time][Vx] = val;
   }

   void SetVy(int time, float val)
   {
      data[time][Vy] = val;
   }
   void SetMass(float val)
   {
      mass = val;
   }
   void SetBodyNumber(int val)
   {
      bodyNumber = val;
   }

   float GetPx(int time) const
   {
      return data[time][Px];
   }
   float GetPy(int time) const
   {
      return data[time][Py];
   }

   float GetVx(int time) const
   {
      return data[time][Vx];
   }

   float GetVy(int time) const
   {
      return data[time][Vy];
   }
   float GetMass()
   {
      return mass;
   }
   int GetBodyNumber() const
   {
      return bodyNumber;
   }

   void PrintTimeStepToFile(ofstream& file, int time)
   {
      float Px = GetPx(time - 1);
      float Py = GetPy(time - 1);
      float Vx = GetVx(time - 1);
      float Vy = GetVy(time - 1);
      file << Px << " " << Py
           << " " << Vx << " " << Vy << endl;
      
   }
};



class Simulation
{
private:

   int numBodies;
   Body list[MAX_BODIES];
   SimType initial;
   int currentStep;
   float timeStep;
   ofstream outputFile;


public:

   Simulation(int bodies,string initialValuesType, float duration)
   {
      numBodies = bodies;
      if( initialValuesType == "Random")
         initial = RANDOM;
      else
         initial = FILE_INPUT;
      timeStep = duration / MAX_TIME_STEPS;
      currentStep = 0;

   }

   void SetupSimulation(SimType type)
   {
      if(type == RANDOM)
         for(int i = 0; i < numBodies; i++)
            RandomizeInitials(list[i],i);


      if(type == FILE_INPUT)
      {
      ifstream fin;
      fin.open("Initial.txt");
      for(int i = 0; i < numBodies; i++)
      {
         float mass, px, py, vx, vy;
         fin >> mass >> px >> py >> vx >> vy;
         list[i].SetMass(mass);
         list[i].SetPx(0,px);
         list[i].SetPy(0,py);
         list[i].SetVy(0,vx);
         list[i].SetVy(0,vy);
         list[i].SetBodyNumber(i);

      }

      fin.close();
         

      }
      currentStep++;
   }

   void RandomizeInitials(Body& body, int bodyNumber)
   {
      //time_t timer;
      
      //srand(bodyNumber + 7 + rand() % 100);
      body.SetMass( rand() % MAX_MASS);
      body.SetPx(0, rand() % MAX_DISTANCE);
      body.SetPy(0, rand() % MAX_DISTANCE);
      body.SetVx(0, rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
      body.SetVy(0, rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
      body.SetBodyNumber(bodyNumber);
      
   }

   void PrintMass()
   {
      for(int i = 0; i < numBodies; i++)
      outputFile << list[i].GetMass() << endl;
   }


   void RunSimulation()
   {
      for(int i = 0; i < MAX_TIME_STEPS; i++)
      {
         CalcTimeStep();
         PrintStepToFile();
      }
   outputFile << "done" << endl;
   }

   void CalcTimeStep()
   {
      for(int i = 0; i < numBodies; i++)
      {
         CalcBody(list[i]);
         
      }
      currentStep++;
   }

   void CalcBody(Body& body)
   {
      float   Px = body.GetPx(currentStep - 1);
      float   Py = body.GetPy(currentStep - 1);
      float   Vx = body.GetVx(currentStep - 1);
      float   Vy = body.GetVy(currentStep - 1);
      float mass = body.GetMass();

      float netForcex = 0;
      float netForcey = 0;
      CalcNetForce(body, netForcex, netForcey);


      float newPx = Px + Vx * timeStep + 0.5*(netForcex / mass)*timeStep*timeStep;
      float newPy = Py + Vy * timeStep + 0.5*(netForcey / mass)*timeStep*timeStep;
      float newVx = Vx + (netForcex / mass) * timeStep;
      float newVy = Vy + (netForcey / mass) * timeStep;

      body.SetPx(currentStep, newPx);
      body.SetPy(currentStep, newPy);
      body.SetVx(currentStep, newVx);
      body.SetVy(currentStep, newVy);

   }
   
   void CalcNetForce(Body& body, float& Fx, float& Fy)
   {
      int bodyNumber = body.GetBodyNumber();
      float netFx = 0;
      float netFy = 0;

      for(int i = 0; i < numBodies; i++)
      {
         float tempForce = 0;
         float tempAngle = 0;

         if(bodyNumber != i)
            CalcForce(body,list[i], tempForce, tempAngle);

         float x = tempForce * cos(tempAngle);
         float y = tempForce * sin(tempAngle);
         netFx += x;
         netFy += y;
      }

      Fx = netFx;
      Fy = netFy;
   }

   void CalcForce( Body& body1, Body& body2, float& netForce, float& angle)
   {      
      float m1 = body1.GetMass();
      float m2 = body2.GetMass();
      float d = 0;
      GetDistance(body1, body2, angle, d);
      netForce = (m1 * m2) / pow(d,2);
    }
      

   void GetDistance(Body& body1, Body& body2, float& angle, float& distance)
   {
      float displacmentx = body2.GetPx(currentStep - 1) - body1.GetPx(currentStep - 1);
      float displacmenty = body2.GetPy(currentStep - 1) - body1.GetPy(currentStep - 1);
      float xDist = fabs(displacmentx);
      float yDist = fabs(displacmenty);

      distance = sqrt(pow(xDist,2) + pow(yDist,2));
      if( displacmentx < 0 )
         angle = 3.1415 + atan(displacmenty / displacmentx);
      else
         angle = atan(displacmenty / displacmentx);
   }

   void PrintStepToFile()
   {
      for(int i = 0; i < numBodies; i++)
         list[i].PrintTimeStepToFile(outputFile, currentStep);
      outputFile << currentStep;
      outputFile << endl;
      
   }

   void OpenFile(string fileName)
   {
      outputFile.open(fileName);
      outputFile << numBodies << endl;
      outputFile << MAX_TIME_STEPS << endl;
   }
   void CloseFile()
   {
      outputFile.close();
   }

};




void main()
{
   Simulation Sim(4, "File", 1000);
   Sim.OpenFile("Data.txt");
   srand(time(NULL));
   Sim.SetupSimulation(RANDOM);
   Sim.PrintMass();
   Sim.RunSimulation();
   Sim.CloseFile();
}
