#include <stdexcept>
#include <cmath>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>

#include "capd/krak/krak.h"

#include "dmichelsonpoincaremap.hpp"
#include "oribtfinder.hpp"
#include "dmichelsonnewtonfunction.hpp"

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

void draw_circle(double point, double step = 0.1, int order = 20, double c = 0.49, double r = 1e-2, double num = 20,int iterations = 4){
	double diff = M_PI/num;
	DMichelsonPoincareMap pm(order,step,c);
	
	
	fr.dot(0.0,point, RED);
	for(double angle = 0; angle < M_PI ; angle+= diff ){
		DVector v(2);
		v[0] = r*cos(angle);
		v[1] = point + r*sin(angle);
		try{
			cout << v << endl;
		v = pm.iterate(v,iterations);
			fr.dot(v[0],v[1],GREEN);
			cout << v << endl;
		}
		catch(std::exception & e){
		}
	
	}
	
	//waitBt();
}

void drawOrbitsFromFile(const char * file, set<int> & drawn, double step = 0.1, int order = 20, double c = 0.49, double r = 1e-2, double num = 20 , int iterations = 4){
		ifstream inputFile(file);
		if( inputFile.is_open()){
			string line;
		//	getline (inputFile,line);
			while(inputFile.good()){
				getline(inputFile,line);
				stringstream ss(line);
				cout << " read line : " << line << endl;
				 
				double t,tmp;
				ss >> t;
				ss >> tmp;
				cout << " t : " << t << endl;
				if ( drawn.find(t) == drawn.end()){
					drawn.insert(t);
					draw_orbit(t,20,0.1);
				}
			
			}
		}
		
		inputFile.close();
}

void drawOrbitsFromFiles(vector<int> & filelist, double step = 0.1, int order = 20, double c = 0.49, double r = 1e-2, double num = 20 , int iterations = 4){
	set<int> drawn;
	for(vector<int>::iterator it = filelist.begin() ; it!=filelist.end() ; ++it){
		stringstream in ;
		in << "poss/periodic_" << *it << ".txt";
		cout.precision(20);
		cout << "file : " << in.str() << endl;
		drawOrbitsFromFile(in.str().c_str(),drawn,step,order,c,r,num,iterations);

	}

	waitBt();
}

int main(int , char *[])
{
   initGraphics();
   try{

  
	//getAllPoints();  
	
	vector<int> vec;
	/*for(int i = 1 ; i < 6 ; ++i)
		vec.push_back(i);*/
	//vec.push_back(1);
	vec.push_back(2);
	//vec.push_back(4);
	//vec.push_back(1);
	drawOrbitsFromFiles(vec);
	////DMichelsonPoincareMap pm(20,0.1,0.49);
	//cout << findOrbit(pm,-0.3,-0.2,2);
	/*double tmp;
	  double  p = -1.4207228055138549916;
	  DMichelsonPoincareMap pm(20,0.1,0.49);
	  DMichelsonIterationSymetricOrbitFinder disof(20,0.1,0.49,2);  
	  DVector v(2);
	  v[0]= 0.0;
	  v[1] = p;
	  cout.precision(20);
	  cout << v;
	  cout << disof.eval(p,tmp) << endl;
	  for(int i= 0 ; i < 10 ; ++i){
		  v = pm(v);
		  cout << v << endl;
	  }*/
  }  catch(std::exception& e)
   {
      rootFrame << "\n" << e.what();
      waitBt();
   }

   closeGW();
   return 0;
}

/// @}
