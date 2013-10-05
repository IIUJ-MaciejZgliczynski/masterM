#include<iostream>
using namespace std;

#include "vectorconversion.hpp"
#include "graphicstuff.hpp"
#include "filereading.hpp"
#include "dmichelsonpoincaremap.hpp"
#include "drawingorbits.hpp"


#include <capd/covrel/GraphicsTripleSet.h>
//#include "/home/maciek/Informatyka/Capd/capd/capdDynSys/src/capd/covrel/TripleSet.cpp"
#include <capd/covrel/TripleSet.h>
#include <cmath>
#include "iotripleset.hpp"
#include "higherorderfunction.hpp"
#include "triplesetcreator.hpp"
#include "checkers.hpp"
#include "coveringmaker.hpp"


capd::covrel::TripleSet createTripleSet(DVector & v, DMichelsonIterationPoincareMap & map, double h = 1.0){
	DMatrix der(2,2);
	
	DVector x = map.derivative(v,der);
	cout.precision(20);
	DVector rV(2), iV(2);         // real and imaginary parts of eigenvalues
	DMatrix rVec(2,2), iVec(2,2); // real and imaginary parts of eigenvectors
	capd::alglib::computeEigenvaluesAndEigenvectors(der,rV,iV,rVec,iVec);
	
	capd::covrel::FloatVector stable,unstable;
	int unstableIndex = 0;
	if ( abs(rV[1]) > 1)
		unstableIndex = 1;
	int stableIndex = 1 - unstableIndex;
	for(int i = 0 ; i < 2 ; ++i){
		stable[i] = rVec[i][stableIndex]*h;
		unstable[i] = rVec[i][unstableIndex]*h;
	}
	return capd::covrel::TripleSet(to2D(x), stable,unstable);
	
}
void createTripleSets(vector<DVector> & v, vector<capd::covrel::TripleSet> & output ,int iteration,double scale = 1.0,double c = 0.49){
	DMichelsonIterationPoincareMap map(20,0.1,c,iteration);
	for(vector<DVector>::iterator it = v.begin() ; it!=v.end() ; ++it){
		cout << *it << endl;
		output.push_back(createTripleSet(*it, map, scale));
	}
}



capd::covrel::TripleSet weighedAvrage(const capd::covrel::TripleSet & tset1, const capd::covrel::TripleSet & tset2, double h= 0.5){
	double t = 1-h;
	return capd::covrel::TripleSet(tset1.center*h + t*tset2.center,tset1.stable*h + tset2.stable*t, tset2.unstable*h + tset2.unstable*t);
	// capd::covrel::TripleSet ts((tset1.center+tset2.center)/2, (tset1.stable + tset2.stable)/2, (tset1.unstable + tset2.unstable)/2);
	 //return ts;
}
void zeroutXDoublecoordinate(capd::covrel::TripleSet & tset, double error = 1e-10){
	 capd::covrel::FloatVector x = tset.center;
	 if(abs(x[0]) < error ){
		x[0] = 0.0;
		
		tset = capd::covrel::TripleSet(x,tset.stable,R(tset.stable));
	 }
}
class ZeroOutXDoubleCordinate{
	public:
		void operator ()(capd::covrel::TripleSet & tset){
			zeroutXDoublecoordinate(tset);
		}
};


capd::covrel::TripleSet bounce(const capd::covrel::TripleSet & tset){
	capd::covrel::TripleSet result(R(tset.center),R(tset.unstable),R(tset.stable));
	return result;
}

void createTripleSetsFromOrbitPoints(double scale = 0.1){
	const char * orbitPrefix= "covrel/orbit";
	const char * orbitSuffix= ".txt";
	
	
	const char * triplesetPrefix = "covrel/trisets/tset";
	const char * triplesetSuffix = ".txt";
	
	
	for(int i = 2 ; i <=6 ; i+=2){
		stringstream in ;
		in << orbitPrefix << i << orbitSuffix;
		vector<vector<double> > orbit;
		getValuesFromFile<double>(string(in.str()).c_str(),orbit);
		vector<DVector> v;
		getDVectorsFromRows(orbit,v);
		
		vector<capd::covrel::TripleSet> t;
		createTripleSets(v,t,i,scale);
		ZeroOutXDoubleCordinate zerout;
		hof::foreach(t.begin(),t.end(),zerout);
		for(int j = 0 ; j < t.size() ; ++j){
			stringstream out;
			out << triplesetPrefix << i << "_"<< j<< triplesetSuffix;
			string str(out.str());
			ofstream myfile;
			myfile.open(str.c_str());	
			myfile.precision(20);
			myfile << t[j];
			myfile.close();
		}
	}
	capd::covrel::TripleSet m3_1 = weighedAvrage(getTripleSet("2_1"),getTripleSet("4_1"))	;
	ofstream myfile;
	myfile.open("covrel/trisets/tset3_1.txt");
	myfile.precision(20);
	myfile << m3_1;
	myfile.close();
	
	capd::covrel::TripleSet n4_2 = bounce(getTripleSet("4_3"));
	myfile.open("covrel/trisets/tset4_2.txt");
	myfile.precision(20);
	myfile << n4_2;
	myfile.close();
	
}



int main(){
	cout.precision(20);
	createTripleSetsFromOrbitPoints(0.2);
	//capd::covrel::TripleSet tset = getTripleSet("2_0");
	//cout << tset << endl;
}
