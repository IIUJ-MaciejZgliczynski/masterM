#include <stdexcept>
#include <cmath>
#include <fstream>
#include <sstream>

#include "capd/krak/krak.h"

#include "dmichelsonpoincaremap.hpp"

using namespace capd;
using namespace std;

const int DIMENSION=3;
double minx=-2.2;
double maxx=1.6;
double miny=-2.6;
double maxy=2.;

Frame fr, txt;

// -----------------------------------------------------------------

void initGraphics()
{
   openGW(900,700,WHITE,BLACK);
   rootFrame.Clear();
   txt = Frame(0,0,595,130);
   fr = Frame(5,135,555,585,WHITE,BLACK);
   fr.setWorldCoord(minx,miny,maxx,maxy);
   fr.line(0.0,miny,0.0,maxy,BLACK);
   fr.line(minx,0.0,maxx,0.0,BLACK);
}


void myTry( DMichelsonPoincareMap & pm, double x, int iterations,double c, ofstream & myfile){
	
   DVector v(2);
   v[0] = 0.0;
   v[1] = x;
	for(int i = 0 ; i < iterations ; ++i){
	   v = pm(v);
	 //myfile << x << " " << v[0]  << endl;
   }
   myfile << x << " " << v[0] << endl;
  
}

double mabs(double x){
	if(x < 0)
		return -x;
	return x;
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
double findOrbit(DMichelsonPoincareMap & flpm, double min_x , double max_x ,double precission = 1e-4 , int maxIt = 20){
	DVector v(2);
	DVector res(2);
	DVector beg(2);
	beg[0] = 0.0; beg[1] = min_x;
	double begVal = flpm(beg)[0];
	
    
	for(int i = 0 ; i < maxIt ; ++i){
		double mid = (min_x + max_x)/2;
		v[0] = 0;
		v[1] = mid;
		DVector res = flpm(v);
		
		cout << " iteration " << (i +1) <<  endl;
		cout << v << endl << res << endl;
		if(mabs(res[0]) < precission)
			return mid;
		if(begVal * res[0] < 0)
			max_x = mid;
		else
			min_x = mid;
	}	
	return (max_x + min_x)/2;
	
}

void draw_orbit(double start_x , int order, double step, double c = 0.49, int maxit = 8){
	
	DMichelsonPoincareMap pm(order,step,c);
	DVector v(2);
	v[0] = 0.0;
	v[1] = start_x;
	
	fr.dot(v[0],v[1],RED);
	for(int i = 0 ; i < maxit ; ++i){
		v = pm(v);
		fr.dot(v[0],v[1],RED);
		cout << v << endl;
	}
	
	waitBt();
}

void getPoints(const char * inputfile,const char * outputfile, double precission = 1e-4, double step = 0.01, int order = 20, double c = 0.49){
		ifstream inputFile(inputfile);
		ofstream outputFile;
		outputFile.open(outputfile);
		DMichelsonPoincareMap pm(order,step,c);
		if( inputFile.is_open()){
			string line;
			getline (inputFile,line);
			while(inputFile.good()){
				getline(inputFile,line);
				stringstream ss(line);
				
				double t;
				ss >> t;
				double result = findOrbit(pm,t,t+0.1,precission);
				outputFile << result << endl;
			}
		}
		
		inputFile.close();
		outputFile.close();
}

void getAllPoints(double precission = 1e-4, double step = 0.01, int order = 20, double c = 0.49){
	for(int i = 1; i < 6 ; ++i){
		stringstream in ;
		in << "poss/poss_" << i << ".txt";
		stringstream out;
		out << "poss/periodic_" << i << ".txt";
		getPoints(in.str().c_str(), out.str().c_str(),precission,step, order,c);
	}
}

void C0Test()
{
   double grid = 30;
   double step = 0.3;
   int order = 20;
   IMap f = "par:c;var:x,y,z;fun:y,z,c^2-y-0.5*x*x;";
   // pole wektorowe 
   IFunction s ="var:x,y,z,d;fun:z;";
   // sekcja z == 0
   ITaylor T(f,order,step);
   
   // rozwiazywacz
   IPoincareMap pm(T,s);
   f.setParameter("c",interval(0.5));

   DMap ff= "var:x,y,z,c;fun:y,z,c^2-y-0.5*x*x;";
   DFunction fs ="var:x,y,z,d;fun:z;";
   DTaylor ft(ff,order,step);
   DPoincareMap flpm(ft,fs);
   ff.setParameter("c",0.5);

   IVector iv(3);
   iv[0] = iv[2] = interval(0.0);
   iv[1] = interval(15,16)/interval(10);

   fr << At(11,55) << "y";
   fr << At(0,33) << "y'";
   txt << "Test of class \"PoincareMap\".\n";
   txt << "ODE: Kura-Siva, order:" << order
      << ", Poincare section: y\"=0" <<"\n\n";
   txt << "P - Poincare return map,    The set s=" << iv << "\n";
   txt << "We compute P(s) and P^2(s)\n";

   int c = RED;
   txt.SetFgColor(c);
   txt << "using set arithmetic and class PoincareMap\n";
   fr << At(7,20) << "P^2(s)";
   fr << At(26,26) << "P(s)";

   interval part = interval(iv[1].rightBound()-iv[1].leftBound())/interval(grid);
   pm.setFactor(0.3);

   for(int i=0;i<grid;i++){
      IVector w;
      w.clear();
      w[1]=iv[1].leftBound() + i*part + interval(0,1)*part;
      IVector r=w-midVector(w);

      C0Rect2Set rec(midVector(w),r);
      w = pm(rec);
      fr.boxFill(w[0].leftBound(),w[1].leftBound(),w[0].rightBound(),w[1].rightBound(),c);
      fr.box(w[0].leftBound(),w[1].leftBound(),w[0].rightBound(),w[1].rightBound());

      w = pm(rec);
      fr.boxFill(w[0].leftBound(),w[1].leftBound(),w[0].rightBound(),w[1].rightBound(),c);
      fr.box(w[0].leftBound(),w[1].leftBound(),w[0].rightBound(),w[1].rightBound());
   }

   c = GREEN;
   txt.SetFgColor(c);
   txt << "using vector arithmetic and class PoincareMap\n";
   grid *= 30;

   for(int i=0;i<=grid;i++){
      DVector v(0,1.5,0.);
      v[1] += 0.1*i/grid ;
      v = flpm(v);
      fr.dot(v[0],v[1],c);
      v = flpm(v);
      fr.dot(v[0],v[1],c);
   }

   waitBt();
}



int main(int , char *[])
{
   initGraphics();
   try{

  
	//getAllPoints();  
	draw_orbit(0.889258,20,0.1);
	  
  }  catch(std::exception& e)
   {
      rootFrame << "\n" << e.what();
      waitBt();
   }

   closeGW();
   return 0;
}

/// @}
