
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <ctime>


using namespace std;

const int MAX_BODIES     = 50;
const int MAX_TIME_STEPS = 5000;
const int DYNAMIC_VALUES = 4;

const int MAX_DISTANCE   = 500;
const int MAX_VELOCITY   = 7;
const int MAX_MASS       = 100000;
const float MIN_DISTANCE = 2;

enum SimType      {RANDOM,FILE_INPUT};
enum DynamicsData {Px, Py, Vx, Vy};

class Body
{
private:
   float mass;
   float Px;
   float Py;
   float Vx;
   float Vy;
   float PxLast;
   float PyLast;
   float VxLast;
   float VyLast;

   int bodyNumber;

public:


   Body()
   {
   mass = 0;
   Px = 0;
   Py = 0;
   Vx = 0;
   Vy = 0;
   PxLast = 0;
   PyLast = 0;
   VxLast = 0;
   VyLast = 0;

      
   }

   Body(float initialMass, float Pxi, float Pyi, float Vxi, float Vyi)
   {
   mass = initialMass;
   Px = Pxi;
   Py = Pyi;
   Vx = Vxi;
   Vy = Vyi;
   PxLast = Pxi;
   PyLast = Pyi;
   VxLast = Vxi;
   VyLast = Vyi;

      
   }

   void SetPx(float val)
   {
      Px = val;
   }

   void SetPy(float val)
   {
      Py = val;
   }

   void SetVx(float val)
   {
      Vx = val;
   }

   void SetVy(float val)
   {
      Vy = val;
   }
   void SetMass(float val)
   {
      mass = val;
   }
   void SetBodyNumber(int val)
   {
      bodyNumber = val;
   }

   float GetPx() const
   {
      return Px;
   }
   float GetPy() const
   {
      return Py;
   }

   float GetVx() const
   {
      return Vx;
   }

   float GetVy() const
   {
      return Vy;
   }
   float GetMass()
   {
      return mass;
   }
   int GetBodyNumber() const
   {
      return bodyNumber;
   }


   void PrintTimeStepToFile(ofstream& file)
   {
      float Px = GetPx();
      float Py = GetPy();
      float Vx = GetVx();
      float Vy = GetVy();
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
         list[i].SetPx(px);
         list[i].SetPy(py);
         list[i].SetVx(vx);
         list[i].SetVy(vy);
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
      body.SetPx(rand() % MAX_DISTANCE - MAX_DISTANCE / 2);
      body.SetPy(rand() % MAX_DISTANCE - MAX_DISTANCE / 2);
      body.SetVx(rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
      body.SetVy(rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
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
      float   Px = body.GetPx();
      float   Py = body.GetPy();
      float   Vx = body.GetVx();
      float   Vy = body.GetVy();
      float mass = body.GetMass();

      float netForcex = 0;
      float netForcey = 0;
      CalcNetForce(body, netForcex, netForcey);


      float newPx = Px + Vx * timeStep + 0.5*(netForcex / mass)*timeStep*timeStep;
      float newPy = Py + Vy * timeStep + 0.5*(netForcey / mass)*timeStep*timeStep;
      float newVx = Vx + (netForcex / mass) * timeStep;
      float newVy = Vy + (netForcey / mass) * timeStep;


      body.SetPx(newPx);
      body.SetPy(newPy);
      body.SetVx(newVx);
      body.SetVy(newVy);

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
      float displacmentx = body2.GetPx() - body1.GetPx();
      float displacmenty = body2.GetPy() - body1.GetPy();
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
         list[i].PrintTimeStepToFile(outputFile);
      outputFile << numBodies;
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
   Simulation Sim(8, "Random", 1000);
   Sim.OpenFile("Data1.txt");
   srand(time(NULL));
   Sim.SetupSimulation(RANDOM);
   Sim.PrintMass();
   Sim.RunSimulation();
   Sim.CloseFile();
}
