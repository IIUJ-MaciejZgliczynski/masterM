#include <stdexcept>
#include <cmath>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>



#include "dmichelsonpoincaremap.hpp"
#include "oribtfinder.hpp"
#include "dmichelsonnewtonfunction.hpp"
#include "filereading.hpp"
#include "graphicstuff.hpp"
#include "drawingorbits.hpp"
#include "oribtfinder.hpp"

#include "vectorconversion.hpp"


void myTry( DMichelsonPoincareMap & pm, double x, int iterations,double c, ofstream & myfile){
	
   DVector v(2);
   v[0] = 0.0;
   v[1] = x;
	for(int i = 0 ; i < iterations ; ++i){
	   v = pm(v);
   }
   myfile << x << " " << v[0] << endl;
  
}


void myTry(){
	
	   double order = 20;
	   double step = 0.1;
	   double c = 0.49;
	   
	   DMichelsonPoincareMap pm(order,step,c);
	   for(int iterations = 1 ; iterations <= 10; ++iterations){
	
	   
	   ofstream myfile;
	   stringstream s ;
	   s << "result/" ;
	   s << "result_" << iterations << ".txt";
	   string str(s.str());
	   myfile.open(str.c_str());	
	   
	   for(double x = -5; x <= 2 ; x+=step){
		   try{
			  myTry(pm,x,iterations,c,myfile);
		   }catch(std::exception & e){
			   
		   }
	   }
	   myfile.close();
		}
}






int main(int , char *[])
{
   initGraphics();
   try{

  
	//getAllPoints();  
	
	
	
	double step = 0.1;
	int iterations = 2;
	double c = 0.49;
	cout.precision(20);
	
	for(int order = 20 ; order > 0 ; order -=20){
		cout << "ORDER = " << order << endl;
		DMichelsonIterationSymetricOrbitFinder pm(order,step,c,iterations);
		double start = -1.3514548684335634388 ;
		draw_orbit(start,true,order,0.1,0.49,6);
	}
	
  }  catch(std::exception& e)
   {
      rootFrame << "\n" << e.what();
      waitBt();
   }

   closeGW();
   return 0;
}

/// @}
