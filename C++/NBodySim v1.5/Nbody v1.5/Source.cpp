
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include <ctime>


using namespace std;

const int MAX_BODIES     = 100;
const int MAX_TIME_STEPS = 1000;
const int DYNAMIC_VALUES = 4;

const int MAX_DISTANCE   = 500;
const int MAX_VELOCITY   = 2;
const int MAX_MASS       = 1000;
const float MIN_DISTANCE = 7;

enum SimType      {RANDOM,FILE_INPUT};

class Body
{
private:
   float mass;
   float Px;
   float Py;
   float Pz;
   float Vx;
   float Vy;
   float Vz;
   float PxLast;
   float PyLast;
   float PzLast;
   float VxLast;
   float VyLast;
   float VzLast;
   bool  active;

   int bodyNumber;

public:


   Body()
   {
   mass = 0;
   Px = 0;
   Py = 0;
   Pz = 0;
   Vx = 0;
   Vy = 0;
   Vz = 0;
   active = true;
   }

   Body(float initialMass, float Pxi, float Pyi, float Pzi, float Vxi, float Vyi, float Vzi)
   {
   mass = initialMass;
   Px = Pxi;
   Py = Pyi;
   Pz = Pzi;
   Vx = Vxi;
   Vy = Vyi;
   Vz = Vzi;
   active = true;

      
   }

   void SetPx(float val)
   {
      Px = val;
   }

   void SetPy(float val)
   {
      Py = val;
   }

   void SetPz(float val)
   {
      Pz = val;
   }

   void SetVx(float val)
   {
      Vx = val;
   }

   void SetVy(float val)
   {
      Vy = val;
   }

   void SetVz(float val)
   {
      Vz = val;
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
   float GetPz() const
   {
      return Pz;
   }

   float GetVx() const
   {
      return Vx;
   }

   float GetVy() const
   {
      return Vy;
   }
   float GetVz() const
   {
      return Vz;
   }
   float GetMass()
   {
      return mass;
   }
   int GetBodyNumber() const
   {
      return bodyNumber;
   }

   void Deactivte()
   {
      active = false;
      mass   = 0;
   }

   bool IsActive() const
   {
      return active;
   }


   void PrintTimeStepToFile(ofstream& file)
   {
      float Px = GetPx();
      float Py = GetPy();
      float Pz = GetPz();
      float Vx = GetVx();
      float Vy = GetVy();
      float Vz = GetVz();
      file << Px << " " << Py << " " << Pz
           << " " << Vx << " " << Vy << " " << Vz
           << " " << mass << " " << active << endl;
      
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
      body.SetPz(rand() % MAX_DISTANCE - MAX_DISTANCE / 2);
      body.SetVx(rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
      body.SetVy(rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
      body.SetVz(rand() % MAX_VELOCITY * 2 - MAX_VELOCITY);
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
      if(body.IsActive())
      {
         float   Px = body.GetPx();
         float   Py = body.GetPy();
         float   Pz = body.GetPz();
         float   Vx = body.GetVx();
         float   Vy = body.GetVy();
         float   Vz = body.GetVz();
         float mass = body.GetMass();

         float netForcex = 0;
         float netForcey = 0;
         float netForcez = 0;
         CalcNetForce(body, netForcex, netForcey, netForcez);


         float newPx = Px + Vx * timeStep + 0.5*(netForcex / mass)*timeStep*timeStep;
         float newPy = Py + Vy * timeStep + 0.5*(netForcey / mass)*timeStep*timeStep;
         float newPz = Pz + Vz * timeStep + 0.5*(netForcez / mass)*timeStep*timeStep;
         float newVx = Vx + (netForcex / mass) * timeStep;
         float newVy = Vy + (netForcey / mass) * timeStep;
         float newVz = Vz + (netForcez / mass) * timeStep;


         body.SetPx(newPx);
         body.SetPy(newPy);
         body.SetPz(newPz);
         body.SetVx(newVx);
         body.SetVy(newVy);
         body.SetVz(newVz);
      }
   }
   
   void CalcNetForce(Body& body, float& Fx, float& Fy, float& Fz)
   {
      int bodyNumber = body.GetBodyNumber();
      float netFx = 0;
      float netFy = 0;
      float netFz = 0;

      for(int i = 0; i < numBodies; i++)
      {
         float tempForce = 0;
         float tempAngle = 0;
         float phi       = 0;

         if(bodyNumber != i && list[i].IsActive())
            CalcForce(body,list[i], tempForce, tempAngle, phi);

         float x = tempForce * cos(tempAngle) * cos(phi);
         float y = tempForce * sin(tempAngle) * cos(phi);
         float z = tempForce * sin(phi);
         netFx += x;
         netFy += y;
         netFz += z;
      }

      Fx = netFx;
      Fy = netFy;
      Fz = netFz;
   }

   void CalcForce( Body& body1, Body& body2, float& netForce, float& angle, float& phi)
   {      
      float m1 = body1.GetMass();
      float m2 = body2.GetMass();
      float d = 0;
      GetDistance(body1, body2, angle, phi, d);

      netForce = (m1 * m2) / pow(d,2);
    }


   void GetDistance(Body& body1, Body& body2, float& angle, float& phi, float& distance)
   {
      float displacmentx = body2.GetPx() - body1.GetPx();
      float displacmenty = body2.GetPy() - body1.GetPy();
      float displacmentz = body2.GetPz() - body1.GetPz();
      float xDist = fabs(displacmentx);
      float yDist = fabs(displacmenty);
      float zDist = fabs(displacmentz);

      distance = sqrt(pow(xDist,2) + pow(yDist,2) + pow(zDist,2));

      if( displacmentx < 0 )
         angle = 3.1415 + atan(displacmenty / displacmentx);
      else
         angle = atan(displacmenty / displacmentx);
      if( displacmentz > 0 )
         phi = atan(displacmentz / sqrt(pow(displacmentx,2) + pow(displacmenty, 2)));
      else
         phi = -atan(fabs(displacmentz) / sqrt(pow(displacmentx,2) + pow(displacmenty, 2)));
   }


   void PrintStepToFile()
   {
      for(int i = 0; i < numBodies; i++)
         list[i].PrintTimeStepToFile(outputFile);
      outputFile << GetActiveBodies() << endl;
      
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

   int GetActiveBodies()
   {
      int count = 0;
      for(int i = 0; i < numBodies; i++)
         if(list[i].IsActive())
            count++;
      return count;
   }

};




void main()
{
   Simulation Sim(10, "Random", 1000);
   Sim.OpenFile("Data.txt");
   srand(time(NULL));
   Sim.SetupSimulation(RANDOM);
   Sim.PrintMass();
   Sim.RunSimulation();
   Sim.CloseFile();
}
