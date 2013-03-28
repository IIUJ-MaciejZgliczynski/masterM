#ifndef DRAWINGORBITS
#define DRAWINGORBITS

#include <set>
#include <vector>

#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "dmichelsonpoincaremap.hpp"
#include <capd/covrel/GraphicsTripleSet.h>
#include "iotripleset.hpp"
#include "vectorconversion.hpp"

using namespace std;

void draw_orbit(double start_x,bool fat = false , int order = 20, double step=0.1, double c = 0.49, int maxit = 20){
	
	
	double r  = 5.0;
	DMichelsonPoincareMap pm(order,step,c);
	DVector v(2);
	v[0] = 0.0;
	v[1] = start_x;
	if(fat)
		fr.circle(v[0],v[1],r, GREEN);
	fr.dot(v[0],v[1],RED);
	cout << " start : " << v  << endl; 
	for(int i = 0 ; i < maxit ; ++i){
		v = pm(v);
		if(fat)
			fr.circle(v[0],v[1],r, GREEN);
		fr.dot(v[0],v[1],RED);
		cout << v << endl;
	}
	
	//waitBt();
}

void drawIterationsFromFile(const char * file, set<double> & drawn, int order = 20, double step = 0.1, double c = 0.49, int iterations = 20){
		cout << iterations << endl;
		cout << file << endl;
		cout.precision(20);
		vector<double> vals;
		getColumnFromFile<double>(file,vals,0,0);
		for(vector<double>::iterator it = vals.begin() ; it!=vals.end() ; ++it)
		{
			if(drawn.find(*it) == drawn.end()) 
			{
				drawn.insert(*it);
				draw_orbit(*it,true,order,step,c,iterations);
			}
		}
		
}

void drawIterationsFromFiles(vector<int> & filelist,int order = 20, double step = 0.1,  double c = 0.49, int iterations = 20){
	set<double> drawn;
	for(vector<int>::iterator it = filelist.begin() ; it!=filelist.end() ; ++it){
		stringstream in ;
		in << "poss/periodic_" << *it << ".txt";
		cout.precision(20);
		cout << "file : " << in.str() << endl;
		drawIterationsFromFile(in.str().c_str(),drawn,order,step,c,iterations);

	}

	waitBt();
}


void draw_circle(double point, int order = 20, double step = 0.1, double c = 0.49,double it = 1, double r = 1e-2, double num = 20,int iterations = 4){
	double diff = M_PI/num;
	//DMichelsonPoincareMap pm(order,step,c);
	DMichelsonIterationPoincareMap pm(order,step,c,it);
	
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

void drawCircleFromFile(const char * file, set<double> & drawn,int order = 20, double step = 0.1, double c = 0.49, int its = 1,double r = 1e-2, double num = 20 , int iterations = 8){
		vector<double> vals;
		getColumnFromFile<double>(file,vals,0,0);
		for(vector<double>::iterator it = vals.begin() ; it!=vals.end() ; ++it)
		{
			if(drawn.find(*it) == drawn.end()) 
			{
				drawn.insert(*it);
				draw_circle(*it,order,step,c,its,r,num,iterations);
			}
		}
}

void drawCircleFromFiles(vector<int> & filelist,int order = 20, double step = 0.1,  double c = 0.49, double r = 1e-2, double num = 20 , int iterations = 2){
	set<double> drawn;
	for(vector<int>::iterator it = filelist.begin() ; it!=filelist.end() ; ++it){
		stringstream in ;
		in << "poss/periodic_" << *it << ".txt";
		cout.precision(20);
		cout << "file : " << in.str() << endl;
		drawCircleFromFile(in.str().c_str(),drawn,order,step,c,*it,r,num,iterations);

	}

	waitBt();
}

typedef capd::vectalg::Vector
< double, 2 > 	FloatVector;

void drawStableUnStableDirections(const DVector & v,int iteration, double c = 0.49){
	fr.dot(v[0],v[1], RED);
	DMichelsonIterationPoincareMap map(20,0.1,c,iteration);
	
	DMatrix der(2,2);
	DVector x = map.derivative(v,der);
	cout.precision(20);
	cout << v << endl;
	DVector rV(2), iV(2);         // real and imaginary parts of eigenvalues
	DMatrix rVec(2,2), iVec(2,2); // real and imaginary parts of eigenvectors
	capd::alglib::computeEigenvaluesAndEigenvectors(der,rV,iV,rVec,iVec);
	
	cout << "Matrix : " <<der << endl;
	cout << "eigen values :\n" ;
	for(int i = 0 ; i < 2 ; ++i){
		cout << rV[i] << " + i" << iV[i] << endl;
	}
	
	cout << rVec << "+ i " << iVec << endl;
	
	FloatVector stable,unstable;
	int unstableIndex = 0;
	if ( abs(rV[1]) > 1)
		unstableIndex = 1;
	int stableIndex = 1 - unstableIndex;
	double h = 0.2;
	for(int i = 0 ; i < 2 ; ++i){
		stable[i] = rVec[i][stableIndex]*h;
		unstable[i] = rVec[i][unstableIndex]*h;
	}

	//capd::covrel::TripleSet ts2(to2D(x),unstable,stable);
	capd::covrel::GraphicsTripleSet ts(to2D(x),unstable,stable);
	//cout << ts2;
	fr << ts;
	//cout << ts2.outside(to2D(x)) << endl;
	//cout << ts2.inside(to2D(x)) << endl;
	//waitBt();
	
}




#endif
