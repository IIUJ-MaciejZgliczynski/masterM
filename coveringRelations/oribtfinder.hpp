#ifndef ORBITFINDER
#define ORBITFINDER

#include "dmichelsonnewtonfunction.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include "newtonfunction.hpp"

using namespace std;

void getPoints(const char * inputfile,const char * outputfile,int iterations = 1, double precission = 1e-20, double step = 0.1, int order = 20, double c = 0.49){
		ifstream inputFile(inputfile);
		ofstream outputFile;
		outputFile.open(outputfile);
		outputFile.precision(20);
		double temp;
		DMichelsonIterationSymetricOrbitFinder pm(order,step,c,iterations);
		if( inputFile.is_open()){
			string line;
			getline (inputFile,line);
			while(inputFile.good()){
				getline(inputFile,line);
				stringstream ss(line);
				
				double t;
				ss >> t;	
			
				double result = newtonMethod(pm,t+0.05, precission,20);
				if(result != result){} // test for nans
				else
					outputFile << result <<  " " << pm.eval(result,temp) << endl;
			}
		}
		
		inputFile.close();
		outputFile.close();
}

void getAllPoints(double precission = 1e-20, double step = 0.1, int order = 20, double c = 0.49){
	for(int i = 1; i < 6 ; ++i){
		stringstream in ;
		in << "poss/poss_" << i << ".txt";
		stringstream out;
		out << "poss/periodic_" << i << ".txt";
		getPoints(in.str().c_str(), out.str().c_str(),i,precission,step, order,c);
	}
}

#endif
